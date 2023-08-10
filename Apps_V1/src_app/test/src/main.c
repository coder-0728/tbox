#include <stdio.h>
#include <stdint.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>

#include "OtaNewSecKeyReq.h"

typedef struct {
    BIGNUM* modulus;
    BIGNUM* exponent;
} PublicKey;

void hex_to_bin(const char* hex_str, unsigned char* bin_data, size_t bin_len) {
    for (size_t i = 0; i < bin_len; i++) {
        sscanf(&hex_str[i * 2], "%2hhx", &bin_data[i]);
    }
}

PublicKey* create_public_key(const char* modulus_hex, const char* exponent_hex) {
    PublicKey* pub_key = (PublicKey*)malloc(sizeof(PublicKey));

    pub_key->modulus = BN_new();
    BN_hex2bn(&pub_key->modulus, modulus_hex);

    pub_key->exponent = BN_new();
    BN_hex2bn(&pub_key->exponent, exponent_hex);

    return pub_key;
}

void destroy_public_key(PublicKey* pub_key) {
    if (pub_key) {
        if (pub_key->modulus) BN_free(pub_key->modulus);
        if (pub_key->exponent) BN_free(pub_key->exponent);
        free(pub_key);
    }
}
#if 0
void aes_main()
{
    // 给定的 64 字节公钥模数和指数
    const char* modulus_hex = "483755153c26742c1d0212603716712c443766477d0f683b711e661e0c0a401701212e5a10394e4621651b4456723d641b1f435b4648301c574b25164474793b";
    const char* exponent_hex = "010001";

    // 创建 PublicKey 对象
    PublicKey* pub_key = create_public_key(modulus_hex, exponent_hex);

    // 初始化 RSA 结构体
    RSA* rsa = RSA_new();
    rsa->n = pub_key->modulus;
    rsa->e = BN_dup(pub_key->exponent);

    // 要加密的数据
    const char* plaintext = "1234567890123456TESTSN0000000001";
    unsigned char ciphertext[RSA_size(rsa)];

    // 进行 RSA 加密
    RSA_public_encrypt(strlen(plaintext), (const unsigned char*)plaintext, ciphertext, rsa, RSA_NO_PADDING);

    // 输出加密结果（密文）
    printf("Ciphertext: ");
    for (int i = 0; i < RSA_size(rsa); i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    // 释放资源
    RSA_free(rsa);
    destroy_public_key(pub_key);
}
#endif

void asn1_main()
{
    OtaNewSecKeyReq_t ota_new_sec_key_req;
    unsigned char secret_with_sn[64] = {0x0};
    ota_new_sec_key_req.secKeyAndTboxSn.buf = (unsigned char *)malloc(sizeof(unsigned char) * 64);
    if(ota_new_sec_key_req.secKeyAndTboxSn.buf == NULL)
    {
        printf("malloc failed\n");
        return -1;
    }
    ota_new_sec_key_req.secKeyAndTboxSn.size = 64;
    memcpy(ota_new_sec_key_req.secKeyAndTboxSn.buf, secret_with_sn, 64);
    ota_new_sec_key_req.secKeyAndTboxSn.buf = "70a1597f20df1bb6d0f5557f38f6ffbef00000004c38feb6c0e0627f0100000070a1597ffc1fd7b670a1597fc08f567f20df1bb690fa5f7f60e31bb6c03e8fb6";
    unsigned char out[1024] = {0x0};

    //编码application_data（OtaSecPubKeyReq）
    asn_enc_rval_t rval = uper_encode_to_buffer(&asn_DEF_OtaNewSecKeyReq, &ota_new_sec_key_req, out, 1024);
    if (rval.encoded == -1 )
    {
        printf("der_encode_to_buffer failed, %s\n",rval.failed_type->name);
        free(ota_new_sec_key_req.secKeyAndTboxSn.buf);
        ota_new_sec_key_req.secKeyAndTboxSn.buf = NULL;
        return -1;
    }

    printf("OK\n");
    for(int i=0; i<(rval.encoded+7)/8; i++)
    {
        printf("%02X",out[i]);
    }
    printf("\n");
}

int main() {
    
    asn1_main();
    return 0;
}
