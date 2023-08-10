#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <gio/gio.h>
#include <string.h>

#include "log.h"
#include "error_code.h"
#include "upgrade_api.h"
#include "param_api.h"
#include "pm_api.h"

#include "tsp_activation.h"
#include "tsp_monitor.h"
#include "mcu_can_rx_handle.h"
#include "mcu_can_msg_mapping.h"
#include "mqtt_conn.h"

#define LOG_MODULE_NAME "TSP"

//TODO：这个不能写在这里，后续要写一个接口提供给其他线程调用。
static int g_can_fd = -1;

static void sdk_init(void)
{
    int ret = RET_ERR;

    // 日志初始化
    LOG_INIT(LOG_MODULE_NAME);
    LOG_Debug("tbox sdk init");

    // 参数管理接口(Params)初始化
    do
    {
        ret = InitParamsInterface();
        if(ret == RET_OK)
        {
            LOG_Debug("InitParamsInterface success");
            break;
        }
        else
        {
            LOG_Debug("InitParamsInterface failed");
            sleep(1);
        }
    } while (ret != RET_OK);

    // CAN透传接口初始化
    do
    {
        g_can_fd = InitUpgradeConfigClient();
        if(-1 == g_can_fd)
        {
            LOG_Error("InitUpgradeConfigClient fail!\n");
            sleep(1);
        }      
    } while (g_can_fd == -1);

    do
    {
        ret = InitPmClient("TSP");
        if(ret == RET_OK)
        {
            LOG_Debug("InitPmClient success");
            break;
        }
        else
        {
            LOG_Debug("InitPmClient failed");
            sleep(1);
        }
    } while( ret != RET_OK );
}

int main(int argc, char *argv[])
{
    GMainLoop *loop = NULL;

    loop = g_main_loop_new(NULL, FALSE);

    /** 1. 初始化SDK接口 **/
    sdk_init();

    //连接MQTT服务器, 如果数据库中没有地址，则会运行regist去注册并且获取地址
    int ret = MqttConnToServer();
    if(ret != RET_OK)
    {
        LOG_Error("MqttConnToServer failed\n");
        return -1;
    }

    /** 运行激活流程 **/
    //while(is_tbox_activated() != RET_OK)
    {
        if(TspActivationHandler() == RET_OK)
        {
            ret = 1;
            LOG_Debug("tbox activated\n");
            SetParamsValue(enumHttpActived, &ret);
        }
        sleep(5);
    }

    /** 5. 运行MQTT接收线程 **/
    //pthread_t tid_tsp_request;
    //pthread_create(&tid_tsp_request, NULL, TspRequestHandler, NULL);
    //pthread_detach(tid_tsp_request);



    pthread_t tid_can_rx;
    //pthread_t tid_tsp_monitor;
    pthread_t tid_can_msg_mapping;

    pthread_create(&tid_can_rx, NULL, McuCanRxHandler, &g_can_fd);
    pthread_create(&tid_can_msg_mapping, NULL, McuCanMsgMapping, NULL);
    //pthread_create(&tid_tsp_monitor, NULL, TspMonitorHandler, NULL);
    

    //pthread_detach(tid_tsp_monitor);
    pthread_detach(tid_can_rx);
    pthread_detach(tid_can_msg_mapping);

    g_main_loop_run(loop);
    
    return 0;
}