
// Periferal library
//
// (c) 2014, TS-Labs inc.
// All rights fucked out
//

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "types.h"
#include "hw.h"

void GPIO_Init()
{
    REG_SET_BIT(RCC, AHB1ENR, GPIOAEN, 1);      // enable clock for GPIOA
    GPIO_SET(A, OSPEEDR, 1, GPIO_High_Speed);   // set max clock for GPIOA[1]
    GPIO_SET(A, MODER, 1, GPIO_Mode_AF);        // set AF mode for AY_CLK input
    GPIO_AFR(A, 1, GPIO_AF_TIM2);               // select AF
    REG_SET_BIT(RCC, APB1ENR, TIM2EN, 1);       // enable clock for TIM2
    REGn_SET_FLD(TIM, 2, SMCR, TS, 6);          // select TI2FP2 for input
    REGn_SET_FLD(TIM, 2, SMCR, SMS, 7);         // set external clock source
    REGn_SET_FLD(TIM, 2, CCMR1, IC2F, 2);       // set filter value for TI2
    REGn_SET_BIT(TIM, 2, DIER, UIE, 1);         // enable interrupts
    // TIM2->ARR = 15;                             // set auto reload value to 1.75MHz / 16 = 109.375kHz
    TIM2->ARR = 7;                              // set auto reload value to 1.75MHz / 8 (for YM2149F)
    REGn_SET_BIT(TIM, 2, CR1, CEN, 1);          // enable counter
    
    REG_SET_BIT(RCC, AHB1ENR, GPIOBEN, 1);      // enable clock for GPIOB
    GPIO_SET(B, OSPEEDR, 0, GPIO_High_Speed);   // set max clock for IOs
    GPIO_SET(B, OSPEEDR, 1, GPIO_High_Speed);   //
    GPIO_SET(B, MODER, 0, GPIO_Mode_AF);        // set AF mode for BC1, BDIR inputs
    GPIO_SET(B, MODER, 1, GPIO_Mode_AF);        //
    GPIO_AFR(B, 0, GPIO_AF_TIM3);               // select AF
    GPIO_AFR(B, 1, GPIO_AF_TIM3);               //
    REG_SET_BIT(RCC, APB1ENR, TIM3EN, 1);       // enable clock for TIM3
    REGn_SET_FLD(TIM, 3, CCMR2, CC3S, 1);       // select channels for Capture inputs
    REGn_SET_FLD(TIM, 3, CCMR2, CC4S, 1);       //
    REGn_SET_FLD(TIM, 3, CCMR2, IC3F, 1);       // set filter value
    REGn_SET_FLD(TIM, 3, CCMR2, IC4F, 1);       //
    REGn_SET_BIT(TIM, 3, CCER, CC3E, 1);        // enable Capture inputs
    REGn_SET_BIT(TIM, 3, CCER, CC4E, 1);        //
    REGn_SET_BIT(TIM, 3, DIER, CC3IE, 1);       // enable interrupts
    REGn_SET_BIT(TIM, 3, DIER, CC4IE, 1);       //
    
    *((U16*)(&GPIOB->OSPEEDR) + 1) = 0xFFFF;    // set max clock for GPIOB[15:8]
    
    REG_SET_BIT(RCC, APB1ENR, DACEN, 1);        // enable clock for DAC
    DAC->DHR12RD = 0;                           // set level to zero
    REG_SET_BIT(DAC, CR, EN1, 1);               // enable DAC1
    REG_SET_BIT(DAC, CR, EN2, 1);               // enable DAC2
    
    // GPIO_SET(B, MODER, 2, GPIO_Mode_OUT);
    // GPIO_SET(B, OSPEEDR, 2, GPIO_High_Speed);
}
