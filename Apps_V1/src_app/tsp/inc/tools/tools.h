#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <unistd.h>
#include <stdlib.h>

/**
 * byte数组转为String
*/
void bytesToHexString(const unsigned char* bytes, int length, char* hexString);

/**
 * MD5哈希
*/
void md5_hash(const unsigned char *data, size_t data_length, unsigned char *hash);

/**
 * HMAC-SHA256加密
*/
void hmac_sha256(const unsigned char *key, size_t key_length, const unsigned char *data, size_t data_length, unsigned char *hmac);

/**
 * SHA256加密
*/
int sha256_encrypt(const char *data_in, int data_in_length, unsigned char *data_out);

/**
 * hexChar转int
*/
int hexCharToInt(char c);

/**
 * hexString转byte数组
*/
int hexStringToBytes(const char* hexString, size_t hexStringLength, unsigned char* bytes);

/**
 * 写入到文件
*/
void writeToFile(const char* filepath, const char* content);

// 生成16字节的AES密钥
void generateAESKey(unsigned char *key);

void get_format_time_ms(char *str_time);

#endif