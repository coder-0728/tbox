/**
 * @brief 
 * 
 * @file version.h
 * @author liujilin
 * @date 2018-08-03
 */

#ifndef __VERSION_H
#define __VERSION_H
#include <glib.h>

//XXX XX XXX XXX
//PROJECT_ID MAJOR_ID LINUXAPP_ID MCU_ID
#define PLATFORM_VERSION_LEN (11)
//XXX XX XXX XXX
//PROJECT_ID MAJOR_ID LINUXAPP_ID(OR MCU_ID)
#define LINUXAPP_VERSION_LEN (8)
#define MCU_VERSION_LEN LINUXAPP_VERSION_LEN
#define PROJECTID_STR_LEN (3)
#define MAJORID_STR_LEN (2)
#define LINUXAPPID_STR_LEN (3)
#define MCUID_STR_LEN (3)

#define UPGRADE_BASE_DIR "/oemdata/upgrade/"
/**
 * @brief old package,using for bspatch
 * 
 */
#define UPGRADE_OLD_PKG_PATH "/oemdata/upgrade/old_upgrade.tgz"

/**
 * @brief unzip upgradepkg dir
 * 
 */
#define UPGRADE_UNZIP_DIR "/oemdata/upgrade/upgradepkg/"

#ifdef __cplusplus
extern "C"
{
#endif

    enum PlatformVersionType
    {
        E_PLATFORM_VERSION_TYPE_NONE = 0,
        E_PLATFORM_VERSION_TYPE_LINUXAPP = 1,
        E_PLATFORM_VERSION_TYPE_MCU = 2,
        E_PLATFORM_VERSION_TYPE_ALL = 3,
    };

    /**
     * @brief the struct of version
     * 
     */
    typedef struct
    {
        gint projectId;
        gint majorId;
        gint linuxAppId;
        gint mcuId;
    } PlatformVersion;

    gint ConvertStringToVersion(PlatformVersion *version, gchar versionStr[], guchar type);

    /**
     * @brief Conver TSP String version To Struct PlatformVersion
     * 
     * @param[out]	version	        The point of PlatformVersion
     * @param[in]	versionStr	    Tsp Upgrade Vsersion
     * @retval	    RET_OK          Success
     * @retval      RET_ERR         Failed
     */
    gint ConvertStringToVersion_v3(PlatformVersion *version, const gchar *tspVersionStr);

    /**
     * @brief Get Upgrade Pkg Version 
     * 
     * @param[out]	version	    The point of struct PlatformVersion
     * @retval	    RET_OK      Success
     * @retval      RET_ERR     Failed
     */
    gint GetUpgradePkgVersion(PlatformVersion *version);

    /**
     * @brief  Get Tbox Cur Version 
     * 
     * @param[out]	version	    The point of struct PlatformVersion
     * @retval	    RET_OK      Success
     * @retval      RET_ERR     Failed
     */
    gint GetTBoxAppVersion(PlatformVersion *version);

    gint GetTBoxAppVersionStr(gchar *version);

    gint GetWebTgzAppVersion(PlatformVersion *version);
    
    void UpdateTboxAppVersion(PlatformVersion tboxVer);
#ifdef __cplusplus
}
#endif
#endif
