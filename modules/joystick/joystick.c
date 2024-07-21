/**
**************************************************
  * @file esd.c
  * @author Berkay Özgür, C. Arda Sengenc
  * @version v1.0
  * @date 28.04.2023
  * @brief: Module for using joystick.
@verbatim
==================================================
### Resources used ###
GPIO: PG6, PG9, PG10, PG11, PG12
==================================================
### Usage ###

(#) Call "joystick_init()" to initialize the module and all the
	necessary peripheries.

(#) Call "joystick_read_dir(input)" to read the direction (ipnut)
	from the joystick.

@endverbatim
**************************************************
*/

/* Includes */
#include <joystick.h>
#include "stm32f4xx.h"
#include <utils.h>

/* According to the wiring diagram and jumpers, joystick should be connected to the G pins.
 * They are   A-PG6,
 * 			  B-PG9,
 * 		   	  C-PG10,
 * 			  D-PG11,
 * 			  PRESS-PG12. */

/* Public functions */

/**
  * @brief Initializes the module and all the necessary periphery
  * @param None
  * @return None
  */
void joystick_init(){
	HAL_Init();

	__HAL_RCC_GPIOG_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_g;
	gpio_init_g.Pin = GPIO_PIN_6 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11
			| GPIO_PIN_12;
	/* Configuring pins as input */
	gpio_init_g.Mode = GPIO_MODE_INPUT;
	/* The joystick is connected in the board as PULLDOWN. Therefore it must be
	 * initialized as PULLUP, so that you notice a difference when you move the joystick.
	 * For this we need if-statements, which control if the level sends 0-signal.*/
	gpio_init_g.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOG, &gpio_init_g);
}

/**
  * @brief Function for reading direction (pins) we want
  * @param input direction (port) we want to use
  * @return 0 if the joystick was moved
  */
int joystick_read_dir(joystick_direction_t input) {
	/* Variable for storing the input */
	int read_output = 1;
	switch (input) {
	case JOYSTICK_A:
		/* For the usage of this function, see utils.c */
		read_output = utils_gpio_port_read(GPIOG, GPIO_PIN_6);
		break;

	case JOYSTICK_B:
		read_output = utils_gpio_port_read(GPIOG, GPIO_PIN_9);
		break;

	case JOYSTICK_C:
		read_output = utils_gpio_port_read(GPIOG, GPIO_PIN_10);
		break;

	case JOYSTICK_D:
		read_output = utils_gpio_port_read(GPIOG, GPIO_PIN_11);
		break;

	case JOYSTICK_PRESS:
		read_output = utils_gpio_port_read(GPIOG, GPIO_PIN_12);
		break;
	}

	return read_output;
}
