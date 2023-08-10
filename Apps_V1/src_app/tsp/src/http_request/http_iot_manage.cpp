#include "http_iot_manage.h"
#include "cJSON.h"
#include "log.h"
#include "error_code.h"
#include "param_api.h"
#include "tools.h"
#include "aes_cbc.h"
#include "http_interface.h"

#include <string.h>
#include <time.h>
#include <openssl/md5.h>

#define MD5_LENGTH MD5_DIGEST_LENGTH

static int iot_regist_json_parse(const char* json)
{
    cJSON* root = cJSON_Parse(json);
    if (root == NULL) {
        LOG_Error("Failed to parse JSON.\n");
        return RET_ERR;
    }

    // Parse code field
    int code = cJSON_GetObjectItem(root, "code")->valueint;

    // Parse data field
    cJSON* data = cJSON_GetObjectItem(root, "data");
    if (data != NULL) {
        cJSON* registrationInfo = cJSON_GetObjectItem(data, "registrationInfo");
        if (registrationInfo != NULL) {
            const char* endpoint = cJSON_GetObjectItem(registrationInfo, "endpoint")->valuestring;
            const char* certPrivateKey = cJSON_GetObjectItem(registrationInfo, "certPrivateKey")->valuestring;
            const char* rootCA = cJSON_GetObjectItem(registrationInfo, "rootCA")->valuestring;
            const char* certCertificatePem = cJSON_GetObjectItem(registrationInfo, "certCertificatePem")->valuestring;

            LOG_Debug("Endpoint: %s\n", endpoint);
            LOG_Debug("Cert Private Key: %s\n", certPrivateKey);
            LOG_Debug("Root CA: %s\n", rootCA);
            LOG_Debug("Cert Certificate PEM: %s\n", certCertificatePem);

            SetParamsValue(enumMqttUrl, endpoint);
            writeToFile("/oemdata/ca/certPrivateKey", certPrivateKey);
            writeToFile("/oemdata/ca/rootCA", rootCA);
            writeToFile("/oemdata/ca/certCertificatePem", certCertificatePem);
            
        }
        else
        {
            return RET_ERR;
        }

        // Parse topicList field
        cJSON* topicList = cJSON_GetObjectItem(data, "topicList");
        if(topicList == NULL)
        {
            LOG_Error("topicList is NULL\n");
            return RET_ERR;
        }

        if (topicList != NULL && topicList->type == cJSON_Array) 
        {
            int topicCount = cJSON_GetArraySize(topicList);
            //topic_info_list_t topic_info_list;
            //memset(&topic_info_list, 0, sizeof(topic_info_list_t));
            //topic_info_list.size = topicCount;       

            LOG_Debug("Topic List:\n");
            for (int i = 0; i < topicCount; i++) 
            {
                cJSON* topic = cJSON_GetArrayItem(topicList, i);
                const char* topicName = cJSON_GetObjectItem(topic, "topicName")->valuestring;
                const char* topicType = cJSON_GetObjectItem(topic, "topicType")->valuestring;
                LOG_Debug("Topic %d: %s (Type: %s)\n", i + 1, topicName, topicType);

                if(strstr(topicType,"OTA_UPLINK"))
                {
                    SetParamsValue(enumMqttOtaUpLink, topicName);
                    LOG_Debug("SetParamsValue OTA_UPLINK %s\n", topicName);
                }
                else if(strstr(topicType,"OTA_DOWNLINK"))
                {
                    SetParamsValue(enumMqttOtaDownLink, topicName);
                    LOG_Debug("SetParamsValue OTA_DOWNLINK %s\n", topicName);
                }
                else if(strstr(topicType,"MSG_UPLINK"))
                {
                    SetParamsValue(enumMqttMsgUpLink, topicName);
                    LOG_Debug("SetParamsValue MSG_UPLINK %s\n", topicName);
                }
                else if(strstr(topicType,"MSG_DOWNLINK"))
                {
                    SetParamsValue(enumMqttMsgDownLink, topicName);
                    LOG_Debug("SetParamsValue MSG_DOWNLINK %s\n", topicName);
                }
                else
                {
                    LOG_Error("Unknow topicType %s\n", topicType);
                }
            }
        }
    }

    if(root != NULL)
    {
        cJSON_Delete(root);
        root = NULL;
    }

    return RET_OK;
}

/**
 * 获取IOT TYPE请求
 * 
 * 传给TSP的参数为SHA256加密的VIN
 * 
 * TSP响应SHA256加密的VIN，IOT类型，是否需要包含证书签发请求，CSR信息
 */
static int http_get_iot_type(const char *vin, iot_type_result_t *result)
{
    unsigned char vin_sha256[32] = {0};
    char vin_sha256_str[2 * 32 + 1] = {0x00};
    char url[256] = {0};
    char data_out[512] = {0};
    char tbox_send_date[32] = {0};
    int ret = RET_ERR;
    curl_slist *headers = NULL;
    struct timeval tv;
    
    ret = sha256_encrypt(vin, strlen(vin), vin_sha256);
    if(ret != RET_OK)
    {
        LOG_Debug("sha256_encrypt failed\n");
        return RET_ERR;
    }

    bytesToHexString(vin_sha256, 32, vin_sha256_str);

    gettimeofday(&tv, NULL);
    sprintf(tbox_send_date,"TBOX-SEND-DATE:%ld", tv.tv_sec * 1000 + tv.tv_usec / 1000);

    headers = curl_slist_append(headers, "TBOX-SEND-DATE: 1689662731000");
    headers = curl_slist_append(headers, "TBOX-CONTENT-ENCRYPTED: 0");

    sprintf(url, "%s%s%s", ROOT_URL_DEVELOP, GET_IOT_TYPE, vin_sha256_str);

    ret = http_get(headers, url, data_out);
    if(ret != RET_OK)
    {
        LOG_Debug("http_get failed\n");
        if(headers != NULL)
        {
            curl_slist_free_all(headers);
            headers = NULL;
        }
        return RET_ERR;
    }
    if(headers != NULL)
    {
        curl_slist_free_all(headers);
        headers = NULL;
    }
    LOG_Debug("--------> %s\n",data_out);

    /* TODO:这里解析TSP回过来的数据 */
    cJSON *json = cJSON_Parse(data_out);
    if (json == NULL) {
        LOG_Error("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return RET_ERR;
    }

    int code = cJSON_GetObjectItem(json, "code")->valueint;
    const char *message = cJSON_GetObjectItem(json, "message")->valuestring;

    if(code != 0)
    {
        LOG_Error("code:%d, message:%s\n", code, message);
        cJSON_Delete(json);
        return RET_ERR;
    }

    cJSON *data = cJSON_GetObjectItem(json, "data");
    result->isCsrNeeded = cJSON_GetObjectItem(data, "isCsrNeeded")->valueint;
    memcpy(result->vin, cJSON_GetObjectItem(data, "vin")->valuestring, strlen(cJSON_GetObjectItem(data, "vin")->valuestring));
    result->iotType = cJSON_GetObjectItem(data, "iotType")->valueint;

    cJSON_Delete(json);
    json = NULL;

    return RET_OK;
}

/**
 * IOT注册请求
 * 
 * 传给TSP的参数为
 * SHA256加密的VIN OK
 * AES加密的SN(KEY为VIN明文的MD5)
 * HMAC-SHA256 加密的sign（vin明文+sn明文+TBOX-SEND-DATE,使用key SN明文的MD5
 * csr 证书签发请求
*/
int http_post_iot_regist(const unsigned char *vin, unsigned char *sn,unsigned char *csr = NULL)
{
    unsigned char vin_sha256[32] = {0};
    
    unsigned char url[256] = {0};
    unsigned char data_out[1024 * 10] = {0};
    unsigned char data_out_decrypt[1024 * 10] = {0};
    unsigned char tbox_send_date[32] = {0};
    unsigned char aes_sn[128] = {0};
    
    unsigned char sign[256] = {0};
    unsigned char sign_hmac_sha256[256] = {0};
    
    unsigned char vin_md5[MD5_LENGTH] = {0};
    unsigned char sn_md5[MD5_LENGTH] = {0};
    unsigned char sign_md5[MD5_LENGTH] = {0};

    unsigned char vin_sha256_str[2 * 32 + 1] = {0x00};
    unsigned char aes_sn_str[2 * 128 + 1] = {0};
    unsigned char sign_hmac_sha256_str[2 * 256 + 1] = {0};
    unsigned char vin_md5_str[2 * MD5_LENGTH + 1] = {0};
    unsigned char sn_md5_str[2 * MD5_LENGTH + 1] = {0};
    
    int aes_sn_length = 0;
    int ret = RET_ERR;
    curl_slist *headers = NULL;
    struct timeval tv;
    
    /* 1.获取SHA256加密的VIN */
    ret = sha256_encrypt(vin, strlen(vin), vin_sha256);
    if(ret != RET_OK)
    {
        LOG_Debug("sha256_encrypt failed\n");
        return RET_ERR;
    }

    bytesToHexString(vin_sha256, 32, vin_sha256_str);

    /* 2.加密SN，使用AES加密, KEY使用明文VIN的MD5值 */
    md5_hash(vin, strlen(vin), vin_md5);

    /* 生成sign */
    gettimeofday(&tv, NULL);
    unsigned char timestamp[32] = {0x0};
    get_format_time_ms(timestamp);
    sprintf(sign, "%s%s%s", vin, sn, "1689662731000");
    unsigned char sec_key[16] = {0x0};
    GetParamsValue(enumSecKey,sec_key);

    aes_sn_length = aes_cbc_128_encrypt(sn, strlen(sn), vin_md5, sec_key, aes_sn);
    if(aes_sn_length <= 0 )
    {
        LOG_Error("aes_ecb_encrypt error");
        return RET_ERR;
    }
    bytesToHexString(aes_sn, aes_sn_length, aes_sn_str);

    /* 4.使用SN的明文的MD5值加密sign，使用HMAC_SHA256 */
    md5_hash(sn, strlen(sn), sn_md5);
    bytesToHexString(sn_md5, MD5_DIGEST_LENGTH, sn_md5_str);

    hmac_sha256(sn_md5, MD5_DIGEST_LENGTH, sign, strlen(sign), sign_hmac_sha256);
    bytesToHexString(sign_hmac_sha256, strlen(sign_hmac_sha256), sign_hmac_sha256_str);
    
    //sprintf(tbox_send_date,"TBOX-SEND-DATE:%s",timestamp);
    //printf("%s\n",tbox_send_date);
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "TBOX-SEND-DATE: 1689662731000");
    headers = curl_slist_append(headers, "TBOX-CONTENT-ENCRYPTED: 1");
    headers = curl_slist_append(headers, "Content-Type: text/plain");

    sprintf(url, "%s%s%s", ROOT_URL_DEVELOP, POST_IOT_REGISTER, vin_sha256_str);
    
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "sn", aes_sn_str);
    cJSON_AddStringToObject(root, "sign", sign_hmac_sha256_str);
    //TODO : CSR暂时不加
    //cJSON_AddStringToObject(root, "csr", );

    /* 5.组包JSON */
    char *json_string = cJSON_Print(root);

    char json_aes_cbc[1024] = {0x0};
    char json_aes_cbc_str[1024] = {0x0};
    char iv[16] = {0x0};
    ret = aes_cbc_128_encrypt(json_string, strlen(json_string), sec_key, sec_key, json_aes_cbc);
    bytesToHexString(json_aes_cbc, ret, json_aes_cbc_str);

    ret = http_post(headers, url, json_aes_cbc_str, data_out);
    if(ret != RET_OK)
    {
        LOG_Error("http_get failed\n");
        if(json_string != NULL)
        {
            free(json_string);
            json_string = NULL;
        }

        if(headers != NULL)
        {
            curl_slist_free_all(headers);
            headers = NULL;
        }
        return RET_ERR;
    }

    if(headers != NULL)
    {
        curl_slist_free_all(headers);
        headers = NULL;
    }

    //获取KEY（Secret Key + sn（明文）的MD5）
    unsigned char request_key[64] = {0x0};
    unsigned char request_key_md5[MD5_DIGEST_LENGTH] = {0x0};
    sprintf(request_key, "%s%s", sec_key, sn);
    md5_hash(request_key, strlen(request_key), request_key_md5);

    printf("out -> %s\n",data_out);

    char data_hex[1024*10] = {0x00};
    int data_hex_len = 0;
    data_hex_len = hexStringToBytes(data_out, strlen(data_out), data_hex);
    
    //解密
    ret = aes_cbc_128_decrypt(data_hex, data_hex_len, request_key_md5, sec_key, data_out_decrypt);
    if(ret == RET_ERR)
    {
        LOG_Error("aes_cbc_128_decrypt failed\n");
        if(json_string != NULL)
        {
            free(json_string);
            json_string = NULL;
        }
        return RET_ERR;
    }


    ret = iot_regist_json_parse(data_out_decrypt);

    return ret;
}