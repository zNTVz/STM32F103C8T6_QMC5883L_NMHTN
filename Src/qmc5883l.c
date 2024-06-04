#include "qmc5883l.h"
#include "mSysTick.h"

void QMC5883L_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t config[2] = {0x0B, 0x01};  // Set mode to continuous
    HAL_I2C_Master_Transmit(hi2c, QMC5883L_ADDRESS, config, 2, HAL_MAX_DELAY);
}

static int16_t offsets[3] = { -1732, -414, 1314 };
static float scales[3] = { 1216.0f, 1026.5f, 1218.0f };
static float scaleAvg = 1153.5f;

void QMC5883L_Read(int16_t xyz[3]) {
    uint8_t data[6];
    HAL_I2C_Master_Receive(hi2c, QMC5883L_ADDRESS, data, 6, HAL_MAX_DELAY);
    
    xyz[0] = (data[1] << 8) | data[0];
    xyz[1] = (data[3] << 8) | data[2];
    xyz[2] = (data[5] << 8) | data[4];
}

void QMC5883L_Calibrate(void) {
    uint32_t lastChanged = mSysTick_GetTimeStamp();
    int16_t mins[3] = { 32767, 32767, 32767 };
    int16_t maxes[3] = { -32768, -32768, -32768 };

    while (mSysTick_GetTimeStamp() - lastChanged <= 5000) {
        bool changed = false;
        int16_t xyz[3] = { 0 };
        QMC5883L_Read(xyz[3]);
        for (int i = 0; i < 3; i++) {
            if (xyz[i] < mins[i]) {
                mins[i] = xyz[i];
                changed = true;
            }
            if (xyz[i] > maxes[i]) {
                maxes[i] = xyz[i];
                changed = true;
            }
        }
        if (changed) {
            lastChanged = mSysTick_GetTimeStamp();
        }
        mSysTick_Delay(5);
    }
		scaleAvg = 0;
    for (int i = 0; i < 3; i++) {
        offsets[i] = mins[i] / 2 + maxes[i] / 2;
        scales[i] = (maxes[i] - mins[i]) / 2.0f;
        scaleAvg += scales[i];
			}
    scaleAvg /= 3;
    char scaleAvgPrint[32] = { 0 };
		mFloatToString(scaleAvg, 3, sizeof scaleAvgPrint, scaleAvgPrint);
	}

Void QMC5883L_ReadNormXYZ(float xyz[3]) {
    int16_t xyzRaw[3] = { 0 };
    QMC5883L_ReadRawXYZ(xyzRaw);
    for (int i = 0; i < 3; i++) {
        xyz[i] = (xyzRaw[i] - offsets[i]) * scaleAvg / scales[i];
    }
}
			
			
int mFloatToString(float num, unsigned int decimal, unsigned int size, char buf[size]) {
    const char *sign = "";
    if (num < 0) {
        sign = "-";
        num = -num;
    }
    int fmul = 1;
    for (unsigned int i = 0; i < decimal; i++) {
        fmul *= 10;
    }
    int ipart = num;
    int fpart = (num - ipart) * fmul + 0.5f;
    return snprintf(buf, size, "%s%d.%0*d", sign, ipart, decimal, fpart);
}