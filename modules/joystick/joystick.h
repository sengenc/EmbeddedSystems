/**
**************************************************
* @file joystick.h
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 28.04.2023
* @brief: Module for using joystick.
**************************************************
*/

#ifndef JOYSTICK_JOYSTICK_H_
#define JOYSTICK_JOYSTICK_H_

/* Public enums as shortcuts for joystick directions */
typedef enum{
	JOYSTICK_A,
	JOYSTICK_B,
	JOYSTICK_C,
	JOYSTICK_D,
	JOYSTICK_PRESS,
} joystick_direction_t;

/* Public functions (prototypes) */
int joystick_read_dir(joystick_direction_t input);
void joystick_init();

#endif /* JOYSTICK_JOYSTICK_H_ */
