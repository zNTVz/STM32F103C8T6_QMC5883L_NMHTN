#ifndef LCD1602_I2C_H
#define LCD1602_I2C_H

#include "stm32f1xx_hal.h"

void LCD_INIT(void);
void LCD_SEND_CMD(char cmd);
void LCD_SEND_DATA(char data);
void LCD_SEND_STRING(char *str);
void LCD_SET_CURSOR(int row, int col);
void LCD_CLEAR_DISPLAY(void);

#endif /* LCD1602_I2C_H */
