#include "tsp_event.h"
#include "tsp_event_list.h"
#include "tsp_message_protocol.h"
#include "error_code.h"
#include "log.h"

#include <time.h>
#include <stdio.h>

#define MAX_EVENT_SIZE 20

int CreateEvent(T_TspEvent *event, T_TspMessage *data, tx_func tx_func, rx_func rx_func)
{
    T_TspEvent event_lists[MAX_EVENT_SIZE];
    int event_lists_size = 0;
    if(event == NULL || data == NULL || tx_func == NULL || rx_func == NULL)
    {
        LOG_Error("CreateEvent input failed\n");
        return RET_ERR;
    }

    event_lists_size = GetEventLists(event_lists, MAX_EVENT_SIZE);
    if(event_lists_size == -1)
    {
        LOG_Error("GetEventLists failed\n");
        return RET_ERR;
    }

    for(int i=0; i < event_lists_size ;i++)
    {
        if(atoi(data->body->applicationId.buf) == event_lists[i].event_aid && data->body->messageId == event_lists[i].event_mid)
        {
            event->event_aid = event_lists[i].event_aid;
            event->event_mid = event_lists[i].event_mid;
            event->task = event_lists[i].task;
            event->send = tx_func;
            event->recv = rx_func;
            return RET_OK;
        }
    }

    return RET_ERR;
}

int RunEventTask(T_TspEvent *event, T_TspMessage *data)
{
    if(event == NULL || data == NULL)
    {
        return RET_ERR;
    }

    event->result = event->task(event, data);

    return event->result;
}

int DistoryEvent(T_TspEvent *event)
{
    return RET_OK;
}