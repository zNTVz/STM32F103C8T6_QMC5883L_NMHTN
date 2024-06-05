#ifndef QMC5883L_H
#define QMC5883L_H

#include "stm32f1xx_hal.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void QMC5883L_Init(void);
void QMC5883L_ReadNormXYZ(float xyz[3]);
void QMC5883L_ReadRawXYZ(int16_t xyz[3]);
void QMC5883L_Reset(void);

#endif /* QMC5883L_H */
