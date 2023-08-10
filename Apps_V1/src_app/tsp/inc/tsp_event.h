#ifndef _TSP_EVENT_H_
#define _TSP_EVENT_H_

// #include "tsp_connect.h"
#include "tsp_event_type.h"

typedef int (*tx_func)(T_TspMessage* data);
typedef int (*rx_func)(T_TspMessage* data, int timeout_ms);

int CreateEvent(T_TspEvent *event, T_TspMessage *data, tx_func tx_func, rx_func rx_func);

int RunEventTask(T_TspEvent *event, T_TspMessage *data);

int DistoryEvent(T_TspEvent *event);


#endif