#ifndef _HTTP_IOT_MANAGE_H_
#define _HTTP_IOT_MANAGE_H_

#include <stdio.h>

typedef struct {
    int code;
    int isCsrNeeded;
    char vin[36];
    int iotType;
}iot_type_result_t;

int http_get_iot_type(const char *vin, iot_type_result_t *result);

/**
 * IOT注册请求
 * 
 * 传给TSP的参数为
 * SHA256加密的VIN OK
 * AES加密的SN(KEY为VIN明文的MD5)
 * HMAC-SHA256 加密的sign（vin明文+sn明文+TBOX-SEND-DATE,使用key SN明文的MD5
 * csr 证书签发请求
*/
int http_post_iot_regist(const unsigned char *vin, unsigned char *sn,unsigned char *csr = NULL);

#endif // _HTTP_IOT_MANAGE_H_