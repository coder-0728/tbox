/**
 * @file gb_api.h
 * @author lujiefeng
 * @brief 
 * @version 0.1
 * @date 2019-04-11
 * 
 * @copyright Copyright (SiRun) 2019
 * 
 */
#ifndef _GB_API_H
#define _GB_API_H
#include <glib.h>

G_BEGIN_DECLS

typedef void (*GbRecvRealDataCBack)(gint dataLen, const guchar *data);

/**
 * @brief 
 * 
 * @retval	RET_OK  Success
 *          RET_ERR Failed    
 */
gint InitGbRealDataClient(GbRecvRealDataCBack func);

/**
 * @brief 
 * 
 * @retval	RET_OK  Success
 *          RET_ERR Failed    
 */
gint GetKL15AndChargingStatus(gint *kl15status, gint *chargingStatus);

/**
 * @brief   
 * 
 * @retval	ture -- 当前处于三级报警状态
 *          false -- 当前处于非三节报警状态
 */
gboolean IsAlarm();

G_END_DECLS
#endif