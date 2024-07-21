/**
 ******************************************************************************
 * @file    	main.c
 * @author		Berkay Özgür, C. Arda Sengenc
 * @version 	V1.0
 * @date		25.06.2019
 * @brief       This file contains the main entry point of the program for controlling
 *              and monitoring the fan speed. It initializes the fan control module and
 *              enters a main loop where it continuously updates the fan status and sets
 *              the target RPM.
 ******************************************************************************
 ==================================================
 ### Resources used ###
 (see fan_control.c)
 ==================================================
 */

/* Includes */
#include "stm32f4xx.h"
#include <fan_control.h>
#include <lcd/lcd.h>
#include <stdio.h>


int main(void) {
	/* Initialize fan control module */
	fan_control_init();

	// for displaying target RPM value
	char target_rpm_string[32];

	//for displaying time interval between fan rotations
	char interval_string[32];

	//for displaying current fan rpm.
	char current_rpm_string[32];

	/* Main loop that continuously controls and monitors the fan speed */
	while (1) {
		/* Show fan status on display */

		// Format the target RPM value as a string
		sprintf(target_rpm_string, "Target RPM = %5lu", fan_control_poti_val);
		// Display the target RPM value on the LCD at line 2
		lcd_draw_text_at_line(target_rpm_string, 2, BLACK, 2, WHITE);

		// Format the time interval between fan rotations as a string
		sprintf(interval_string, "Interval : %5lu", fan_control_time_interval);
		// Display the time interval on the LCD at line 4
		lcd_draw_text_at_line(interval_string, 4, BLACK, 2, WHITE);

		// Format the current fan RPM as a string
		sprintf(current_rpm_string, "RPM : %5lu", fan_control_actual_RPM);
		// Display the current fan RPM on the LCD at line 6
		lcd_draw_text_at_line(current_rpm_string, 6, BLACK, 2, WHITE);

		/* Set target RPM based on potentiometer value */
		fan_control_set_rpm();
	}
	/* Program should never reach this point */
}

