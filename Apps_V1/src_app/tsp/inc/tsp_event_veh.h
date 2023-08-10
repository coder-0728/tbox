#ifndef _TSP_EVENT_VEH_H_
#define _TSP_EVENT_VEH_H_

#include "tsp_event_type.h"

// Vehicle status AID 113

//TSP向TBOX请求车辆状态信息， MID 1
int EventTask_VehStatus(struct TspEvent* self, T_TspMessage *data);

//TSP向TBOX设置电子围栏的位置和最大距离, MID 4
int EventTask_GeoPositionSet(struct TspEvent* self, T_TspMessage *data);

//TSP向TBOX请求车辆分类状态信息，MID 11
int EventTask_VehClassifiedStatus(struct TspEvent* self, T_TspMessage *data);

#endif