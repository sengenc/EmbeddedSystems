/**
**************************************************
  * @file my_lcd.c
  * @author Berkay Özgür, C. Arda Sengenc
  * @version v2.0
  * @date 29.04.2023
  * @brief: Module for LCD Screen on the microchip.
@verbatim
==================================================
### Resources used ###
No Input | Output Channel has been used. LCD Screen is embedded on the chip.
==================================================
### Usage ###

(#) Call "HAL_Init();" to initialize the general chip commands.

(#) Call "lcd_init()" to initialize the module and all the
	necessary peripheries.

(#) Call "countdown()" to make a countdown,that counts from 10 to 1 every second.

(#) Call "my_lcd_draw_baargraph()" to create horizontal bar graph that imitates a "loading screen".

(#) Call "my_lcd_draw_x" Use it to draw a cross with the dimensions 100x100 pixels


@endverbatim
**************************************************
*/

/* Includes */
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include "my_lcd.h"
#include <stdio.h>

/**
  * @brief Function for counting down from 10 to 0 and display it to LCD screen that embedded on the chip.
  * @param no parameter has been used based on giving instructions, possible parameters had been hard coded.
  * @return none
  */
void my_lcd_countdown(int input) {
	char buffer[16];
	for (int i = input; i >= 0; i--) {
		lcd_fill_screen(WHITE);
		sprintf(buffer, "Zahl = %3d", i);
		lcd_draw_text_at_line(buffer, 4, BLACK, 2, WHITE);
		HAL_Delay(800);
	}
}

/**
  * @brief Function for creating horizontal bar graph. It can be imitated as loading screen when in loop called.
  * @param unsigned short x and y for the coordinates that gives the lower left corner, as starting point
  * @param unsigned short width and height are given for the width and height of the bar graph
  * @param unsigned short value for the bar graph
  * @param unsigned short variable color, for the color of the graph
  * @param unsigned short variable bgcolor, for the background color of the graph.
  * @return none
  */
void my_lcd_draw_baargraph(uint16_t x, uint16_t y, uint16_t width,
		uint16_t height, uint16_t value, uint16_t color, uint16_t bgcolor) {
	/* Input as lower left corner, so you can convert the y-coordinate with width, you don't have to change x value */
	int temp_x = x;
	int temp_y = y;
	int promille;
	/*
	 * If you have exactly the same value as x as width, nothing happens, because the width is used as the second x-coordinate.
	 * According to the question, we should not specify a second coordinate as width, but really the width.
	 * Therefore I added x with width, so that I always have a width, even if the two have the same value.
	 */
	int temp_width = x + width;
	if (height > y) {
		temp_y = 0;
	} else {
		temp_y = y - height;
	}
	int temp_height = temp_y + height;
	if(value < 1) {
		promille = 1;
	} else {
		promille = (value*width)/1000;
	}


	/*
	 * Initial idea was in order to update the screen, paint the screen completely with
	 * white and upload again with the current value.
     * That led to visible and annoying flickering on display. Thus, this  was mainly
     * caused because with each updating the screen we changed the value of green beam.
     * More elegant solution would be letting green
	 * beam as it in full length and fill it instead with white color.
	 * So we could have the image that green beam is growing or shrinking depends on the way.
	 *
	 * Our problem was that each time we painted the screen with white, thus something was changed.
	 * This leads to flickering on the display. This is solved by painting the other part of the bar with white.
	 */

	/* Red Frame, sets the outline of the bar grapf */
	ILI9341_Draw_Hollow_Rectangle_Coord(temp_x, temp_y, temp_width, temp_height, color);
	/* Green Beam, which will be placed into red bar graph */
	//ILI9341_Draw_Filled_Rectangle_Coord(temp_x, temp_y+1, x + promille, height, bgcolor);

	/*
	 * White bar. When the bar is supposed to be full, there is a problem between green and white bar.
	 * That is why white bar should not be shown on the display.
	 */
	if (promille != width) {
		if (promille>1) {
			ILI9341_Draw_Filled_Rectangle_Coord(temp_x+1, temp_y+1, x + promille, temp_height, bgcolor);
			ILI9341_Draw_Filled_Rectangle_Coord(temp_x + promille, temp_y+1, temp_width, temp_height, WHITE);
		}
	}
}

/**
  * @brief Function for drawing an X on the LCD screen
  * @param takes an integer value called size which defines the size of to be drawn X.
  * @return none
  */
void my_lcd_draw_x(int size) {
	/*
     * To draw an X, we need two lines crossing each other. One way would be making nested loops, whereas would result
     * higher complexity time, same effort can be reached with one for loop.
     * In the first lcd_draw_pixel() function-call first line will be drawn.
	 * In the second lcd_draw_pixel() function-call the parameters supposed to be different.
	 * X coordinate must start at the last value and be decreased by i at each iteration. by i at each iteration.
	 * Y-coordinate, on the other hand, must be increased by i as with the first.
	 */
	for (int i = 0; i <= size; i++) {
		lcd_draw_pixel(i, i, BLACK);
		lcd_draw_pixel(size - i, i, BLACK);
	}
}

