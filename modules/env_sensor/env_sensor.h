/**
**************************************************
* @file env_sensor.h
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 14.07.2023
* @brief: module for using Bosch BME280 Sensor.
**************************************************
*/

#ifndef ENV_SENSOR_ENV_SENSOR_H_
#define ENV_SENSOR_ENV_SENSOR_H_

/* Public enums as shortcuts for each sensor */
typedef enum {
	ENV_TEMPERATURE,
	ENV_HUMIDITY,
	ENV_PRESSURE,
} env_sensor_t;

/* Public functions (prototypes)*/
void env_sensor_init(void);
float env_sensor_get_value(env_sensor_t input);


#endif /* ENV_SENSOR_ENV_SENSOR_H_ */
