#include "QMC5883L.h"
#include "stm32f1xx_hal.h"
#include <math.h>

#define ADDR 0x0D
#define REG_OUT_X_LSB 0x00
#define REG_STATUS 0x06
#define REG_CTRL1 0x09
#define REG_CTRL2 0x0A
#define REG_SET 0x0B

static int16_t offsets[3] = { -1732, -414, 1314 };
static float scales[3] = { 1216.0f, 1026.5f, 1218.0f };
static float scaleAvg = 1153.5f;

extern I2C_HandleTypeDef hi2c1;

void QMC5883L_Init(void) {
    uint8_t regCtrl1[] = { REG_CTRL1, 0x1D };
		uint8_t regCtrl2[] = { REG_CTRL2, 0x80 };
		uint8_t regSet[] = { REG_SET, 0x01 };

    HAL_I2C_Master_Transmit(&hi2c1, ADDR << 1, regCtrl2, 2, HAL_MAX_DELAY);
    HAL_I2C_Master_Transmit(&hi2c1, ADDR << 1, regCtrl1, 2, HAL_MAX_DELAY);
    regCtrl2[1] = 0x41;
    HAL_I2C_Master_Transmit(&hi2c1, ADDR << 1, regCtrl2, 2, HAL_MAX_DELAY);
    HAL_I2C_Master_Transmit(&hi2c1, ADDR << 1, regSet, 2, HAL_MAX_DELAY);

    HAL_Delay(100);
}

void QMC5883L_ReadRawXYZ(int16_t xyz[3]) {
    uint8_t buf[6] = { 0 };

    HAL_I2C_Mem_Read(&hi2c1, ADDR << 1, REG_OUT_X_LSB, I2C_MEMADD_SIZE_8BIT, buf, 6, HAL_MAX_DELAY);

    xyz[0] = buf[0] | (buf[1] << 8);
    xyz[1] = buf[2] | (buf[3] << 8);
    xyz[2] = buf[4] | (buf[5] << 8);
}

void QMC5883L_ReadNormXYZ(float xyz[3]) {
    int16_t xyzRaw[3] = { 0 };
    QMC5883L_ReadRawXYZ(xyzRaw);
    for (int i = 0; i < 3; i++) {
        xyz[i] = (xyzRaw[i] - offsets[i]) * scaleAvg / scales[i];
    }
}

void QMC5883L_Reset(void) {
    uint8_t regCtrl1[] = { REG_CTRL1, 0x1D };
    uint8_t regCtrl2[] = { REG_CTRL2, 0x80 };
    uint8_t regSet[] = { REG_SET, 0x01 };

    HAL_I2C_Master_Transmit(&hi2c1, ADDR << 1, regCtrl2, 2, HAL_MAX_DELAY);
    HAL_I2C_Master_Transmit(&hi2c1, ADDR << 1, regCtrl1, 2, HAL_MAX_DELAY);
    regCtrl2[1] = 0x41;
    HAL_I2C_Master_Transmit(&hi2c1, ADDR << 1, regCtrl2, 2, HAL_MAX_DELAY);
    HAL_I2C_Master_Transmit(&hi2c1, ADDR << 1, regSet, 2, HAL_MAX_DELAY);

    HAL_Delay(100);
}
