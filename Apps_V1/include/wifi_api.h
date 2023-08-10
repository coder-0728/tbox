/**
 * @file wifi_api.h
 * @author yangyefeng
 * @brief 
 * @version 0.1
 * @date 2018-11-28
 * 
 * @copyright Copyright (SiRun) 2018
 * 
 */

#ifndef __WIFI_API_H__
#define __WIFI_API_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <glib.h>

#define WIFI_MODE_AP		(0)
#define WIFI_MODE_STA		(!WIFI_MODE_AP)
/*MAX Len of WiFi Name*/
#define WIFI_AP_NAME_MAX_LEN	(32)

/*MAX Len of WiFi Passwd*/
#define WIFI_AP_PASSWD_MAX_LEN	(32)

/*MIN Len of WiFi Passwd*/
#define WIFI_AP_PASSWD_MIN_LEN	(8)

/*Name string cmp*/
#define WIFI_AP_CMP_STRING_NAME		"ssid"
#define WIFI_AP_CMP_STRING_PASSWD		"wpa_passphrase"

/*hostapd conf file path*/
#define WIFI_AP_CONF_FILE		"/usrdata/wifi/hostapd.conf"




/*wifi ap set mask*/
#define WIFI_AP_SET_MASK_NAME		(0x0001)
#define WIFI_AP_SET_MASK_PASSWD		(0x0002)
#define WIFI_AP_SET_MASK_STATUS		(0x0004)
/*WIFI Staus*/
typedef enum
{
	enum_wifiApClose = 0,
	enum_wifiApOpen,
	enum_wifiStaClose,
	enum_wifiStaOpen
}WiFiStatus;
/**
 * @brief 
 * 
 */
typedef struct
{
	guchar name[WIFI_AP_NAME_MAX_LEN];
	guchar passwd[WIFI_AP_PASSWD_MAX_LEN];
	WiFiStatus status;
}WiFiApStr;
/**
 * @brief 
 * 
 * @return gint 
 */
gint InitWifiInterface (void);
/**
 * @brief Get the Wifi Status object
 * 
 * @param wifiStatus 
 * @return gchar* 
 */
gint ClientGetWifiStatus(void);
/**
 * @brief Set the Wi Fi Mode object
 * 
 * @param wifiMode 
 * @return gint 
 */
gint SetWiFiMode(int wifiMode);
/**
 * @brief DisableWiFi
 * 
 */
gchar DisableWifi(void);
/**
 * @brief 
 * 
 */
gchar EnableWiFi(void);

gint GetWiFiApInfo(WiFiApStr *apInfo);
gint SetWiFiApInfo(WiFiApStr *apInfo,gint setMask);
gint StopWiFiAp();
gint StartWiFiAp();
gint StopWiFiSta();
gint StartWiFiSta();
gint ConnectWiFiStaToApp(const gchar *ssid, const gchar *passwd);

#ifdef __cplusplus
}
#endif
#endif