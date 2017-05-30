
// AYX-32 main module header (Capt, you?)
//
// AYX-32 project
// (c) TS-Labs
//
// Verivalan taika on aina suojanamme

#pragma once

/// - Types ---
typedef void (*TASK)();

typedef PA1  AY_CLK;    // can be served by TIM2
typedef PA2  AY_RST;
typedef PB0  AY_BDIR;   // can be served by TIM3
typedef PB1  AY_BC1;    // can be served by TIM3
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

typedef PC0 AY_IOA0;
typedef PC1 AY_IOA1;
typedef PC2 AY_IOA2;
typedef PC3 AY_IOA3;
typedef PC4 AY_IOA4;
typedef PC5 AY_IOA5;
typedef PC6 AY_IOA6;
typedef PC7 AY_IOA7;

typedef PA4  AU_L;
typedef PA5  AU_R;

typedef PA9  U_TX;
typedef PA10 U_RX;

typedef PB3  TEST;

#ifndef BOOT
// Audio
typedef TIM6 AU_TIM;
typedef TIM7 EVT_TIM;
typedef DMA1_STREAM1 AU_DMA;
#define AU_DMA_IRQ DMA1_Stream1
#define AU_DMA_CH CHANNEL_7
#endif

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

typedef EXTI2 RST;
#define RST_IRQ EXTI2
#define RST_PORT syscfg::exticr::PA
#define RST_PIN 2

#pragma pack(1)
typedef struct
{
  void *start_addr;
  u8 cprstring[32];
  u8 bldstring[32];
  struct
  {
    u16 hw;
    u16 fw;
    u16 cf;
  } ver;
} BOOT_HDR;
#pragma pack()

enum
{
  // priority for interrupts
  RST_PRIORITY          = 1,
  BDIR_PRIORITY         = 2,
  BC1_PRIORITY          = 3,
  AU_DMA_PRIORITY       = 4,
  SYSTICK_PRIORITY      = 5,
  UART_CONSOLE_PRIORITY = 6,

  UART_CONSOLE_BAUD_RATE  = 115200,

  // buffers size
  UART_CONSOLE_INBUF  = 256,
  UART_CONSOLE_OUTBUF = 4096,
  PSG_BUS_EVENTS_SIZE = 2048,   // must be a multiple of 4
  WS_CMDLIST_SIZE     = 4096,
  WS_EVENTS_SIZE      = 32,
  DAC_FIFO_SIZE       = 16384,
  DAC_SAMPLES_COUNT   = 500,
  DAC_WS_RATIO        = 5,
  WS_SAMPLES_COUNT    = DAC_SAMPLES_COUNT / DAC_WS_RATIO,

  // system
  SYSTICK_PERIOD = 1000,  // us

  // audio
  PSG_CLK  = 1750000,   // Hz
  WS_FREQ  = 44100,     // Hz

  // PSG parameters
  PSG_CHIPS_MAX  = 4,     // Max number of virtual PSG chips
  PSG_VOL_AL_DEF = 32,    // PSG channel default volumes
  PSG_VOL_AR_DEF = 32,    //
  PSG_VOL_BL_DEF = 32,    //
  PSG_VOL_BR_DEF = 32,    //
  PSG_VOL_CL_DEF = 32,    //
  PSG_VOL_CR_DEF = 32,    //
  DAC_PUSHUP_DEF = 4,     // DAC push up

  // WS parameters
  WS_CHAN_MAX = 32,       // Max number of WS channels
  WS_SAMP_MAX = 256,      // Max number of WS samples

  // Flash
  CONF_ADDR     = 0x08004000, // Config address
  CONF_SIZE     = 0x4000,     // Config size
  CONF_PAD_SIZE = 128,        // Config pad size
  MAIN_ADDR     = 0x08008000, // Main code address
  DEV_SIG       = 0xAA55,     // Device signature
  HW_VER        = 1,          // Hardware version
  FW_VER        = 5,          // Firmware version
  FWHDR_VER     = 1,          // Firmware header version
  CF_VER        = 2,          // Config Pad version
};

#ifndef BOOT
enum
{
  AU_FREQ = PSG_CLK / 8,    // 218.75kHz
  EVT_FREQ = PSG_CLK / 8,   // 218.75kHz
};
#endif

#define CPR_STRING "AYX-32, (c)TSL"
#define BLD_STRING __DATE__ ", " __TIME__

/// - Variables ---
volatile u32 time_ms;
volatile u32 time_s;

#ifndef BOOT
volatile u8 cpu_load;
volatile u32 cpu_load_cnt;
volatile bool rq_cpu_load_cnt_res;
#endif

volatile TASK bg_task;      // background task, processed in main()
volatile bool is_bg_task;
u32 core_freq = clk::SYSTEM;


/// - Functions ---
void clear_bg_task();
void no_task() {}
