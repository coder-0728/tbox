#include "mqtt_conn.h"
#include "log.h"
#include "error_code.h"
#include "tsp_message_encrypt.h"
#include "cJSON.h"
#include "param_api.h"
#include "pm_api.h"
#include "aes_cbc.h"
#include "tsp_activation.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define CLIENTID "ExampleClientPub"
#define MQTT_TRUSTSTORE_PATH "/oemdata/ca/rootCA"
#define MQTT_KEYSTORE_PATH "/oemdata/ca/certCertificatePem"
#define MQTT_PRIVATE_KEY_PATH "/oemdata/ca/certPrivateKey"

#define TIMEOUT 10000L
#define QOS 0

static void bytesToHexString(const unsigned char* bytes, size_t length, char* hexString) 
{
    for (size_t i = 0; i < length; i++) {
        sprintf(hexString + (i * 2), "%02x", bytes[i]);
    }
}

static MqttConnParams_t g_mqttConnParams = {{0x0}, {0x0}, NULL, MQTT_NOT_INIT, PTHREAD_MUTEX_INITIALIZER};

static int MqttParamsInit()
{
    /** 获取MQTT地址, 获取失败直接返回 **/

    if(g_mqttConnParams.conn_status != MQTT_NOT_INIT)
    {
        LOG_Error("MqttParamsInit failed\n");
        return RET_ERR;
    }

    int rc = RET_ERR;
    char url[128] = {0};
    rc = GetParamsValue(enumMqttUrl,url);
    if(rc != RET_OK)
    {
        LOG_Error("GetParamsValue enumMqttUrl failed\n");
        return RET_ERR;
    }

    /** MQTT地址为默认，返回失败 **/
    if (strstr(url, "0000000000"))
    {
        LOG_Error("enumMqttUrl is Default , run Iot Regist\n");
        rc = iot_regist();
        if(rc != RET_OK)
        {
            LOG_Error("iot_regist failed\n");
            return RET_ERR;
        }

        rc = GetParamsValue(enumMqttUrl,url);
        if(rc != RET_OK)
        {
            LOG_Error("GetParamsValue enumMqttUrl failed\n");
            return RET_ERR;
        }
    }

    pthread_mutex_lock(&g_mqttConnParams.mutex);
    memcpy(g_mqttConnParams.clientId, CLIENTID, strlen(CLIENTID));
    memcpy(g_mqttConnParams.url, url, strlen(url));
    g_mqttConnParams.conn_status = MQTT_INIT;
    pthread_mutex_unlock(&g_mqttConnParams.mutex);
    return RET_OK;
}

int MqttConnToServer()
{
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc = RET_ERR;
    char address[128] = {0};
    char mqtt_topic_name_ota_downlink[128] = {0};
    char mqtt_topic_name_msg_downlink[128] = {0};

    rc = MqttParamsInit();
    if(rc != RET_OK)
    {
        LOG_Error("MqttParamsInit failed\n");
        return RET_ERR;
    }

    /** 检测证书是否存在，没有证书直接返回失败 **/
    if(access(MQTT_TRUSTSTORE_PATH, F_OK) != 0)
    {
        LOG_Error("MQTT_TRUSTSTORE_PATH not exist\n");
        return RET_ERR;
    }

    if(access(MQTT_KEYSTORE_PATH, F_OK) != 0)
    {
        LOG_Error("MQTT_KEYSTORE_PATH not exist\n");
        return RET_ERR;
    }

    if(access(MQTT_PRIVATE_KEY_PATH, F_OK) != 0)
    {
        LOG_Error("MQTT_PRIVATE_KEY_PATH not exist\n");
        return RET_ERR;
    }
    

    /** Load Topic-Type和Topic-Name, 获取失败则直接返回 **/
    rc = GetParamsValue(enumMqttOtaDownLink, mqtt_topic_name_ota_downlink);
    if(rc != RET_OK)
    {
        LOG_Error("GetParamsValue enumMqttOtaDownLink failed\n");
        return RET_ERR;
    }

    rc = GetParamsValue(enumMqttMsgDownLink, mqtt_topic_name_msg_downlink);
    if(rc != RET_OK)
    {
        LOG_Error("GetParamsValue enumMqttMsgDownLink failed\n");
        return RET_ERR;
    }

    if (strstr(mqtt_topic_name_ota_downlink, "0000000000"))
    {
        LOG_Error("enumMqttOtaDownLink is Default , failed\n");
        return RET_ERR;
    }

    if (strstr(mqtt_topic_name_msg_downlink, "0000000000"))
    {
        LOG_Error("enumMqttMsgDownLink is Default , failed\n");
        return RET_ERR;
    }

    pthread_mutex_lock(&g_mqttConnParams.mutex);

    /** 拼接地址为ssl://xxxxx:8883格式 **/
    /** 使用8883端口是使用ssl的缘故 **/
    sprintf(address, "ssl://%s:8883", g_mqttConnParams.url);
    LOG_Debug("MqttConnToServer %s\n", address);

    /** ClientID没有特殊要求，所以随便写一个 **/
    rc = MQTTClient_create(&g_mqttConnParams.client, address, CLIENTID,MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if(rc != MQTTCLIENT_SUCCESS)
    {
        LOG_Error("Failed to create pub_client, return code %d\n", rc);
        pthread_mutex_unlock(&g_mqttConnParams.mutex);
        return RET_ERR;
    }

    /** 设置连接选项 **/
    /** 设置TLS选项，包括根证书、设备证书和设备私钥 **/
    MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
    ssl_opts.enableServerCertAuth = true; 
    ssl_opts.trustStore = MQTT_TRUSTSTORE_PATH;
    ssl_opts.keyStore   = MQTT_KEYSTORE_PATH;
    ssl_opts.privateKey = MQTT_PRIVATE_KEY_PATH;
    
    /** 将TLS选项传递给连接选项 **/
    conn_opts.ssl = &ssl_opts; 
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    g_mqttConnParams.conn_status = MQTT_CONNECTING;
    LOG_Debug("MQTT Start Connect To Server ...\n");
    rc = MQTTClient_connect(g_mqttConnParams.client, &conn_opts);
    if (rc != MQTTCLIENT_SUCCESS)
    {
        g_mqttConnParams.conn_status = MQTT_CONNECT_FAILURE;
        pthread_mutex_unlock(&g_mqttConnParams.mutex);
        LOG_Error("MQTT Failed to connect, return code %d\n", rc);
        return RET_ERR;
    }

    g_mqttConnParams.conn_status = MQTT_CONNECT_SUCCESS;
    LOG_Debug("MQTT Connect To Server Success\n");

    /** 订阅TOPIC **/
    rc = MQTTClient_subscribe(g_mqttConnParams.client, mqtt_topic_name_ota_downlink, 0);
    if (rc != MQTTCLIENT_SUCCESS)
    {
        LOG_Error("Failed to subscribe, return code %d\n", rc);
        MQTTClient_disconnect(g_mqttConnParams.client, 10000);
        pthread_mutex_unlock(&g_mqttConnParams.mutex);
        return RET_ERR;
    }
            
    LOG_Debug("Subscribe %s\n",mqtt_topic_name_ota_downlink);

    rc = MQTTClient_subscribe(g_mqttConnParams.client, mqtt_topic_name_msg_downlink, 0);
    if (rc != MQTTCLIENT_SUCCESS)
    {
        LOG_Error("Failed to subscribe, return code %d\n", rc);
        //TODO :对句柄的操作要上锁
        MQTTClient_disconnect(g_mqttConnParams.client, 0);
        MQTTClient_destroy(g_mqttConnParams.client);
        g_mqttConnParams.client = NULL;
        pthread_mutex_unlock(&g_mqttConnParams.mutex);
        return RET_ERR;
    }

    LOG_Debug("Subscribe %s\n",mqtt_topic_name_msg_downlink);
    pthread_mutex_unlock(&g_mqttConnParams.mutex);

    return RET_OK;
}

int MqttRecv(MqttMessage_t *mqttMessage, int timeout_ms)
{
    if(NULL == mqttMessage)
    {
        return RET_ERR;
    }
    sleep(3);
    MQTTClient_message* server_msg = NULL;
    char* topicName = NULL;
    int topicLen;

    pthread_mutex_lock(&g_mqttConnParams.mutex);
    if(NULL != g_mqttConnParams.client)
    {
        if(MQTTCLIENT_SUCCESS != MQTTClient_receive(g_mqttConnParams.client, &topicName, &topicLen, &server_msg, timeout_ms))
        {
            printf("MQTTClient_receive failed\n");
            return RET_ERR;
        }

        if (topicName)
        {
            //休眠状态下收到消息，唤醒TBOX，唤醒预源为网络唤醒
            if(PM_STATE_SLEEP == GetPmState())
            {
                Wakeup(PM_WAKEUP_SOURCE_TCP);
            }

            printf("topic:%s,payload:%s,payloadlen :%d\n", topicName, server_msg->payload, server_msg->payloadlen);
            mqttMessage->payload_size = server_msg->payloadlen;
            memcpy(mqttMessage->topicName, topicName, strlen(topicName));
            memcpy(mqttMessage->payload, server_msg->payload, server_msg->payloadlen);

            MQTTClient_free(topicName);
            MQTTClient_freeMessage(&server_msg);
            topicName = NULL;
            server_msg = NULL;

            pthread_mutex_unlock(&g_mqttConnParams.mutex);
            return RET_OK;
        }
    }
    else
    {
        LOG_Error("g_mqttConnParams.client Is NULL");
    }

    pthread_mutex_unlock(&g_mqttConnParams.mutex);
    return RET_ERR;
}

int MqttSend(MqttMessage_t *mqttMessage)
{
    if(NULL == mqttMessage || NULL == g_mqttConnParams.client)
    {
        return RET_ERR;
    }

    int rc = RET_ERR;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    uint32_t timeStamp = time(NULL);
    char buffer[20] = {0x0};
    unsigned char iv[16] = {0x0};

    sprintf(buffer, "%d", timeStamp);


    // 组包json
    cJSON* root = cJSON_CreateObject();
    if (root == NULL) {
        LOG_Debug("Failed to create JSON object\n");
        return RET_ERR;
    }
    cJSON_AddStringToObject(root, "messageContent", mqttMessage->payload);
    cJSON_AddStringToObject(root, "messageId", buffer);
    cJSON_AddNumberToObject(root, "messageTime", timeStamp);
    cJSON_AddStringToObject(root, "messageType", "OTA_MESSAGE");


    char* jsonStr = cJSON_Print(root);
    pubmsg.payload = jsonStr;
    pubmsg.payloadlen = (int)strlen((char *)jsonStr);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    pthread_mutex_lock(&g_mqttConnParams.mutex);
    rc = MQTTClient_publishMessage(g_mqttConnParams.client, mqttMessage->topicName, &pubmsg, &token);
    if (rc != MQTTCLIENT_SUCCESS)
    {
        LOG_Debug("Failed to publish message, return code %d\n", rc);
        pthread_mutex_unlock(&g_mqttConnParams.mutex);
        cJSON_Delete(root);
        free(jsonStr);
        root = NULL;
        jsonStr = NULL;
        return -1;
    }

    printf("Waiting for up to %d seconds for publication of %s\n"
           "on topic %s for pub_client with ClientID: %s\n",
           (int)(TIMEOUT / 1000), jsonStr, mqttMessage->topicName, CLIENTID);
    MQTTClient_waitForCompletion(g_mqttConnParams.client, token, TIMEOUT);
    LOG_Debug("Message with delivery token %d delivered\n", token);

    cJSON_Delete(root);
    free(jsonStr);
    root = NULL;
    jsonStr = NULL;

    pthread_mutex_unlock(&g_mqttConnParams.mutex);
    return RET_OK;
}

int MqttDisconn()
{
    pthread_mutex_lock(&g_mqttConnParams.mutex);
    if(NULL != g_mqttConnParams.client)
    {
        MQTTClient_disconnect(g_mqttConnParams.client, 0);
        MQTTClient_destroy(g_mqttConnParams.client);
        g_mqttConnParams.client = NULL;
    }
    pthread_mutex_unlock(&g_mqttConnParams.mutex);
    return RET_OK;
}

int MqttConnStatus()
{
    pthread_mutex_lock(&g_mqttConnParams.mutex);
    int status = g_mqttConnParams.conn_status;
    pthread_mutex_unlock(&g_mqttConnParams.mutex);
    return status;
}