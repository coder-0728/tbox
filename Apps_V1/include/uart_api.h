/**
 * @file uart_api.h
 * @author lujiefeng
 * @brief 
 * @version 0.1
 * @date 2019-01-16
 * 
 * @copyright Copyright (SiRun) 2019
 * 
 */
#ifndef _UART_API_H
#define _UART_API_H

#include <glib.h>

G_BEGIN_DECLS
#include <termios.h>

/*uart set speed*/
#define UART_SET_SPEED_9600 (9600)
#define UART_SET_SPEED_38400 (38400)
#define UART_SET_SPEED_57600 (57600)
#define UART_SET_SPEED_115200 (115200)
#define UART_SET_SPEED_460800 (460800)
#define UART_SET_SPEED_576000 (576000)
#define UART_SET_SPEED_921600 (921600)
#define UART_SET_SPEED_1_5M (1500000)
#define UART_SET_SPEED_2M (2000000)
#define UART_SET_SPEED_2_5M (2500000)
#define UART_SET_SPEED_3M (3000000)
#define UART_SET_SPEED_3_5M (3500000)
#define UART_SET_SPEED_4M (4000000)

/*uart set flowctrl*/
#define UART_SET_FLOWCTRL_NONE (0)
#define UART_SET_FLOWCTRL_HARD (1)
#define UART_SET_FLOWCTRL_SOFT (2)

/*uart set databit*/
#define UART_SET_DATABITS_5 (5)
#define UART_SET_DATABITS_6 (6)
#define UART_SET_DATABITS_7 (7)
#define UART_SET_DATABITS_8 (8)

/*uart set stopbit*/
#define UART_SET_STOPBIT_1 (1)
#define UART_SET_STOPBIT_2 (2)

/*uart set parity*/
#define UART_SET_PARITY_NONE (0)
#define UART_SET_PARITY_ODD (1)
#define UART_SET_PARITY_EVEN (2)

/**
 * @brief 
 * 
 * @param[in]	uartDev	
 * @param[in]	speed	
 * @param[in]	flowCtrl	
 * @param[in]	dataBit	
 * @param[in]	stopBit	
 * @param[in]	parity	
 * @retval	    >0: fd
 *              <= 0 :error
 */
gint UartInit(const guchar *uartDev, guint32 speed, guint8 flowCtrl, guint8 dataBit, guint8 stopBit, guint8 parity);

void UartClose(gint fd);

/**
 * @brief 
 * 
 * @param[in]	data	
 * @param[in]	dataLen	
 * @retval
 * - RET_ERR :error
 * - ERR_PUBLIC_POINT_NULL: the point param is NULL
 * - > 0 :send data len	
 */
gint UartSendData(gint fd, guchar *data, gint dataLen);

/**
 * @brief Recv Uart Data with block
 * 
 * @param[in]	recvData	
 * @param[in]	recvDataLen	
 * @retval	
 * - RET_ERR :error
 * - ERR_PUBLIC_POINT_NULL: the point param is NULL
 * - > 0 :recv data len
 */
gint UartRecvData(gint fd, guchar *recvData, gint recvDataLen);

/**
 * @brief flush uart buff
 * 
 */
void UartFlush(gint fd);

/**
 * @brief 
 * 
 * @param[in]	fd	
 * @param[in]	vTime	
 * @param[in]	vMin	
 */
void UartSetCC(gint fd, gchar vTime, gchar vMin);

G_END_DECLS

#endif
