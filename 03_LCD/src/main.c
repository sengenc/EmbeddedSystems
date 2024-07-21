/**
**************************************************
* @file main.c
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 28.04.2023
* @brief: Module for LCD screen on the microchip.
*
* @usage: In order to use this module, you need to exclude the module "stopwatch" from
* 		  the build.
**************************************************
==================================================
### Resources used ###
	(see my_lcd.c)
==================================================
*/

/* Includes */
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <my_lcd.h>


int main(void) {
	HAL_Init();

	/* Initialization of the LCD */
	lcd_init();

	/* Variables for countdown (and for the text on LCD) */
	char buffer[16];
	int input = 20;
	while (1) {

		/* This for loop combines the countdown with our horizontal bar graph */
		for (int i = input; i >= 0; i--) {
			my_lcd_draw_baargraph(10, 40, 200, 35, i*50, RED, GREEN);
			sprintf(buffer, "Zahl = %2d", i);
			lcd_draw_text_at_line(buffer, 4, BLACK, 2, WHITE);
			HAL_Delay(800);
		}
		/* Function call for horizontal bar graph */
		//my_lcd_draw_baargraph(10, 40, 200, 35, 750, RED, GREEN);
	}
}
