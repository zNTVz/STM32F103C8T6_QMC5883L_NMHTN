#include "mSysTick.h"
#include "stm32f103xb.h"

volatile uint32_t mSysTick_counter = 0;

void mSysTick_Init(void);
void SysTick_Handler(void);

void mSysTick_Init(void) {
    SysTick->LOAD = F_CPU * 0.001;
    SysTick->CTRL |=
            SysTick_CTRL_ENABLE_Msk |
            SysTick_CTRL_CLKSOURCE_Msk |
            SysTick_CTRL_TICKINT_Msk;
}

void mSysTick_Delay(uint32_t ms) {
    uint32_t start = mSysTick_counter;
    while (mSysTick_counter - start < ms);
}

uint32_t mSysTick_GetTimeStamp(void) {
    return mSysTick_counter;
}

void SysTick_Handler(void) {
    ++mSysTick_counter;
}