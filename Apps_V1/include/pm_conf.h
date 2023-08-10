/**
 * @file pm_conf.h
 * @author lujiefeng
 * @brief 
 * @version 0.1
 * @date 2019-02-22
 * 
 * @copyright Copyright (SiRun) 2019
 * 
 */
#ifndef _PM_CONF_H
#define _PM_CONF_H
#ifdef __cplusplus
extern "C"
{
#endif

#define PM_MCU_HEARTBEAT_NORMAL (1)
#define PM_MCU_HEARTBEAT_LOST   (0)

//PM State
/**
 * @brief PM Sleep State 
 * 
 */
#define PM_STATE_SLEEP 1

/**
 * @brief PM normal State 
 * 
 */
#define PM_STATE_NORMAL 2

/**
 * @brief PM initial state before normal state 
 */
#define PM_STATE_INIT 0 

//PM WakeUp Source
#define PM_WAKEUP_SOURCE_NONE 0
#define PM_WAKEUP_SOURCE_MCU 1
#define PM_WAKEUP_SOURCE_TCP 2
#define PM_WAKEUP_SOURCE_RING 3
#define PM_WAKEUP_SOURCE_SMS 4
#define PM_WAKEUP_SOURCE_BT 5
#define PM_WAKEUP_SOURCE_ECALL_BUTTON 6
#define PM_WAKEUP_SOURCE_G_SENSOR 7
#define PM_WAKEUP_SOURCE_RTC 8
#define PM_WAKEUP_SOURCE_RTC_SEND_HEARTBEAT (9)

//Sleep Event,upgrade and ecall will block sleep,must be call SetSleepEvent when upgrade or ecall .
#define PM_SLEEP_EVENT_UPGRADE "upgrade"
#define PM_SLEEP_EVENT_ECALL "ecall"

#ifdef __cplusplus
}
#endif
#endif
