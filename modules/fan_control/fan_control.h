/**
**************************************************
* @file fan_control.h
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 20.06.2023
* @brief: Module for using periphery fan.
**************************************************
*/

#ifndef FAN_CONTROL_FAN_CONTROL_H_
#define FAN_CONTROL_FAN_CONTROL_H_

/* Public functions (prototypes )*/
void fan_control_init();
void fan_control_set_rpm();
void fan_control_show_status();

/* Public variables (for printing in main.c)*/
extern volatile uint32_t fan_control_time_interval;
extern volatile uint32_t fan_control_actual_RPM;
extern volatile uint32_t fan_control_poti_val;

#endif /* FAN_CONTROL_FAN_CONTROL_H_ */
