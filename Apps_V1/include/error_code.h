/*************************************************************************
	> File Name: error_code.h
    > Author: 
	> Mail: 
	> Created Time: 2018年11月06日 星期二 09时55分53秒
 ************************************************************************/

#ifndef _ERROR_CODE_H
#define _ERROR_CODE_H

#define RET_OK (0)
#define RET_ERR (-1)

#define ERR_PUBLIC_POINT_NULL (RET_ERR - 1)

/*public error code : */
#define ERR_MCU_PUBLIC (RET_OK - 10)
#define ERR_AT_PUBLIC (RET_OK - 20)
#define ERR_PM_PUBLIC (RET_OK - 30)
#define ERR_NET_PUBLIC (RET_OK - 40)
#define ERR_PARAM_PUBLIC (RET_OK - 50)
#define ERR_GNSS_PUBLIC (RET_OK - 60)
#define ERR_UPDATE_PUBLIC (RET_OK - 70)
#define ERR_BT_PUBLIC (RET_OK - 80)
#define ERR_WIFI_PUBLIC (RET_OK - 90)
#define ERR_GSENSOR_PUBLIC (RET_OK - 100)

/*MCU SERVIC error code*/
#define ERR_MCU_FIXHEAD (ERR_MCU_PUBLIC - 0)
#define ERR_MCU_READ_UART (ERR_MCU_PUBLIC - 1)
#define ERR_MCU_RECVQUEUE_TIMEOUT (ERR_MCU_PUBLIC - 2)

/*AT SERVICE error code*/
#define ERR_AT_OPEN_UART        ( ERR_AT_PUBLIC - 0 )
#define ERR_AT_RIL_OPEN         ( ERR_AT_PUBLIC - 1 )
#define ERR_AT_CMD_TO_SERVER    ( ERR_AT_PUBLIC - 2 )
#define ERR_AT_CMD_TO_BOARD     ( ERR_AT_PUBLIC - 4 )
#define ERR_AT_CMD_EXECUTE      ( ERR_AT_PUBLIC - 4 )
#define ERR_AT_PARAMS           ( ERR_AT_PUBLIC - 5 )
#define ERR_AT_MODEM_INIT       ( ERR_AT_PUBLIC - 6 )

#define ERR_NET_PROFILE_INDEX_INVALID      ( ERR_NET_PUBLIC - 0 )


#endif
