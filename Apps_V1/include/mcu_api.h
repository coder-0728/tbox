/**
 * @file mcu_api.h
 * @author lujiefeng
 * @brief 
 * @version 0.1
 * @date 2018-12-06
 * 
 * @copyright Copyright (SiRun) 2018
 * 
 */
#ifndef _MCU_API_H
#define _MCU_API_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "mcu_msg.h"

/**
* @brief Init Mcu Msg Queue
* 
* @retval	RET_ERR:error
*          > 0 :msg queue keyid
*/
gint InitMcuMsg();


gint DestroyMcuMsg(int keyId);
    /**
     * @brief 
     * 
     * @param[in]	subMsg	
     * @retval	RET_ERR:error
     *          RET_OK : OK
     */
    gint SubscriptionMcuMsg(guint32 keyId, MsgQueueType type);

    /**
     * @brief 
     * 
     * @param[in]	subMsg	
     * @retval	RET_ERR:error
     *          RET_OK : OK
     */
    gint UnSubscriptionMcuMsg(guint32 keyId, MsgQueueType type);

    /**
     * @brief Get the Mcu Msg object
     * 
     * @param[in]	keyId	
     * @param[in]	msgData	
     * @param[in]	timeOut	-1 block,0 once,>0 timeout second
     * @retval	    RET_ERR:error
     *              >0 : OK
     */
    gint GetMcuMsg(gint keyId, MsgQueueData *msgData, gint timeOut);

    /**
     * @brief 
     * 
     * @param[in]	type	
     * @param[in]	dataLen	
     * @param[in]	subCmd	
     * @param[in]	data	
     * @retval	    RET_ERR:error
     *              RET_OK : OK
     */
    gint SendMcuMsg(MsgQueueType type, gshort dataLen, guchar subCmd, void *data);

#ifdef __cplusplus
}
#endif
#endif