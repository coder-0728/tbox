#include "mcu_can_msg_mapping.h"
#include "mcu_can_rx_handle.h"

#include "log.h"
#include "error_code.h"
#include "cJSON.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>

#define GET_BIT(num, pos) (((num) >> (pos)) & 1)
#define SET_BIT(num, pos, bit) ((num) = ((num) & ~(1 << (pos))) | ((bit) << (pos)))

G_LOCK_DEFINE_STATIC(g_statusMsgLock);
G_LOCK_DEFINE_STATIC(g_alertMsgLock);

static mcu_vehicle_alert_t g_mcu_vehicle_alert = {0};
static int g_mapping_mode = MAPPING_MODE_REALTIME;

void set_mapping_mode(int mode)
{
    g_mapping_mode = mode;
}

static uint32_t get_bits_from_array(uint8_t *arr, int start_bit, int length)
{
    uint32_t result = 0;

    for (int i = 0; i < length; i++) {
        int byte_index = (start_bit + i) / 8;
        int bit_index = (start_bit + i) % 8;

        uint8_t byte = arr[byte_index];
        uint32_t bit_value = (byte >> bit_index) & 1;

        result |= (bit_value << i);
    }

    return result;
}

int get_mcu_vehicle_status(mcu_vehicle_status_t *mcu_vehicle_status)
{
    if(mcu_vehicle_status == NULL)
    {
        return RET_ERR;
    }

    //解析JSON文件中的数据
    if(g_mapping_mode == MAPPING_MODE_JSON)
    {
        FILE* file = fopen("path/to/your/file.json", "r");
        if (file == NULL) 
        {
            // 文件打开失败，处理错误
            return RET_ERR;
        }

        // 计算文件大小
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        // 分配内存来存储文件内容
        char* json_buffer = (char*)malloc(file_size + 1);
        if (json_buffer == NULL) 
        {
            return RET_ERR;
        }

        // 读取文件内容到缓冲区
        size_t read_size = fread(json_buffer, 1, file_size, file);
        if (read_size != file_size) 
        {
            free(json_buffer);
            json_buffer = NULL;
            return RET_ERR;
        }

        // 添加字符串结束符
        json_buffer[file_size] = '\0';

        // 关闭文件
        fclose(file);

        cJSON* root = cJSON_Parse(json_buffer);
        if (root == NULL) 
        {
            // JSON解析失败，处理错误
            const char* error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL) 
            {
                // 打印错误位置
                LOG_Debug("Error before: %s\n", error_ptr);
            }
            // 释放内存并退出
            free(json_buffer);
            json_buffer = NULL;
            return RET_ERR;
        }

        free(json_buffer);
        json_buffer = NULL;

        mcu_vehicle_status->driver_door = cJSON_GetObjectItem(root, "driver_door")->valueint;
        mcu_vehicle_status->passenger_door = cJSON_GetObjectItem(root, "passenger_door")->valueint;
        mcu_vehicle_status->rear_left_door = cJSON_GetObjectItem(root, "rear_left_door")->valueint;
        mcu_vehicle_status->rear_right_door = cJSON_GetObjectItem(root, "rear_right_door")->valueint;
        mcu_vehicle_status->boot_status = cJSON_GetObjectItem(root, "boot_status")->valueint;
        mcu_vehicle_status->bonnet_status = cJSON_GetObjectItem(root, "bonnet_status")->valueint;
        mcu_vehicle_status->lock_status = cJSON_GetObjectItem(root, "lock_status")->valueint;
        mcu_vehicle_status->driver_window = cJSON_GetObjectItem(root, "driver_window")->valuedouble;
        mcu_vehicle_status->passenger_window = cJSON_GetObjectItem(root, "passenger_window")->valuedouble;
        mcu_vehicle_status->rear_left_window = cJSON_GetObjectItem(root, "rear_left_window")->valuedouble;
        mcu_vehicle_status->rear_right_window = cJSON_GetObjectItem(root, "rear_right_window")->valuedouble;
        mcu_vehicle_status->sunroof_status = cJSON_GetObjectItem(root, "sunroof_status")->valueint;
        mcu_vehicle_status->front_right_tyre_pressure = cJSON_GetObjectItem(root, "front_right_tyre_pressure")->valueint;
        mcu_vehicle_status->front_left_tyre_pressure = cJSON_GetObjectItem(root, "front_left_tyre_pressure")->valueint;
        mcu_vehicle_status->rear_right_tyre_pressure = cJSON_GetObjectItem(root, "rear_right_tyre_pressure")->valueint;
        mcu_vehicle_status->rear_left_tyre_pressure = cJSON_GetObjectItem(root, "rear_left_tyre_pressure")->valueint;
        mcu_vehicle_status->wheel_tyre_monitor_status = cJSON_GetObjectItem(root, "wheel_tyre_monitor_status")->valueint;
        mcu_vehicle_status->side_light_status = cJSON_GetObjectItem(root, "side_light_status")->valueint;
        mcu_vehicle_status->dipped_beam_status = cJSON_GetObjectItem(root, "dipped_beam_status")->valueint;
        mcu_vehicle_status->main_beam_status = cJSON_GetObjectItem(root, "main_beam_status")->valueint;
        mcu_vehicle_status->vehicle_alarm_status = cJSON_GetObjectItem(root, "vehicle_alarm_status")->valueint;
        mcu_vehicle_status->engine_status = cJSON_GetObjectItem(root, "engine_status")->valueint;
        mcu_vehicle_status->power_mode = cJSON_GetObjectItem(root, "power_mode")->valueint;
        mcu_vehicle_status->last_key_seen = cJSON_GetObjectItem(root, "last_key_seen")->valueint;
        mcu_vehicle_status->current_journey_distance = cJSON_GetObjectItem(root, "current_journey_distance")->valueint;
        mcu_vehicle_status->current_journey_id = cJSON_GetObjectItem(root, "current_journey_id")->valueint;
        mcu_vehicle_status->interior_temperature = cJSON_GetObjectItem(root, "interior_temperature")->valuedouble;
        mcu_vehicle_status->exterior_temperature = cJSON_GetObjectItem(root, "exterior_temperature")->valuedouble;
        mcu_vehicle_status->fuel_level_prc = cJSON_GetObjectItem(root, "fuel_level_prc")->valueint;
        mcu_vehicle_status->fuel_range = cJSON_GetObjectItem(root, "fuel_range")->valueint;
        mcu_vehicle_status->remote_climate_status = cJSON_GetObjectItem(root, "remote_climate_status")->valueint;
        mcu_vehicle_status->front_left_seat_heat_level = cJSON_GetObjectItem(root, "front_left_seat_heat_level")->valueint;
        mcu_vehicle_status->front_right_seat_heat_level = cJSON_GetObjectItem(root, "front_right_seat_heat_level")->valueint;
        mcu_vehicle_status->can_bus_active = cJSON_GetObjectItem(root, "can_bus_active")->valueint;
        mcu_vehicle_status->time_of_last_can_bus_activity = cJSON_GetObjectItem(root, "time_of_last_can_bus_activity")->valueint;
        mcu_vehicle_status->clastr_dspd_fuel_lvl_sgmt = cJSON_GetObjectItem(root, "clastr_dspd_fuel_lvl_sgmt")->valueint;
        mcu_vehicle_status->mileage = cJSON_GetObjectItem(root, "mileage")->valueint;
        mcu_vehicle_status->battery_voltage = cJSON_GetObjectItem(root, "battery_voltage")->valueint;
        mcu_vehicle_status->hand_brake = cJSON_GetObjectItem(root, "hand_brake")->valueint;
        mcu_vehicle_status->veh_elec_rng_dsp = cJSON_GetObjectItem(root, "veh_elec_rng_dsp")->valueint;
        mcu_vehicle_status->fuel_range_elec = cJSON_GetObjectItem(root, "fuel_range_elec")->valueint;
        mcu_vehicle_status->rmt_htd_rr_wnd_st = cJSON_GetObjectItem(root, "rmt_htd_rr_wnd_st")->valueint;
        mcu_vehicle_status->extended_data_1 = cJSON_GetObjectItem(root, "extended_data_1")->valueint;
        mcu_vehicle_status->extended_data_2 = cJSON_GetObjectItem(root, "extended_data_2")->valueint;
    
        cJSON_Delete(root);
        root = NULL;

        return RET_OK;
    }

    can_frame_t can_frame;
    int ret = RET_ERR;
    ret = get_can_frame(&can_frame);
    if(RET_OK != ret)
    {
        LOG_Debug("get_can_frame failed\n");
        return RET_ERR;
    }

    G_LOCK(g_statusMsgLock);
    for(int i=0; i < can_frame.size; i++)
    {
        switch(can_frame.data[i].channelCanId)
        {
            case GW_CONNECTED_BCM_FRP04:
                mcu_vehicle_status->driver_door        = get_bits_from_array(can_frame.data[i].data, 46, 2) * 1;
                mcu_vehicle_status->passenger_door     = get_bits_from_array(can_frame.data[i].data, 42, 2) * 1;
                mcu_vehicle_status->rear_left_door     = get_bits_from_array(can_frame.data[i].data, 49, 2) * 1;
                mcu_vehicle_status->rear_right_door    = get_bits_from_array(can_frame.data[i].data, 40, 2) * 1;
                mcu_vehicle_status->boot_status        = get_bits_from_array(can_frame.data[i].data, 32, 2) * 1;
                mcu_vehicle_status->bonnet_status      = get_bits_from_array(can_frame.data[i].data, 44, 2) * 1;
                mcu_vehicle_status->lock_status        = get_bits_from_array(can_frame.data[i].data, 53, 2) * 1;
                mcu_vehicle_status->side_light_status  = get_bits_from_array(can_frame.data[i].data, 51, 2) * 1;
                mcu_vehicle_status->dipped_beam_status = get_bits_from_array(can_frame.data[i].data, 63, 1) * 1;
                mcu_vehicle_status->main_beam_status   = get_bits_from_array(can_frame.data[i].data, 36, 1) * 1;
#ifdef DEBUG
                LOG_Debug("driver_door: %d\n", mcu_vehicle_status->driver_door);
                LOG_Debug("passenger_door: %d\n", mcu_vehicle_status->passenger_door);
                LOG_Debug("rear_left_door: %d\n", mcu_vehicle_status->rear_left_door);
                LOG_Debug("rear_right_door: %d\n", mcu_vehicle_status->rear_right_door);
                LOG_Debug("boot_status: %d\n", mcu_vehicle_status->boot_status);
                LOG_Debug("bonnet_status: %d\n", mcu_vehicle_status->bonnet_status);
                LOG_Debug("lock_status: %d\n", mcu_vehicle_status->lock_status);
                LOG_Debug("side_light_status: %d\n", mcu_vehicle_status->side_light_status);
                LOG_Debug("dipped_beam_status: %d\n", mcu_vehicle_status->dipped_beam_status);
                LOG_Debug("main_beam_status: %d\n", mcu_vehicle_status->main_beam_status);
#endif
                break;
            case GW_Connected_TPMS_FrP01:
                mcu_vehicle_status->front_right_tyre_pressure = get_bits_from_array(can_frame.data[i].data, 40, 7) * 4;
                mcu_vehicle_status->front_left_tyre_pressure  = get_bits_from_array(can_frame.data[i].data, 56, 7) * 4;
                mcu_vehicle_status->rear_right_tyre_pressure  = get_bits_from_array(can_frame.data[i].data,  8, 7) * 4;
                mcu_vehicle_status->rear_left_tyre_pressure   = get_bits_from_array(can_frame.data[i].data, 24, 7) * 4;
#ifdef DEBUG
                LOG_Debug("front_right_tyre_pressure: %d\n", mcu_vehicle_status->front_right_tyre_pressure);
                LOG_Debug("front_left_tyre_pressure: %d\n", mcu_vehicle_status->front_left_tyre_pressure);
                LOG_Debug("rear_right_tyre_pressure: %d\n", mcu_vehicle_status->rear_right_tyre_pressure);
                LOG_Debug("rear_left_tyre_pressure: %d\n", mcu_vehicle_status->rear_left_tyre_pressure);
#endif
                break;
            case GW_Connected_BCM_FrP09:
                mcu_vehicle_status->driver_window = get_bits_from_array(can_frame.data[i].data, 56, 8) * 0.5;
#ifdef DEBUG
                LOG_Debug("driver_window: %d\n", mcu_vehicle_status->driver_window);
#endif
                break;
            case GW_WF_FrP01:
                mcu_vehicle_status->power_mode = get_bits_from_array(can_frame.data[i].data, 0, 2) * 1;
#ifdef DEBUG
                LOG_Debug("power_mode: %d\n", mcu_vehicle_status->power_mode);
#endif
                break;
            case GW_Connected_AC_FrP01:
                mcu_vehicle_status->interior_temperature  = get_bits_from_array(can_frame.data[i].data, 56, 8) * 0.5;
                mcu_vehicle_status->remote_climate_status = get_bits_from_array(can_frame.data[i].data, 48, 3) * 1;
#ifdef DEBUG
                LOG_Debug("interior_temperature: %d\n", mcu_vehicle_status->interior_temperature);
                LOG_Debug("remote_climate_status: %d\n", mcu_vehicle_status->remote_climate_status);
#endif
                break;
            case GW_WF_FrP10:
                mcu_vehicle_status->exterior_temperature = get_bits_from_array(can_frame.data[i].data, 56, 8) * 0.5;
#ifdef DEBUG
                LOG_Debug("exterior_temperature: %d\n", mcu_vehicle_status->exterior_temperature);
#endif
                break;
            case GW_Connected_IPK_FrP03:
                mcu_vehicle_status->fuel_level_prc = get_bits_from_array(can_frame.data[i].data, 40, 8) * 0.392157;
#ifdef DEBUG
                LOG_Debug("fuel_level_prc: %d\n", mcu_vehicle_status->fuel_level_prc);
#endif
                break;
            case GW_Connected_BMS_FrP11:
                mcu_vehicle_status->fuel_range = get_bits_from_array(can_frame.data[i].data, 56, 11) * 1;
#ifdef DEBUG
                LOG_Debug("fuel_range: %d\n", mcu_vehicle_status->fuel_range);
#endif
                break;
            
        }
    }
    G_UNLOCK(g_statusMsgLock);
    LOG_Debug("get_mcu_vehicle_status\n");
    return RET_OK;
}

int get_mcu_vehicle_alert(mcu_vehicle_alert_t *mcu_vehicle_alert)
{
    LOG_Debug("get_mcu_vehicle_alert\n");
    G_LOCK(g_alertMsgLock);
    mcu_vehicle_alert = &g_mcu_vehicle_alert;
    G_UNLOCK(g_alertMsgLock);
    return RET_ERR;
}

/**
 * @brief McuCanMsgMapping
 * 设计阶段设计的该线程，实际上调用函数直接获取即可，不需要线程
 * 但是为了保持接口一致性，暂时保留
*/
void *McuCanMsgMapping(void *arg)
{
    while(1)
    {
        can_frame_t can_frame;
        int ret = RET_ERR;
        ret = get_can_frame(&can_frame);
        if(RET_OK != ret)
        {
            LOG_Debug("get_can_frame failed\n");
            usleep(400 * 1000);
            continue;
        }

        G_LOCK(g_alertMsgLock);
        for(int i=0; i < can_frame.size; i++)
        {
            switch(can_frame.data[i].channelCanId)
            {
                case GW_Connected_TPMS_FrP02:
                    g_mcu_vehicle_alert.fr_tire_sts = get_bits_from_array(can_frame.data[i].data, 2, 3) * 1;
                    g_mcu_vehicle_alert.fl_tire_sts = get_bits_from_array(can_frame.data[i].data, 5, 3) * 1;
                    g_mcu_vehicle_alert.rl_tire_sts = get_bits_from_array(can_frame.data[i].data, 13, 3) * 1;
                    g_mcu_vehicle_alert.rr_tire_sts = get_bits_from_array(can_frame.data[i].data, 10, 3) * 1;

#ifdef DEBUG
                    LOG_Debug("g_mcu_vehicle_alert.fr_tire_sts = %d\n", g_mcu_vehicle_alert.fr_tire_sts);
                    LOG_Debug("g_mcu_vehicle_alert.fl_tire_sts = %d\n", g_mcu_vehicle_alert.fl_tire_sts);
                    LOG_Debug("g_mcu_vehicle_alert.rl_tire_sts = %d\n", g_mcu_vehicle_alert.rl_tire_sts);
                    LOG_Debug("g_mcu_vehicle_alert.rr_tire_sts = %d\n", g_mcu_vehicle_alert.rr_tire_sts);
#endif
                    break;
                case GW_Connect_IPK_FrP09:
                    g_mcu_vehicle_alert.tyre_pressure_status = get_bits_from_array(can_frame.data[i].data, 34, 2) * 1;
                    g_mcu_vehicle_alert.electric_power_steering_warning = get_bits_from_array(can_frame.data[i].data, 56, 2) * 1;
                    g_mcu_vehicle_alert.airbag_warning = get_bits_from_array(can_frame.data[i].data, 48, 2) * 1;

#ifdef DEBUG
                    LOG_Debug("g_mcu_vehicle_alert.tyre_pressure_status = %d\n", g_mcu_vehicle_alert.tyre_pressure_status);
                    LOG_Debug("g_mcu_vehicle_alert.electric_power_steering_warning = %d\n", g_mcu_vehicle_alert.electric_power_steering_warning);
                    LOG_Debug("g_mcu_vehicle_alert.airbag_warning = %d\n", g_mcu_vehicle_alert.airbag_warning);
#endif
                    break;
            }
        }
        G_UNLOCK(g_alertMsgLock);

        usleep(400 * 1000);
    }
}