#ifndef _TSP_MONITOR_H_
#define _TSP_MONITOR_H_

#include <stdint.h>

typedef void (*AlertMonitorCb)(uint8_t *alert_id_bitmap, uint8_t *alert_data);

void *TspMonitorHandler(void *arg);

#endif