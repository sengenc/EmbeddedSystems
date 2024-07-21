/**
**************************************************
* @file main.c
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 07.05.2023
* @brief: Module for potentiometers (ad-conversion) and LCD screen on the microchip.
*
* @usage: In order to use this module, you need to exclude the module "stopwatch" from
* 		  the build.
**************************************************
==================================================
### Resources used ###
	(see potis.c)
==================================================
*/

/* Includes */
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include "stdio.h"
#include <potis.h>
#include <my_lcd.h>

int main(void) {
	HAL_Init();

	/* Initialization of the LCD */
	lcd_init();

	/* Initialization of the AD-Converter, for the module potis.c */
	potis_init();

	/* Two arrays for the text on the LCD*/
	char buffer[16];
	char buffer_2[32];

	/* Auxiliary variables for saving the conversion for each channel */
	uint32_t ADC_val;
	uint32_t ADC_val_2;

	while (1) {
		/* Function call for the first potentiometer */
		ADC_val = potis_get_val(POTIS_DMA_1);
		/* Converting the analog signal into millivolts*/
		ADC_val = (3300*ADC_val)/4095;
		my_lcd_draw_baargraph(10, 40, 200, 35, (ADC_val*20)/66, RED, GREEN);
											/* Can also be reduced by 50, the graph becomes more precise
											 * (ADC_val*1000)/3300 -> (ADC_val*20)/66*/
		sprintf(buffer, "ADC1 = %5lu mV", ADC_val);
		lcd_draw_text_at_line(buffer, 2, BLACK, 2, WHITE);

		/* Function call for the second potentiometer */
		ADC_val_2 = potis_get_val(POTIS_DMA_2);
		ADC_val_2 = (3300*ADC_val_2)/4095;
		my_lcd_draw_baargraph(10, 175, 200, 35, (ADC_val_2*20)/66, RED, GREEN);
		sprintf(buffer_2, "ADC2 = %5lu mV", ADC_val_2);
		lcd_draw_text_at_line(buffer_2, 6, BLACK, 2, WHITE);
	}
}
