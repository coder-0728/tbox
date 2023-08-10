#include "tsp_activation.h"

#include "log.h"
#include "error_code.h"
#include "param_api.h"
#include "at_api.h"
#include "tbox_uds_service.h"
#include "pm_api.h"
#include "http_interface.h"
#include "cJSON.h"
#include "aes_cbc.h"
#include "mqtt_conn.h"
#include "tsp_message_protocol.h"
#include "tsp_event.h"
#include "asn_pack.h"
#include "tsp_message_encrypt.h"
#include "tools.h"
#include "http_iot_manage.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <openssl/evp.h>
#include <openssl/md5.h>

#define DEFAULT_SECKEY "1234567890123456"
#define SLEEP_EVENT_TSP_ACTIVATION  "tsp_activation"

int iot_regist()
{
    int ret = 0;
    char vin[17+1] = {0};
    char sn[20+1] = {0};
    unsigned char *csr_info = NULL;

    ret = GetParamsValue(enumVin, vin);
    if(RET_OK != ret)
    {
        LOG_Debug("GetParamsValue enumVin failed\n");
        return RET_ERR;
    }

    iot_type_result_t result;
    ret = http_get_iot_type(vin, &result);
    if(ret != RET_OK)
    {
        LOG_Debug("http_get_iot_type failed\n");
        return RET_ERR;
    }

    memset(vin,0x0,sizeof(vin));
    ret = GetParamsValue(enumVin, vin);
    if(RET_OK != ret)
    {
        LOG_Debug("GetParamsValue enumVin failed\n");
        return RET_ERR;
    }


    ret = GetParamsValue(enumTboxSn, sn);
    if(RET_OK != ret)
    {
        LOG_Debug("GetParamsValue enumVin failed\n");
        return RET_ERR;
    }

    
    if(result.isCsrNeeded != 0)
    {
        LOG_Debug("isCsrNeeded is 0, so without csr info\n");
        //csr_info = (unsigned char *)malloc(1024);
        //if(csr_info == NULL)
        //{
        //    LOG_Error("malloc failed\n");
        //    return RET_ERR;
        //}

        //memset(csr_info, 0, 1024);

        //这里填写csr_info
    }

#if 1
    ret = http_post_iot_regist(vin, sn, csr_info);
    LOG_Debug("http_post_iot_regist %d\n",ret);

    if(ret != RET_OK)
    {
        LOG_Debug("http_post_iot_regist failed\n");
        return RET_ERR;
    }
#endif

    return RET_OK;
}

//获取数据库中的激活标志位来判断是否激活
int is_tbox_activated(void)
{
    LOG_Debug("is_tbox_activated\n");
    unsigned char httpActived = 0;
    int ret = GetParamsValue(enumHttpActived, &httpActived);
    if(RET_OK != ret)
    {
        LOG_Debug("GetParamsValue enumVin failed\n");
        return RET_ERR;
    }

    if(httpActived == 1)
    {
        return RET_OK;
    }

    return RET_ERR;
}

/**
 * 是否满足激活条件
 * 1. vin是否写入(params -> vin)
 * 2. sn是否写入(params -> sn)
 * 3. sim是否插入(at -> at+cpin?)
 * 4. 网络是否正常
 * 5. acc_on
 * 6. did b200 处于未激活状态
 *  */
int is_meet_active_conditions(void)
{
    LOG_Debug("is_meet_active_conditions\n");
    char vin[17] = {0};
    char sn[17] = {0};
    char sim_status = 0;
    unsigned char b200_status[128] = {0};
    char pm_state = 0;
    int ret = 0;

    ret = GetParamsValue(enumVin, vin);
    if(RET_OK != ret)
    {
        LOG_Debug("GetParamsValue enumVin failed\n");
        return RET_ERR;
    }

    ret = GetParamsValue(enumTboxSn, sn);
    if(RET_OK != ret)
    {
        LOG_Debug("GetParamsValue enumSn failed\n");
        return RET_ERR;
    }

    /**
     * 通过AT接口获取sim卡状态
     * 应该返回OK，+CPIN: READY
    */
    char out[128] = {0x0,}; 
    ret = SendAT("AT+CPIN?", "+CPIN:", RESPONSE_ONE_LINE, out);
    if(strstr(out, "OK") == NULL)
    {
        LOG_Debug("SendAT AT+CPIN? failed\n");
        return RET_ERR;
    }

    unsigned int b200_length = 0;
    CTboxUdsService uds_service;
    uds_service.Open(0);
    uds_service.ExcuteReadDataByIdentifier(0xB200, b200_status ,&b200_length);
    uds_service.Close();
    if(b200_status != 0)
    {
        LOG_Debug("b200_status is not 0\n");
        return RET_ERR;
    }

    pm_state = GetPmState();
    if(pm_state != PM_STATE_NORMAL)
    {
        LOG_Debug("pm_state is not PM_STATE_NORMAL\n");
        return RET_ERR;
    }

    return RET_OK;
}

int ota_tx_func(T_TspMessage* data)
{
    //获取MQTT连接状态
    int ret = RET_ERR;
    unsigned char topic_name[128] = {0};
    unsigned char iv[16] = {0x0};
    unsigned char tmp_data[1024*2] = {0x0};

    unsigned char sec_key[16] = {0x0};
    GetParamsValue(enumSecKey,sec_key);
    printf("Tx Use Sec Key -> %s\n",sec_key);

    //发送之前的最后一步，按照协议编码
    ret = Tsp_Message_Encrypt(data, (unsigned char *)sec_key, iv, (void *)tmp_data);
    if (ret == RET_ERR)
    {
        LOG_Error("Tsp_Message_Encrypt failed\n");
        return RET_ERR;
    }

    MqttMessage_t *mqttMessage = (MqttMessage_t *)malloc(sizeof(MqttMessage_t) + ret);
    if(mqttMessage == NULL)
    {
        LOG_Error("malloc failed\n");
        return RET_ERR;
    }

    memcpy(mqttMessage->payload, tmp_data, ret);
    mqttMessage->payload_size = ret;

    ret = GetParamsValue(enumMqttOtaUpLink, topic_name);
    if(RET_OK != ret)
    {
        LOG_Debug("GetParamsValue enumMqttOtaUpLink failed\n");
        free(mqttMessage);
        mqttMessage = NULL;
        return RET_ERR;
    }

    memcpy(mqttMessage->topicName, topic_name, strlen(topic_name));

    ret = MqttSend(mqttMessage);
    if(ret != RET_OK)
    {
        LOG_Error("mqttSendToTsp failed\n");
        free(mqttMessage);
        mqttMessage = NULL;
        return RET_ERR;
    }

    free(mqttMessage);
    mqttMessage = NULL;

    return ret;
}

int ota_rx_func(T_TspMessage* data, int timeout_ms)
{
    //获取MQTT连接状态
    int ret = RET_ERR;
    unsigned char data_tmp[1024] = {0};
    unsigned char iv[16] = {0x0};

    MqttMessage_t *mqttMessage = (MqttMessage_t *)malloc(sizeof(MqttMessage_t) + 1024);
    if(mqttMessage == NULL)
    {
        LOG_Error("malloc failed\n");
        return RET_ERR;
    }

    ret = MqttRecv(mqttMessage, timeout_ms);
    if(ret < 0 )
    {
        free(mqttMessage);
        printf("MqttRecv failed\n");
        return RET_ERR;
    }
    //json解析
    cJSON *json = cJSON_Parse(mqttMessage->payload);
    if (json == NULL) {
        free(mqttMessage);
        printf("Error parsing JSON.\n");
        return 1;
    }

    cJSON *messageContent = cJSON_GetObjectItem(json, "messageContent");

    if(strstr(messageContent->valuestring,"f_decode/encode"))
    {
        free(mqttMessage);
        cJSON_Delete(json);
        printf("平台返回了 f_decode/encode\n");
        return RET_ERR;
    }

    unsigned char sec_key[16] = {0x0};
    GetParamsValue(enumSecKey,sec_key);
    printf("Rx Use Sec Key -> %s\n",sec_key);

    //要在这里解析出数据
    ret = Tsp_Message_Decrypt(messageContent->valuestring, strlen(messageContent->valuestring), (unsigned char *)sec_key, iv, data);
    if (ret == RET_ERR)
    {
        LOG_Error("Tsp_Message_Decrypt failed\n");
        free(mqttMessage);
        cJSON_Delete(json);
        return RET_ERR;
    }
    cJSON_Delete(json);

    LOG_Debug("%s\n",mqttMessage->payload);
    free(mqttMessage);
    return RET_OK;
}

int TspActivationHandler()
{
    int ret = 0;
    int pm_state = GetPmState();
    T_TspEvent event;
    T_TspMessage recvData;

    if(pm_state != PM_STATE_NORMAL)
    {
        LOG_Error("pm_state is not PM_STATE_NORMAL\n");
        return RET_ERR;
    }

    SetSleepEvent(SLEEP_EVENT_TSP_ACTIVATION);

    recvData.body = NULL;
    CreateDispatcherBody(&recvData.body);
    CreateApplicationId(&recvData.body->applicationId,"100",3);
    recvData.body->messageId = MID_NewPubKeyReq;
    
    CreateEvent(&event, &recvData, ota_tx_func, ota_rx_func);
    ret = RunEventTask(&event, &recvData);

    //FreeDispatcherBody(recvData.body);

    CleanSleepEvent(SLEEP_EVENT_TSP_ACTIVATION);

    return ret;
}