/**
* 该头文件用来描述平台报文结构。
*/

#ifndef _TSP_MESSAGE_PROTOCOL_H_
#define _TSP_MESSAGE_PROTOCOL_H_

#include "DispatcherBody.h"

#define PROTO_VERSION 0x21
#define APPLICATION_DATA_VERSION 0x6501

typedef enum {
    AID_Activation_And_Configuration = 100,
    AID_Remote_Control = 111,
    AID_Remote_Journey,
    AID_Vehicle_Status,
    AID_Stolen_Vehicle_Tracking,
    AID_Key_Management_and_Authentication,
    AID_Message_Center,
    AID_Outing_Plan_Message,
    AID_POI_Message,
    AID_eCall = 901,
    AID_bCall,
    AID_iCall,
}E_Application_Id;

typedef enum{
    MID_ConfigurationCheckReq = 1,
    MID_ConfigurationCheckRes,
    MID_ConfigurationConnReq,
    MID_ConfigurationConnRes,
    MID_ConfigurationEndReq,
    MID_ConfigurationGetReq,
    MID_ConfigurationGetRes,
    MID_ConfigurationReadReq,
    MID_ConfigurationReadRes,
    MID_NewPubKeyReq,
    MID_NewPubKeyRes,
    MID_NewSecKeyReq,
    MID_NewSecKeyRes,
}E_Activation_And_Configuration_Mid;

typedef enum{
    MID_RVMVehicleStatusReq = 1,
    MID_RVMVehicleStatusResp,
    MID_RVMVehicleStatusAlert,
    MID_RVMGeoPositionSet,
    MID_RVMVehicleStatusInfo,
    MID_RVMVehicleImportantStatus = 13,
    MID_RVMMsgNotice = 7,
    MID_RVMVehicleChargingStatusReq,
    MID_RVMVehicleChargingStatusResp,
    MID_RVMVehicleChargingStatusInfo,
    MID_RVMVehicleClassifiedStatusReq,
    MID_RVMVehicleClassifiedStatusResp,
    MID_RVMVehicleJourneyStatus = 14,
    MID_OBFCMInfo,
    MID_APNAllowedReq,
    MID_APNAllowedRes
}E_Vehicle_Status_Mid;

typedef enum{
    MID_RVCReq = 1,
    MID_RVCResp,
    MID_RVCStatus,
}E_Remote_Control_Mid;

typedef struct {
    unsigned char protocol_version;
    unsigned char dispatcher_message_length;
    unsigned char dispatcher_body_encoding;
    unsigned char tbox_identification_type;
    char tbox_id[12];
    char tbox_sn[16];
}T_DispatcherHeader;

typedef struct {
    T_DispatcherHeader header;
    DispatcherBody_t *body;
    unsigned char *application_data;
}T_TspMessage;


typedef struct {
    char flag;
    unsigned short length;               //整个包的长度，包括flag + length + header + random + body + application_data;
    T_DispatcherHeader header;           //无编码
    unsigned char random[16];            //使用128位AES-CBC加密，以时间位种子，为了区分相同内容的数据包 
    DispatcherBody_t *body;              //使用128位AES-CBC加密，ASN.1编码
    unsigned char *application_data;     //使用128位AES-CBC加密, ASN.1编码
}T_TransmitLayerMessage;


#define AES_KEY_DEFAULT "1234567890123456"

#endif
