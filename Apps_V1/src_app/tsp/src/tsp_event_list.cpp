#include <stdio.h>
#include "tsp_event_list.h"
#include "tsp_event_secret.h"
#include "tsp_event_veh.h"

const static T_TspEvent g_event_lists[] = {
    { AID_Activation_And_Configuration, MID_NewPubKeyReq, 0, 0, 0, nullptr, nullptr, EventTask_SecretKeyUpdate, 0 },
    { AID_Vehicle_Status, MID_RVMVehicleStatusReq, 0, 0, 0, nullptr, nullptr, EventTask_VehStatus, 0 },
    { AID_Vehicle_Status, MID_RVMGeoPositionSet, 0, 0, 0, nullptr, nullptr, EventTask_GeoPositionSet, 0 },
    { AID_Vehicle_Status, MID_RVMVehicleClassifiedStatusReq, 0, 0, 0, nullptr, nullptr, EventTask_VehClassifiedStatus, 0}
};

int GetEventLists(T_TspEvent *events, int size)
{
    if(events == NULL)
    {
        return -1;
    }

    int length = sizeof(g_event_lists)/sizeof(T_TspEvent);

    if(size < length)
    {
        return -1;
    }

    for(int i=0; i<length; i++)
    {
        events[i].event_aid = g_event_lists[i].event_aid;
        events[i].event_mid = g_event_lists[i].event_mid;
        events[i].task = g_event_lists[i].task;
        events[i].event_creation_time = g_event_lists[i].event_creation_time;
        events[i].event_id = g_event_lists[i].event_id;
        events[i].message_counter_acked = g_event_lists[i].message_counter_acked;
        events[i].send = g_event_lists[i].send;
        events[i].recv = g_event_lists[i].recv;
        events[i].task = g_event_lists[i].task;
        events[i].result = g_event_lists[i].result;
    }

    return length;
}