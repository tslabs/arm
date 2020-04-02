
// Interrupts definitions
//
// AYX-32 project
// (c) TS-Labs
//
// Am să te chem să mergem printre nori

#pragma once

/// Enums
// Z80 bus
enum
{
  AYCB_BDIR  = 1 << 0,
  AYCB_BC1   = 1 << 1,
  AYCB_BC2   = 1 << 4,
  AYCB_A8    = 1 << 6,
  AYCB_A9    = 1 << 7,
  AYCB_MASK  = AYCB_BDIR | AYCB_BC1 | AYCB_BC2 | AYCB_A8 | AYCB_A9,
  AYCB_WADDR = AYCB_BDIR | AYCB_BC1 | AYCB_BC2 | AYCB_A8,
  AYCB_WREG  = AYCB_BDIR | AYCB_BC2 | AYCB_A8,
  AYCB_RREG  = AYCB_BC1 | AYCB_BC2 | AYCB_A8
};

/// Types
typedef struct
{
  void *Stack;
  TASK Reset;
  TASK NMI;
  TASK HardFault;
  TASK MemManage;
  TASK BusFault;
  TASK UsageFault;
  TASK _pad0;
  TASK _pad1;
  TASK _pad2;
  TASK _pad3;
  TASK SVCall;
  TASK DebugMon;
  TASK _pad4;
  TASK PendSV;
  TASK SysTick;
  
  TASK WWDG;
  TASK PVD;
  TASK TAMP_STAMP;
  TASK RTC_WKUP;
  TASK FLASH;
  TASK RCC;
  TASK EXTI0;
  TASK EXTI1;
  TASK EXTI2;
  TASK EXTI3;
  TASK EXTI4;
  TASK DMA1_Stream0;
  TASK DMA1_Stream1;
  TASK DMA1_Stream2;
  TASK DMA1_Stream3;
  TASK DMA1_Stream4;
  TASK DMA1_Stream5;
  TASK DMA1_Stream6;
  TASK ADC;
  TASK CAN1_TX;
  TASK CAN1_RX0;
  TASK CAN1_RX1;
  TASK CAN1_SCE;
  TASK EXTI9_5;
  TASK TIM1_BRK_TIM9;
  TASK TIM1_UP_TIM10;
  TASK TIM1_TRG_COM_TIM11;
  TASK TIM1_CC;
  TASK TIM2;
  TASK TIM3;
  TASK TIM4;
  TASK I2C1_EV;
  TASK I2C1_ER;
  TASK I2C2_EV;
  TASK I2C2_ER;
  TASK SPI1;
  TASK SPI2;
  TASK USART1;
  TASK USART2;
  TASK USART3;
  TASK EXTI15_10;
  TASK RTCAlarm;
  TASK OTG_FS_WKUP;
  TASK TIM8_BRK_TIM12;
  TASK TIM8_UP_TIM13;
  TASK TIM8_TRG_COM_TIM14;
  TASK TIM8_CC;
  TASK ADC3;
  TASK FSMC;
  TASK SDIO;
  TASK TIM5;
  TASK SPI3;
  TASK UART4;
  TASK UART5;
  TASK TIM6_DAC;
  TASK TIM7;
  TASK DMA2_Stream0;
  TASK DMA2_Stream1;
  TASK DMA2_Stream2;
  TASK DMA2_Stream3;
  TASK DMA2_Stream4;
  TASK ETH;
  TASK ETH_WKUP;
  TASK CAN2_TX;
  TASK CAN2_RX0;
  TASK CAN2_RX1;
  TASK CAN2_SCE;
  TASK OTG_FS;
  TASK DMA2_Stream5;
  TASK DMA2_Stream6;
  TASK DMA2_Stream7;
  TASK USART6;
  TASK I2C3_EV;
  TASK I2C3_ER;
  TASK OTG_HS_EP1_OUT;
  TASK OTG_HS_EP1_IN;
  TASK OTG_HS_WKUP;
  TASK OTG_HS;
  TASK DCMI;
  TASK CRYP;
  TASK HASH_RNG;
  TASK FPU;
} VECT_TABLE;

/// Functions
void ISR_Default() __attribute__ ((section(".ram_text")));
void ISR_Bad() __attribute__ ((section(".ram_text")));
void SysTick() __attribute__ ((section(".ram_text")));
void BDIR();
void BDIR_ram() __attribute__ ((section(".ram_text")));
void BC1() __attribute__ ((naked));
void BC1_ram() __attribute__ ((naked, section(".ram_text")));
void BC1_epi();
void BC1_epi_ram() __attribute__ ((noinline, section(".ram_text")));
void RST() __attribute__ ((section(".ram_text")));
void UART_CONSOLE();
#ifndef BOOT
void AU_DMA() __attribute__ ((section(".ram_text")));
#endif
void initializeIrqTab();
void setIrqTab();
void setIrqWork();
void setIrqIdle();
