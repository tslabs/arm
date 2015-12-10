
// AY-ARM main module header (Captain, you?)
//
// AY-ARM project
// (c)2015 TS-Labs
//
// Verivalan taika on aina suojanamme

#pragma once

enum
{
  SYSTICK_PERIOD          = 100000,   // us
  
  // priority for interrupts
  SYSTICK_PRIORITY        = 4,
  UART_CONSOLE_PRIORITY   = 5,
  
  UART_CONSOLE_BAUD_RATE  = 115200,
  
  // size of buffers
  UART_CONSOLE_INBUF      = 256,
  UART_CONSOLE_OUTBUF     = 1024
};
  
typedef PA1  AY_CLK;    // served by TIM2
typedef PA2  AY_RST;
typedef PB0  AY_BDIR;   // served by TIM3
typedef PB1  AY_BC1;    //
typedef PB4  AY_BC2;
typedef PB6  AY_A8;
typedef PB7  AY_A9;
typedef PB5  AY_SEL;

typedef PB8  AY_D0;
typedef PB9  AY_D1;
typedef PB10 AY_D2;
typedef PB11 AY_D3;
typedef PB12 AY_D4;
typedef PB13 AY_D5;
typedef PB14 AY_D6;
typedef PB15 AY_D7;

typedef PA4  AU_L;
typedef PA5  AU_R;

typedef PA9  U_TX;
typedef PA10 U_RX;

// UART
// Warning: UART number is also hardcoded in "common\interrupts.cpp"
typedef USART1 UART_CONSOLE;
#define UART_CONSOLE_REGS USART1_REGS
