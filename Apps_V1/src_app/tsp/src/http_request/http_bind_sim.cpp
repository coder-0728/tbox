#include "http_bind_sim.h"
#include "param_api.h"
#include "tools.h"
#include "http_interface.h"
#include "cJSON.h"
#include "error_code.h"
#include "aes_cbc.h"
#include "log.h"

#include <malloc.h>
#include <openssl/sha.h>
#include <string.h>

int http_post_bind_sim(unsigned char *vin, unsigned char *sim_info,unsigned char *country_code, unsigned char *iccid, int *support_ipv6=NULL, unsigned char *ipv6_address=NULL)
{
    unsigned char vin_sha256[32] = {0x0};
    unsigned char vin_sha256_str[sizeof(vin_sha256) * 2] = {0x0};
    unsigned char iccid_str[64] = {0x0};
    unsigned char simInfo_str[64] = {0x0};
    unsigned char send_date[32] = {0x0};
    unsigned char send_data_str[64] = {0x0};
    unsigned char tbox_id[12] = {0x0};
    unsigned char country_code_str[4] = {0x0};
    unsigned char tbox_signature[256] = {0x0};
    unsigned char tbox_signature_str[256] = {0x0};
    unsigned char secret_key[64] = {0x0};
    unsigned char tbox_signature_sha256[32] = {0x0};

    SHA256(vin, strlen(vin), vin_sha256);
    bytesToHexString(vin_sha256, 32, vin_sha256_str);

    bytesToHexString(sim_info, strlen(sim_info), simInfo_str);

    bytesToHexString(iccid, strlen(iccid), iccid_str);

    bytesToHexString(country_code,strlen(country_code),country_code_str);

    curl_slist *headers = NULL;
    //get_format_time_ms(send_date);
    //sprintf(send_data_str,"TBOX-SEND-DATE:%s",send_date);

    GetParamsValue(enumTboxId,tbox_id);
    GetParamsValue(enumSecKey, secret_key);
    sprintf(tbox_signature, "%s%s%s%s%s", tbox_id, send_date, "1", vin_sha256_str, simInfo_str);
    hmac_sha256(secret_key, strlen(secret_key), tbox_signature, strlen(tbox_signature), tbox_signature_sha256);
    sprintf(tbox_signature_str, "TBOX-SIGNATURE:%s", tbox_signature_sha256);

    headers = curl_slist_append(headers, "TBOX-SEND-DATE: 1689662731000");
    headers = curl_slist_append(headers, "TBOX-CONTENT-ENCRYPTED: 1");
    headers = curl_slist_append(headers, "Content-Type: text/plain");
    headers = curl_slist_append(headers, tbox_signature_str);
    //TBOX-SIGNATURE（SHA256加密） -> tbox-id + TBOX-SEND-DATE + TBOX-CONTENT-ENCRYPTED + VIN + SN

    unsigned char url[256] = {0};
    sprintf(url, "%s%s", ROOT_URL_DEVELOP, POST_BIND_SIM_CARD);
    
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "vin", vin_sha256_str);
    cJSON_AddStringToObject(root, "simInfo", simInfo_str);
    cJSON_AddStringToObject(root, "contryCode", country_code_str);
    cJSON_AddStringToObject(root, "iccid", iccid_str);

    char *json_string = cJSON_Print(root);

    char json_aes_cbc[1024] = {0x0};
    char json_aes_cbc_str[1024] = {0x0};

    char iv[16] = {0};
    int ret = aes_cbc_128_encrypt(json_string, strlen(json_string), secret_key, iv, json_aes_cbc);
    bytesToHexString(json_aes_cbc, ret, json_aes_cbc_str);
    cJSON_Delete(root);
    root = NULL;

    unsigned char data_out[1024] = {0x0};
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

    cJSON *json = cJSON_Parse(data_out);
    if (json == NULL) {
        LOG_Error("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return RET_ERR;
    }

    int code = cJSON_GetObjectItem(json, "code")->valueint;
    if(code != 0)
    {
        LOG_Error("sim bind error: %s\n", cJSON_GetErrorPtr());
        return RET_ERR;
    }

    cJSON_Delete(json);
    json = NULL;

    return RET_OK;
}

int http_post_bind_sim_verification(unsigned char *vin, unsigned char *sim_info, unsigned char verification_code)
{
    unsigned char vin_sha256[32] = {0x0};
    unsigned char vin_sha256_str[sizeof(vin_sha256) * 2] = {0x0};
    unsigned char simInfo_str[64] = {0x0};
    unsigned char verification_code_str[32] = {0x0};



    SHA256(vin, strlen(vin), vin_sha256);
    bytesToHexString(vin_sha256, 32, vin_sha256_str);

    bytesToHexString(sim_info, strlen(sim_info), simInfo_str);

    bytesToHexString(verification_code, strlen(verification_code), verification_code_str);

    curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "TBOX-SEND-DATE: 1689662731000");
    headers = curl_slist_append(headers, "TBOX-CONTENT-ENCRYPTED: 1");
    headers = curl_slist_append(headers, "Content-Type: text/plain");
    //TBOX-SIGNATURE（SHA256加密） -> tbox-id + TBOX-SEND-DATE + TBOX-CONTENT-ENCRYPTED + VIN + SN

    unsigned char url[256] = {0};
    sprintf(url, "%s%s", ROOT_URL_DEVELOP, POST_VERIFY_SIM_CARD);
    
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "vin", vin_sha256_str);
    cJSON_AddStringToObject(root, "simInfo", simInfo_str);
    cJSON_AddStringToObject(root, "verificationCode", verification_code_str);

    char *json_string = cJSON_Print(root);

    char json_aes_cbc[1024] = {0x0};
    char json_aes_cbc_str[1024] = {0x0};

    char iv[16] = {0};
    int ret = aes_cbc_128_encrypt(json_string, strlen(json_string), "1234567890123456", "1234567890123456", json_aes_cbc);
    bytesToHexString(json_aes_cbc, ret, json_aes_cbc_str);
    cJSON_Delete(root);
    root = NULL;

    unsigned char data_out[1024] = {0x0};
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

    cJSON *json = cJSON_Parse(data_out);
    if (json == NULL) {
        LOG_Error("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return RET_ERR;
    }

    int code = cJSON_GetObjectItem(json, "code")->valueint;
    if(code != 0)
    {
        LOG_Error("sim bind error: %s\n", cJSON_GetErrorPtr());
        return RET_ERR;
    }

    cJSON_Delete(json);
    json = NULL;

    return RET_OK;
}