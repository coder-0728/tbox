#ifndef _MCU_CAN_RX_HANDLE_H_
#define _MCU_CAN_RX_HANDLE_H_

#include "upgrade_api.h"

#define GW_CONNECTED_BCM_FRP04  0x2000046A
#define GW_Connected_BCM_FrP09  0x2000021D
#define GW_Connected_TPMS_FrP01 0x2000047D
#define GW_WF_FrP01             0x200001F1
#define GW_Connected_AC_FrP01   0x20000361
#define GW_WF_FrP10             0x200003F1
#define GW_Connected_IPK_FrP03  0x20000416
#define GW_Connected_BMS_FrP11  0x200002A2
#define GW_Connected_TPMS_FrP02 0x2000047E
#define GW_Connect_IPK_FrP09    0x20000417

typedef struct 
{
    PenetrateData data[CONFIG_MAX_LEN];
    int size;
}can_frame_t;

/**
 * @brief 获取CAN透传数据
 * 
 */
int get_can_frame(can_frame_t *can_frame);

/**
 * @brief CAN透传数据接收主函数
*/
void *McuCanRxHandler(void *arg);

#endif