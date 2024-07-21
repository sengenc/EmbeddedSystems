/**
 **************************************************
 * @file utils.c
 * @author Berkay Özgür, C. Arda Sengenc
 * @version v1.0
 * @date 28.04.2023
 * @brief: Module for using necessary functions like delay and IO.
 @verbatim
 ==================================================
 ### Usage ###

 (#) Call "HAL_Init()" to use the functions down below.

 @endverbatim
 **************************************************
 */

/* Includes */
#include "stm32f4xx.h"
#include <utils.h>

TIM_HandleTypeDef tim_handle_struct_utils;
uint16_t moin = 0;

void utils_init_gpio(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t mode,
		uint32_t pull, uint32_t alternate, uint32_t speed) {
	switch ((uint32_t) GPIOx) {
	case GPIOA_BASE:
		__HAL_RCC_GPIOA_CLK_ENABLE();
		break;
	case GPIOB_BASE:
		__HAL_RCC_GPIOB_CLK_ENABLE();
		break;
	case GPIOC_BASE:
		__HAL_RCC_GPIOC_CLK_ENABLE();
		break;
	case GPIOD_BASE:
		__HAL_RCC_GPIOD_CLK_ENABLE();
		break;
	case GPIOE_BASE:
		__HAL_RCC_GPIOE_CLK_ENABLE();
		break;
	case GPIOF_BASE:
		__HAL_RCC_GPIOF_CLK_ENABLE();
		break;
	}
	GPIO_InitTypeDef gpio_init;
	gpio_init.Pin = GPIO_Pin;
	gpio_init.Mode = mode;
	gpio_init.Pull = pull;
	gpio_init.Alternate = alternate;
	gpio_init.Speed = speed;

	HAL_GPIO_Init(GPIOx, &gpio_init);
}

/* Muss auch noch kommentiert werden. Stand: 23.05.23*/
void utils_init_timer() {
	HAL_Init();

	__HAL_RCC_TIM1_CLK_ENABLE();

	tim_handle_struct_utils.Instance = TIM1;
	/* CoreClock ist 16000000 */
	/* Prescaler ist in der Einleitung 16000000/10000 = 1600 - 1 Hz*/
	/* Wenn wir 10KHz wollen, sollte nicht dann der Prescaler 10000 sein?? */
	/* Funktionert der Prescaler so, dass es bei jedem Wert am Ende immer
	 * CoreClock / Prescaler rauskommt?? Wenn ja, dann brauchen wir für 10KHz -> 1600,
	 * dann bekommen wir 16000000/1600 = 10000. */
	/* Wenn man aber 1599 reinschreibt, wird es nicht so wie bei unten!!!!*/
	tim_handle_struct_utils.Init.Prescaler = (SystemCoreClock / 1000000) - 1;
	/* Periodendauer ist 9999, entspricht bei 10KHz genau eine Sekunde */
	tim_handle_struct_utils.Init.Period = 1000000 - 1;
	/* Aufwärts zahlen lassen */
	tim_handle_struct_utils.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_handle_struct_utils.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_handle_struct_utils.Init.AutoReloadPreload =
			TIM_AUTORELOAD_PRELOAD_DISABLE;
	/* Hier gibt man an, nach wievielen Überläufen ein Event generiert werden soll */
	tim_handle_struct_utils.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&tim_handle_struct_utils);
}

/**
 * @brief Function for turning just the bit pattern on, others off
 * @param utils_port port which we want to use
 * @param utils_pin pin(s) we want to turn on
 * @return none
 */
void utils_gpio_port_write(GPIO_TypeDef *utils_port, uint16_t utils_pin) {
	/* Since we should set the pins that were already set to zero
	 * and only turn on the bit patterns, we would need to look into
	 * GPIO_TypeDef and the individual modes. ODR - Output Data Register fits us,
	 * because we turn on only the bits that are entered as parameter and turn off
	 * the bits that were 1.
	 * */
	utils_port->ODR = utils_pin;
}

/**
 * @brief Function for reading pins we want
 * @param utils_port port which we want to use
 * @param utils_pin pin(s) we want to read
 * @return 1 if the port is active, 0 if the port is inactive
 */
int utils_gpio_port_read(GPIO_TypeDef *utils_port, uint16_t utils_pin) {
	/* Simple bitwise and, because we only control the bit pattern whether
	 * it is on or off. */
	return utils_port->IDR & utils_pin;
}

/**
 * @brief Function for delay in milliseconds
 * @param t time to wait in milliseconds
 * @return none
 */
void utils_delay_ms(uint32_t t) {
	utils_init_timer(); /*mach den init woanders*/
	uint32_t timer = 0;

	//HAL_Delay(t);
	HAL_TIM_Base_Start(&tim_handle_struct_utils);
	while (__HAL_TIM_GET_COUNTER(&tim_handle_struct_utils) < t) {
		timer = __HAL_TIM_GET_COUNTER(&tim_handle_struct_utils);
		if (timer > 30000) {
			moin++;
		}

	}
	HAL_TIM_Base_Stop(&tim_handle_struct_utils);
	__HAL_TIM_SET_COUNTER(&tim_handle_struct_utils, 0);

}
