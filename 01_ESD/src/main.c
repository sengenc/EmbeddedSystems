/**
**************************************************
* @file main.c
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 28.04.2023
* @brief: Module for using the the 8 segment board.

==================================================
### Resources used ###
	(see esd.c)
==================================================
*/

/* Includes */
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <esd.h>

int main(void) {
	HAL_Init();
	esd_init();

	/* Initialization of the LCD */
	lcd_init();

	/* Draw text to LCD */
	lcd_draw_text_at_line("   Hello World!", 4, BLACK, 2, WHITE);
	lcd_draw_text_at_line("    Welcome to", 6, BLACK, 2, WHITE);
	lcd_draw_text_at_line("       ESD", 7, BLACK, 2, WHITE);

	/* In this loop we show a countdown on the 8 segment display.
	 * First loop is for the positions, we begin with the first one and iterate
	 * up to the last one.
	 *
	 * Second loop is for the digits. We begin with 9 and iterate down to 0. */
	while (1) {
		/* In the C language enums are interpreted as integers,
		 * so you can use them in the loops just as you would use use integers. */
		for (int i_pos = ESD_POSITION_1; i_pos <= ESD_POSITION_ALL; i_pos++) {
			for (int i_digit = ESD_DIGIT_9; i_digit >= ESD_DIGIT_0; i_digit--) {
				esd_show_digit(i_digit, i_pos);
				HAL_Delay(500);
			}
		}

	}
}
