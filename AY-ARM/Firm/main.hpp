
// AY-ARM main module header (Captain, you?)
//
// AY-ARM project
// (c)2015 TS-Labs
//
// Verivalan taika on aina suojanamme

#pragma once

typedef PA1  AY_CLK;    // served by TIM2
typedef PA2  AY_RST;
typedef PB0  AY_BDIR;   // served by TIM3
typedef PB1  AY_BC1;    // served by TIM3
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

// Audio
typedef TIM6 AU_TIM;
typedef DMA1_STREAM1 AU_DMA;
#define AU_DMA_IRQ DMA1_Stream1
#define AU_DMA_CH CHANNEL_7

// UART
typedef USART1 UART_CONSOLE;
#define UART_CONSOLE_IRQ USART1
#define UART_CONSOLE_REGS USART1_REGS

// AY bus
#define AYBUS_USES_EXTI
#define AYCLK_INTERNAL

#ifdef AYBUS_USES_EXTI
  typedef EXTI0 BDIR;
  #define BDIR_IRQ EXTI0
  #define BDIR_PORT syscfg::exticr::PB
  #define BDIR_PIN 0
  typedef EXTI1 BC1;
  #define BC1_IRQ EXTI1
  #define BC1_PORT syscfg::exticr::PB
  #define BC1_PIN 1
#else
#endif

enum
{
  // priority for interrupts
  BDIR_PRIORITY         = 1,
  BC1_PRIORITY          = 2,
  AU_DMA_PRIORITY       = 3,
  SYSTICK_PRIORITY      = 4,
  UART_CONSOLE_PRIORITY = 5,

  UART_CONSOLE_BAUD_RATE  = 115200,

  // buffers size
  UART_CONSOLE_INBUF  = 256,
  UART_CONSOLE_OUTBUF = 1024,
  
  // system
  SYSTICK_PERIOD = 100000,  // us

  // audio
  AU_FREQ  = 1750000,   // Hz
  WS_FREQ  = 44100,     // Hz

  // PSG parameters
  AY_CHIPS_MAX  = 16,     // Max number of virtual PSG chips
  AY_CHIPS_DEF  = 2,      // Default number of virtual PSG chips
  TURBO_AY_MASK = 0xF8,   // Range for Turbo AY decoding (0xF8 - 0xF8..FF)
  
  // WS parameters
  WS_CH_MAX = 64,         // Max number of WS channels
  WS_CH_DEF = 32,         // Default number of WS channels
  
  // magic constants
  NUM_VER   = 1,          // Version
  NUM_SVER  = 0,          // Sub-Version
  DEV_BYTE0 = 0x55,       // Device Byte0
  DEV_BYTE1 = 0xAA,       // Device Byte1
  MAGIC_FWM = 0x552A773C, // 'FW Mode' magic
  MAGIC_FFW = 0x7841AA55, // 'Flash Firmware' magic
};

#define __SW(a, b) case a: b; break;

#define CHIP_STRING "AY-ARM Sound Chip, v.1.00"
#define CPR_STRING "(c)2015 TS-Labs"
