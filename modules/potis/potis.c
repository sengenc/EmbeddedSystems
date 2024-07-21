/**
**************************************************
  * @file potis.c
  * @author Berkay Özgür, C. Arda Sengenc
  * @version v1.0
  * @date 09.05.2023
  * @brief: Module for using potentiometer peripheral for the microchip with AD-Converter.
@verbatim
==================================================
### Resources used ###
GPIO: GPIO_PIN_6 and GPIO_PIN_7.
ADC: ADC1.
ADC-Channels: ADC_CHANNEL_6 and ADC_CHANNEL_7.
==================================================
### Usage ###

(#) Call "potis_init()" to initialize the module and all the
	necessary channels for using AD-Converter.

(#) Call "potis_get_val(uint8_t poti_num)" to get the value of the desired potentiometer.
	For input (poti_num) see potis.h to use macros.

@endverbatim
**************************************************
*/


/* Includes */
#include "stm32f4xx.h"
#include <potis.h>

/* Module variable */
ADC_HandleTypeDef ADC_handle_structure;
ADC_ChannelConfTypeDef ADC_channel_structure;


/**
  * @brief Initialises the module and all the necessary peripherals according to the instructions given.
  * @param None
  * @return None
  */
void potis_init() {
	/* Classic HAL_Init() function, required for each module, point of entry. */
	HAL_Init();
	/* Pins must also be configured with the correct mode. The correct modes can be found in the relevant pages and sheets of the manual. */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_a;
	/* The upper potentiometer is connected to pin 6,
	 * while the lower potentiometer is connected to pin 7.*/
	gpio_init_a.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	/* Since we are doing analog-to-digital conversion, we need analog mode. */
	gpio_init_a.Mode = GPIO_MODE_ANALOG;
	/* Pullup oder pulldown is not required in this project, because
	 * the resistance does not play a role in this project. */
	gpio_init_a.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOA, &gpio_init_a);

	/* Initialization of ADC1 */
	__HAL_RCC_ADC1_CLK_ENABLE();
	/* The input channels are connected to ADC1 according to manual */
	ADC_handle_structure.Instance = ADC1;
	ADC_handle_structure.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	ADC_handle_structure.Init.Resolution = ADC_RESOLUTION_12B;
	ADC_handle_structure.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC_handle_structure.Init.ScanConvMode = DISABLE;
	ADC_handle_structure.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	ADC_handle_structure.Init.ContinuousConvMode = DISABLE;
	ADC_handle_structure.Init.NbrOfConversion = 1;
	ADC_handle_structure.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	HAL_ADC_Init(&ADC_handle_structure);
}

/**
  * @brief Function for getting the to value of the desired potentiometer.
  * @param unsigned integer with 8bits named poti_num
  * @return unsigned 32 bit integer value called ADC_Val
  */
uint32_t potis_get_val(uint8_t poti_num) {
	/* Configuration of the desired input channels. Where the analog board is connected,
	 * there are GPIO-PA-6 and 7. The first potentiometer is connected to 6 and the second
	 * is connected to 7. So when it will be called in main.c, there will
	 * be two choices; Either poti_num=1 or poti_num=2
	 */
	/* Macros for choosing the desired channel */
	if (poti_num == POTIS_DMA_1) {
		ADC_channel_structure.Channel = ADC_CHANNEL_6;
	}
	/* Macros for choosing the desired channel */
	if (poti_num == POTIS_DMA_2) {
		ADC_channel_structure.Channel = ADC_CHANNEL_7;
	}
	ADC_channel_structure.Rank = 1;
	ADC_channel_structure.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	HAL_ADC_ConfigChannel(&ADC_handle_structure, &ADC_channel_structure);

	/* HAL_ADC_Start must be started here first, and stopped after the reading is complete. */
	HAL_ADC_Start(&ADC_handle_structure);

	uint32_t ADC_val;
	long ret = HAL_ADC_PollForConversion(&ADC_handle_structure, 1000);
	if (ret != HAL_TIMEOUT) {
		/* Not millivolts, the maximum value is 4095 because it is 12 bits.
		 * Conversion for the voltage: 3.3V or (3300mV*ADC_val)/4095 but one
		 * has to be careful because the value is floating point and might cause
		 * problems when we store floating value here. (Conversion is in main.c) */
		ADC_val = HAL_ADC_GetValue(&ADC_handle_structure);
	}

	/* HAL_ADC_Stop will be called in order to stop conversion. */
	HAL_ADC_Stop(&ADC_handle_structure);
	return ADC_val;
}

