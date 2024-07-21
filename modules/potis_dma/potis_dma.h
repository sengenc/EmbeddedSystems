/**
**************************************************
* @file potis_dma.h
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 09.05.2023
* @brief: Module for using DMA.
**************************************************
*/
#ifndef POTIS_DMA_POTIS_DMA_H_
#define POTIS_DMA_POTIS_DMA_H_

/* Public preprocessor macros */
#define POTIS_DMA_1 1
#define POTIS_DMA_2 2

/* Public functions (prototypes) */
void potis_dma_init(void);
uint32_t potis_dma_get_avg(uint8_t input);


#endif /* POTIS_DMA_POTIS_DMA_H_ */
