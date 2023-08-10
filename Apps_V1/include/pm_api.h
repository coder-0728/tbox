/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-09 10:31:08
 * @LastEditTime: 2019-10-09 10:22:20
 * @LastEditors: Please set LastEditors
 */
/**
 * @file pm_api.h
 * @author lujiefeng
 * @brief 
 * @version 0.1
 * @date 2019-02-21
 * 
 * @copyright Copyright (SiRun) 2019
 * 
 */

#ifndef _PM_API_H
#define _PM_API_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "pm_conf.h"
#include <glib.h>

typedef void (*PmStateChangedCBack)(guint8 state, guint8 wakeupSource, gint sleepAlarmEventId);
typedef void (*PmGsensorWakeUpCBack)(void);
/**
 * @brief 
 * 
 * @param[in]	modelName	
 * @retval	    RET_OK  Success
 *              RET_ERR Failed  
 */
int InitPmClient(const gchar *moduleName);

/**
* @brief wakeup system RPC (made for platform)
*
*@param wakeupSource  wakeup source
*
* @return RET_ERR failure
*         RET_OK  successful
*/
gint Wakeup(gint wakeupSource);


/**
* @brief get mcu heartbeat status 
*
* @return 
*   0:
*/
gint GetMcuHeartbeatStatus();

/**
 * @brief Get the Pm State object
 * 
 * @retval	RET_ERR     Error
 *          PM_STATE_SLEEP   Sleep
 *          PM_STATE_NORMAL   normal
 */
gint GetPmState();

/**
 * @brief Get the Pm wakeup source
 * 
 * @retval	RET_ERR     Error
 *          WakeUpSource
 */
gint GetPmWakeUpSource();

/**
 * @brief   Set SleepEvent 
 * 
 * @param[in]	sleepEvent	
 * 
 * @retval	RET_ERR     Error
 *          RET_OK      Success
 */
gint SetSleepEvent(const gchar *sleepEvent);

/**
 * @brief Clear SleepEvent
 * 
 * @param[in]	sleepEvent	
 * @retval	RET_ERR     Error
 *          RET_OK      Success
 */
gint CleanSleepEvent(const gchar *sleepEvent);

/**
 * @brief    Register Recv Pm state change call back   
 * 
 * @param[in]	call back func	
 * @retval	    RET_OK  Success
 *              RET_ERR Failed
 */
gint RegisterPmStateChangeCBack(PmStateChangedCBack func);

/**
 * @brief Create a Sleep Alarm object
 * 
 * @param[in]	sleepAlarmTimeOutSec	
 * @param[in]	sleepAlarmEventId	
 * @retval	    RET_OK Success 
 *              RET_ERR Failed
 */
gint CreateSleepAlarm(gint sleepAlarmTimeOutSec, gint sleepAlarmEventId);

/**
 * @brief   Delete SleepAlarm by timeId
 * 
 * @param[in]	timeId	    result of CreateSleepAlarm
 * @retval	    RET_OK  Success
 *              RET_ERR Failed
 */
void DeleteSleepAlarm(gint sleepAlarmEventId);
/**
 * @brief 
 * 
 * @param func 
 * @return gint 
 */
gint RegisterPmGsensorWakeUpCBack(PmGsensorWakeUpCBack func);
#ifdef __cplusplus
}
#endif
#endif
