/**
**************************************************
* @file main.c
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 28.04.2023
* @brief: Module for controlling the the 8 segment board with joystick.
*
* @usage: In order to use this module, you need to exclude the module "stopwatch" from
* 		  the build.

==================================================
### Resources used ###
	(see esd.c and joystick.c)
==================================================
*/

/* According to the wiring diagram and jumpers, joystick should be connected to the G pins.
 * They are   A-PG6,
 * 			  B-PG9,
 * 		   	  C-PG10,
 * 			  D-PG11,
 * 			  PRESS-PG12. */

/* Includes */
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <joystick.h>
#include <esd.h>

int main(void) {
	HAL_Init();
	joystick_init();
	esd_init();
	/* According to the question, the number 0 and the position 1 should be
	 * at the beginning.*/
	esd_show_digit(ESD_DIGIT_0, ESD_POSITION_1);

	/* Since we don't have an order of how the numbers and positions change,
	 * it makes sense to store them in variables. This way we have a better overview as
	 * soon as we move the joystick.*/
	int current_digit = ESD_DIGIT_0;
	int current_pos = ESD_POSITION_1;

	/* Initialization of the LCD */
	lcd_init();

	/* Draw text to LCD */
	lcd_draw_text_at_line("   Hello World!", 4, BLACK, 2, WHITE);
	lcd_draw_text_at_line("    Welcome to", 6, BLACK, 2, WHITE);
	lcd_draw_text_at_line("    Joystick!", 7, BLACK, 2, WHITE);

	while (1) {
		/* If we move the joystick without delay, we can quickly lose track,
		 * so moving once can quickly be read 2 or 3 times. That's why we use
		 * delays to keep track. */

		/* if instructions for reading the directions */
		if (joystick_read_dir(JOYSTICK_A) == 0) {
			if (current_digit == ESD_DIGIT_0) {
				current_digit = ESD_DIGIT_9;
			} else {
				current_digit--;
			}
			esd_show_digit(current_digit, current_pos);
			HAL_Delay(200);
		}
		if (joystick_read_dir(JOYSTICK_B) == 0) {
			if (current_pos == ESD_POSITION_1) {
				current_pos = ESD_POSITION_4;
			} else {
				current_pos--;
			}
			esd_show_digit(current_digit, current_pos);
			HAL_Delay(200);
		}
		if (joystick_read_dir(JOYSTICK_C) == 0) {
			if (current_pos == ESD_POSITION_4) {
				current_pos = ESD_POSITION_1;
			} else {
				current_pos++;
			}
			esd_show_digit(current_digit, current_pos);
			HAL_Delay(200);
		}

		if (joystick_read_dir(JOYSTICK_D) == 0) {
			if (current_digit == ESD_DIGIT_9) {
				current_digit = ESD_DIGIT_0;
			} else {
				current_digit++;
			}
			esd_show_digit(current_digit, current_pos);
			HAL_Delay(200);
		}

		/* when we press the joystick, a countdown starts*/
		if (joystick_read_dir(JOYSTICK_PRESS) == 0) {
			/* if the current digit is not 0, we begin to
			 * count the digit down */
			if(current_digit != ESD_DIGIT_0) {
				for (int i_digit = current_digit; i_digit >= ESD_DIGIT_0; i_digit--) {
					esd_show_digit(i_digit, current_pos);
					HAL_Delay(1000);
				}
			}
			/* At the end of the loop we set the digit back to its previous value */
			esd_show_digit(current_digit, current_pos);
			HAL_Delay(100);
		}

	}
}
