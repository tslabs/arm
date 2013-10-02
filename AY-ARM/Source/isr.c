
// STM32 ISR functions
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// I hate to interrupt you, but...

#include "stm32f4xx.h"
#include "types.h"
#include "hw.h"

U8 inp;
u32 lmin = 100000;
u32 lmax = 0;

// EXTI0
// AY_CLK
// Input: Timer? (driven by PA0 or by internal clock)
void EXTI0_IRQHandler()
{
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	IO_Bit_Set(EXTI, PR, PR0, ON);
}

// EXTI1
// AY_BC1
// Input: PA1, raising edge
void EXTI1_IRQHandler()
{
	// u32 t = 0;
	
	GPIO_Bit_Set(D, 15, ON);
	
	IO_Bit_Set(EXTI, PR, PR1, ON);
	
	while (GPIOA->IDR & 2);
		// t++;
	
	GPIO_Bit_Set(D, 15, OFF);
	
	// if (lmin>t)
		// lmin = t;
	// if (lmax<t)
		// lmax = t;
}
	// NVIC_ClearPendingIRQ(EXTI1_IRQn);

// EXTI2
// AY_BDIR
// Input: PA2, raising edge
void EXTI2_IRQHandler()
{
	// if(!GPIO_Bit_Read(A, 2)) return;
	// if(!GPIO_Bit_Read(A, 2)) return;
	
	inp = GPIOB->IDR >> 8;
	// NVIC_ClearPendingIRQ(EXTI2_IRQn);
	IO_Bit_Set(EXTI, PR, PR2, ON);
}
