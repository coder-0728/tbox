/*
 * @Description: In User Settings Edit
 * @Author: your name
<<<<<<< HEAD
 * @Date: 2019-09-04 10:56:46
 * @LastEditTime: 2019-09-11 17:03:14
=======
 * @Date: 2019-08-30 16:57:13
 * @LastEditTime: 2019-09-04 11:46:40
>>>>>>> origin/develop
 * @LastEditors: Please set LastEditors
 */
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
#ifndef _MCUMSG_H
#define _MCUMSG_H

#ifdef __cplusplus
extern "C"
{
#endif
#include <glib.h>

#define MCU_MSG_HEAD_LEN (8)
#define MCU_MSG_SERVERDATA_MAX_LEN (5120)

#define MCU_RECV_QUEUE_KEY (1)

    /**
     * @brief Msq Queue Type
     * 
     */
    typedef enum
    {
        MSG_QUEUE_SUBSCRIPTION = 1,
        MSG_QUEUE_UNSUBSCRIPTION = 2,
        /*AID 0x01 server*/
        MSG_QUEUE_SYNC_POWER_STATUS = 0x0101, //enumMsgSyncLine
        MSG_QUEUE_SYNC_TIME = 0x0102,         //enumMsgSyncTimeToMcu
        MSG_QUEUE_SYNC_CPU_STATUS = 0x0103,   //enumMsgSyncCpuStatus
        
        MSG_QUEUE_CAN_PARSER = 0x0104,        //enumMsgSyncKeepAlive
        MSG_QUEUE_SYNC_PARAM = 0x0105,        //enumMsgSyncParmData
        MSG_QUEUE_SYNC_WORK_MODE = 0x0106,    //enumMsgSyncPowerMode
        MSG_QUEUE_SYNC_SELF_ALL = 0x0107,     //enumMsgSyncSelfInspectionAll
        MSG_QUEUE_SYNC_SELF_SINGLE = 0x0108,  //enumMsgSyncSelfInspectionSingle
        MSG_QUEUE_CTR_CPU = 0x0109,           //enumMsgMcuControlCpu
        MSG_QUEUE_GET_CPU_INCO = 0x010A,      //enumMsgMcuGetCpuInformation

        /*AID 0x02 server */
        MSG_QUEUE_VEHICLE_BASE = 0x0201,   //enumMsgVehicleBaseInfo
        MSG_QUEUE_VEHICLE_BODY = 0x0202,   //enumMsgVehicleBodyData
        MSG_QUEUE_VEHICLE_ENGINE = 0x0203, //enumMsgVehicleEngineInfo
        MSG_QUEUE_VEHICLE_TEMPS = 0x0204,  //enumMsgVehicleTemps
        MSG_QUEUE_VEHICLE_AIRBAG = 0x0205, //enumMsgVehicleAirBagStatus
        MSG_QUEUE_VEHICLE_AC = 0x0206,
        MSG_QUEUE_VEHICLE_DIAGNOSIS = 0x0208, //enumMsgVehicleDiagnosis
        //MSG_QUEUE_VEHICLE_MALFUNCTION = 0x0208,//
        MSG_QUEUE_VEHICLE_CONTROL = 0x0240,      //enumMsgVehicleControl
        MSG_QUEUE_VEHICLE_CONTROL_DATA = 0x0241, //enumMsgVehicleControlData
        MSG_QUEUE_VEHICLE_MALFUNCTION = 0x0250,  //enumMsgVehicleMalfunction
        MSG_QUEUE_VEHICLE_GB_INFO = 0x0281,      //enumMsgVehicleGbInfo
        MSG_QUEUE_VEHICLE_GB_MOTOR = 0x0282,     //enumMsgVehicleGbMotor
        MSG_QUEUE_VEHICLE_GB_FUEL_CELL = 0x0283, //enumMsgVehicleGbBattery
        MSG_QUEUE_VEHICLE_GB_ENGINE = 0x0284,    //enumMsgVehicleGbEngine
        MSG_QUEUE_VEHICLE_GB_POSITION = 0x0285,  //enumMsgVehicleGbPosition
        MSG_QUEUE_VEHICLE_GB_EXTREMUM = 0x0286,  //enumMsgVehicleGbExtremum
        MSG_QUEUE_VEHICLE_GB_ALARM = 0x0287,     //enumMsgVehicleGbAlarm
        MSG_QUEUE_VEHICLE_GB_BAT_VOL = 0x0288,   //enumMsgVehicleGbBatteryVoltage
        MSG_QUEUE_VEHICLE_GB_BAT_TEMP = 0x0289,  //enumMsgVehicleGbBatteryTemperature

        /*AID 0x03 server*/
        MSG_QUEUE_UPGRADE_GET_MCU_VERSION = 0x0301, //enumMsgMcuUpgradeVersion
        MSG_QUEUE_UPGRADE_GET_MCU_SEED = 0x0302,    //enumMsgMcuUpgradeSeed
        MSG_QUEUE_UPGRADE_SETKEY = 0x0303,          //enumMsgMcuUpgradeSetKey
        MSG_QUEUE_UPGRADE_DOWNLOAD_MODE = 0x0304,   //enumMsgMcuUpgradeDownloadMode
        MSG_QUEUE_UPGRADE_EREASE_FLASH = 0x0305,    //enumMsgMcuUpgradeEreaseFlash
        MSG_QUEUE_UPGRADE_DOWNLOAD_DATA = 0x0306,   //enumMsgMcuUpgradeDownload
        MSG_QUEUE_UPGRADE_FINISH = 0x0307,          //enumMsgMcuUpgradeFinish
        MSG_QUEUE_UPGRADE_RESET_MCU = 0x0308,       //enumMsgMcuUpgradeReset
        MSG_QUEUE_UPGRADE_MCU_RUN_STATUS = 0x0309,  //enumMsgMcuUpgradeGetRunStatus

        /*AID 0x05 server*/
        MSG_QUEUE_REQ_VERSION = 0x0501, //enumMsgRequestVersion

		/*UDS*/
		MSG_QUEUE_UDS_CAN0 = 0x0701, //CAN0
		MSG_QUEUE_UDS_CAN1 = 0x0702, //CAN1
		MSG_QUEUE_UDS_CAN2 = 0x0703, //CAN2
		MSG_QUEUE_UDS_CAN3 = 0x0704, //CAN3
        MSG_QUEUE_UDS_TBOX = 0x0740, //tbox

        MSG_QUEUE_MAX_LIMIT = 0x3FFF
    } MsgQueueType;

/* SYNC PARAM Subcmd*/
#define MSG_SUBCMD_SYNC_PARAM_REQUEST (0x01)
#define MSG_SUBCMD_SYNC_PARAM_RESPONSE (0x02)
#define MSG_SUBCMD_SYNC_PARAM_SYNC (0x03)
#define MSG_SUBCMD_SYNC_PARAM_SYNCACK (0x04)

/*sync wore mode Subcmd*/
#define MSG_SUBCMD_SYNC_WORKMODE_MCU_REQUEST_CPU_SLEEP (0x01)
#define MSG_SUBCMD_SYNC_WORKMODE_MCU_REQUEST_CPU_WAKEUP_SOURCE (0x02)
#define MSG_SUBCMD_SYNC_WORKMODE_MCU_REQUEST_CPU_RUN_STATE (0x03)
#define MSG_SUBCMD_SYNC_WORKMODE_MCU_NOTIFY_CPU_WAKEUP_SOURCE (0x04)
#define MSG_SUBCMD_SYNC_WORKMODE_MCU_SYNC_TO_CPU (0x05)
#define MSG_SUBCMD_SYNC_WORKMODE_CPU_SYNC_TO_MCU (0x06)

/* SYNC FUNC Status Subcmd*/
#define MSG_SUBCMD_SYNC_FUNC_REQUEST (0x01)
#define MSG_SUBCMD_SYNC_FUNC_RESPONSE (0x02)
#define MSG_SUBCMD_SYNC_FUNC_SYNC (0x03)
#define MSG_SUBCMD_SYNC_FUNC_SYNCACK (0x04)

/*Control subcmd*/
#define MSG_SUBCMD_CONTROL_MCU_ACK (0x6E)
#define MSG_SUBCMD_CONTROL_RESULT (0x6F)
#define MSG_SUBCMD_CONTROL_OPEN_DOOR (0x01)
#define MSG_SUBCMD_CONTROL_CLOSE_DOOR (0x02)
#define MSG_SUBCMD_CONTROL_OPEN_WINDOW (0x03)
#define MSG_SUBCMD_CONTROL_CLOSE_WINDOW (0x04)
#define MSG_SUBCMD_CONTROL_FLASH_WHISTLE (0x05)
#define MSG_SUBCMD_CONTROL_OPEN_AC (0x06)
#define MSG_SUBCMD_CONTROL_CLOSE_AC (0x07)
#define MSG_SUBCMD_CONTROL_START_ENGINE (0x08)
#define MSG_SUBCMD_CONTROL_TURNOFF_ENGINE (0x09)
#define MSG_SUBCMD_CONTROL_OPEN_SKYLIGHT (0x0A)
#define MSG_SUBCMD_CONTROL_CLOSE_SKYLIGHT (0x0B)
#define MSG_SUBCMD_CONTROL_OPEN_TRUNK (0x0C)
#define MSG_SUBCMD_CONTROL_CLOSE_TRUNK (0x0D)
#define MSG_SUBCMD_CONTROL_REMOTE_CHARGING (0x0E)
#define MSG_SUBCMD_CONTROL_ODER_CHARGING (0x0F)
#define MSG_SUBCMD_CONTROL_REMOTE_SPEED_LIMIT (0x10)
#define MSG_SUBCMD_CONTROL_PARKING_HEAT (0x11)
#define MSG_SUBCMD_CONTROL_CHARGE_HEAT (0x12)
#define MSG_SUBCMD_CONTROL_INTHE_CONTROL (0x013)
#define MSG_SUBCMD_CONTROL_SHARED_CAR_PREBOOK (0x014)
#define MSG_SUBCMD_CONTROL_SHARED_CAR_PREBOOK_CANCEL (0x015)
#define MSG_SUBCMD_CONTROL_SHARED_CAR_RETURN (0x016)

    /**
     * @brief Control message structure ,
     * !!!This defined for Subscription and UnSubscription,not for control cmd !!!
     * !!!This defined for Subscription and UnSubscription,not for control cmd !!!
     * !!!This defined for Subscription and UnSubscription,not for control cmd !!!
     * 
     */
    typedef struct
    {
        MsgQueueType msgType; /**< MsgQueueType*/
        guint32 keyID;        /**< MsgQueue keyID process ID*/
    } CtrlMsg;

    /**
     * @brief  mcu message structure
     * 
     */
    typedef struct
    {
        MsgQueueType type;    /**< type of MsgQueueType*/
        guint16 len;          /**< the len of serverData*/
        guint8 subcmd;        /**< subCmd*/
        guint8 checkSum;      /**< checkSum*/
        guint8 serverData[0]; /**< serverData*/
    } McuMsg;

    /**
    * @brief Msg Queue structure
     * 
     */
    typedef struct
    {
        glong type; /**< type of MsgQueueType*/
        union {
            CtrlMsg ctrl;
            McuMsg msg;
        } data; /**< The union CtrlMsg and McuMsg*/
    } MsgQueueData;

#ifdef __cplusplus
}
#endif

#endif
