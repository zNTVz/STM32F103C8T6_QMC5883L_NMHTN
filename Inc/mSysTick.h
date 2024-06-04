#ifndef MSYSTICK_H
#define MSYSTICK_H

#include <stdio.h>

void mSysTick_Init(void);
void mSysTick_Delay(uint32_t ms);
uint32_t mSysTick_GetTimeStamp(void);