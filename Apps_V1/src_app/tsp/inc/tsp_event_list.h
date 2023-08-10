#ifndef _TSP_EVENT_LIST_H_
#define _TSP_EVENT_LIST_H_

#include "tsp_event_type.h"

/**
* 获取Events列表,如果size 小于Evtens列表，则返回失败(-1)
*/
int GetEventLists(T_TspEvent *events, int size);

#endif