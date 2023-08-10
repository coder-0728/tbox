/**
 * @file mcu_msg.h 
 * @author lujiefeng
 * @brief 
 * @version 0.1
 * @date 2018-11-12
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef __PARAM_MSG_H__
#define __PARAM_MSG_H__

#ifdef __cplusplus
extern "C"
{
#endif
#define REGISTER_FLAG_NO (0)
#define REGISTER_FLAG_ING (1)
#define REGISTER_FLAG_OK (2)

#define PARAM_SYNC_REQ 0x01
#define PARAM_SYNC_RES 0x02
#define PARAM_SYNC_SYNC 0x03
#define PARAM_SYNC_SYNC_ACK 0x04


#define PARAM_TYPE_INT 1
#define PARAM_TYPE_STR 0

typedef enum
{
    enumDefaultAESKey = 0,  /*默认通信秘钥        */
    enumParamAddr,          /*TSP服务器地址 */
    enumParamPort,          /*服务器端口号        */
    enumAesKey,             /*通讯秘钥          */
    enumECallTelNumber,     /*E-Call 电话号码*/
    enumBCallTelNumber,     /*B-Call 电话号码 ,保留字段*/
    enumICallTelNumber,     /*I-Call 电话号码 ,保留字段*/
    enumTSPCallNumber1,     /*TSP振铃通知号码 1 ，允许号段模式，后面用“*”补齐至17字节，示例：1511234*/
    enumTSPCallNumber2,     /*TSP振铃通知号码 2 ，允许号段模式，后面用“*”补齐至17字节，示例：1511234*/
    enumTSPCallNumber3,     /*TSP振铃通知号码 3  ，用作E-Call回拨号码，指定固定号码，可以支持号码段，后面用0x00补全至17字节。*/
    enumVin,                /*车架号 */
    enumBId,                /*Box注册分配的唯一ID */
    enumIiccid,             /*IC卡的唯一识别号码          */
    enumTboxSn,             /*T-Box序列号           */
    enumImsi,               /*国际移动用户识别码          */
    enumImei,               /*tbox识别码            */
    enumRegisterFlag,       /*注册标志 0：未注册 1:正在注册 2：注册成功*/
    enumCallNumber,         /*tbox手机号码*/
    enumKeytype,            /*Keytype       */
    enumTspMsgNumber1,      /*平台短消息号码1*/
    enumTspMsgNumber2,      /*平台短消息号码2*/
    enumCanLoadCycle,       /*车况上报周期*/
    enumEcuParatNumber,     /*电控单元零件号*/
    enumSysSupIdentifier,   /*系统供应商标识号*/
    enumEcuManufactureDate, /*ECU制造日期*/
    enumEcuHardwareVer,     /*ECU硬件版本号*/
    enumEcuSoftwareVer,     /*ECU软件版本号*/
    enumToken,              /*V3平台是token值*/
    enumSign,               /*V3平台*/
    enumBluetoothVersion,   /*蓝牙钥匙版本*/
    enumKeyExpiration,      /*蓝牙钥匙过期时间*/
    enumBluetoothKeyRnd,    /*蓝牙钥匙keyRnd*/
    enumBluetoothKeyCode,   /*蓝牙钥匙KeyCode*/
    enumGBParamAddr,        /*GB服务器地址 */
    enumGBParamPort,        /*GB服务器端口号        */
    enumspotCheckAddress,   /*抽查服务器地址 */
    enumspotCheckPort,      /*抽查服务端口号*/
    enumspotCheckLink,      /*抽查服务是否开启*/
    enumsUploadCount,       /*数据上传次数*/
    enumMqttUrl,            /*数据上传次数*/
    enumHttpActived,        /*是否激活*/
    enumMqttOtaDownLink,    /*OTA下行Topic Name*/
    enumMqttOtaUpLink,      /*OTA上行Topic Name*/
    enumMqttMsgDownLink,    /*MSG下行Topic Name*/
    enumMqttMsgUpLink,      /*MSG上行Topic Name*/
    enumTboxId,             /*TBOX ID*/
    enumSecKey,             /*SecKey*/
    enumParamEnd            /*结束标志请勿删除*/
} DeviceParamEnum;

#ifdef __cplusplus
}
#endif

#endif
