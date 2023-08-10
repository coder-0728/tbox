#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <time.h>
#include "aes_cbc.h"

#define AES_KEY_LENGTH 128
#define AES_BLOCK_SIZE 16

// PKCS7 Padding function
void pkcs7_padding(unsigned char *data, int data_len, int block_size) {
    int padding_value = block_size - (data_len % block_size);
    for (int i = 0; i < padding_value; i++) {
        data[data_len + i] = (unsigned char)padding_value;
    }
}

int get_aes_ecb_encrypt_length(int input_length)
{
    return ((input_length + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
}

int aes_ecb_encrypt(const unsigned char *key, const unsigned char *input, int input_length, unsigned char *output)
{
    if(NULL == key || NULL == input || input_length <= 0 || NULL == output)
    {
        printf("aes_ecb_encrypt input failed");
        return -1;
    }

    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
        return -1;
    }

    // Calculate the number of blocks needed
    int num_blocks = (input_length + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;
    int padded_length = num_blocks * AES_BLOCK_SIZE;

    unsigned char *padded_input = (unsigned char *)malloc(padded_length);
    if (padded_input == NULL) 
    {
        return -1;
    }

    // Copy input and perform PKCS7 padding
    memcpy(padded_input, input, input_length);
    unsigned char padding_value = padded_length - input_length;
    memset(padded_input + input_length, padding_value, padded_length - input_length);

    // Encrypt each block
    for (size_t i = 0; i < num_blocks; i++) 
    {
        AES_encrypt(padded_input + (i * AES_BLOCK_SIZE), output + (i * AES_BLOCK_SIZE), &aes_key);
    }

    free(padded_input);
    return get_aes_ecb_encrypt_length(input_length);
}

int aes_cbc_128_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext)
{
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, AES_KEY_LENGTH, &aes_key) < 0) {
        fprintf(stderr, "AES_set_encrypt_key() failed\n");
        return -1;
    }

    // Calculate the number of blocks required for padding
    int num_blocks = (plaintext_len + AES_BLOCK_SIZE ) / AES_BLOCK_SIZE;
    int padded_len = num_blocks * AES_BLOCK_SIZE;

    // Allocate memory for padded plaintext
    unsigned char *padded_plaintext = (unsigned char *)malloc(padded_len);
    if (!padded_plaintext) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    // Copy the original plaintext
    memcpy(padded_plaintext, plaintext, plaintext_len);

    // Apply PKCS7 padding
    pkcs7_padding(padded_plaintext, plaintext_len, AES_BLOCK_SIZE);

    // Initialize previous ciphertext block with IV
    unsigned char prev_ciphertext[AES_BLOCK_SIZE];
    memcpy(prev_ciphertext, iv, AES_BLOCK_SIZE);

    int ciphertext_len = 0;
    for (int i = 0; i < padded_len; i += AES_BLOCK_SIZE) {
        unsigned char block[AES_BLOCK_SIZE];
        memcpy(block, padded_plaintext + i, AES_BLOCK_SIZE);

        for (int j = 0; j < AES_BLOCK_SIZE; j++) {
            block[j] ^= prev_ciphertext[j];
        }

        AES_encrypt(block, ciphertext + i, &aes_key);
        memcpy(prev_ciphertext, ciphertext + i, AES_BLOCK_SIZE);
        ciphertext_len += AES_BLOCK_SIZE;
    }

    if(padded_plaintext != NULL)
    {
        free(padded_plaintext);
        padded_plaintext = NULL;
    }
    

    return ciphertext_len;
}

int aes_cbc_128_decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key, unsigned char* iv, unsigned char* plaintext)
{
    AES_KEY aes_key;
    if (AES_set_decrypt_key(key, AES_KEY_LENGTH, &aes_key) < 0) 
    {
        fprintf(stderr, "AES_set_decrypt_key() failed\n");
        return -1;
    }

    int plaintext_len = 0;
    unsigned char prev_ciphertext[AES_BLOCK_SIZE+1] = {0x0};
    memcpy(prev_ciphertext, iv, AES_BLOCK_SIZE);

    for (int i = 0; i < ciphertext_len; i += AES_BLOCK_SIZE) 
    {
        unsigned char block[AES_BLOCK_SIZE];
        memcpy(block, ciphertext + i, AES_BLOCK_SIZE);

        AES_decrypt(block, plaintext + i, &aes_key);

        for (int j = 0; j < AES_BLOCK_SIZE; j++) 
        {
            plaintext[i + j] ^= prev_ciphertext[j];
        }

        memcpy(prev_ciphertext, block, AES_BLOCK_SIZE);
        plaintext_len += AES_BLOCK_SIZE;
    }

    // PKCS7 Padding removal
    int padding_len = plaintext[plaintext_len - 1];
    if (padding_len > 0 && padding_len <= AES_BLOCK_SIZE) 
    {
        plaintext_len -= padding_len;

        // Fill the padding bytes with the value of padding_len
        for (int i = ciphertext_len - padding_len; i < ciphertext_len; i++)
        {
            if (plaintext[i] != padding_len)
            {
                fprintf(stderr, "Invalid PKCS7 padding\n");
                return -1;
            }
        }
    }
    else
    {
        fprintf(stderr, "Invalid PKCS7 padding\n");
        return -1;
    }

    return plaintext_len;
}

int aes_cbc_256_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) 
{
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, 256, &aes_key) < 0) {
        fprintf(stderr, "AES_set_encrypt_key() failed\n");
        return -1;
    }

    // Calculate the number of blocks required for padding
    int num_blocks = (plaintext_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;
    int padded_len = num_blocks * AES_BLOCK_SIZE;

    // Allocate memory for padded plaintext
    unsigned char *padded_plaintext = (unsigned char *)malloc(padded_len);
    if (!padded_plaintext) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    // Copy the original plaintext and add PKCS7 padding
    memcpy(padded_plaintext, plaintext, plaintext_len);
    int padding_value = AES_BLOCK_SIZE - (plaintext_len % AES_BLOCK_SIZE);
    memset(padded_plaintext + plaintext_len, (unsigned char)padding_value, padding_value);

    // Initialize previous ciphertext block with IV
    unsigned char prev_ciphertext[AES_BLOCK_SIZE];
    memcpy(prev_ciphertext, iv, AES_BLOCK_SIZE);

    int ciphertext_len = 0;
    for (int i = 0; i < padded_len; i += AES_BLOCK_SIZE) {
        unsigned char block[AES_BLOCK_SIZE];
        memcpy(block, padded_plaintext + i, AES_BLOCK_SIZE);

        for (int j = 0; j < AES_BLOCK_SIZE; j++) {
            block[j] ^= prev_ciphertext[j];
        }

        AES_encrypt(block, ciphertext + i, &aes_key);
        memcpy(prev_ciphertext, ciphertext + i, AES_BLOCK_SIZE);
        ciphertext_len += AES_BLOCK_SIZE;
    }

    free(padded_plaintext);

    return ciphertext_len;
}

int create_random_array(int array_size, unsigned char *random_array_out)
{
    if(random_array_out == NULL)
    {
        return -1;
    }

    srand(time(NULL));

    for (int i = 0; i < array_size; i++) {
        random_array_out[i] = rand() % 256;
    }

    return 0;
}