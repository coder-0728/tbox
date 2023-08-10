/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-06 17:01:17
 * @LastEditTime: 2019-09-06 17:01:49
 * @LastEditors: Please set LastEditors
 */
/*************************************************************************
	> File Name: dbus_interface_name.h
	> Author: 
	> Mail: 
	> Created Time: 2018/11/27
 ************************************************************************/

#ifndef _DBUS_INTERFACE_NAME_H
#define _DBUS_INTERFACE_NAME_H

/*AT service*/
#define DBUS_AT_CONNECTTION_NAME "sr.at" /*connection name */
#define DBUS_AT_OBJECTPATH "/sr/at"		 /*object name*/

/*params service*/
#define DBUS_PARAM_CONNECTTION_NAME "sr.param"
#define DBUS_PARAM_OBJECTPATH "/sr/param"

/*power management service*/
#define DBUS_PM_CONNECTION_NAME "sr.pm"
#define DBUS_PM_OBJECTPATH "/sr/pm"

/*net service*/
#define DBUS_NET_CONNECTION_NAME "sr.net"
#define DBUS_NET_OBJECTPATH "/sr/net"

/*gnss service*/
#define DBUS_GNSS_CONNECTION_NAME "sr.gnss"
#define DBUS_GNSS_OBJECTPAHT "/sr/gnss"

/*gsensor service*/
#define DBUS_GSENSOR_CONNECTION_NAME "sr.gsensor"
#define DBUS_GSENSOR_OBJECTPATH "/sr/gsensor"

/*bt service*/
#define DBUS_BT_CONNCETION_NAME "sr.bt"
#define DBUS_BT_OBJECTPATH "/sr/bt"

/*wifi service*/
#define DBUS_WIFI_CONNECTION_NAME "sr.wifi"
#define DBUS_WIFI_OBJECTPATH "/sr/wifi"

/*update service*/
#define DBUS_UPDATE_CONNECTION_NAME "sr.update"
#define DBUS_UPDATE_OBJECTPATH "/sr/update"

/*ecall service*/
#define DBUS_ECALL_CONNECTION_NAME "sr.ecall"
#define DBUS_ECALL_OBJECTPATH "/sr/ecall"

/*gbrealdata service*/
#define DBUS_GBREAL_CONNECTION_NAME "sr.gbreal"
#define DBUS_GBREAL_OBJECTPATH "/sr/gbreal"

/*gbrealdata service*/
#define DBUS_MONITOR_CONNECTION_NAME "sr.monitor"
#define DBUS_MONITOR_OBJECTPATH "/sr/monitor"
#endif
