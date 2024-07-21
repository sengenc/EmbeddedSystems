/**
**************************************************
* @file esd.h
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 28.04.2023
* @brief: Module for using util functions
**************************************************
*/


#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

/* Public functions (prototypes) */
void utils_gpio_port_write(GPIO_TypeDef* utils_port, uint16_t utils_pin);
int utils_gpio_port_read(GPIO_TypeDef* utils_port, uint16_t utils_pin);
void utils_init_gpio(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t mode,
		uint32_t pull, uint32_t alternate, uint32_t speed);
void utils_delay_ms(uint32_t t);

#endif /* UTILS_UTILS_H_ */
