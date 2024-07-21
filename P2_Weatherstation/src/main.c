/**
 ******************************************************************************
 * @file    	main.c
 * @author		Berkay Özgür, C. Arda Sengenc
 * @version 	V1.0
 * @date		14.07.2023
 * @brief  	    This file contains the main entry point of the program for reading
 * 				desired data from our sensor. It initializes the Bosch BME280 sensor and
 *              enters a main loop where it continuously updates the status of each
 *              sensor data.
 ******************************************************************************
 */

/* Includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include "bme280.h"
#include "utils.h"
#include "env_sensor.h"

/* Module Variables */
float main_temperature;
float main_humidity;
float main_pressure;
char hum_string[50];
char temp_string[50];
char press_string[50];


/*
 * @brief In the main function, the sequence of initializing BME280 sensor.
 * 		  Then, the env_sensor_get_value function is called to retrieve the sensor data.
 * 		  If the sensor data retrieval is successful, the temperature, humidity, and pressure values are formatted as strings using sprintf.
 * 		  These formatted strings are then displayed on the LCD using the lcd_draw_text_at_line function.
 */
int main(void) {
	/* Initialization of the module */
	env_sensor_init();

	/* Initialization of the LCD */
	lcd_init();

	while (1) {
		/* Get the value of temperature from the sensor */
		main_temperature = env_sensor_get_value(ENV_TEMPERATURE);
		/* Formatting it as a string */
		sprintf(temp_string, "Temperature %03.1f ", main_temperature);
		lcd_draw_text_at_line(temp_string, 2, BLACK, 2, WHITE);

		/* Get the value of humidity from the sensor */
		main_humidity = env_sensor_get_value(ENV_HUMIDITY);
		/* Formatting it as a string */
		sprintf(hum_string, "Humidity %03.1f ", main_humidity);
		lcd_draw_text_at_line(hum_string, 4, BLACK, 2, WHITE);

		/* Get the value of pressure from the sensor */
		main_pressure = env_sensor_get_value(ENV_PRESSURE);
		sprintf(press_string, "Pressure %03.1f ", main_pressure/100);
		lcd_draw_text_at_line(press_string, 6, BLACK, 2, WHITE);

	}
}
