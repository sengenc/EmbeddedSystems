/**
**************************************************
* @file main.c
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 28.04.2023
* @brief: Module for using the LEDs.

==================================================
### Resources used ###
GPIO:PG13, PG14
==================================================
*/

/* Includes */
#include <lcd/lcd.h>
#include <stdio.h>
#include "stm32f4xx.h"


int main(void)
{
	HAL_Init();

	/* Initialization for the Pin(s), which we want to activate.*/
	/* Clock Control, clock of GPIO-Port-G must be activated before we use it. */
	__HAL_RCC_GPIOG_CLK_ENABLE();
	/* Creating the configuration structure. */
	GPIO_InitTypeDef gpio_init;
	/* Both pins, that we use PG13: green, PG14: red */
	gpio_init.Pin = GPIO_PIN_13 | GPIO_PIN_14;
	/* Configuring pins as output */
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_MEDIUM;
	/* Initializing ports. */
	HAL_GPIO_Init(GPIOG, &gpio_init);

	/* Initialization of the LCD */
	lcd_init();

	/* A value to monitor in debug mode. */
	uint8_t val = 0;

	/* Draw text to LCD */
	lcd_draw_text_at_line("   Hello World!", 4, BLACK, 2, WHITE);
	lcd_draw_text_at_line("  This is fifth!", 5, BLACK, 2, WHITE);
	lcd_draw_text_at_line("    Welcome to", 6, BLACK, 2, WHITE);
	lcd_draw_text_at_line("       PES", 7, BLACK, 2, WHITE);


	while(1) {
		/* GPIO_PIN_RESET and GPIO_PIN_SET are defined as enum,
		 * so we can also use integers 0 and 1 instead. */
		/* The activation of both LEDs
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, 1);
		HAL_Delay(500);
		// The deactivation of both LEDs
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, 0);
		HAL_Delay(500); */

		/* First activate the green one, then toggle the two.
		 * (with delay so we can see what's going on) */
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, 1);
		HAL_Delay(500);
		HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14);
		HAL_Delay(500);
	}
}
