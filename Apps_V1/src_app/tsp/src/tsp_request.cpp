#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "tsp_request.h"
#include "pm_api.h"
#include "param_api.h"
#include "error_code.h"
#include "log.h"
#include "tsp_message_protocol.h"
#include "tsp_event_type.h"
#include "tsp_message_encrypt.h"
#include "tsp_event.h"
#include "tsp_monitor.h"
#include "gnss_api.h"
#include "cJSON.h"
#include "mqtt_conn.h"

#define TIMEOUT 500

static AlertMonitorCb alertMonitorCb = NULL;

void SetAlertMonitorCb(AlertMonitorCb cb)
{
    alertMonitorCb = cb;
}

void getAndSendAlertMsg(uint8_t *alert_id_bitmap, uint8_t *alert_data)
{
    GNRMC *gnssData;
    uint32_t timeStamp = time(NULL);
    GetGpsData(gnssData);

    // 组包
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        LOG_Debug("Failed to create JSON object\n");
        return;
    }

    cJSON_AddNumberToObject(root, "alertTime", timeStamp);

    cJSON *RVS_POSITOIN = cJSON_CreateObject();
    if (RVS_POSITOIN == NULL)
    {
        LOG_Debug("Failed to create JSON object\n");
        cJSON_Delete(root);
        return;
    }
    cJSON_AddItemToObject(root, "gpsPosition", RVS_POSITOIN);

    cJSON *RVS_WAY_POINT = cJSON_CreateObject();
    if (RVS_WAY_POINT == NULL)
    {
        LOG_Debug("Failed to create JSON object\n");
        cJSON_Delete(root);
        cJSON_Delete(RVS_POSITOIN);
        return;
    }
    cJSON_AddItemToObject(RVS_POSITOIN, "wayPoint", RVS_WAY_POINT);

    cJSON *RVS_WGS84_POINT = cJSON_CreateObject();
    if (RVS_WGS84_POINT == NULL)
    {
        LOG_Debug("Failed to create JSON object\n");
        cJSON_Delete(root);
        cJSON_Delete(RVS_POSITOIN);
        cJSON_Delete(RVS_WAY_POINT);
        return;
    }
    cJSON_AddItemToObject(RVS_WAY_POINT, "position", RVS_WGS84_POINT);
    cJSON_AddNumberToObject(RVS_WGS84_POINT, "latitude", gnssData->latitudeValue);
    cJSON_AddNumberToObject(RVS_WGS84_POINT, "longitude", gnssData->longtitudeValue);
    cJSON_AddNumberToObject(RVS_WGS84_POINT, "altitude", gnssData->seaHeight);

    cJSON_AddNumberToObject(RVS_WAY_POINT, "heading", gnssData->direction);
    cJSON_AddNumberToObject(RVS_WAY_POINT, "speed", gnssData->speed);
    cJSON_AddNumberToObject(RVS_WAY_POINT, "hdop", gnssData->epeHorizontal);
    cJSON_AddNumberToObject(RVS_WAY_POINT, "satellites", gnssData->moonLinkNum);

    cJSON_AddNumberToObject(RVS_POSITOIN, "timeStamp", gnssData->utcTime_t);
    cJSON_AddNumberToObject(RVS_POSITOIN, "gpsStatus", gnssData->status);

    cJSON_AddStringToObject(root, "alertIdBitmap", alert_id_bitmap);
    cJSON_AddStringToObject(root, "alertData", alert_data);

    char *jsonStr = cJSON_Print(root);
    // 发送json
    LOG_Debug("JSON: %s\n", jsonStr);

    MqttMessage_t *mqttMessage = (MqttMessage_t *)malloc(sizeof(MqttMessage_t) + strlen(jsonStr));
    if (mqttMessage == NULL)
    {
        LOG_Error("malloc failed\n");
        return RET_ERR;
    }
    memcpy(mqttMessage->payload, jsonStr, strlen(jsonStr));
    mqttMessage->payload_size = strlen(jsonStr);

    MqttSend(mqttMessage);
    free(mqttMessage);
    mqttMessage = NULL;
    cJSON_Delete(root);
    free(jsonStr);
}

void *TspRequestHandler(void *arg)
{
    T_TspMessage *recvData;
    T_TspEvent event;

    recvData = (T_TspMessage *)malloc(sizeof(T_TspMessage));
    if (recvData == NULL)
    {
        LOG_Debug("malloc failed\n");
        return;
    }

    char *topicName;
    int topicLen;
    unsigned long timeout = TIMEOUT;
    int rc;
    int waitCount = 0;
    int maxWaitCount = timeout / 100; // 设置最大等待次数，根据超时时间计算

    SetAlertMonitorCb(getAndSendAlertMsg);

    while (1)
    {
        LOG_Debug("TspRequestHandler ...\n");
        sleep(1);
    }

#if 0
    while (1)
    {

        LOG_Debug("-------------- EVENT START\n");
        unsigned char iv[16] = {0x0};

        MqttMessage_t *mqttMessage = (MqttMessage_t *)malloc(sizeof(MqttMessage_t) + 1024);
        if (mqttMessage == NULL)
        {
            LOG_Error("malloc failed\n");
            return RET_ERR;
        }

        int ret = MqttRecv(mqttMessage, timeout);
        if (ret < 0)
        {
            free(mqttMessage);
            printf("MqttRecv failed\n");
            return RET_ERR;
        }

        cJSON *json = cJSON_Parse(mqttMessage->payload);
        if (json == NULL)
        {
            printf("Error parsing JSON.\n");
            return 1;
        }

        cJSON *messageContent = cJSON_GetObjectItem(json, "messageContent");

        ret = Tsp_Message_Decrypt(messageContent->valuestring, strlen(messageContent->valuestring), (unsigned char *)sec_key, iv, recvData);
        if (ret == RET_ERR)
        {
            LOG_Error("Tsp_Message_Decrypt failed\n");
            free(mqttMessage);
            cJSON_Delete(json);
            return RET_ERR;
        }
        cJSON_Delete(json);

        LOG_Debug("%s\n", mqttMessage->payload);
        free(mqttMessage);

        // ret = CreateEvent(&event, recvData, mqttSendToTsp, mqttMsgSubscribe);
        // if (ret == RET_ERR)
        // {
        //     LOG_Debug("CreateEvent failed\n");
        //     sleep(1);
        //     continue;
        // }

        // ret = RunEventTask(&event, recvData);
        LOG_Debug("-------------- EVENT END ret %d\n", ret);
        sleep(1);
    }
    MQTTClient_disconnect(g_mqttClientInfo->sub_client, 10000);
    MQTTClient_destroy(&g_mqttClientInfo->sub_client);

    free(recvData);
    recvData = NULL;
    cleanMqttClientInfo();
#endif
}