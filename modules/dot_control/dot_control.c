/**
 **************************************************
 * @file dot_control.c
 * @author Berkay Özgür, C. Arda Sengenc
 * @version v1.0
 * @date 17.05.2023
 * @brief: Module for Dimming and Blinky dot function on the timer periphery on the microchip.
 @verbatim
 ==================================================
 ### Resources used ###

 GPIOD: D Channel for the periphery, 8-segment board.
 GPIOE: E Channel for the 4 dots on 8-segment board.
 TIM_HandleTypeDef: Timer1 Initializing instance
 TIM_OC_InitTypeDef: Timer Output Compare instance (PWM1)

 ==================================================
 ### Usage ###

 (#) Call "dot_control_init()" to initialise 8-segment board, potentiometer and the timer for this project.

 (#) Call "dot_control_change_dimming()" in main functions while-loop to change the frequency and the brightness of the dots.
 First potentiometer changes the blink speed of the dots, second potentiometer changes output compare
 for "brightness".

 (#) Call "dot_control_change_frequency()" in main functions while-loop to change the frequency of the dot.

 @endverbatim
 **************************************************
 */

/* Includes */
#include <dot_control.h>
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <potis_dma.h>
#include <utils.h>
#include <my_timer.h>

/* Preprocessor macros */
/* In order to meet the requirements given on the paper, we are required to create an Interval Value between 1 to 10 */
#define MAX_INTERVAL(x) (x*(10-1))
#define CORE_CLOCK(x) ((SystemCoreClock / (10000*x)) - 1)

/**
 * @brief Initializes the dimming control.
 *        This function initializes the necessary components for dimming control, including the timer,
 *        output compare channel, GPIO pins, potentiometer for AD-conversion, and other settings.
 *
 * @param none
 * @return none
 */
void dot_control_init() {
	HAL_Init();

	/**
	 * The instance of TIM_HandleTypeDef is called "tim_handle_struct" here.
	 * The Prescale field is not constant here. Therefore, an unsigned integer 32-bit variable
	 * named scaler_var is defined and assigned to the prescaler field when the timer is initialized.
	 * In order to get 10kHz, we need to assign to the prescaler 1600. (160000000 / 1600 = 10000)
	 */
	my_timer_init(TIMER_INSTANCE_1, TIMER_MODE_BASE, 10000, 10000);
	my_timer_oc_init(INSTANCE_PWM_1, MODE_OC, 5000, TIM_CHANNEL_2);

	/* CNTL1, CNTL2, CNTL3, CNTL4 are connected the pins PD14-PD15-PD0-PD1 */
	/* We need to initialize other pins too, so we can make sure the unwanted pins stay off  */
	utils_init_gpio(GPIOD,
			GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_1 | GPIO_PIN_0 | GPIO_PIN_7
					| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_12
					| GPIO_PIN_11, GPIO_MODE_OUTPUT_PP,
			GPIO_NOPULL, 0, GPIO_SPEED_MEDIUM);
	/* The digits where dots are connected, should be set to high to stay off */
	HAL_GPIO_WritePin(GPIOD,
			GPIO_PIN_7 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_12
					| GPIO_PIN_11, 1);
	HAL_GPIO_WritePin(GPIOD,
	GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1, 1);

	/* The corresponding pin must also be activated for the blinking dots. */
	utils_init_gpio(GPIOE, GPIO_PIN_11, GPIO_MODE_AF_PP, GPIO_NOPULL,
	GPIO_AF1_TIM1, GPIO_SPEED_MEDIUM);

	/* Starting the Timer Output Compare Channel */
	my_timer_start(MODE_OC, TIM_CHANNEL_2);

	/* Initializing the potentiometer for AD-conversion */
	potis_dma_init();
}

/**
 * @brief Changes the frequency and brightness for dimming control.
 * 		  This function reads the ADC values of two potentiometers and uses them to determine
 *        the desired frequency and brightness for dimming control.
 *        The first potentiometer value is used to calculate the frequency scaler,
 *        and the second potentiometer value is used to adjust the brightness of the LEDs.
 *
 * @param none
 * @return none
 */
void dot_control_change_dimming() {
	/* Reading the ADC value of the first potentiometer*/
	float poti_1_val = potis_dma_get_avg(POTIS_DMA_1);
	/* Reading the ADC value of the second potentiometer*/
	float poti_2_val = potis_dma_get_avg(POTIS_DMA_2);

	/* The value from potentiometer is maximum 1 */
	float new_scaler = poti_1_val / 4095.0;
	/* Setting the interval between 1Hz and 10Hz */
	float set_interval = MAX_INTERVAL(new_scaler) + 1.0;
	/* Setting the prescaler to the desired frequency */
	my_timer_set_prescaler(CORE_CLOCK(set_interval));
	/* Setting the compare unit to the desired frequency, in order to change
	 * the "brightness" of the LEDs */
	my_timer_set_compare(TIM_CHANNEL_2, (poti_2_val * 5000) / 4095);
}

/**
 * @brief Changes the frequency of the dot control based on the potentiometer value.
 *
 * @param none
 * @return none
 */
void dot_control_change_frequency() {
	/* Reading the ADC value of the first potentiometer*/
	float poti_1_get_val = potis_dma_get_avg(POTIS_DMA_1);

	/* The value from potentiometer is maximum 1 */
	float new_scaler = poti_1_get_val / 4095.0;

	/* Setting the interval between 1Hz and 10Hz */
	float set_interval = MAX_INTERVAL(new_scaler) + 1.0;

	/* Setting the prescaler to the desired frequency */
	my_timer_set_prescaler(CORE_CLOCK(set_interval));
}
