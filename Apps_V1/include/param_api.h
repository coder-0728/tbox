/**
 * @file param_api.h
 * @author lujiefeng
 * @brief 
 * @version 0.1
 * @date 2018-11-28
 * 
 * @copyright Copyright (SiRun) 2018
 * 
 */

#ifndef __PARAM_API_H__
#define __PARAM_API_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <glib.h>
#include "param_msg.h"

/**
 * @brief 
 * 
 * @return gint 
 */
gint InitParamsInterface(void);
/**
 * @brief Get the Params Value object
 * 
 * @param paramidx 
 * @param paramValue 
 * @return gint 
 */
gint GetParamsValue(DeviceParamEnum paramidx, gpointer paramValue);
/**
 * @brief Set the Params Value object
 * 
 * @param paramidx 
 * @param paramValue 
 * @return gint 
 */
gint SetParamsValue(DeviceParamEnum paramidx, gpointer paramValue);

/**
 * @brief Set the Params Max and Min
 * 
 * @param paramidx 
 * @param paramValue 
 * @return gint 
 */
gint SetParamsMaxAndMin(DeviceParamEnum paramidx,gint paramMax, gint paramMin);
#ifdef __cplusplus
}
#endif
#endif