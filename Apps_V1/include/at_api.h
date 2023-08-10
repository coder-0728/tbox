/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-09 14:06:12
 * @LastEditTime: 2019-09-11 11:26:27
 * @LastEditors: Please set LastEditors
 */
/**
 * @file at_api.h
 * @brief AT API
 * @author jason
 * @version 
 * @date 2019-05-14
 */

#ifndef _AT_API_H
#define _AT_API_H
#include <glib.h>
#include <glib-object.h>
#include "at_sirun.h"

#ifdef __cplusplus
extern "C"
{
#endif
	typedef void (*Callback)(GObject *object, const gchar *urc, const gchar *sms, gpointer userData);
/**
 * @brief  init at server
 * 
 * @param handleSingnle 
 * @return gint 
 */
	gint InitAtInterface(Callback handleSingnle);
/**
 * @brief 
 * 
 * @param atCmd 
 * @param prefix 
 * @param responseLines 
 * @param response 
 * @return gint 
 */
	gint SendAT(const gchar *atCmd, const gchar *prefix, ResponseLines responseLines, gchar *response);
/**
 * @brief Get the Modem ICCID object
 * 
 * @param iccid 
 * @return gint 
 */
	gint GetModemICCID(gchar *iccid);
/**
 * @brief Get the Modem IMEI object
 * 
 * @param imei 
 * @return gint 
 */
	gint GetModemIMEI(gchar *imei);
/**
 * @brief Get the Modem IMSI object
 * 
 * @param imsi 
 * @return gint 
 */
	gint GetModemIMSI(gchar *imsi);
/**
 * @brief init gps 
 * 
 * @return gint 
 */
	gint GpsInit(void);
/**
 * @brief  disable gps
 * 
 * @return gint 
 */
	gint GpsDisable(void);
/**
 * @brief enable gps 
 * 
 * @return gint 
 */
	gint GpsEnable(void);
/**
 * @brief 
 * 
 * @param channel 
 * @return gint 
 */

gint ReadADC(gint channel,char * value);
	
/**
 * @brief Get the Modem C S Q object
 * 
 * @param csq 
 * @return gint 
 */
gint GetModemCSQ(gchar *csq);

#ifdef __cplusplus
}
#endif

#endif
