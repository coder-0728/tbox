#include <malloc.h>
#include <unistd.h>

#include "tsp_event_secret.h"
#include "error_code.h"
#include "param_api.h"
#include "log.h"
#include "asn_pack.h"
#include "OtaNewPubKeyRes.h"
#include "OtaNewSecKeyReq.h"
#include "OtaNewSecKeyRes.h"
#include "aes_cbc.h"
#include <stdio.h>
#include <stdint.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string.h>
#include "tools.h"

static void bytesToHexString(const unsigned char* bytes, size_t length, char* hexString) 
{
    for (size_t i = 0; i < length; i++) {
        sprintf(hexString + (i * 2), "%02x", bytes[i]);
    }
}

// 加载 PEM 格式公钥文件
RSA *load_public_key(const char *pubkey_path) {
    FILE *fp = fopen(pubkey_path, "rb");
    if (!fp) {
        perror("Failed to open public key file");
        return NULL;
    }

    RSA *rsa_key = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
    fclose(fp);

    if (!rsa_key) {
        ERR_print_errors_fp(stderr);
        return NULL;
    }

    return rsa_key;
}

// 使用公钥加密数据
int encrypt_data(const unsigned char *plaintext, int plaintext_len, RSA *rsa_key, unsigned char *encrypted_data) {
    int rsa_size = RSA_size(rsa_key);
    int encrypted_length = RSA_public_encrypt(plaintext_len, plaintext, encrypted_data, rsa_key, RSA_NO_PADDING);

    if (encrypted_length == -1) {
        ERR_print_errors_fp(stderr);
        return 0;
    }

    return encrypted_length;
}

static int PackOtaSecPubKeyReq(unsigned char *secret_with_sn,unsigned char *out, int out_size)
{
    if(secret_with_sn == NULL )
    {
        return RET_ERR;
    }

    OtaNewSecKeyReq_t ota_new_sec_key_req;
    ota_new_sec_key_req.secKeyAndTboxSn.buf = (unsigned char *)malloc(sizeof(unsigned char) * 64);
    if(ota_new_sec_key_req.secKeyAndTboxSn.buf == NULL)
    {
        LOG_Error("malloc failed\n");
        return RET_ERR;
    }
    ota_new_sec_key_req.secKeyAndTboxSn.size = 64;
    //ota_new_sec_key_req.secKeyAndTboxSn.buf = secret_with_sn;
    memcpy(ota_new_sec_key_req.secKeyAndTboxSn.buf, secret_with_sn, 64);

    //编码application_data（OtaSecPubKeyReq）
    asn_enc_rval_t rval = uper_encode_to_buffer(&asn_DEF_OtaNewSecKeyReq, &ota_new_sec_key_req, out, out_size);
    if (rval.encoded == -1 )
    {
        LOG_Error("der_encode_to_buffer failed, %s\n",rval.failed_type->name);
        free(ota_new_sec_key_req.secKeyAndTboxSn.buf);
        ota_new_sec_key_req.secKeyAndTboxSn.buf = NULL;
        return RET_ERR;
    }

    free(ota_new_sec_key_req.secKeyAndTboxSn.buf);
    ota_new_sec_key_req.secKeyAndTboxSn.buf = NULL;

    return (rval.encoded + 7) / 8;
}

static int PackOtaNewPubKeyReq(unsigned char *sim_info, unsigned char *vin, unsigned char *iccid, unsigned char *out, int size)
{
    if(sim_info == NULL || vin == NULL || iccid == NULL)
    {
        return RET_ERR;
    }

    OtaNewPubKeyReq_t *ota_new_pub_key_req = NULL;

    //初始化application_data（OtaNewPubKeyReq）
    CreateOtaNewPubKeyReq(&ota_new_pub_key_req);
    if(ota_new_pub_key_req == NULL)
    {
        LOG_Error("malloc failed\n");
        return RET_ERR;
    }

    CreateVinCode(&ota_new_pub_key_req->vinCode, vin, strlen(vin));
    CreateIccid(&ota_new_pub_key_req->iccid, iccid, strlen(iccid));
    CreateSimInfo(&ota_new_pub_key_req->simInfo, sim_info, strlen(sim_info));

    //编码application_data（OtaNewPubKeyReq）
    asn_enc_rval_t rval = uper_encode_to_buffer(&asn_DEF_OtaNewPubKeyReq, ota_new_pub_key_req, out, size);
    if (rval.encoded == -1 )
    {
        LOG_Error("der_encode_to_buffer failed, %s\n",rval.failed_type->name);
        return RET_ERR;
    }

    FreeOtaNewPubKeyReq(ota_new_pub_key_req);

    return (rval.encoded + 7) / 8;
}

static int OtaNewPubKeyHandle(struct TspEvent *self, T_TspMessage *data, unsigned char *public_key_out, int *out_length)
{
    if(self == NULL || data == NULL || public_key_out == NULL || out_length == NULL )
    {
        return RET_ERR;
    }
    
    memcpy(data->body->applicationId.buf, "100", 3);
    data->body->applicationId.size = 3;
    data->body->messageId = MID_NewPubKeyReq;


    unsigned char *sim_info= "0000000018912493979";
    unsigned char *vin = NULL;
    unsigned char *iccid = NULL;
    unsigned char application_data_asn1[1024] = {0x0};

    vin = (unsigned char *)malloc(sizeof(unsigned char) * 18);
    if(vin == NULL)
    {
        LOG_Error("malloc failed\n");
        return RET_ERR;
    }

    iccid = (unsigned char *)malloc(sizeof(unsigned char) * 21);
    if(iccid == NULL)
    {
        LOG_Error("malloc failed\n");
        free(vin);
        vin = NULL;
        return RET_ERR;
    }

    GetParamsValue(enumVin, vin);
    GetParamsValue(enumIiccid, iccid);
    int ret = PackOtaNewPubKeyReq(sim_info, vin, iccid, application_data_asn1, sizeof(application_data_asn1));
    if(ret < 0)
    {
        free(vin);
        vin = NULL;
        free(iccid);
        iccid = NULL;
        LOG_Error("PackOtaNewPubKeyReq failed\n");
        return RET_ERR;
    }

    free(vin);
    free(iccid);
    vin = NULL;
    iccid = NULL;

    *(data->body->applicationDataLength) = ret;
    memcpy(data->application_data, application_data_asn1, *(data->body->applicationDataLength));

    ret = self->send(data);
    if(ret < 0)
    {
        LOG_Error("send failed\n");
        return RET_ERR;
    }

    T_TspMessage recv_data;
    ret = self->recv(&recv_data, 10 * 1000);
    if(ret < 0)
    {
        LOG_Error("recv failed\n");
        return RET_ERR;
    }

    printf("Get Public Key ...\n");

    if(recv_data.body->messageId != MID_NewPubKeyRes)
    {
        LOG_Error("messageId or applicationId is error\n");
        return RET_ERR;
    }

    OtaNewPubKeyRes_t *ota_new_pub_key_res = NULL;
    //解码application_data（OtaNewPubKeyRes）
    asn_dec_rval_t rc = uper_decode(0, &asn_DEF_OtaNewPubKeyRes, (void **)&ota_new_pub_key_res, recv_data.application_data, *(recv_data.body->applicationDataLength),0,0);
    if(rc.code != RC_OK)
    {
        LOG_Error("decode failed\n");
        return RET_ERR;
    }

    if(ota_new_pub_key_res->publicKeyGranted != true)
    {
        LOG_Error("publicKeyGranted is false\n");
        return RET_ERR;
    }

    //获取application_data中的result
    memcpy(public_key_out, ota_new_pub_key_res->publicKey->buf, ota_new_pub_key_res->publicKey->size);
    *out_length = ota_new_pub_key_res->publicKey->size;

    ASN_STRUCT_FREE(asn_DEF_OtaNewPubKeyRes, ota_new_pub_key_res);
    return RET_OK;
}

static int OtaNewSecKeyHandle(struct TspEvent *self, T_TspMessage *data, unsigned char *tbox_id_out, int *out_length)
{
    if (self == NULL || data == NULL || tbox_id_out == NULL)
    {
        return RET_ERR;
    }

    printf("OtaNewSecKeyHandle ...\n");

    memcpy(data->body->applicationId.buf, "100", 3);
    data->body->applicationId.size = 3;
    data->body->messageId = MID_NewSecKeyReq;

    unsigned char sec_and_sn[64] = {0x0};
    unsigned char sn[16] = {0x0};
    GetParamsValue(enumTboxSn, sn);


    unsigned char aesKey[8] = {0x0};
    unsigned char aesKey_str[16] = {0x0};
    generateAESKey(aesKey);
    bytesToHexString(aesKey, 8, aesKey_str);
    
    printf("aesKey_str:\n%s\n", aesKey_str);

    memset(sec_and_sn, 0x0, 64);
    memcpy(sec_and_sn, aesKey_str, 16);
    memcpy(sec_and_sn + 16, sn, 16);

    printf("sec_and_sn:\n");
    for(int i=0;i<64;i++)
    {
        printf("%02X", sec_and_sn[i]);
    }
    printf("\n");

    //加载证书
    RSA *rsa_key = load_public_key("/oemdata/pub_key.pem");
    if (!rsa_key) {
        return -1;
    }

    // 计算加密后数据所需的缓冲区大小
    int rsa_size = RSA_size(rsa_key);
    unsigned char encrypted_data[rsa_size] = {0x0};

    // 使用公钥加密数据
    int encrypted_length = encrypt_data(sec_and_sn, 64, rsa_key, encrypted_data);
    if (encrypted_length == 0) {
        RSA_free(rsa_key);
        return -1;
    }
    
    // 输出加密结果（密文）
    printf("Ciphertext[%d]: ", encrypted_length);
    for (int i = 0; i < 64; i++) {
        printf("%02x", encrypted_data[i]);
    }
    printf("\n");

    RSA_free(rsa_key);

    unsigned char application_data_asn1[1024] = {0x0};
    memset(application_data_asn1, 0x0, sizeof(application_data_asn1));
    int ret = PackOtaSecPubKeyReq(encrypted_data, application_data_asn1, 1024);
    if(ret == RET_ERR)
    {
        FreeDispatcherBody(data->body);
        LOG_Error("PackOtaSecPubKeyReq failed\n");
        return RET_ERR;
    }

    *(data->body->applicationDataLength) = ret;

    memset(data->application_data, 0x0, 1024);
    memcpy(data->application_data, application_data_asn1, *(data->body->applicationDataLength));
    
    ret = self->send(data);
    if(ret < 0)
    {
        LOG_Error("send failed\n");
        return RET_ERR;
    }

    //备份seckey
    unsigned char tmp_sec_key[16] = {0x0};
    GetParamsValue(enumSecKey, tmp_sec_key);

    SetParamsValue(enumSecKey, aesKey_str);

    T_TspMessage recv_data;
    ret = self->recv(&recv_data, 10 * 1000);
    if(ret < 0)
    {
        SetParamsValue(enumSecKey, tmp_sec_key);
        LOG_Error("recv failed\n");
        return RET_ERR;
    }

    printf("Get Tbox ID ...\n");

    if(recv_data.body->messageId != MID_NewSecKeyRes)
    {
        SetParamsValue(enumSecKey, tmp_sec_key);
        LOG_Error("messageId or applicationId is error\n");
        return RET_ERR;
    }

    OtaNewSecKeyRes_t *ota_new_sec_key_res = NULL;
    //解码application_data（OtaNewPubKeyRes）
    asn_dec_rval_t rc = uper_decode(0, &asn_DEF_OtaNewSecKeyRes, (void **)&ota_new_sec_key_res, recv_data.application_data, *(recv_data.body->applicationDataLength),0,0);
    if(rc.code != RC_OK)
    {
        SetParamsValue(enumSecKey, tmp_sec_key);
        printf("decode failed\n");
        return RET_ERR;
    }

    if(ota_new_sec_key_res->secKeyReq != true)
    {
        SetParamsValue(enumSecKey, tmp_sec_key);
        printf("secKeyReq is false\n");
        return RET_ERR;
    }

    //获取application_data中的result
    memcpy(tbox_id_out, ota_new_sec_key_res->tboxID->buf, ota_new_sec_key_res->tboxID->size);
    *out_length = ota_new_sec_key_res->tboxID->size;
    printf("tbox_id_out: %s\n",tbox_id_out);

    ASN_STRUCT_FREE(asn_DEF_OtaNewSecKeyRes, ota_new_sec_key_res);

    //TODO ：释放内存
    return RET_OK;
}

// 函数生成PEM格式的公钥证书
int generate_public_key_certificate(const char *n_hex_array, const char *e_hex_array, const char *certificate_path) {
    RSA *rsa_key = RSA_new();

    // 分配内存并将 n 和 e 设置为大整数
    rsa_key->n = BN_bin2bn(n_hex_array, 64, rsa_key->n);
    rsa_key->e = BN_bin2bn(e_hex_array, 3, rsa_key->e);

    if (rsa_key->n == NULL || rsa_key->e == NULL) {
        printf("Failed to set RSA public key components.\n");
        RSA_free(rsa_key);
        return -1; // 返回失败
    }

    // 创建BIO，用于将RSA结构体写入PEM格式
    BIO *bio_pubkey = BIO_new(BIO_s_file());

    if (BIO_write_filename(bio_pubkey, (void *)certificate_path) <= 0) {
        printf("Failed to open certificate file for writing.\n");
        BIO_free(bio_pubkey);
        RSA_free(rsa_key);
        return -1; // 返回失败
    }

    if (PEM_write_bio_RSA_PUBKEY(bio_pubkey, rsa_key) == 0) {
        printf("Failed to write RSA public key in PEM format.\n");
        BIO_free(bio_pubkey);
        RSA_free(rsa_key);
        return -1; // 返回失败
    }

    // 释放资源
    BIO_free(bio_pubkey);
    RSA_free(rsa_key);

    return 0; // 返回成功
}

/**
 * @brief 生成新的密钥
 * @param self 事件对象
 * @param data 事件数据
 * 
 * @return 0:成功，-1:失败
*/
int EventTask_SecretKeyUpdate(struct TspEvent* self, T_TspMessage *data)
{
    LOG_Debug("EventTask_SecretKeyUpdate\n");

    unsigned char sn[16] = {0x0};
    char tbox_id[12] = {0x0};
    T_TspMessage tsp_send_msg;

    int ret = -1;
    uint32_t timeStamp = time(NULL);

    tsp_send_msg.application_data = (unsigned char *)malloc(sizeof(unsigned char) * 1024);
    if (tsp_send_msg.application_data == NULL)
    {
        LOG_Debug("malloc tsp_send_msg.application_data failed\n");
        return RET_ERR;
    }

    //记录Event Start时间
    //self->event_creation_time = GetCreationTime();

    //初始化Header
    tsp_send_msg.header.protocol_version = 0x21;
    tsp_send_msg.header.dispatcher_message_length = 0;
    tsp_send_msg.header.dispatcher_body_encoding = 0;
    GetParamsValue(enumTboxSn, sn);
    tsp_send_msg.header.tbox_identification_type = 1;
    memcpy(tsp_send_msg.header.tbox_sn, sn, 16);

#if 0
    //激活指挥用
    GetParamsValue(enumTboxId, tbox_id);
    if(strstr(tbox_id,"000000000000") == NULL)
    {
        tsp_send_msg.header.tbox_identification_type = 0;
    }
    else
    {
        tsp_send_msg.header.tbox_identification_type = 1;
    }

    if(tsp_send_msg.header.tbox_identification_type == 1)
    {
        GetParamsValue(enumTboxSn, sn);
        memcpy(tsp_send_msg.header.tbox_sn, sn, 16);
    }
    else
    {
        memcpy(tsp_send_msg.header.tbox_id, tbox_id, 12);
    }
#endif
    
    

    //初始化Body
    tsp_send_msg.body = NULL;
    CreateDispatcherBody(&tsp_send_msg.body);
    CreateApplicationId(&tsp_send_msg.body->applicationId, "100", 3);
    tsp_send_msg.body->messageId = MID_NewPubKeyReq;
    tsp_send_msg.body->eventCreationTime = timeStamp;
    tsp_send_msg.body->eventID = 0;
    CreateUlMessageCounter(&tsp_send_msg.body->ulMessageCounter,0);
    CreateDlMessageCounter(&tsp_send_msg.body->dlMessageCounter,0);
    CreateAckMessageCounter(&tsp_send_msg.body->ackMessageCounter,0);
    CreateAckRequired(&tsp_send_msg.body->ackRequired,0);
    CreateApplicationDataLength(&tsp_send_msg.body->applicationDataLength,0);
    CreateDataEncodingType(&tsp_send_msg.body->dataEncodingType,0);
    CreateApplicationDataProtocolVersion(&tsp_send_msg.body->applicationDataProtocolVersion, APPLICATION_DATA_VERSION);
    CreateTestFlag(&tsp_send_msg.body->testFlag,1);
    CreateResult(&tsp_send_msg.body->result,0);

    //执行OtaNewPubKey请求
    unsigned char public_key[64] = {0x0};
    int public_key_length = 0;
    ret = OtaNewPubKeyHandle(self, &tsp_send_msg, public_key, &public_key_length);
    if(ret != RET_OK || public_key_length != 64)
    {
        LOG_Error("OtaNewPubKeyHandle failed\n");
        ASN_STRUCT_FREE(asn_DEF_DispatcherBody, tsp_send_msg.body);
        free(tsp_send_msg.application_data);
        tsp_send_msg.application_data = NULL;
        return RET_ERR;
    }

    for(int i = 0; i < 64; i++)
    {
        printf("%02x", public_key[i]);
    }
    printf("\n");

    //生成PEM证书
    unsigned char e_hex_array[] = { 0x01, 0x00, 0x01 };
    ret = generate_public_key_certificate(public_key, e_hex_array,"/oemdata/pub_key.pem");
    if (ret == 0) {
        printf("Public key certificate generated successfully.\n");
    } else {
        printf("Failed to generate public key certificate.\n");
        return -1;
    }


    //执行OtaNewSecKey请求
    unsigned char tbox_id_out[12] = {0x0};
    int tbox_id_length = 0;
    ret = OtaNewSecKeyHandle(self, &tsp_send_msg, tbox_id_out, &tbox_id_length);
    if(ret != RET_OK )
    {
        LOG_Error("OtaNewSecKeyHandle failed\n");
        ASN_STRUCT_FREE(asn_DEF_DispatcherBody, tsp_send_msg.body);
        free(tsp_send_msg.application_data);
        tsp_send_msg.application_data = NULL;
        return RET_ERR;
    }

    //保存tbox_id
    printf("tbox_id:%s\n", tbox_id_out);
    SetParamsValue(enumTboxId, tbox_id_out);
    ASN_STRUCT_FREE(asn_DEF_DispatcherBody, tsp_send_msg.body);

    return RET_OK;

}