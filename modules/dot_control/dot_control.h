/**
**************************************************
* @file dot_control.h
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 20.06.2023
* @brief: Module for Dimming Dot function on the timer periphery on the microchip.
* This module provides functions to initialize and control the dimming functionality
* for the timer periphery on the microchip. It allows changing the frequency and controlling
* the brightness of the LEDs.
**************************************************
*/

#ifndef DOT_CONTROL_H_
#define DOT_CONTROL_H_

/* Public functions (prototypes )*/
void dot_control_init(); //Initializes all necessary peripherals for controlling frequency and brightness of LEDs.
void dot_control_change_dimming(); //Changes the frequency and brightness of the LEDs based on the potentiometer values.
void dot_control_change_frequency(); //Changes the frequency of the LEDs.


#endif /* DOT_CONTROL_H_ */
