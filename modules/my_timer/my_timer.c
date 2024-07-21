/**
 **************************************************
 * @file my_timer.c
 * @author Berkay Özgür, C. Arda Sengenc
 * @version v1.0
 * @date 19.06.2023
 * @brief: Helper module for initialising and using timers.
 ******************************************************************************
 * @verbatim
 * ### Description ###
 * This file contains the implementation of a timer control module, which provides
 * functions for initializing and configuring timers, enabling interrupts, setting
 * compare values, starting timers, and retrieving timer handle structures. It also
 * includes functions for initializing and configuring timer output compare channels.
 *
 * @note This file depends on the HAL library and the HAL_TIM module.
 * The my_timer_handle_struct and my_timer_oc_handle_struct variables are used internally
 * for storing the timer handle and output compare handle structures, respectively.
 *
 * ### Usage ###
 *
 * (#) Call "my_timer_init()" to initialize the timer and set the desired parameters.
 *
 * (#) Call "my_timer_enable_interrupt()" to enable interrupts for the timer.
 *
 * (#) Call "my_timer_oc_init()" to initialize and configure the output compare channel.
 *
 * (#) Call "my_timer_start()" to start the timer in the desired mode and channel.
 *
 * (#) Call "my_timer_set_prescaler()" to set the prescaler value for the timer.
 *
 * (#) Call "my_timer_set_compare()" to set the compare value for the timer channel.
 *
 * (#) Call "my_timer_get_struct()" to retrieve the timer handle structure.
 *
 * (#) Call "my_timer_get_oc_struct()" to retrieve the output compare handle structure.
 *
 * @endverbatim
 ******************************************************************************
 */

/* Includes */
#include "stm32f4xx.h"
#include <my_timer.h>

/* Module variables */
TIM_HandleTypeDef my_timer_handle_struct;
TIM_OC_InitTypeDef my_timer_oc_handle_struct;

/**
 * @brief Initializes a timer with the specified parameters.
 *
 * This function initializes a timer with the specified parameters by performing the following steps:
 * - Calls HAL_Init() to initialize the HAL library.
 * - Configures the timer instance, clock source, prescaler, period, counter mode, clock division, auto-reload preload,
 *   and repetition counter based on the provided parameters.
 * - Enables the clock for the corresponding timer instance (TIM1, TIM2, or TIM3).
 * - Initializes the timer with the configured handle structure using either HAL_TIM_Base_Init() or HAL_TIM_PWM_Init()
 *   depending on the specified mode.
 *
 * @param instance The timer instance to be initialized (TIMER_INSTANCE_1, TIMER_INSTANCE_2, or TIMER_INSTANCE_3).
 * @param mode The mode of operation for the timer (TIMER_MODE_BASE or TIMER_MODE_PWM).
 * @param prescaler The prescaler value to divide the timer clock.
 * @param period The period value to determine the time before the timer resets.
 *
 * @return None
 */
void my_timer_init(timer_instance instance, timer_mode mode, uint32_t prescaler,
		uint32_t period) {

	// Initialize the HAL library.
	HAL_Init();
	switch (instance) {
	case TIMER_INSTANCE_1:
		__HAL_RCC_TIM1_CLK_ENABLE();
		my_timer_handle_struct.Instance = TIM1;

		/* Upper defined prescaler will be assigned here. */
		my_timer_handle_struct.Init.Prescaler = (SystemCoreClock / prescaler)
				- 1;
		/**
		 * This allows you to set the value to which the timer will count. (how much time elapses)
		 * before it is reset and starts again from the beginning. Assuming that the prescaler has been
		 * set as described above so that the timer is supplied with a 10 kHz clock, a setting of Period = 9999
		 * (i.e. 10000-1) would cause the counter to always count from 0-9999 and then reset to 0.
		 */
		my_timer_handle_struct.Init.Period = period - 1;

		/* Counter mode is assigned to count up */
		my_timer_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
		my_timer_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		my_timer_handle_struct.Init.AutoReloadPreload =
		TIM_AUTORELOAD_PRELOAD_DISABLE;
		/* Here you specify after how many overflows an event should be generated */
		my_timer_handle_struct.Init.RepetitionCounter = 0;
		break;

	case TIMER_INSTANCE_2:
		/* Clock for Timer2 */
		__HAL_RCC_TIM2_CLK_ENABLE();
		my_timer_handle_struct.Instance = TIM2;

		/* Upper defined prescaler will be assigned here. */
		/* Timer2 has a different maximum clock, so it must be divided by two again. */
		my_timer_handle_struct.Init.Prescaler =
				(SystemCoreClock / 2 / prescaler) - 1;
		/**
		 * This allows you to set the value to which the timer will count. (how much time elapses)
		 * before it is reset and starts again from the beginning. Assuming that the prescaler has been
		 * set as described above so that the timer is supplied with a 10 kHz clock, a setting of Period = 9999
		 * (i.e. 10000-1) would cause the counter to always count from 0-9999 and then reset to 0.
		 */
		my_timer_handle_struct.Init.Period = period - 1;

		/* Counter mode is assigned to count up */
		my_timer_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
		my_timer_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		my_timer_handle_struct.Init.AutoReloadPreload =
		TIM_AUTORELOAD_PRELOAD_DISABLE;
		/* Here you specify after how many overflows an event should be generated */
		my_timer_handle_struct.Init.RepetitionCounter = 0;
		break;

	case TIMER_INSTANCE_3:
		/* Clock for Timer3 */
		__HAL_RCC_TIM3_CLK_ENABLE();
		my_timer_handle_struct.Instance = TIM3;

		/* Upper defined prescaler will be assigned here. */
		/* Timer3 has a different maximum clock, so it must be divided by two again. */
		my_timer_handle_struct.Init.Prescaler =
				(SystemCoreClock / 2 / prescaler) - 1;
		/**
		 * This allows you to set the value to which the timer will count. (how much time elapses)
		 * before it is reset and starts again from the beginning. Assuming that the prescaler has been
		 * set as described above so that the timer is supplied with a 10 kHz clock, a setting of Period = 9999
		 * (i.e. 10000-1) would cause the counter to always count from 0-9999 and then reset to 0.
		 */
		my_timer_handle_struct.Init.Period = period - 1;

		/* Counter mode is assigned to count up */
		my_timer_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
		my_timer_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		my_timer_handle_struct.Init.AutoReloadPreload =
		TIM_AUTORELOAD_PRELOAD_DISABLE;
		/* Here you specify after how many overflows an event should be generated */
		my_timer_handle_struct.Init.RepetitionCounter = 0;
		break;
	}

	if (mode == TIMER_MODE_BASE) {
		/* Initialization of the Timer for base. */
		HAL_TIM_Base_Init(&my_timer_handle_struct);
	} else if (mode == TIMER_MODE_PWM) {
		/* Initialization of the Timer for PWM. */
		HAL_TIM_PWM_Init(&my_timer_handle_struct);
	}
}

/**
 * @brief Enables interrupts for the specified timer.
 *
 * This function enables interrupts for the specified timer by calling __HAL_TIM_ENABLE_IT()
 * with the timer handle structure and the TIM_IT_UPDATE flag as arguments.
 * This allows the timer to generate interrupt events on update.
 *
 * @param none
 * @return none
 */
void my_timer_enable_interrupt() {
	__HAL_TIM_ENABLE_IT(&my_timer_handle_struct, TIM_IT_UPDATE);
}

/**
 * @brief Initializes the output compare mode for the specified timer channel.
 *
 * This function initializes the output compare mode for the specified timer channel based on the provided parameters.
 * The function performs the following steps:
 * - Configures the output compare mode, pulse value, idle state, polarity, and idle state for the output compare negative
 *   (OCN) channel based on the specified instance.
 * - Calls either HAL_TIM_OC_ConfigChannel() or HAL_TIM_PWM_ConfigChannel() depending on the specified mode to configure
 *   the timer channel with the configured output compare handle structure.
 *
 * @param instance The output compare instance to be initialized (INSTANCE_PWM_1 or INSTANCE_PWM_2).
 * @param mode The mode of operation for the output compare (MODE_OC or MODE_PWM).
 * @param pulse The pulse value for the output compare mode.
 * @param channel The timer channel to be configured.
 *
 * @return none
 */
void my_timer_oc_init(timer_oc_instance instance, timer_oc_mode mode,
		uint32_t pulse, uint32_t channel) {
	switch (instance) {
	case INSTANCE_PWM_1:
		// Configure output compare mode for PWM1
		my_timer_oc_handle_struct.OCMode = TIM_OCMODE_PWM1;
		/* Must be in period, compare-value*/
		my_timer_oc_handle_struct.Pulse = pulse;
		my_timer_oc_handle_struct.OCIdleState = TIM_OCIDLESTATE_SET;
		/* Here we have set it to low. This is because the pins are activated at low level. */
		my_timer_oc_handle_struct.OCPolarity = TIM_OCPOLARITY_LOW;
		my_timer_oc_handle_struct.OCNIdleState = TIM_OCNIDLESTATE_RESET;
		my_timer_oc_handle_struct.OCNPolarity = TIM_OCNPOLARITY_HIGH;
		break;

	case INSTANCE_PWM_2:
		// Configure output compare mode for PWM2
		my_timer_oc_handle_struct.OCMode = TIM_OCMODE_PWM2;
		/* Must be in period, compare-value*/
		my_timer_oc_handle_struct.Pulse = pulse;
		my_timer_oc_handle_struct.OCIdleState = TIM_OCIDLESTATE_SET;
		/* Here we have set it to low. This is because the pins are activated at low level. */
		my_timer_oc_handle_struct.OCPolarity = TIM_OCPOLARITY_LOW;
		my_timer_oc_handle_struct.OCNIdleState = TIM_OCNIDLESTATE_RESET;
		my_timer_oc_handle_struct.OCNPolarity = TIM_OCNPOLARITY_HIGH;
		break;
	}

	if (mode == MODE_OC) {
		// Configure timer channel with output compare handle structure for output compare mode
		HAL_TIM_OC_ConfigChannel(&my_timer_handle_struct,
				&my_timer_oc_handle_struct, channel);
	} else if (mode == MODE_PWM) {
		// Configure timer channel with output compare handle structure for PWM mode
		HAL_TIM_PWM_ConfigChannel(&my_timer_handle_struct,
				&my_timer_oc_handle_struct, channel);
	}

}

/**
 * @brief Starts the specified timer channel based on the mode of operation.
 *
 * This function starts the specified timer channel based on the provided mode of operation.
 * The function checks the mode and calls either HAL_TIM_OC_Start() or HAL_TIM_PWM_Start()
 * to start the timer channel.
 *
 * @param mode The mode of operation for the timer channel (MODE_OC or MODE_PWM).
 * @param channel The timer channel to be started.
 *
 * @return none
 */
void my_timer_start(timer_oc_mode mode, uint32_t channel) {
	if (mode == MODE_OC) {
		// Start the timer channel in output compare mode
		HAL_TIM_OC_Start(&my_timer_handle_struct, channel);
	} else if (mode == MODE_PWM) {
		// Start the timer channel in PWM mode
		HAL_TIM_PWM_Start(&my_timer_handle_struct, channel);
	}
}

/**
 * @brief Sets the prescaler value for the timer.
 *
 * This function sets the prescaler value for the timer based on the provided value.
 * It uses the __HAL_TIM_SET_PRESCALER macro to update the prescaler value of the timer handle.
 *
 * @param value The prescaler value to be set.
 *
 * @return none
 */
void my_timer_set_prescaler(uint32_t value) {
	__HAL_TIM_SET_PRESCALER(&my_timer_handle_struct, value);
}

/**
 * @brief Sets the compare value for the specified timer channel.
 *
 * This function sets the compare value for the specified timer channel based on the provided
 * channel and value. It uses the __HAL_TIM_SET_COMPARE macro to update the compare value
 * of the timer handle.
 *
 * @param channel The timer channel for which the compare value should be set.
 * @param value The compare value to be set.
 *
 * @return none
 */
void my_timer_set_compare(uint32_t channel, uint32_t value) {
	__HAL_TIM_SET_COMPARE(&my_timer_handle_struct, channel, value);
}

/**
 * @brief Retrieves the timer handle structure.
 *
 * This function returns the timer handle structure, which contains the configuration and
 * state information of the timer. It simply returns the `my_timer_handle_struct` variable.
 *
 * @return The timer handle structure.
 */
TIM_HandleTypeDef my_timer_get_struct() {
	return my_timer_handle_struct;
}

/**
 * @brief Retrieves the timer output compare handle structure.
 *
 * This function returns the timer output compare handle structure, which contains the configuration
 * and state information of the timer output compare channel. It simply returns the `my_timer_oc_handle_struct` variable.
 *
 * @return The timer output compare handle structure.
 *
 */
TIM_OC_InitTypeDef my_timer_get_oc_struct() {
	return my_timer_oc_handle_struct;
}

