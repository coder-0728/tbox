#include <malloc.h>
#include <glib.h>
#include <string.h>
#include <unistd.h>

#include "mcu_can_rx_handle.h"

#include "log.h"
#include "error_code.h"

G_LOCK_DEFINE_STATIC(g_canMsgLock);

static can_frame_t g_can_frame;

int get_can_frame(can_frame_t *can_frame)
{
    if(NULL == can_frame)
    {
        return RET_ERR;
    }

    G_LOCK(g_canMsgLock);
    memcpy(can_frame, &g_can_frame, sizeof(can_frame_t));
    G_UNLOCK(g_canMsgLock);

    return RET_OK;
}

static int set_can_frame(const ConfigQueueData *recv_data)
{
    
    int count = 0;

    if(count > CONFIG_MAX_LEN)
    {
        LOG_Debug("recv Len IS too long !!!\n");
        return RET_ERR;
    }

    G_LOCK(g_canMsgLock);
    g_can_frame.size = recv_data->data.msg.signalLen;
    for(count = 0; count < recv_data->data.msg.signalLen; count++)
    {
        memcpy(g_can_frame.data[count].data, &(recv_data->data.msg.signalValue[count].value),8);
        g_can_frame.data[count].signalIndex = recv_data->data.msg.signalValue[count].signalIndex;
        g_can_frame.data[count].channelCanId = recv_data->data.msg.signalValue[count].channelCanId;
    }
    G_UNLOCK(g_canMsgLock);

    return RET_OK;
}

static int config_pentrate_data_handle(const ConfigQueueData *recv_data)
{   
    if(NULL == recv_data)
    {
        return RET_ERR;
    }

    if(CONFIG_TYPE_IS_PENETRATE != recv_data->type)
    {
        return RET_ERR;
    }

    return set_can_frame(recv_data);
}

void *McuCanRxHandler(void *arg)
{
    LOG_Debug("pthread_mcu_handle\n");

    int can_fd = *(int *)arg;
    int ret = RET_ERR;

    /* 只订阅透传数据 */
    SubscriptionConfigMsg(can_fd, CONFIG_TYPE_IS_PENETRATE);

    ConfigQueueData *recv_data = NULL;
    do
    {
        recv_data = (ConfigQueueData *)malloc(sizeof(ConfigQueueData) + CONFIG_MAX_LEN);
        if(NULL == recv_data)
        {
            LOG_Error("can recv data malloc fail!\n");
            g_usleep(G_USEC_PER_SEC * 3);
        }
    }while(NULL == recv_data);


    while(1)
    {
        /* Config Server 固定400ms 连续发送一次所有类型数据 */
        memset((gchar *)recv_data, 0, sizeof(ConfigQueueData) + CONFIG_MAX_LEN);
        ret = GetConfigMsg(can_fd, recv_data, 400);
        if(ret < 0)
        {
            LOG_Error("Get Config Data Error, Break for cycle");
            continue;
        }

        config_pentrate_data_handle(recv_data);
        
        usleep(100000);
    }
}