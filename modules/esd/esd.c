/**
**************************************************
  * @file esd.c
  * @author Berkay Özgür, C. Arda Sengenc
  * @version v1.0
  * @date 28.04.2023
  * @brief: Module for using periphery 8 segment board.
@verbatim
==================================================
### Resources used ###
GPIO: PD14-PD15-PD0-PD1, PD7-PD4-PD5-PD6-PD12-PD11
==================================================
### Usage ###

(#) Call "esd_init()" to initialize the module and all the
	necessary peripheries.

(#) Call "esd_show_digit(digit, pos)" to show a digit at the
	desired position.

@endverbatim
**************************************************
*/


/* Includes */
#include "stm32f4xx.h"
#include <esd.h>

/* Public functions */

/**
  * @brief Initializes the module and all the necessary periphery
  * @param None
  * @return None
  */
void esd_init() {
	HAL_Init();

	/* Activating the G pins
	 * CNTL1, CNTL2, CNTL3, CNTL4 are connected the pins PD14-PD15-PD0_PD1.
	 * a, b, c, d, e, f are connected the pins PD7-PD4-PD5-PD6-PD12-PD11. */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_d;
	gpio_init_d.Pin = GPIO_PIN_All;
	gpio_init_d.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_d.Pull = GPIO_NOPULL;
	gpio_init_d.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOD, &gpio_init_d);

	/* Activating E pins */
	/* g, dot and point are connected the pins PE12-PE11-PE7. */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_e;
	gpio_init_e.Pin = GPIO_PIN_12 | GPIO_PIN_11 | GPIO_PIN_7;
	gpio_init_e.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_e.Pull = GPIO_NOPULL;
	gpio_init_e.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOE, &gpio_init_e);
}

/**
 * If you activate one of the CNTLs, the digit will turn on.
 * So you should leave the digit pattern low, that you want to see and turn on the pattern
 * that should be dark.*/

/* PD7 = a
 * PD4 = b
 * PD5 = c
 * PD6 = d
 * PD12 = e
 * PD11 = f
 * PE12 = g
 * PE11 = dot
 * PE7 = point*/


/**
  * @brief Displaying the digit and the position
  * @param digit the digit we want to see in 8 segment board
  * @param pos the position for the digit, whicht we want to see
  * @return none
  */
void esd_show_digit(esd_digit_t digit, esd_position_t pos) {
	/* Set all pins to high so that it resets at each number. */
	HAL_GPIO_WritePin(GPIOD,
			GPIO_PIN_7 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_12
					| GPIO_PIN_11, 1);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 1);
	HAL_GPIO_WritePin(GPIOD,
			GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1, 0);
	/* Encapsulated functions for displaying the number and position.*/
	draw_position(pos);
	draw_digit(digit);
}


/*
 * PIN14, first position
 * PIN15, second position
 * PIN0, third position
 * PIN1, fourth position*/

/* GPIO_PIN_RESET and GPIO_PIN_SET are defined as enum,
 * so we can also use integers 0 and 1 instead. */

/**
  * @brief Turning on the desired position
  * @param input position, which we want to activate
  * @return none
  */
void draw_position(esd_position_t input) {
	/* CNTL1*/
	if (input == ESD_POSITION_1) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 1);
	}
	/* CNTL2*/
	if (input == ESD_POSITION_2) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 1);
	}
	/* CNTL3*/
	if (input == ESD_POSITION_3) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, 1);
	}
	/* CNTL4*/
	if (input == ESD_POSITION_4) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 1);
	}
	/* All positions */
	if (input == ESD_POSITION_ALL) {
		HAL_GPIO_WritePin(GPIOD,
				GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1, 1);
	}
}

/**
  * @brief Turning on the digit, self explanatory
  * @param input digit, which we want to activate
  * @return none
  */
void draw_digit(esd_digit_t input) {
	/* Pins are set to low so that the digit is written. */
	if (input == ESD_DIGIT_0) {
		HAL_GPIO_WritePin(GPIOD,
				GPIO_PIN_7 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_12
						| GPIO_PIN_11, 0);
	}

	if (input == ESD_DIGIT_1) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4 | GPIO_PIN_5, 0);
	}

	if (input == ESD_DIGIT_2) {
		HAL_GPIO_WritePin(GPIOD,
				GPIO_PIN_7 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_12, 0);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
	}

	if (input == ESD_DIGIT_3) {
		HAL_GPIO_WritePin(GPIOD,
				GPIO_PIN_7 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_5, 0);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
	}

	if (input == ESD_DIGIT_4) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11, 0);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
	}

	if (input == ESD_DIGIT_5) {
		HAL_GPIO_WritePin(GPIOD,
				GPIO_PIN_7 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_11, 0);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
	}

	if (input == ESD_DIGIT_6) {
		HAL_GPIO_WritePin(GPIOD,
				GPIO_PIN_7 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_12 | GPIO_PIN_11,
				0);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
	}

	if (input == ESD_DIGIT_7) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7 | GPIO_PIN_4 | GPIO_PIN_5, 0);
	}

	if (input == ESD_DIGIT_8) {
		HAL_GPIO_WritePin(GPIOD,
				GPIO_PIN_7 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_12
						| GPIO_PIN_11, 0);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
	}

	if (input == ESD_DIGIT_9) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_11, 0);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
	}

}

