/**
**************************************************
* @file potis.h
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 09.05.2023
* @brief: Module for using ADC.
**************************************************
*/

#ifndef POTIS_POTIS_H_
#define POTIS_POTIS_H_

/* Public preprocessor macros */
#define POTIS_DMA_1 1
#define POTIS_DMA_2 2

/* Public functions (prototypes) */
void potis_init(void);
uint32_t potis_get_val(uint8_t poti_num);


#endif /* POTIS_POTIS_H_ */
