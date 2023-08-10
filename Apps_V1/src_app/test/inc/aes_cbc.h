#ifndef _AES_CBC_H_
#define _AES_CBC_H_

int get_aes_ecb_encrypt_length(int input_length);
int aes_ecb_encrypt(const unsigned char *key, const unsigned char *input, int input_length, unsigned char *output);

int aes_cbc_128_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext);
int aes_cbc_128_decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key, unsigned char* iv, unsigned char* plaintext);

int aes_cbc_256_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
// 使用当前时间为种子生成随机数
int create_random_array(int array_size, unsigned char *random_array_out);

#endif