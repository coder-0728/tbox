/**
 * @file gnss_if.h
 * @author liujilin
 * @brief 
 * @version 0.1
 * @date 2018-11-28
 * 
 * @copyright Copyright (SiRun) 2018
 * 
 */
#ifndef __GNSS_IF_H__
#define __GNSS_IF_H__
#include <glib.h>
#include <time.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
#define PUBLICNETPROFILEINDEX  1
#define PRIVATENETPROFILEINDEX 2
#define PROFILEINDEXMAX        15
#define PROFILEINDEXMIN        0
}NetNum;

#define NETWAORKSTATUSCALLBACK_LIST_NUM (20)
/**
 * @brief 
 * 
 */
typedef struct{
guint8  profileIndex;
gint  dialStatus;
}NetInfo;

typedef void (*GetNetStatusCallback)(gint profileIndex,gint status);
/*注册网络状态变化回调函数*/
gint RegisterNetWorkStatusChangeCBack(GetNetStatusCallback);
/**
 * @brief InitNetInterface
 * 初始化网络状态客户端
 * @return int 
 */
gint InitNetInterface( void );
 /**
 * @brief Get the Net Work IP object
 * 获取网络IP
 * @param profileIndex 
 * @return int 
 */
gint GetNetWorkIp(gint profileIndex, gchar *pIp);
/**
 * @brief Get the Net Work Status object
 * 获取网络状态
 * @param profileIndex 
 * @return int 
 */
gint GetNetWorkStatus(gint profileIndex);
#ifdef __cplusplus
}
#endif
#endif 