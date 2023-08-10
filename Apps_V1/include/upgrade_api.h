/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-08-28 16:12:47
 * @LastEditTime: 2019-08-28 16:13:09
 * @LastEditors: Please set LastEditors
 */
#ifndef __UPGRADE_API_H__
#define __UPGRADE_API_H__

#include <glib.h>
#include "config_type.h"
#ifdef __cplusplus
extern "C" {
#endif

#define  CAN_PARSER_IS_PARSER       (2)
#define  CAN_PARSER_NOT_PARSER      (1)

typedef struct 
{
    guint16 signalIndex;  /**< signal index*/
    guint32 channelCanId;   
    guchar data[8];
}PenetrateData;

/**
 * @brief 
 * 
 */
typedef void (*StartUpgradeStateChangedCB)(gboolean started);
/**
 * @brief 
 * 
 * @return gint 
 */
gint InitUpgradeClient(void);
/**
 * @brief 
 * 
 * @return gint 
 */
gint InitUpgradeConfigClient(void);
/**
 * @brief 
 * 
 * @return int 
 */
gint BindUpgradeTask(void);
/**
 * @brief 
 * 
 * @param resetMcu 
 * @return gint 
 */
gint UnbindUpgradeTask(gboolean resetMcu);
/**
 * @brief 
 * 
 * @param version 
 * @param uflag 
 * @param errlog 
 * @return gint 
 */
gint CheckUpgradeVersion(const gchar *version, guchar *uflag, gchar *errlog);
/**
 * @brief 
 * 
 * @param url 
 * @param filepath 
 * @param errlog 
 * @return gint 
 */
gint DownloadUpgradeFile(const gchar *url, const gchar *filepath, gchar *errlog);
/**
 * @brief 
 * 
 * @param depath 
 * @param md5 
 * @param errlog 
 * @return int 
 */
gint CheckUpgradePkgMd5(const gchar *depath, const gchar *md5, gchar *errlog);
/**
 * @brief 
 * 
 * @param pDepath 
 * @param pSha256 
 * @param pErr 
 * @return gint 
 */
gint CheckUpgradePkgSha256(const gchar *pDepath, const gchar *pSha256, gchar *pErr);
/**
 * @brief 
 * 
 * @param depath 
 * @param realpath 
 * @param type 
 * @param errlog 
 * @return int 
 */
gint RestoreUpgradePkg(const gchar *depath, const gchar *realpath, guchar type, gchar *errlog);
/**
 * @brief 
 * 
 * @param realpath 
 * @param uflag 
 * @param errlog 
 * @return int 
 */
gint StartUpgradePkg(const gchar *realpath, gchar uflag, gchar *errlog);

/**
 * @brief 
 * 
 * @param changedCallback 
 * @return gint 
 */
gint RegisterStatueChangedCb(StartUpgradeStateChangedCB changedCallback);


/**
 * @brief 
 * 
 * @param changedCallback 
 * @return gint 
 */
gint SubscriptionConfigMsg(guint32 keyId, ConfigType type);
/**
 * @brief 
 * 
 * @param changedCallback 
 * @return gint 
 */
gint UnSubscriptionConfigMsg(guint32 keyId, ConfigType type);
/**
 * @brief 
 * 
 * @param changedCallback 
 * @return gint 
 */
gint GetConfigMsg(gint keyId, ConfigQueueData *msgData, gint timeOut);

/**
 * @brief GetCanParserStatus
 * 
 * @param pStatus 
 * @return gint 
 */
gint GetCanParserStatus(gint *pStatus);

#ifdef __cplusplus
}
#endif
#endif