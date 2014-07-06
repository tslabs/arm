
// Interrupt functions
//
// (c) 2014, TS-Labs inc.
// All rights fucked out
//
// I hate to interrupt you, but...

#include "stm32f4xx.h"
#include "types.h"
#include "hw.h"

    U8 out;

        // GPIOB->BSRRH = 4;   // debug !!!
        // GPIOB->BSRRL = 4;   // debug !!!

void TIM2_IRQHandler()
{
    TIM2->SR = 0;     // clear interrupt flag
    // NVIC_ClearPendingIRQ(TIM2_IRQn);

    GPIOB->BSRRL = 4;   // debug !!!

    __asm
    {
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
    }

    GPIOB->BSRRH = 4;   // debug !!!

    {
        // U8 tmp = TIM2->CCR2;     // clear interrupt flag
    }
}

void Int_Init()
{
	NVIC_SetPriority(TIM2_IRQn, 1);     // sound generation
	NVIC_SetPriority(TIM3_IRQn, 0);     // bus slave
    NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM3_IRQn);
}
