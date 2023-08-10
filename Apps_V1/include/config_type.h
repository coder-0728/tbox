/**
 * @file config_type.h 
 * @author fxl
 * @brief 
 * @version 0.1
 * @date 2018-11-12
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef _CONFIG_TYPE_H_
#define _CONFIG_TYPE_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <glib.h>


#define CONFIG_TYPE_KEY (2)

    /**
     * @brief Msq Queue Type
     * 
     */
    typedef enum
    {
        CONFIG_TYPE_SUBSCRIPTION = 1,
        CONFIG_TYPE_UNSUBSCRIPTION = 2,
        CONFIG_TYPE_IS_GB = 3,
        CONFIG_TYPE_IS_ALARM = 4,
        CONFIG_TYPE_IS_EVENT = 5,
        CONFIG_TYPE_IS_PENETRATE = 6,

        CONFIG_TYPE_MAX_LIMIT = 0x3FFF
    } ConfigType;

    /**
     * @brief Control message structure ,
     * !!!This defined for Subscription and UnSubscription,not for control cmd !!!
     * !!!This defined for Subscription and UnSubscription,not for control cmd !!!
     * !!!This defined for Subscription and UnSubscription,not for control cmd !!!
     * 
     */
    #pragma pack(1)
    typedef struct
    {
        ConfigType msgType; /**< MsgQueueType*/
        guint32 keyID;        /**< MsgQueue keyID*/
    } ConfigCtrlMsg;

    #pragma pack(1)
    typedef struct
    {
        guint16 signalIndex;  /**< signal index*/
        guint32 channelCanId;
        guint64 value; /**< serverData*/
    }SignalValue;
    /**
     * @brief  mcu message structure
     * 
     */
    #pragma pack(1)
    typedef struct
    {
        ConfigType type;    /**< type of MsgQueueType*/
        guint16 signalLen;  /**< signal index*/
        SignalValue signalValue[0]; /**< serverData*/
    } ConfigMsg;

    /**
    * @brief Msg Queue structure
     * 
     */
    typedef struct
    {
        glong type; /**< type of MsgQueueType*/
        union {
            ConfigCtrlMsg ctrl;
            ConfigMsg msg;
        } data; /**< The union CtrlMsg and McuMsg*/
    } ConfigQueueData;

#define CONFIG_MAX_LEN (300 * sizeof(SignalValue))

#ifdef __cplusplus
}
#endif

#endif
