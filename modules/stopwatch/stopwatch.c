/**
 **************************************************
 * @file stopwatch.c
 * @author Berkay Özgür, C. Arda Sengenc
 * @version v1.0
 * @date 23.05.2023
 * @brief: stopwatch.c module for using our microchip as a stopwatch with the user button.
 @verbatim
 ==================================================
 ### Resources used ###
 GPIO: GPIOA_PIN_0 for button
 TIMER: Timer 2 for precise counting.
 EXTI: EXTIO Interrupt Request Handler in order to send interrupt signal when button is pressed.
 TIM2_IRQHandler: Interrupt Request Handler for Timer 2 so it will send an interrupt signal each second (10Khz), so we will be able
 to capture time.

 ==================================================
 ### Usage ###

 (#) Call "stopwatch_init" to initialize the module and all the
 necessary peripheries.

 (#) Call "stopwatch_start" at the main-function in while loop for using
 our "stopwatch".

 @endverbatim
 **************************************************
 */
/* Includes */
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <stdio.h>
#include <my_timer.h>
#include <stopwatch.h>

/* Module functions (prototypes) */
void EXTI0_IRQHandler(void);
void TIM2_IRQHandler(void);
static void LCD_DisplayTime();
static void stopwatch_init_timer();
static void stopwatch_enable_interrupt();
static void stopwatch_enable_button();

/* Module variables */
TIM_HandleTypeDef timer_stopwatch_handle_struct;

/* The variable "start_flag" acts as a flag and checks to see if the timer has been started or not. */
/* Volatile tells the compiler that the value of the variable may change at any time*/
volatile uint8_t start_flag = 0;

/* Auxiliary variables for the configuration and the calculation of the time */
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;
volatile uint32_t milliseconds = 0;

/* When the key is pressed and an external interrupt is triggered,
 * this variable is incremented by one value to indicate where to write next on LCD. */
volatile uint8_t line_num = 1;

/* Buffer array for printing numbers on the LCD-screen */
char buf[32];

/**
 * @brief Initializes the stopwatch.
 *
 * This function initializes the stopwatch by performing the following steps:
 * - Initializes the timer.
 * - Initializes the LCD display.
 * - Enables interrupts for the stopwatch.
 * - Enables button functionality.
 *
 * @note This function calls the following overridden functions:
 * - EXTI0_IRQHandler() to handle external interrupt 0.
 * - TIM2_IRQHandler() to handle timer 2 interrupt.
 *
 * @param None
 * @return None
 */
void stopwatch_init() {
	stopwatch_init_timer(); // Initialize the timer.

	lcd_init(); // Initialize the LCD display.

	stopwatch_enable_interrupt(); // Enable interrupts for the stopwatch.

	stopwatch_enable_button(); // Enable button functionality.

	/* Calling the overridden functions */
	EXTI0_IRQHandler(); // Call the overridden EXTI0_IRQHandler() function to handle external interrupt 0.

	TIM2_IRQHandler(); // Call the overridden TIM2_IRQHandler() function to handle timer 2 interrupt.
}

/**
 * @brief Starts the stopwatch.
 *
 * This function starts the stopwatch and updates the LCD display if the start flag is set to 1.
 * It retrieves the current milliseconds value from the timer and formats it along with minutes and seconds.
 * The formatted string is then displayed on the LCD.
 *
 * @note This function relies on the start_flag, timer_stopwatch_handle_struct, milliseconds, minutes,
 *       seconds, buf, and lcd_draw_text_at_line() variables.
 *
 * @param None
 * @return None
 */
void stopwatch_start() {
	if (start_flag == 1) {
		milliseconds = __HAL_TIM_GET_COUNTER(&timer_stopwatch_handle_struct);

		// Format the time string with minutes, seconds, and milliseconds
		sprintf(buf, "%2d:%2d:%4lu", minutes, seconds, milliseconds);

		// Update the LCD display with the formatted time string
		lcd_draw_text_at_line(buf, 0, BLACK, 2, WHITE);
	}
}

/**
 * @brief Initializes the timer for the stopwatch.
 *
 * This function initializes the timer for the stopwatch by performing the following steps:
 * - Calls HAL_Init() to initialize the HAL library.
 * - Initializes Timer2 with a specified mode, period, and prescaler using the my_timer_init() function.
 * - Retrieves the timer handle structure using the my_timer_get_struct() function.
 * - Calls HAL_TIM_Base_Init() to initialize the timer with the retrieved handle structure.
 *
 * @note This function relies on the HAL library, my_timer_init(), my_timer_get_struct(), and the
 *       timer_stopwatch_handle_struct variable.
 *
 * @param None
 * @return None
 */
static void stopwatch_init_timer() {
    HAL_Init(); // Initialize the HAL library.

    // Initialize Timer2 with a specified mode, period, and prescaler using my_timer_init() function.
    my_timer_init(TIMER_INSTANCE_2, TIMER_MODE_BASE, 10000, 10000);

    // Retrieve the timer handle structure using my_timer_get_struct() function.
    timer_stopwatch_handle_struct = my_timer_get_struct();

    // Initialize the timer with the retrieved handle structure using HAL_TIM_Base_Init().
    HAL_TIM_Base_Init(&timer_stopwatch_handle_struct);
}


/**
 * @brief Enables interrupts for the stopwatch timer.
 *
 * This function enables interrupts for the stopwatch timer by performing the following steps:
 * - Sets the interrupt priority for Timer2 using the HAL_NVIC_SetPriority() function.
 * - Enables the interrupts for Timer2 using the HAL_NVIC_EnableIRQ() function.
 * - Enables the interrupt update event for the stopwatch timer using the __HAL_TIM_ENABLE_IT() function.
 *
 * @note This function relies on the HAL_NVIC_SetPriority(), HAL_NVIC_EnableIRQ(), and the
 *       timer_stopwatch_handle_struct variable.
 *
 * @param None
 * @return None
 */
static void stopwatch_enable_interrupt() {
    // Set the interrupt priority for Timer2 using HAL_NVIC_SetPriority().
    HAL_NVIC_SetPriority(TIM2_IRQn, 1, 6);

    // Enable the interrupts for Timer2 using HAL_NVIC_EnableIRQ().
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    // Enable the interrupt update event for the stopwatch timer using __HAL_TIM_ENABLE_IT().
    __HAL_TIM_ENABLE_IT(&timer_stopwatch_handle_struct, TIM_IT_UPDATE);
}


/**
 * @brief Enables interrupts for the user button.
 *
 * This function enables interrupts for the user button by performing the following steps:
 * - Initializes GPIO pin 0 of GPIOA as a falling edge-triggered interrupt using the utils_init_gpio() function.
 * - Sets the interrupt priority for the EXTI0_IRQn (external interrupt 0) using the HAL_NVIC_SetPriority() function.
 * - Enables the interrupts for the user button using the HAL_NVIC_EnableIRQ() function.
 *
 *
 * @note This function relies on the utils_init_gpio(), HAL_NVIC_SetPriority(), and the EXTI0_IRQn variables.
 *
 * @param None
 * @return None
 */
static void stopwatch_enable_button() {
    // Initialize GPIO pin 0 of GPIOA as a falling edge-triggered interrupt using utils_init_gpio().
    utils_init_gpio(GPIOA, GPIO_PIN_0, GPIO_MODE_IT_FALLING, GPIO_NOPULL, NULL, NULL);

    // Set the interrupt priority for EXTI0_IRQn (external interrupt 0) using HAL_NVIC_SetPriority().
    HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);

    // Enable the interrupts for the user button using HAL_NVIC_EnableIRQ().
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}


/**
 * @brief EXTI0_IRQHandler means EXTI for Interrupt-Request Handler. Here we check if there is any interrupt
 * signal from button will be created. Default Handler is defined by the manufacturer as "weak", so if the
 * programmer does not provide a new functionality, it will use the predefined functionality. In this case
 * we have defined a new functionality for EXTIO_IRQHandler, so it will be overwritten. Here we check if the
 * button is pressed, with checking if the interrupt is coming from falling edge When is that the case, then
 * HAL_GPIO_EXTI_IRQHandler will be called, and it will call the HAL_GPIO_EXTI_Callback function, which we also
 * overwrite below.
 *
 * @param None
 * @return None
 */
void EXTI0_IRQHandler(void) {
	EXTI_HandleTypeDef hexti;
	HAL_EXTI_GetHandle(&hexti, EXTI_LINE_0);

	if (HAL_EXTI_GetPending(&hexti, EXTI_TRIGGER_FALLING)) {
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
	}
}

/**
 * @brief TIM2_IRQHandler means a Timer2 for Interrupt-Request Handler. Here we check if timer2 has caused any interrupt signal.
 * Timer2 supposed to trigger an interrupt for each second. Because of the timer configuration where period is set 10Khz, which
 * corresponds to 1 second. So we are able to measure time.
 * The Function will call HAL_TIM_IRQHandler which  handles interrupts requests for Timer 2.
 * That is also the reason why we assigned the address of tim_handle_struct here. The general IRQ_Handler will call
 * HAL_TIM_PeriodElapsedCallback method when CALLBACK is set 1.
 *
 * @param None
 * @return None
 */
void TIM2_IRQHandler(void) {
	HAL_TIM_IRQHandler(&timer_stopwatch_handle_struct);
}

/**
 * @brief When the timer triggers an interrupt, the program jumps here and executes the function.
 * The function basically works when the defined timer handler instance(htim) is called with the corresponding
 * timer structure and the start flag is set to 1. When the above conditions are met, we increment the seconds by one, and
 * when the seconds reach 60, as expected, the seconds are reset and the minutes are incremented.
 *
 * @param htim, the pointer for our Timer-Handler, it is declared as global variable.
 * @return none
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &timer_stopwatch_handle_struct && start_flag == 1) {
		seconds++;
		if (seconds >= 60) {
			seconds = 0;
			minutes++;
		}
	}
}

/**
 * @brief If the button causes an interrupt(when pressed), the program jumps here and executes the function. The
 * first time the button is pressed, the timer is started. Because the starting flag is set to 0.
 * It will set to 1 first, after the button is pressed.
 * After that, each time the button is pressed, the lap time is displayed.
 * Once the starting_flag is set to 1, will always call LCD_DisplayTime method to display round time.
 *
 * @param GPIO_Pin, function controls the pin, from which the interrupt comes.
 * @return none
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
	case GPIO_PIN_0:
		if (start_flag == 0) {
			HAL_TIM_Base_Start_IT(&timer_stopwatch_handle_struct);
			start_flag = 1;
		} else {
			LCD_DisplayTime();
		}
		break;
	default:
		break;
	}
}

/**
 * @brief Shows the current time on the display.
 * Each time method is called, number of lines will be increased by one
 * in order to record the timelapse.
 *
 * @param none
 * @return none
 */
static void LCD_DisplayTime() {
	char buf[32];
	sprintf(buf, "%2d:%2d:%4lu", minutes, seconds, milliseconds);
	lcd_draw_text_at_line(buf, line_num, BLACK, 2, WHITE);
	line_num++;
	/* If there is no place to write the newest timelapse, the program will
	 * delete all timelapses and begin from the first line. */
	if (line_num > 15) {
		line_num = 1;
		lcd_fill_screen(WHITE);
	}
}

