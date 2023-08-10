#ifndef _MCU_CAN_MSG_MAPPING_H_
#define _MCU_CAN_MSG_MAPPING_H_

#include <stdint.h>

#define MAPPING_MODE_REALTIME 0
#define MAPPING_MODE_JSON 1

typedef struct 
{
    uint8_t driver_door;                    //DrvrDoorOpenSts    | (0x46A)GW_Connected_BCM_FrP04  | start_bit:46 | bit_length:2 | Initial value:0 | factor:1 
    uint8_t passenger_door;                 //FrtPsngDoorOpebSts | (0x46A)GW_Connected_BCM_FrP04  | start_bit:42 | bit_length:2 | Initial value:0 | factor:1 
    uint8_t rear_left_door;                 //RLDoorOpenSts      | (0x46A)GW_Connected_BCM_FrP04  | start_bit:49 | bit_length:2 | Initial value:0 | factor:1
    uint8_t rear_right_door;                //RRDoorOpenSts      | (0x46A)GW_Connected_BCM_FrP04  | start_bit:40 | bit_length:2 | Initial value:0 | factor:1
    uint8_t boot_status;                    //LdspcOpenSts       | (0x46A)GW_Connected_BCM_FrP04  | start_bit:32 | bit_length:2 | Initial value:0 | factor:1
    uint8_t bonnet_status;                  //BntOpenSts         | (0x46A)GW_Connected_BCM_FrP04  | start_bit:44 | bit_length:2 | Initial value:0 | factor:1
    uint8_t lock_status;                    //VehLckngSta        | (0x46A)GW_Connected_BCM_FrP04  | start_bit:53 | bit_length:3 | Initial value:7 | factor:1
    uint8_t driver_window;                  //DrvrWndDclnSpc     | (0x21D)GW_Connected_BCM_FrP09  | start_bit:56 | bit_length:8 | Initial value:127.5 | factor:0.5
    uint8_t passenger_window;               //TODO 没找到
    uint8_t rear_left_window;               //TODO 没找到
    uint8_t rear_right_window;              //TODO 没找到
    uint8_t sunroof_status;                 //TODO 没找到
    uint8_t front_right_tyre_pressure;      //FRTirePrs          | (0x47D)GW_Connected_TPMS_FrP01 | start_bit:40 | bit_length:7 | Initial value:220 | factor:4
    uint8_t front_left_tyre_pressure;       //FLTirePrs          | (0x47D)GW_Connected_TPMS_FrP01 | start_bit:56 | bit_length:7 | Initial value:220 | factor:4
    uint8_t rear_right_tyre_pressure;       //RRTirePrs          | (0x47D)GW_Connected_TPMS_FrP01 | start_bit:8  | bit_length:7 | Initial value:220 | factor:4
    uint8_t rear_left_tyre_pressure;        //RLTirePrs          | (0x47D)GW_Connected_TPMS_FrP01 | start_bit:24 | bit_length:7 | Initial value:220 | factor:4
    uint8_t wheel_tyre_monitor_status;      //TODO 没找到
    uint8_t side_light_status;              //VehSideLghtSts     | (0x46A)GW_Connected_BCM_FrP04  | start_bit:51 | bit_length:2 | Initial value:0 | factor:1
    uint8_t dipped_beam_status;             //DipdBeamLghtOn     | (0x46A)GW_Connected_BCM_FrP04  | start_bit:63 | bit_length:1 | Initial value:0 | factor:1
    uint8_t main_beam_status;               //MainBeamLghtOn     | (0x46A)GW_Connected_BCM_FrP04  | start_bit:36 | bit_length:1 | Initial value:0 | factor:1
    uint8_t vehicle_alarm_status;           //TODO 没找到
    uint8_t engine_status;                  //TODO 没找到
    uint8_t power_mode;                     //SysPwrMd           | (0x1F1)GW_WF_FrP01             | start_bit:0  | bit_length:2 | Initial value:0 | factor:1
    uint16_t last_key_seen;                 //TODO 没找到
    uint16_t current_journey_distance;      //TODO 没找到
    uint32_t current_journey_id;            //TODO 没找到
    int8_t interior_temperature;            //ACInCarTem         | (0x361)GW_Connected_AC_FrP01   | start_bit:56 | bit_length:8 | Initial value:3.5 | factor:0.5
    int8_t exterior_temperature;            //OtsdAirTemCrVal    | (0x3F1)GW_WF_FrP10             | start_bit:56 | bit_length:8 | Initial value:-40 | factor:0.5
    uint8_t fuel_level_prc;                 //FuelLvlPcnt        | (0x416)GW_Connected_IPK_FrP03  | start_bit:40 | bit_length:8 | Initial value:0   | factor:0.392157
    uint16_t fuel_range;                    //VehElecRng         | (0x2A2)GW_Connected_BMS_FrP11  | start_bit:56 | bit_length:11| Initial value:2046 | factor:1
    uint8_t remote_climate_status;          //RmtACSts           | (0x361)GW_Connected_AC_FrP01   | start_bit:48 | bit_length:3 | Initial value:0   | factor:1
    uint8_t front_left_seat_heat_level;     //TODO 没找到
    uint8_t front_right_seat_heat_level;    //TODO 没找到
    uint8_t can_bus_active;                 //TODO 自行获取,不需要提供信号
    uint32_t time_of_last_can_bus_activity; //TODO 自行获取,不需要提供信号
    uint8_t clastr_dspd_fuel_lvl_sgmt;
    uint32_t mileage;                       //VehOdo             | (0x540)GW_WF_FrP08             | start_bit:16 | bit_length:24 | Initial value:0   | factor:1
    uint16_t battery_voltage;               //SysVol             | (0x1F1)GW_WF_FrP01             | start_bit:40 | bit_length:8  | Initial value:0   | factor:0.1   
    uint8_t hand_brake;                     //BrkPdlPos          | (0xF1)GW_WF_FrP02              | start_bit:8  | bit_length:8  | Initial value:0   | factor:0.392157
    uint8_t veh_elec_rng_dsp;               //TODO 没找到，待确认
    uint16_t fuel_range_elec;
    uint8_t rmt_htd_rr_wnd_st;
    uint32_t extended_data_1;
    uint32_t extended_data_2;
} mcu_vehicle_status_t;

typedef struct 
{
    uint8_t cluster_display_alernator_warning;
    uint8_t fr_tire_sts; //FRTireSts | (0x47E)GW_Connected_TPMS_FrP02   | start_bit:2 | bit_length:3 | Initial value:0   | factor:1
    uint8_t fl_tire_sts; //FLTireSts | (0x47E)GW_Connected_TPMS_FrP02   | start_bit:5 | bit_length:3 | Initial value:0   | factor:1
    uint8_t rl_tire_sts; //FLTireSts | (0x47E)GW_Connected_TPMS_FrP02   | start_bit:13 | bit_length:3 | Initial value:0   | factor:1
    uint8_t rr_tire_sts; //FLTireSts | (0x47E)GW_Connected_TPMS_FrP02   | start_bit:10 | bit_length:3 | Initial value:0   | factor:1
    uint8_t tyre_pressure_status; //ClstrDspdTyrePrsSts | (0x417)GW_Connect_IPK_FrP09 | start_bit:34 | bit_length:2 | Initial value:0   | factor:1
    uint8_t oil_pressure_low_warning; //TODO 没有信号
    uint8_t left_brake_light_warning;
    uint8_t right_brake_light_warning;
    uint8_t left_direction_indication_light_warning;
    uint8_t right_direction_indication_light_warning;
    uint8_t left_dipped_beam_light_warning;
    uint8_t right_dipped_beam_light_warning;
    uint8_t advanced_front_lighting_system_warning;
    uint8_t stability_control_system_warning;
    uint8_t traction_control_system_warning;
    uint8_t antilock_brake_system_warning;
    uint8_t coolant_temperature_warning;
    uint8_t electrical_brake_distribution_warning;
    uint8_t fuel_status;
    uint8_t fuel_sensor_warning;
    uint8_t stop_start_warning;
    uint8_t stop_start_button_warning;
    uint8_t hill_descent_control_warning;
    uint8_t electric_power_steering_warning; // ClstrDspdEPSWrong | (0x417)GW_Connected_IPS_FrP09   | start_bit:56 | bit_length:2 | Initial value:0   | factor:1
    uint8_t steering_angle_sensor_warning;
    uint8_t steering_angle_sensor_calibration_warning;
    uint8_t airbag_warning; // ClstrDspdAirbagWrnng | (0x417)GW_Connected_IPK_FrP09   | start_bit:48 | bit_length:2 | Initial value:0   | factor:1
    uint8_t invalid_key_warning;
    uint8_t security_key_battery_low_warning;
    uint8_t clutch_switch_warning;
    uint8_t hill_hold_warning;
    uint8_t autonomous_emergency_braking_system_warning;
    uint8_t ignition_relay_warning;
    uint8_t washer_fluid_low_warning;
    uint8_t attery_replacement_required_warning;
    uint8_t clutch_overheated_warning;
    uint8_t park_distance_control_warning;
    uint8_t transmission_warning;
    uint8_t collision_alarm;
    uint8_t alfunction_indicator_light_warning;
    uint8_t engine_drive_by_wire_warning;
    uint8_t electronic_parking_brake_warning;
    uint8_t cruise_control_warning;
    uint8_t over_speed_alarm;
    uint8_t left_side_light_warning;
    uint8_t right_side_light_warning;
    uint8_t day_time_running_lamp_warning;
    uint8_t rear_fog_light_warning;
    uint8_t reverse_lamp_warning;
    uint8_t autohold_warning;
    uint8_t transfer_case_control_module_warning;
    uint8_t passive_entry_warning;
    uint8_t lane_departure_warning_system_fault;
    uint8_t adaptive_cruise_control_system_warning;
    uint8_t front_view_camera_module_sensor_warning;
    uint8_t electric_powertrain_warning;
    uint8_t high_voltage_direct_current_direct_current_charge_warning;
    uint8_t battery_management_system_warning;
    uint8_t oil_change_request_warning;
    uint8_t illegal_invasion_alarm;
    uint8_t consumer_geofence_alert;
    uint8_t telematics_failure_warning;
} mcu_vehicle_alert_t;

int get_mcu_vehicle_status(mcu_vehicle_status_t *mcu_vehicle_status);

int get_mcu_vehicle_alert(mcu_vehicle_alert_t *mcu_vehicle_alert);

void set_mapping_mode(int mode);

void *McuCanMsgMapping(void *arg);

#endif