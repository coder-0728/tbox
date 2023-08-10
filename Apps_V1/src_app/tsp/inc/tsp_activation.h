#ifndef _TSP_ACTIVATION_H_
#define _TSP_ACTIVATION_H_

/**
 * @brief 判断是否已激活
*/
int is_tbox_activated(void);

/**
 * @brief 判断是否满足自动激活条件
 * 
*/
int is_meet_active_conditions(void);

/**
 * @brief 注册iot，获取MQTT连接信息
 * 
*/
int iot_regist();

/**
 * @brief 激活处理流程
 * 
*/
int TspActivationHandler();

#endif