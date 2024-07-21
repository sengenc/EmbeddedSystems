/**
 **************************************************
 * @file fan_control.c
 * @author Berkay Özgür, C. Arda Sengenc
 * @version v1.0
 * @date 20.06.2023
 *
 * @brief: The main purpose of the code is to be controlling the speed of a fan based on a target RPM (Revolutions Per Minute).
 * This will be guaranteed by using the fan speed is controlled using PWM signals. PWM (Pulse Width Modulation) is a technique where the average
 * value of a signal is adjusted by varying the width of its pulses while keeping the frequency constant. By adjusting the duty cycle
 * of the PWM signal, the average voltage or power delivered to the fan motor can be controlled. This, in turn, affects the fan's speed.
 * Higher duty cycles result in higher average voltage/power and faster fan speeds, while lower duty cycles result in lower speeds.
 *
 @verbatim
 ==================================================
 ### Resources used ###

 TIM2: It is  used to measure the time intervals between consecutive interrupts triggered by a GPIO pin (PB1).
 It is started when the first interrupt occurs and stopped when the second interrupt occurs. The time interval between
 these two interrupts is then used to calculate the actual RPM of the fan. Timer TIM2 is configured in the code to count up with
 a specific prescaler and period, allowing accurate measurement of time intervals.

 TIM3: Timer TIM3 is used to generate the PWM signal that controls the fan speed. It is responsible for generating
 the PWM signal with a specific frequency (f_pwm) and duty cycle (controlled by the control output calculated in the regulateFanSpeed() function).
 Timer TIM3 is configured to count up with a specific prescaler and period, which determines the PWM frequency and resolution.

 PB1: configured for interrupt-based tachometer output (Grün-TACHO_AUSGANG).

 PB5: configured for PWM input (BLAU-PWM-Eingang) signal.


 ==================================================
 ### Usage ###

 (#) Call "fan_control_init()" to initialize the module and all the necessary peripheries.

 (#) Call "fan_control_show_status()" in main-function's while-loop to see target RPM, and current RPM.
 	 Alternative: We are now printing the same values in main.c with extern variables.

 (#) Call "fan_control_set_rpm()" in main-function's while loop to set the target RPM with potentiometer.



 ### Static Functions ###

 (#) " EXTI1_IRQHandler" for the interrupt handler for the EXTI1 (External Interrupt Line 1) interrupt.
 For more detailed information, see below.

 (#) " HAL_GPIO_EXTI_Callback" for handling the EXTI interrupt triggered by PB1.
 It starts and stops Timer 2 to measure the time interval of the fan rotation and adjusts the fan speed accordingly.
 For more detailed information, see below.

 (#) "regulateFanSpeed()" for calculating the actual RPM of the fan based on the measured time interval and
 adjusts the fan speed using a proportional-integral (PI) control algorithm. It limits the output value within the
 specified range and updates the error sum accordingly.

 @endverbatim
 **************************************************
 */

/* Includes */
#include "stm32f4xx.h"
#include <lcd/lcd.h>
#include <stdio.h>
#include <fan_control.h>
#include <my_timer.h>
#include <potis_dma.h>
#include <utils.h>

/**
 * The Proportional-Integral (PI) controller is commonly used in control systems to regulate a process variable based on an error signal.
 * In the context of fan speed control, a PI controller is used to adjust the fan speed and maintain it at a desired setpoint.
 *
 * Proportional Control: The proportional term (Kp * error) in the controller output allows for an immediate response to the error signal.
 * It provides a control action that is proportional to the difference between the desired fan speed (target RPM) and the actual
 * fan speed (measured RPM). The proportional control helps to reduce the steady-state error and improve the system's responsiveness.
 *
 * Integral Control: The integral term (Ki * errorSum) in the controller output takes into account the accumulated error over time.
 * It helps to eliminate the steady-state error by continuously integrating the error signal. The integral control component is
 * particularly useful when there are factors such as friction or external disturbances that can cause a deviation from the desired fan speed.
 *
 * By combining both proportional and integral control, the PI controller provides a balance between
 * immediate response (proportional control) and long-term error correction (integral control).
 * It helps to stabilize the fan speed control system, minimize the deviation from the target RPM,
 * and improve the overall performance and accuracy of the fan speed regulation.
 */

/* Static module functions */
void EXTI1_IRQHandler(void);
static void regulateFanSpeed();
static void fan_control_pins_init();
static void fan_control_timer_2_init();
static void fan_control_timer_3_init();

/* Preprocessor macros */
#define MAX_PWM 199
#define MIN_PWM 15

// BLUE is connected to PB5. PWM_INPUT - Open-Drain. Has the TIM3_CH2 function.
// GREEN is connected to PB1. TACHO_OUTPUT - pull-up resistor.
// YELLOW is connected  to an 5V Energy Source
// ORANGE is connected an GND which stands for ground

/* Module variables */
volatile uint32_t fan_control_time_interval = 0;
volatile uint8_t fan_start_flag = 0;
volatile uint32_t frequency = 10000;
volatile uint32_t fan_control_actual_RPM = 0;
volatile uint32_t targetRPM = 0;
volatile int32_t errorSum = 0;
volatile uint32_t fan_control_poti_val = 0;
// The suitable parameters for the PI Controller
volatile float Kp = 0.98;
volatile float Ki = 2.1;
volatile float output = 0.0;

TIM_HandleTypeDef fan_control_tim_2_handle_struct;
TIM_HandleTypeDef fan_control_tim_3_handle_struct;
TIM_OC_InitTypeDef fan_control_tim_oc_handle_struct;

/* Public functions */

/**
 * @brief Initializes the whole fan control system.
 *        This function initializes the necessary components and peripherals for the fan control system,
 *        including HAL, potentiometer DMA, LCD, fan control pins, timer 2 and timer 3 with output compare.
 *
 * @param none
 * @return none
 *
 */
void fan_control_init() {
	HAL_Init();

	potis_dma_init();

	lcd_init();

	fan_control_pins_init();

	fan_control_timer_2_init();

	fan_control_timer_3_init();
}

/**
 * @brief Displays the fan control status on an LCD.
 *        This function formats and displays the target RPM, time interval between fan rotations, and current fan RPM on an LCD.
 *        The values are converted to strings using the sprintf function and then displayed using the lcd_draw_text_at_line function.
 *
 * @param none
 * @return none
 *
 */
void fan_control_show_status() {
	// for displaying target RPM value
//	char target_rpm_string[32];
//
//	//for displaying time interval between fan rotations
//	char interval_string[32];
//
//	//for displaying current fan rpm.
//	char current_rpm_string[32];
//
//	// Format the target RPM value as a string
//	sprintf(target_rpm_string, "Target RPM = %5lu", fan_control_poti_val);
//	// Display the target RPM value on the LCD at line 2
//	lcd_draw_text_at_line(target_rpm_string, 2, BLACK, 2, WHITE);
//
//	// Format the time interval between fan rotations as a string
//	sprintf(interval_string, "Interval : %5lu", fan_control_time_interval);
//	// Display the time interval on the LCD at line 4
//	lcd_draw_text_at_line(interval_string, 4, BLACK, 2, WHITE);
//
//	// Format the current fan RPM as a string
//	sprintf(current_rpm_string, "RPM : %5lu", fan_control_actual_RPM);
//	// Display the current fan RPM on the LCD at line 6
//	lcd_draw_text_at_line(current_rpm_string, 6, BLACK, 2, WHITE);
}

/**
 * @brief Sets the target RPM for fan control.
 *        This function reads the value from the potentiometer and scales it to a target RPM value.
 *        The calculated target RPM value is then assigned to the global variable `targetRPM`.
 *
 * @param none
 * @return none
 *
 */
void fan_control_set_rpm() {
	/**
	 * This line reads the value from the potentiometer DMA (Direct Memory Access) and scales it to a target RPM value.
	 * The potentiometer value is multiplied by 4500 and divided by 4095 to convert it to the desired range.
	 */
	fan_control_poti_val = (potis_dma_get_avg(POTIS_DMA_1) * 4500) / 4095;
	targetRPM = fan_control_poti_val;
}



/* Static module functions (for implementation) */

/**
 * @brief Initializes Timer 3 for fan control using PWM. This function initializes Timer 3
 * 		  with the necessary settings to control a fan using PWM (Pulse Width Modulation). The fan
 *        speed is controlled by varying the duty cycle of the PWM signal. The frequency of the PWM
 *        signal is set to 200Hz, and the timer period is calculated accordingly. The PWM signal is output on Timer 3 Channel 2.
 *
 * @param none
 * @return none
 */
static void fan_control_timer_3_init() {
	// Desired PWM frequency in Hz
	uint32_t f_pwm = 200;
	// Desired timer frequency based on PWM frequency.
	uint32_t f_timer = f_pwm * 27000;

	/* Enable clock for Timer 3 */
	__HAL_RCC_TIM3_CLK_ENABLE();
	fan_control_tim_3_handle_struct.Instance = TIM3;
	fan_control_tim_3_handle_struct.Init.Prescaler = (SystemCoreClock / 2
			/ f_timer) - 1;
	fan_control_tim_3_handle_struct.Init.Period = f_pwm - 1;
	/* Counter mode is assigned to count up */
	fan_control_tim_3_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
	fan_control_tim_3_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	fan_control_tim_3_handle_struct.Init.AutoReloadPreload =
	TIM_AUTORELOAD_PRELOAD_DISABLE;
	/* Here you specify after how many overflows an event should be generated */
	fan_control_tim_3_handle_struct.Init.RepetitionCounter = 0;
	HAL_TIM_PWM_Init(&fan_control_tim_3_handle_struct);

	fan_control_tim_oc_handle_struct.OCMode = TIM_OCMODE_PWM1;
	/* Must be in period, compare-value*/
	fan_control_tim_oc_handle_struct.Pulse = 0;
	fan_control_tim_oc_handle_struct.OCIdleState = TIM_OCIDLESTATE_SET;

	fan_control_tim_oc_handle_struct.OCPolarity = TIM_OCPOLARITY_HIGH;
	fan_control_tim_oc_handle_struct.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	fan_control_tim_oc_handle_struct.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	/* Configuration of the output compare channel with timer3 and corresponding channel */
	HAL_TIM_PWM_ConfigChannel(&fan_control_tim_3_handle_struct,
			&fan_control_tim_oc_handle_struct,
			TIM_CHANNEL_2);

	/* Start PWM on Timer 3, Channel 2 */
	HAL_TIM_PWM_Start(&fan_control_tim_3_handle_struct, TIM_CHANNEL_2);
}

/**
 * @brief Initializes Timer 2 for the time between two half rotations.
 *        This function initializes Timer 2 for fan control by calling the `my_timer_init()` function.
 *        Timer 2 is configured in base mode with a prescaler of 10000 and a period of 10000.
 *        The TIM_HandleTypeDef structure is then obtained using the `my_timer_get_struct()` function.
 *
 * @param none
 * @return none
 */
static void fan_control_timer_2_init() {
	my_timer_init(TIMER_INSTANCE_2, TIMER_MODE_BASE, 10000, 10000);
	fan_control_tim_2_handle_struct = my_timer_get_struct();
}

/**
 * @brief Initializes the GPIO pins for fan control.
 *        This function configures the necessary GPIO pins for fan control.
 *        The BLUE PWM input is connected to PB5 and is configured as an alternate function output open-drain.
 *        The Green-TACHO_OUTPUT is connected to PB1 and is initialized as an input with a pull-up resistor.
 *        Additionally, the interrupt priority and enable settings for TACHO_OUTPUT are also configured.
 *
 * @param none
 * @return none
 */
static void fan_control_pins_init() {
	// BLUE PWM input is connected to PB5.
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_b;

	/* Configure BLUE PWM input pin (PB5), must be initialised as alternate function - open drain
	 * and the Alternate Field must be set to GPIO_AF2_TIM3 in order to connect Timer 3 for reading pwm signal  */
	gpio_init_b.Pin = GPIO_PIN_5;
	gpio_init_b.Mode = GPIO_MODE_AF_OD;
	gpio_init_b.Pull = GPIO_PULLDOWN;
	gpio_init_b.Speed = GPIO_SPEED_MEDIUM;
	gpio_init_b.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB, &gpio_init_b);

	// Green-TACHO_OUTPUT is connected to PB1 and must be initialized as INPUT and PULLUP
	gpio_init_b.Pin = GPIO_PIN_1;
	gpio_init_b.Mode = GPIO_MODE_IT_RISING;
	gpio_init_b.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &gpio_init_b);

	/* Interrupt-Priority for TACHO_OUTPUT */
	HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
	/* Enabling the interrupts for TACHO_OUTPUT */
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

/**
 * @brief Interrupt Request Handler
 *
 * This function serves as an interrupt handler for EXTI1, which is triggered when a rising edge is detected on the
 * corresponding GPIO pin (GPIO_PIN_1). It retrieves the EXTI handle, checks if the interrupt is pending, and
 * then calls the appropriate handler for further processing.
 *
 * @param none
 * @return none
 *
 */
void EXTI1_IRQHandler(void) {
	EXTI_HandleTypeDef hexti;
	HAL_EXTI_GetHandle(&hexti, EXTI_LINE_1);

	if (HAL_EXTI_GetPending(&hexti, EXTI_TRIGGER_RISING)) {
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
	}
}

/**
 * @brief GPIO EXTI callback function.
 *
 * This function is called when an interrupt event occurs on a GPIO pin.
 * It performs specific actions based on the GPIO pin that triggered the interrupt.
 *
 * @param GPIO_Pin The GPIO pin number that triggered the interrupt.
 * @return none
 *
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
	case GPIO_PIN_1:
		/* At the beginning, the start_flag is false, so the program enters the first block.
		 * During the second half of the full rotation, the program enters the second block.
		 * The timer is stopped, and its value is assigned to the time_interval variable.
		 * time_interval is the time between two half rotations.
		 * The timer is then reset to zero.
		 */
		if (!fan_start_flag) {
			HAL_TIM_Base_Start(&fan_control_tim_2_handle_struct);
			fan_start_flag = 1;
		} else {
			HAL_TIM_Base_Stop(&fan_control_tim_2_handle_struct);
			fan_control_time_interval = __HAL_TIM_GET_COUNTER(
					&fan_control_tim_2_handle_struct);
			regulateFanSpeed();
			__HAL_TIM_SET_COUNTER(&fan_control_tim_2_handle_struct, 0);
			fan_start_flag = 0;
		}
		break;
	default:
		break;
	}
}

/**
 * @brief GPIO EXTI callback function
 *
 * Regulates the speed of a fan based on target RPM and measured RPM.
 * This function calculates the required fan speed adjustment to achieve the target RPM
 * based on the measured RPM. It uses a proportional-integral (PI) controller algorithm
 * to compute the output PWM value that controls the fan speed.
 * The time_interval variable represents the time difference between each half revolution.
 * To obtain the time for a complete revolution, it needs to be multiplied by 2.
 *
 * @param none
 * @return none
 */
static void regulateFanSpeed() {
	/**
	 * actualRPM explains that the time_interval variable represents the time difference between each half revolution of the fan.
	 * To calculate the time for a complete revolution, it needs to be multiplied by 2.
	 * The actualRPM is then calculated by dividing the frequency by the adjusted time_interval and
	 * multiplying it by 60 to convert it to RPM.
	 */
	fan_control_actual_RPM = (frequency / (fan_control_time_interval * 2)) * 60;

	/**
	 * The error is calculated by subtracting the actualRPM from the targetRPM,
	 * representing the deviation between the desired and actual fan speeds. The errorSum is updated
	 * by adding the error value to accumulate the cumulative error over time. The output is calculated using
	 * a proportional-integral (PI) controller algorithm, where the proportional term (Kp * error)
	 * and integral term with sampling period (Ki * errorSum * Ta) are combined.
	 */
	int32_t error = targetRPM - fan_control_actual_RPM;
	/* Ta is the sampling period */
	float Ta = fan_control_time_interval / frequency;
	errorSum = errorSum + error;
	output = Kp * error + Ki * errorSum * Ta;

	/**
	 * This section checks if the output exceeds the maximum allowable PWM value (MAX_PWM) or
	 * falls below the minimum allowable PWM value (MIN_PWM). If either condition is true, the output
	 * is limited to the corresponding limit, and the errorSum is adjusted by subtracting the error value
	 * to prevent excessive accumulation of error.
	 */
	if (output > MAX_PWM) {
		output = MAX_PWM;
		errorSum = errorSum - error;
	} else if (output < MIN_PWM) {
		output = MIN_PWM;
		errorSum = errorSum - error;
	}

	// This line sets the pulse width modulation (PWM) compare value for channel 2 of tim_3_handle_struct.
	// It controls the fan speed by adjusting the duty cycle of the PWM signal. The value of output determines the PWM compare value.
	__HAL_TIM_SET_COMPARE(&fan_control_tim_3_handle_struct, TIM_CHANNEL_2,
			output);
}

