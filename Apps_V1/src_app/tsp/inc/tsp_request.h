#ifndef _TSP_REQUEST_H_
#define _TSP_REQUEST_H_
#include <stdint.h>

typedef void (*AlertMonitorCb)(uint8_t *alert_id_bitmap, uint8_t *alert_data);
void SetAlertMonitorCb(AlertMonitorCb cb);
void getAndSendAlertMsg(uint8_t *alert_id_bitmap, uint8_t *alert_data);
void *TspRequestHandler(void *arg);

#endif