/**
 **************************************************
 * @file main.c
 * @author Berkay Özgür, C. Arda Sengenc
 * @version v1.1
 * @date 17.05.2023
 * @brief: Module for BlinkyDot function with the timer periphery on the microchip.
 *
 * @usage: In order to use this module, you need to exclude the module "stopwatch" from
* 		   the build.
 @verbatim
 ==================================================
 ### Resources used ###
 (see dot_control.c)

 ==================================================

 @endverbatim
 **************************************************
 */

/* Includes */
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <potis_dma.h>
#include <stdio.h>
#include <my_timer.h>
#include <utils.h>
#include <dot_control.h>


int main(void) {
	/* Initializes all necessary peripherals for controlling frequency and brightness of LEDs. */
	dot_control_init();

	while (1) {
		/* Changes the frequency of the LEDs. */
		dot_control_change_frequency();
	}
}
