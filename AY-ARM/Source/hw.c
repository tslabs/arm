
// Periferal library
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "types.h"
#include "hw.h"

void GPIO_Init()
{
	IO_Bit_Set(RCC, AHB1ENR, GPIOAEN, ON);		// Enable GPIOA clock
	IO_Bit_Set(RCC, AHB1ENR, GPIOBEN, ON);		// Enable GPIOB clock
	IO_Bit_Set(RCC, AHB1ENR, GPIODEN, ON);		// Enable GPIOD clock
	IO_Bit_Set(RCC, APB2ENR, SYSCFGEN, ON);		// Enable SYSCFG clock
	
	// Configure PA0-2 as input floating
	GPIO_Tune(A, 0, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Tune(A, 1, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Tune(A, 2, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	
	// Configure PA0-2 as AF
	GPIO_Tune(A, 0, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Tune(A, 1, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Tune(A, 2, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	
	GPIO_AFR(A, 0, 1);		// PA0 to TIM2_CH1
	// GPIO_AFR(A, 1, 2);		// PA1 to TIM5_CH2
	GPIO_AFR(A, 2, 3);		// PA2 to TIM9_CH1
	
	// Configure PB8-15 as 8 bit input
	GPIO_Tune8(B, 8, GPIO_Mode_IN, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);

	// Configure LEDs
	GPIO_Tune(D, 14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Tune(D, 15, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	
	// Configure Timer2 CH1 to PA0
	// IO_Fld_Set(TIM, 2, CCMR1, CC1S, TIM_ICSelection_DirectTI);
	// IO_Fld_Set(TIM, 2, CCMR1, IC1F, 0);
	// IO_Fld_Set(TIM, 2, CCER, CC1P, 0);
	// IO_Fld_Set(TIM, 2, CCER, CC1NP, 0);
	// IO_Fld_Set(TIM, 2, SMCR, SMS, TIM_SlaveMode_External1);
	// IO_Fld_Set(TIM, 2, SMCR, TS, TIM_TS_TI1FP1);
	// IO_Fld_Set(TIM, 2, DIER, TIE, TIM_IT_Trigger);
	// IO_Fld_Set(TIM, 2, CR1, CEN, TIM_CR1_CEN);
	
	// Configure Timer5 CH2 to PA1
	// IO_Fld_Set(TIM, 5, CCMR1, CC1S, TIM_ICSelection_IndirectTI);
	// IO_Fld_Set(TIM, 5, CCMR1, IC1F, 0);
	// IO_Fld_Set(TIM, 5, CCER, CC1P, 0);
	// IO_Fld_Set(TIM, 5, CCER, CC1NP, 0);
	// IO_Fld_Set(TIM, 5, SMCR, SMS, TIM_SlaveMode_External1);
	// IO_Fld_Set(TIM, 5, SMCR, TS, TIM_TS_TI2FP2);
	// IO_Fld_Set(TIM, 5, DIER, TIE, TIM_IT_Trigger);
	// IO_Fld_Set(TIM, 5, CR1, CEN, TIM_CR1_CEN);
	
	// Configure Timer9 CH1 to PA2
	// IO_Fld_Set(TIM, 9, CCMR1, CC1S, TIM_ICSelection_DirectTI);
	// IO_Fld_Set(TIM, 9, CCMR1, IC1F, 0);
	// IO_Fld_Set(TIM, 9, CCER, CC1P, 0);
	// IO_Fld_Set(TIM, 9, CCER, CC1NP, 0);
	// IO_Fld_Set(TIM, 9, SMCR, SMS, TIM_SlaveMode_External1);
	// IO_Fld_Set(TIM, 9, SMCR, TS, TIM_TS_TI1FP1);
	// IO_Fld_Set(TIM, 9, DIER, TIE, TIM_IT_Trigger);
	// IO_Fld_Set(TIM, 9, CR1, CEN, TIM_CR1_CEN);
}

void INT_Init()
{
	// IO_Fld_Set(SYSCFG,, EXTICR[1], EXTI1, 0);		// Connect EXTI Line1 to PA1 pin
	SYSCFG->EXTICR[0] = SYSCFG->EXTICR[0] & ~(0xF0);	// Connect EXTI Line1 to PA1 pin

	// Configure EXTI Line1
	IO_Bit_Set(EXTI, IMR, MR1, 1);
	IO_Bit_Set(EXTI, RTSR, TR1, 1);

	// Enable and set EXTI Line1 Interrupt
	NVIC_SetPriority(EXTI1_IRQn, 8);
	NVIC_EnableIRQ(EXTI1_IRQn);
	
	//SYSCFG->EXTICR1.EXTI2 = 0;		// Connect EXTI Line2 to PA2 pin

	// Configure EXTI Line2
	// IO_Bit_Set(EXTI, IMR, MR2, 1);
	// IO_Bit_Set(EXTI, RTSR, TR2, 1);

	// Enable and set EXTI Line2 Interrupt
	// NVIC_SetPriority(EXTI2_IRQn, 8);
	// NVIC_EnableIRQ(EXTI2_IRQn);
}
