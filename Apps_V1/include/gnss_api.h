/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-09 10:31:08
 * @LastEditTime: 2019-09-18 14:16:52
 * @LastEditors: Please set LastEditors
 */
/**
 * @file gnss_api.h
 * @author liujilin
 * @brief 
 * @version 0.1
 * @date 2018-11-28
 * 
 * @copyright Copyright (SiRun) 2018
 * 
 */
#ifndef __GNSS_API_H__
#define __GNSS_API_H__
#include <glib.h>
#include <time.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    gint status;
    gint moonLinkNum;
    gint seaHeight;
    gint direction;
    gint latitudeValue;
    gint longtitudeValue;
    gint speed;
    time_t utcTime_t;
    gint epeHorizontal;
    gint epeVertical;
}GNRMC;

typedef void (*OriginalCallback)(const gchar* pBuf);
typedef void (*ParseredCallback)(const GNRMC* pData);
/**
 * @brief 
 * 
 * @param OriginalCb 
 * @param ParseredCb 
 * @return guchar 
 */
guchar InitGNSSInterface(void(*OriginalCb)(const gchar* pBuf), void (*ParseredCb)(const GNRMC* pData));
/**
 * @brief Get the Gps Data object
 * 
 * @param gps_time_data 
 */
guchar GetGpsData(GNRMC *gps_time_data);
gint GetForceSyncFlag(void);
#ifdef __cplusplus
}
#endif
#endif 
