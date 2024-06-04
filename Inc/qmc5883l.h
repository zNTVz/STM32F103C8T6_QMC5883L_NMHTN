#ifndef QMC5883L_H
#define QMC5883L_H

#include "stm32f1xx_hal.h"

#define QMC5883L_ADDRESS 0x0D << 1

void QMC5883L_Init(I2C_HandleTypeDef *hi2c);
void QMC5883L_Read(I2C_HandleTypeDef *hi2c, int16_t *x, int16_t *y, int16_t *z);

#endif