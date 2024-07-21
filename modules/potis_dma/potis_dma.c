/**
**************************************************
  * @file potis_dma.c
  * @author Berkay Özgür, C. Arda Sengenc
  * @version v1.0
  * @date 09.05.2023
  * @brief: Module for using potentiometer peripheral for the microchip with DMA.
@verbatim
==================================================
### Resources used ###
DMA: DMA2_Stream0 and DMA_CHANNEL_0.
GPIO: GPIO_PIN_6 and GPIO_PIN_7.
ADC: ADC1.
ADC-Channels: ADC_CHANNEL_6 and ADC_CHANNEL_7.
==================================================
### Usage ###

(#) Call "potis_dma_init()" to initialize the module and all the
	necessary channels for using DMA.

(#) Call "potis_dma_get_avg(uint8_t input)" to get the value of the desired potentiometer.
	For input see potis_dma.h to use macros.

@endverbatim
**************************************************
*/

/* Includes */
#include "stm32f4xx.h"
#include <potis_dma.h>

/* Module variable */
uint32_t dma_address[200];

/**
  * @brief Initializes the module and all the necessary periphery
  * @param None
  * @return None
  */
void potis_dma_init() {
	HAL_Init();

	/* Structures for initializing corresponding unit. */
	ADC_HandleTypeDef ADC_handle_structure;
	DMA_HandleTypeDef DMA_handle_structure;
	ADC_ChannelConfTypeDef ADC_channel_structure1;
	ADC_ChannelConfTypeDef ADC_channel_structure2;

	/* The pins to which the potentiometers are connected must also be initialized */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_a;
	/* The potentiometers are connected to pins 6 and 7 */
	gpio_init_a.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	/* For the AD-conversion, we need analog mode */
	gpio_init_a.Mode = GPIO_MODE_ANALOG;
	/* Pullup or pulldown is not needed */
	gpio_init_a.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOA, &gpio_init_a);

	/* Clock enabling for DMA */
	__HAL_RCC_DMA2_CLK_ENABLE();

	/* For ADC1 we need according to Table 44 DMA2_Stream0 and DMA_CHANNEL_0. */
	DMA_handle_structure.Instance = DMA2_Stream0;
	DMA_handle_structure.Init.Channel = DMA_CHANNEL_0;
	/* We are reading the values from peripheral and transporting them to memory*/
	DMA_handle_structure.Init.Direction = DMA_PERIPH_TO_MEMORY;
	DMA_handle_structure.Init.PeriphInc = DMA_PINC_DISABLE;
	/* Memory increment is enabled, because we need to write the values to the memory
	 * and increment the address in each iteration. */
	DMA_handle_structure.Init.MemInc = DMA_MINC_ENABLE;
	/* 32 bit words */
	DMA_handle_structure.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	DMA_handle_structure.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;

	/* We read two inputs one after the other, therefore circular*/
	DMA_handle_structure.Init.Mode = DMA_CIRCULAR;
	DMA_handle_structure.Init.Priority = DMA_PRIORITY_HIGH;
	DMA_handle_structure.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	HAL_DMA_Init(&DMA_handle_structure);

	/* Initialization of ADC1 */
	__HAL_RCC_ADC1_CLK_ENABLE();
	ADC_handle_structure.Instance = ADC1;
	ADC_handle_structure.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	ADC_handle_structure.Init.Resolution = ADC_RESOLUTION_12B;
	ADC_handle_structure.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC_handle_structure.Init.DiscontinuousConvMode = DISABLE;


	/* ScanConv, SequentialConversion and ContinuousMode must be enabled,
	 * because we have two different channels to read */
	ADC_handle_structure.Init.ScanConvMode = ENABLE;
	ADC_handle_structure.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	ADC_handle_structure.Init.ContinuousConvMode = ENABLE;

	/* It is set to 2, because we have 2 channels to read */
	ADC_handle_structure.Init.NbrOfConversion = 2;
	ADC_handle_structure.Init.ExternalTrigConv = ADC_SOFTWARE_START;

	/* ADC needs to know, how the conversion will be made*/
	ADC_handle_structure.DMA_Handle = &DMA_handle_structure;
	ADC_handle_structure.Init.DMAContinuousRequests = ENABLE;
	HAL_ADC_Init(&ADC_handle_structure);

	/* DMA for Channel 6 */
	ADC_channel_structure1.Channel = ADC_CHANNEL_6;
	ADC_channel_structure1.Rank = 1;
	ADC_channel_structure1.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	HAL_ADC_ConfigChannel(&ADC_handle_structure, &ADC_channel_structure1);

	/*  DMA for Channel 7 */
	ADC_channel_structure2.Channel = ADC_CHANNEL_7;
	ADC_channel_structure2.Rank = 2;
	ADC_channel_structure2.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	HAL_ADC_ConfigChannel(&ADC_handle_structure, &ADC_channel_structure2);

	/* Starting the DMA */
	HAL_ADC_Start_DMA(&ADC_handle_structure, dma_address, 200);
}

/**
  * @brief Function for getting the average of 100 conversions for desired channel.
  * @param input, desired channel we want to see it's average.
  * @return average of 100 conversions for desired channel.
  */
uint32_t potis_dma_get_avg(uint8_t input) {
	/* Auxiliary variable for saving the average for desired channel */
	uint32_t tmp = 0;

	/* Macros for choosing the desired channel */
	if (input == POTIS_DMA_1) {
		for (int i = 0; i < 200; i = i + 2) {
			tmp += dma_address[i];
		}
	}

	/* Macros for choosing the desired channel */
	if (input == POTIS_DMA_2) {
		for (int i = 0; i < 200; i = i + 2) {
			tmp += dma_address[i + 1];
		}
	}

	return (tmp / 100);
}


