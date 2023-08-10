#ifndef _HTTP_INTERFACE_H_
#define _HTTP_INTERFACE_H_

#include <curl/curl.h>

#define ROOT_URL_DEVELOP     "http://61.169.5.67:32012/api.tbox/v1"
#define ROOT_URL_TEST        "http://36.152.36.6:8093/api.tbox/v1"
#define ROOT_URL_PRE_RELEASE "http://tgateway.soimt.com/api.tbox/v1"
#define ROOT_URL_RELEASE     "http://gateway.soimt.com/api.tbox/v1"

#define GET_IOT_TYPE           "/message/iotDevice/iotType/"
#define POST_IOT_REGISTER      "/message/iotDevice/registration/"
#define POST_BIND_SIM_CARD     "/vehicle/simCard/"
#define POST_VERIFY_SIM_CARD   "/vehicle/simCard/verification/"
#define POST_IPV6_INFO_UPLOAD  "/vehicle/simCard/ipv6/"
#define POST_TBOX_INFO_UPLOAD  "/vehicle/tbox/"
#define POST_COMMON_CONFIG     "/vehicle/commonConfig/"
#define POST_LOG_UPLOAD_CONFIG "/vehicle/tbox/logUploadConfig/"

typedef enum {
    HTTP_OK = 200,
    HTTP_CREATED = 201,
    HTTP_NO_CONTENT = 204,
    HTTP_INVALID_REQUEST = 400,
    HTTP_NOT_FOUND = 404,
    HTTP_INTERNAL_SERVER_ERROR = 500,
} http_code_e;

typedef struct {
    http_code_e code;
    char message[512];
    char data[512];
} http_response_t;



int http_post(curl_slist *headers, const char *url, const char *data, char *data_out);

int http_get(curl_slist *headers, const char *url, char *data_out);

#endif
