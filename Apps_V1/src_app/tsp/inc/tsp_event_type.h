#ifndef _TSP_EVENT_TYPE_H_
#define _TSP_EVENT_TYPE_H_

#include "tsp_message_protocol.h"

typedef struct TspEvent{
    char event_aid;
    unsigned char event_mid;
    unsigned int event_creation_time;
    unsigned int event_id;
    unsigned short message_counter_acked;
    int (*send)(T_TspMessage *data);
    int (*recv)(T_TspMessage *data, int timeout_ms);
    int (*task)(struct TspEvent* self, T_TspMessage *data);
    unsigned short result;
}T_TspEvent;

#endif