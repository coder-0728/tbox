#ifndef _MQTT_CONN_H_
#define _MQTT_CONN_H_

#include "MQTTClient.h"

#include <pthread.h>

typedef enum
{
    MQTT_NOT_INIT,
    MQTT_INIT,
    MQTT_CONNECTING,
    MQTT_CONNECT_SUCCESS,
    MQTT_CONNECT_FAILURE
}MqttConnectStatus;

typedef struct
{
    char url[128];
    char clientId[128];
    MQTTClient client;
    MqttConnectStatus conn_status;
    pthread_mutex_t mutex;
}MqttConnParams_t;

typedef struct
{
    unsigned char topicName[128];
    int payload_size;
    unsigned char payload[0];
}MqttMessage_t;



int MqttConnToServer();

int MqttRecv(MqttMessage_t *mqttMessage, int timeout_ms);

int MqttSend(MqttMessage_t *mqttMessage);

int MqttDisconn();

int MqttConnStatus();

#endif