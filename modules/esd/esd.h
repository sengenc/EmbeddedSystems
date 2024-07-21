/**
**************************************************
* @file esd.h
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 28.04.2023
* @brief: Module for using the periphery 8 segment board
**************************************************
*/

#ifndef ESD_ESD_H_
#define ESD_ESD_H_

/* Public enums as shortcuts for digits */
typedef enum {
	ESD_DIGIT_0,
	ESD_DIGIT_1,
	ESD_DIGIT_2,
	ESD_DIGIT_3,
	ESD_DIGIT_4,
	ESD_DIGIT_5,
	ESD_DIGIT_6,
	ESD_DIGIT_7,
	ESD_DIGIT_8,
	ESD_DIGIT_9,
} esd_digit_t;

/* Public enums as shortcuts for position */
typedef enum {
	ESD_POSITION_1,
	ESD_POSITION_2,
	ESD_POSITION_3,
	ESD_POSITION_4,
	ESD_POSITION_ALL,
} esd_position_t;

/* Public functions (prototypes) */
void esd_init(void);
void esd_show_digit(esd_digit_t digit, esd_position_t pos);

#endif /* ESD_ESD_H_ */
