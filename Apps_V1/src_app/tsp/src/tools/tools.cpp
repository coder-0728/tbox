#include "tools.h"
#include "log.h"
#include "error_code.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdio.h>
#include <openssl/md5.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/aes.h>

void bytesToHexString(const unsigned char* bytes, int length, char* hexString) 
{
    for (int i = 0; i < length; i++) {
        sprintf(hexString + (i * 2), "%02x", bytes[i]);
    }
}

void md5_hash(const unsigned char *data, size_t data_length, unsigned char *hash) 
{
    if( NULL == data || data_length <= 0 || NULL == hash )
    {
        LOG_Error("md5_hash error");
        return;
    }
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, data, data_length);
    MD5_Final(hash, &ctx);
}

void hmac_sha256(const unsigned char *key, size_t key_length, const unsigned char *data, size_t data_length, unsigned char *hmac) 
{
    if( NULL == key || key_length <= 0 || NULL == data || data_length <= 0 || NULL == hmac )
    {
        LOG_Error("hmac_sha256 error");
        return;
    }

    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);
    HMAC_Init_ex(&ctx, key, key_length, EVP_sha256(), NULL);
    HMAC_Update(&ctx, data, data_length);
    unsigned int hmac_length;
    HMAC_Final(&ctx, hmac, &hmac_length);
    HMAC_CTX_cleanup(&ctx);
}

int sha256_encrypt(const char *data_in, int data_in_length, unsigned char *data_out) {
#if 0    
    SHA256_CTX ctx;
    if (!SHA256_Init(&ctx)) {
        fprintf(stderr, "SHA256_Init failed\n");
        return RET_ERR;
    }

    if (!SHA256_Update(&ctx, data_in, data_in_length)) {
        fprintf(stderr, "SHA256_Update failed\n");
        return RET_ERR;
    }

    if (!SHA256_Final(data_out, &ctx)) {
        fprintf(stderr, "SHA256_Final failed\n");
        return RET_ERR;
    }
#endif

    SHA256(data_in, data_in_length, data_out);
    return RET_OK;
}

int hexCharToInt(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    else if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    else
        return -1; // 非法的十六进制字符
}

int hexStringToBytes(const char* hexString, size_t hexStringLength, unsigned char* bytes) 
{
    size_t numBytes = hexStringLength / 2;
    for (size_t i = 0; i < numBytes; i++) {
        int highNibble = hexCharToInt(hexString[i * 2]);
        int lowNibble = hexCharToInt(hexString[i * 2 + 1]);

        if (highNibble == -1 || lowNibble == -1) {
            // 非法的十六进制字符，处理错误情况或者抛出异常
            // 这里我们简单地返回-1表示失败
            return -1;
        } else {
            bytes[i] = (highNibble << 4) | lowNibble;
        }
    }

    return numBytes; // 返回转换后的字节数据的长度
}

void writeToFile(const char* filepath, const char* content) 
{
    FILE* file = fopen(filepath, "w");
    if (file == NULL) {
        LOG_Error("Failed to open file: %s\n", filepath);
        return;
    }

    size_t len = strlen(content);
    if (fwrite(content, 1, len, file) != len) {
        LOG_Error("Failed to write to file: %s\n", filepath);
    }

    fclose(file);
}

// 生成16字节的AES密钥
void generateAESKey(unsigned char *key) {
    srand(time(NULL)); // 使用当前时间作为随机数生成器的种子

    for (int i = 0; i < 8; i++) {
        key[i] = rand() % 256; // 生成0到255之间的随机数
    }
}

void get_format_time_ms(char *str_time) { 
    struct timeval tv;
    gettimeofday(&tv, NULL);

    long milliseconds = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);

    snprintf(str_time, 32, "%ld", milliseconds);
}