#include "http_interface.h"
#include "error_code.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ResponseData {
    char *data;
    size_t size;
};

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) 
{
    ResponseData *response = (ResponseData *)userdata;

    size_t total_size = size * nmemb;
    response->data = realloc(response->data, response->size + total_size + 1);
    if (response->data == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        return 0; // Returning 0 from the write callback will signal libcurl to stop the download.
    }
    memcpy(response->data + response->size, ptr, total_size);
    response->size += total_size;
    response->data[response->size] = '\0'; // Null-terminate the data for safer string operations.
    return total_size;
}

int http_post(curl_slist *headers, const char *url, const char *data, char *data_out) 
{
    if (headers == NULL || url == NULL || data == NULL || data_out == NULL) {
        return RET_ERR;
    }

    CURL *curl = NULL;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

        struct ResponseData response;
        response.data = NULL;
        response.size = 0;

        // 设置超时时间为3秒
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            if(response.data != NULL)
            {
                free(response.data);
                response.data = NULL;
            }
            return RET_ERR;
        }

        curl_easy_cleanup(curl);
        

        memcpy(data_out, response.data, response.size);
        if(response.data != NULL)
        {
            free(response.data);
            response.data = NULL;
        }
        return RET_OK;
    }

    curl_global_cleanup();
    return RET_ERR;
}

int http_get(curl_slist *headers, const char *url, char *data_out) 
{
    if(headers == NULL || url == NULL || data_out == NULL)
    {
        return RET_ERR;
    }

    CURL *curl = NULL;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) 
    {
        LOG_Debug("url %s\n", url);
        curl_easy_setopt(curl, CURLOPT_URL, url);

        struct ResponseData response;
        response.data = (char *)malloc(1024);
        response.size = 0;

        // 设置超时时间为3秒
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            if(response.data != NULL)
            {
                free(response.data);
                response.data = NULL;
            }
            return RET_ERR;
        }
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        memcpy(data_out, response.data, response.size);
        if(response.data != NULL)
        {
            free(response.data);
            response.data = NULL;
        }
        return RET_OK;
    }

    curl_global_cleanup();
    return RET_ERR;
}
