#include "tsp_monitor.h"
#include "mcu_can_msg_mapping.h"
#include "error_code.h"
#include "log.h"
#include "gnss_api.h"
#include "cJSON.h"
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>

static AlertMonitorCb alertMonitorCb = NULL;

//TODO : 删除了原来的mqtt接口，新的接口在mqtt_conn.h中，需要修改
//MqttClientInfo *g_monitor_mqttClientInfo = NULL;

void setBit(uint8_t* arr, int pos, uint8_t bit) {
    int index = pos / 8;      // 获取位所在的字节索引
    int bitPos = pos % 8;     // 获取位在字节中的位置

    arr[index] = (arr[index] & ~(1 << bitPos)) | (bit << bitPos);
}

void *TspMonitorHandler(void *arg)
{
    mcu_vehicle_alert_t mcu_vehicle_alert = {0x0};
    mcu_vehicle_alert_t tmp_mcu_vehicle_alert = {0x0};
    uint8_t alert_id_bitmap[8] = {0x0};
    uint8_t alert_data[64] = {0x0};
    uint8_t tmp_alert_data[64] = {0x0};
    int ret = RET_ERR;
    int alert_flag = 0;

    while(1)
    {
        ret = get_mcu_vehicle_alert(&mcu_vehicle_alert);
        if(ret == RET_ERR)
        {
            LOG_Error("get_mcu_vehicle_alert failed\n");
            sleep(1);
            continue;
        }

        memcpy(alert_data, &mcu_vehicle_alert, sizeof(mcu_vehicle_alert_t));
        memcpy(tmp_alert_data, &tmp_mcu_vehicle_alert, sizeof(mcu_vehicle_alert_t));
        
        for(int i=0; i<sizeof(mcu_vehicle_alert_t)/sizeof(uint8_t); i++)
        {
            //状态发生跳变
            if(alert_data[i] != tmp_alert_data[i])
            {
                setBit(alert_id_bitmap, i, 1);
                alert_flag++;
            }
        }

        //这里调用回调函数
        if(alert_flag > 0)
        {
            if(alertMonitorCb != NULL)
            {
                alertMonitorCb(alert_id_bitmap, alert_data);
            }

        }

        alert_flag = 0;
        memcpy(tmp_alert_data, alert_data, 64);
    }
    
}