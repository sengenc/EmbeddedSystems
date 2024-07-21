/**
**************************************************
* @file main.c
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 09.05.2023
* @brief: Module for DMA function on the microchip.
*
* @usage: In order to use this module, you need to exclude the module "stopwatch" from
* 		  the build.
**************************************************
==================================================
### Resources used ###
	(see potis_dma.c)
==================================================
*/

/* Includes */

#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include <potis_dma.h>

int main(void) {
	HAL_Init();

	/* Initialization of the LCD */
	lcd_init();

	/* Initialization of DMA */
	potis_dma_init();

	/* Two arrays for the text on the LCD*/
	char buffer[16];
	char buffer_2[16];

	while (1) {
		/* The first average will be written on display */
		sprintf(buffer, "Adress1 = %4lu", potis_dma_get_avg(POTIS_DMA_1));
		lcd_draw_text_at_line(buffer, 2, BLACK, 2, WHITE);

		/* The second average will be written on display */
		sprintf(buffer_2, "Adress2 = %4lu", potis_dma_get_avg(POTIS_DMA_2));
		lcd_draw_text_at_line(buffer_2, 6, BLACK, 2, WHITE);

	}
}

