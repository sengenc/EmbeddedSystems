/**
 **************************************************
 * @file main.c
 * @author Berkay Özgür, C. Arda Sengenc
 * @version v1.1
 * @date 17.06.2023
 * @brief: Module for Dimming Dot function on the timer periphery on the microchip.
 *
 * @usage: In order to use this module, you need to exclude the module "stopwatch" from
* 		   the build.
 @verbatim
 ==================================================
 ### Resources used ###

 (see dot_control.c)

 @endverbatim
 **************************************************
 */

/* Includes */
#include <dot_control.h>
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <potis_dma.h>
#include <utils.h>
#include <stdio.h>
#include <my_timer.h>


int main(void) {
	//Initializes the dimming control.
	dot_control_init();

	while (1) {
		/* Changes the frequency and brightness of the LEDs based on the potentiometer values. */
		dot_control_change_dimming();

	}
}
