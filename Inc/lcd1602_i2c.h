#ifndef LCD1602_I2C_H
#define LCD1602_I2C_H

#include "stm32f1xx_hal.h"

#define LCD_ADDRESS 0x27 << 1  

void LCD_INIT (void);   // initialize lcd

void LCD_SEND_CMD (char cmd);  // send command to the lcd

void LCD_SEND_DATA (char data);  // send data to the lcd

void LCD_SEND_STRING (char *str);  // send string to the lcd

void LCD_CLEAR_DISPLAY (void);	//clear display lcd

void LCD_SET_CURSOR (int row, int col); //set proper location on screen

#endif