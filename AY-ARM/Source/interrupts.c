
// Interrupt functions
//
// (c) 2014, TS-Labs inc.
// All rights fucked out
//
// I hate to interrupt you, but...

#include "stm32f4xx.h"
#include "types.h"
#include "hw.h"
#include "ay.h"

    U8 out;
    extern	DAC_t DACOut;
    extern volatile U32 idle_cnt;
    extern volatile U32 idle_cnt_upd;    
    U32 idle_cnt_copy;

    // GPIOB->BSRRH = 4;   // debug !!!
    // GPIOB->BSRRL = 4;   // debug !!!

void TIM2_IRQHandler()
{
    TIM2->SR = 0;     // clear interrupt flag, must be at the beginning of ISR
    DAC->DHR12RD = DACOut.w;
	AY_Tick();
}

void SysTick_Handler(void)
{
    idle_cnt_copy = idle_cnt;
    idle_cnt_upd = 1;
}

void SysTick_Init(U32 us)
{
    U32 ticks;
    SystemCoreClockUpdate();
    ticks = 1ULL * us * SystemCoreClock / 1000000;
    SysTick->LOAD  = ticks - 1;                                  /* set reload register */
    SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
}

void Int_Init()
{
	NVIC_SetPriority(TIM3_IRQn, 0);     // bus slave
	NVIC_SetPriority(TIM2_IRQn, 1);     // sound generation
    SysTick_Init(1000);
	NVIC_SetPriority(SysTick_IRQn, 2);  // SysTick
    NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_EnableIRQ(SysTick_IRQn);
}
