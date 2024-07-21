/**
**************************************************
* @file my_lcd.h
* @author Berkay Özgür, C. Arda Sengenc
* @version v1.0
* @date 29.04.2023
* @brief: Header File for implementing LCD Module.
**************************************************
*/

#ifndef MY_LCD_MY_LCD_H_
#define MY_LCD_MY_LCD_H_

/* Public functions (prototypes) */
void my_lcd_countdown(int input);
void my_lcd_draw_baargraph(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t value, uint16_t color, uint16_t bgcolor);
void my_lcd_draw_x(int size);


#endif /* MY_LCD_MY_LCD_H_ */
