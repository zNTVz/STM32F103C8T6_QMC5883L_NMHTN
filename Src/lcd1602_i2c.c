#include "lcd1602_i2c.h"

extern I2C_HandleTypeDef hi2c1;

#define SLAVE_ADDRESS_LCD 0x4E

void LCD_SEND_CMD(char cmd) {
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xf0);
    data_l = ((cmd << 4) & 0xf0);
    data_t[0] = data_u | 0x0C;  //en=1, rs=0
    data_t[1] = data_u | 0x08;  //en=0, rs=0
    data_t[2] = data_l | 0x0C;  //en=1, rs=0
    data_t[3] = data_l | 0x08;  //en=0, rs=0
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, data_t, 4, 100);
}

void LCD_SEND_DATA(char data) {
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | 0x0D;  //en=1, rs=1
    data_t[1] = data_u | 0x09;  //en=0, rs=1
    data_t[2] = data_l | 0x0D;  //en=1, rs=1
    data_t[3] = data_l | 0x09;  //en=0, rs=1
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, data_t, 4, 100);
}

void LCD_INIT(void) {
    HAL_Delay(50);
    LCD_SEND_CMD(0x33); /* set 4-bits interface */
    LCD_SEND_CMD(0x32);
    HAL_Delay(50);
    LCD_SEND_CMD(0x28); /* start to set LCD function */
    HAL_Delay(50);
    LCD_SEND_CMD(0x01); /* clear display */
    HAL_Delay(50);
    LCD_SEND_CMD(0x06); /* set entry mode */
    HAL_Delay(50);
    LCD_SEND_CMD(0x0c); /* set display to on */
    HAL_Delay(50);
    LCD_SEND_CMD(0x02); /* move cursor to home and set data address to 0 */
    HAL_Delay(50);
    LCD_SEND_CMD(0x80);
}

void LCD_SEND_STRING(char *str) {
    while (*str) LCD_SEND_DATA(*str++);
}

void LCD_CLEAR_DISPLAY(void) {
    LCD_SEND_CMD(0x01); //clear display
}

void LCD_SET_CURSOR(int row, int col) {
    uint8_t pos_Addr;
    if (row == 1) {
        pos_Addr = 0x80 + row - 1 + col;
    } else {
        pos_Addr = 0x80 | (0x40 + col);
    }
    LCD_SEND_CMD(pos_Addr);
}
