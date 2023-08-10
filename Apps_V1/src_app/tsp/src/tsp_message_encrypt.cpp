#include "tsp_message_encrypt.h"
#include "aes_cbc.h"
#include "error_code.h"

#include <stdio.h>
#include <string.h>
#include <openssl/asn1.h>
#include <openssl/asn1t.h>

#define RANDOM_LENGTH 16

static short hexStringToShort(const char* hexString, size_t length) {
    if (length > 4) {
        // Input string is too long to fit in a short, handle error
        // For example, print an error message or return a default value
        printf("Error: Hexadecimal string is too long for short\n");
        return 0; // Return a default value or an appropriate error handling
    }
    
    // Make a copy of the hexadecimal string with a null terminator
    char hexCopy[5];
    strncpy(hexCopy, hexString, length);
    hexCopy[length] = '\0';
    
    char* endPtr;
    unsigned long value = strtoul(hexCopy, &endPtr, 16);
    
    if (*endPtr != '\0') {
        // The conversion failed, handle error
        // For example, print an error message or return a default value
        printf("Error: Invalid hexadecimal string\n");
        return 0; // Return a default value or an appropriate error handling
    }
    
    // Check if the value is within the range of a short
    if (value > 0xFFFF) {
        // Value is too large to fit in a short, handle error
        // For example, print an error message or return a default value
        printf("Error: Hexadecimal value out of range for short\n");
        return 0; // Return a default value or an appropriate error handling
    }
    
    // Conversion was successful, return the short value
    return (short)value;
}

static void bytesToHexString(const unsigned char* bytes, size_t length, char* hexString) 
{
    for (size_t i = 0; i < length; i++) {
        sprintf(hexString + (i * 2), "%02x", bytes[i]);
    }
}


// 将十六进制字符转换为对应的整数值
int hexCharToValue(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return 10 + (c - 'A');
    } else if (c >= 'a' && c <= 'f') {
        return 10 + (c - 'a');
    }
    // 非法字符
    return -1;
}

// 将十六进制字符串转换为字节数组
// 返回值为字节数组的长度
size_t hexStringToBytes(const char* hexString, size_t hexStringLength, unsigned char* byteArray, size_t byteArraySize) {
    if (hexStringLength % 2 != 0) {
        // 非法的十六进制字符串，必须是偶数个字符
        return 0;
    }

    size_t numBytes = hexStringLength;
    if (numBytes > byteArraySize) {
        // 目标字节数组不够大
        return 0;
    }

    for (size_t i = 0; i < numBytes; i++) {
        int upperNibble = hexCharToValue(hexString[i * 2]);
        int lowerNibble = hexCharToValue(hexString[i * 2 + 1]);
        if (upperNibble == -1 || lowerNibble == -1) {
            // 非法的十六进制字符
            return 0;
        }

        byteArray[i] = (unsigned char)((upperNibble << 4) | lowerNibble);
    }

    return numBytes;
}

/** 传applicationData近来之前，使用ASN1 UPER进行编码，因为传进来后要判断类型再去编码，太麻烦 **/
int Tsp_Message_Encrypt(T_TspMessage *data, unsigned char *key, unsigned char *iv, void *transmit_data)
{
    if(data == NULL || transmit_data == NULL)
    {
        return RET_ERR;
    }

    int ret = RET_ERR;

    //0. 初始化结构体
    T_TransmitLayerMessage *transmit_message = NULL;
    transmit_message = (T_TransmitLayerMessage *)malloc(sizeof(T_TransmitLayerMessage));
    if(transmit_message == NULL)
    {
        return RET_ERR;
    }

    //flag 为ascii码 1
    transmit_message->flag = 0x01;
    transmit_message->header = data->header;
    transmit_message->body = data->body;
    transmit_message->application_data = (unsigned char *)malloc(*(data->body->applicationDataLength));
    memcpy(transmit_message->application_data, data->application_data, *(data->body->applicationDataLength));
    memcpy(transmit_message->random, iv, 16);

    //2. ASN.1编码body和application datas
    unsigned char encode_data_body[1024] = {0x0};
    unsigned char encode_data_header[1024] = {0x0};
    unsigned char final_data[1024] = {0x0};
    int final_data_len = 0;
    int offset = 0;
    int encoded_body_Length = 0;

    //BODY进行ASN.1 UPER编码
    asn_enc_rval_t rval = uper_encode_to_buffer(&asn_DEF_DispatcherBody, transmit_message->body, encode_data_body, 1024);
    if (rval.encoded == -1) {
        fprintf(stderr, "Encoding failed: %s\n", rval.failed_type->name);
        free(transmit_message->application_data);
        transmit_message->application_data = NULL;
        free(transmit_message);
        transmit_message = NULL;
        return -1;
    }

    encoded_body_Length = (rval.encoded + 7)/8;


    //3. tmp_data = random + body + application data (AES-128-CBC)
    unsigned char tmp_data[1024] = {0x0};
    unsigned char encrypt_data[1024] = {0x0};
    offset = 0;
    memcpy(tmp_data, (unsigned char *)&transmit_message->random, 16);
    offset += 16;
    memcpy(tmp_data + offset, encode_data_body, encoded_body_Length);
    offset += encoded_body_Length;
    memcpy(tmp_data + offset, transmit_message->application_data, *(transmit_message->body->applicationDataLength));
    offset += *(transmit_message->body->applicationDataLength);
    
    ret = aes_cbc_128_encrypt(tmp_data, offset, key, iv, encrypt_data);

    char hexString[1024] = {0x0};
    unsigned char hexString_tmp[1024] = {0x0};
    int head_length = 0;

    // header的长度位为 header length + body length
    // 要在把header的拉成数组前，先把header的长度位计算出来
    if(transmit_message->header.tbox_identification_type == 0)
    {
        transmit_message->header.dispatcher_message_length = 4 + 12 + encoded_body_Length;
    }
    else
    {
        transmit_message->header.dispatcher_message_length = 4 + 16 + encoded_body_Length;
    }

    memcpy(hexString_tmp , (void *)&transmit_message->header, 4);
    head_length += 4;
    if(transmit_message->header.tbox_identification_type == 0)
    {
        memcpy(hexString_tmp + head_length, (void *)&transmit_message->header.tbox_id, 12);
        head_length += 12;
    }
    else
    {
        memcpy(hexString_tmp + head_length, (void *)&transmit_message->header.tbox_sn, 16);
        head_length += 16;
    }
    
    memcpy(hexString_tmp + head_length, encrypt_data, ret);
    bytesToHexString(hexString_tmp, head_length + ret, hexString);

    //首位FLAG，强制写成string的1
    memcpy(final_data, "1", 1);
    final_data_len += 1;
    
    //长度为flag(1) + length(2) + header(4+16|12) + aes(random+body+application data)，大小端对调
    transmit_message->length = 3 + strlen(hexString)/2;
    char length_str[4] = {0x0};
    bytesToHexString((unsigned char *)&transmit_message->length, 2, length_str);
    memcpy(final_data + final_data_len, length_str+2, 2);
    memcpy(final_data + final_data_len + 2, length_str, 2);
    final_data_len += 4;

    //header + random（16位 aes_cbc_128）+ body (asn1 + aes_cbc_128) + application data (asn1 aes_cbc_128) hexstring
    memcpy(final_data + final_data_len, hexString, strlen(hexString));
    final_data_len += strlen(hexString);


    memcpy(transmit_data, final_data, final_data_len);
    
    if(transmit_message != NULL)
    {
        free(transmit_message);
        transmit_message = NULL;
    }

    return final_data_len;
}

int Tsp_Message_Decrypt(void *data_in, int data_len , unsigned char *key, unsigned char *iv, T_TspMessage *data_out)
{
    if(data_in == NULL || data_out == NULL)
    {
        return RET_ERR;
    }

    unsigned char dispatcher_body[1024] = {0x0};    
    int dispatcher_body_len = 0;

    unsigned char aes_decrypt_data[1024] = {0x0};
    unsigned int aes_decrypt_data_len = 0;

    char *data = (char *)data_in;
    int tbox_identification_length = 0;

    //数据的第0位为flag，1-2为short类型的长度
    unsigned short length = 0;
    length = hexStringToShort(data+1,4);

    unsigned char data_byte[1024] = {0x0};
    int data_byte_len = 0;

    data_byte_len = hexStringToBytes(data+5, length-3, data_byte, 1024);
    data_out->header.protocol_version = data_byte[0];
    data_out->header.dispatcher_message_length = data_byte[1];
    data_out->header.dispatcher_body_encoding = data_byte[2];
    data_out->header.tbox_identification_type = data_byte[3];
    if(data_out->header.tbox_identification_type == 0)
    {
        tbox_identification_length = 12;
    }
    else
    {
        tbox_identification_length = 16;
    }

    //此处data_byte + 4，这个4为header前四个元素的长度
    memcpy(data_out->header.tbox_sn, data_byte+4, tbox_identification_length);

    //此处为header前四个元素的长度 + tbox_identification_length + random(16)
    int header_length = 4 + tbox_identification_length;
    memcpy(dispatcher_body, data_byte + header_length + RANDOM_LENGTH, data_byte_len - header_length - RANDOM_LENGTH);
    dispatcher_body_len = data_byte_len - header_length - RANDOM_LENGTH;

    //aes_decrypt_data的开始位置为random的开始位置，去掉了header
    aes_decrypt_data_len = aes_cbc_128_decrypt(data_byte + header_length, data_byte_len - header_length, key, iv, aes_decrypt_data);
    if(aes_decrypt_data_len == -1)
    {
        printf("aes_cbc_128_decrypt error\n");
        return -1;
    }

    unsigned char random[RANDOM_LENGTH] = {0x0};
    memcpy(random, aes_decrypt_data, RANDOM_LENGTH);
    
    //asn1解码dispatch_body, dispatcher_body_len为asn1的长度
    int body_length = data_out->header.dispatcher_message_length - header_length;
    asn_dec_rval_t  rval = uper_decode(0, &asn_DEF_DispatcherBody, (void **)&data_out->body, aes_decrypt_data + RANDOM_LENGTH, body_length,0,0);
    if (rval.code != RC_OK) {
        fprintf(stderr, "uper_decode() failed: %d\n", rval.code);
        return -1;
    }

    data_out->application_data = (unsigned char *)malloc(*(data_out->body->applicationDataLength));
    if (data_out->application_data == NULL)
    {
        printf("malloc error\n");
        return -1;
    }
    
    memcpy(data_out->application_data, aes_decrypt_data + RANDOM_LENGTH + body_length, *(data_out->body->applicationDataLength));

    return RET_OK;
}