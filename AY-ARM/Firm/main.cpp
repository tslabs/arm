
// AY-ARM main module
//
// AY-ARM project
// (c)2015 TS-Labs
//
// Demoscene is alive!

/// - Header includes ---
#include <stdarg.h>
#include <limits.h>
#include "exception.cpp"
#include "startup.cpp"
#include "string.h"
#include "clock.hpp"
#include "interrupt.hpp"
#include "core/stk.hpp"
#include "core/scb.hpp"
#include "peripheral/gpio.hpp"
#include "peripheral/tim.hpp"
#include "peripheral/dma.hpp"
#include "peripheral/usart.hpp"
#include "peripheral/devsig.hpp"
#include "peripheral/dac.hpp"
#include "peripheral/exti.hpp"
#include "peripheral/syscfg.hpp"
#include "common/math.cpp"
#include "../other/fifo.hpp"
#include "types.hpp"
#include "main.hpp"

// #include "ay.h"
// #include "hw.h"
// #include "isr.h"
// #include "ws.h"
// #include "ay-arm.h"
// #include "bus.h"
// #include "interrupts.h"

/// - Variables ---
u8 console_uart_inbuf[UART_CONSOLE_INBUF];
u8 console_uart_outbuf[UART_CONSOLE_OUTBUF];
FIFO console_uart_in;
FIFO console_uart_out;

u32 dac_buf1[512] __attribute__((section(".sram2")));
u32 dac_buf2[512] __attribute__((section(".sram2")));

/// - Code includes ---
#include "console/console.cpp"
#include "common/hw.cpp"
#include "sound/sound.cpp"
#include "common/interrupts.cpp"

/// - Functions ---
void initializeRuntime()
{
  console_uart_in.init(console_uart_inbuf, UART_CONSOLE_INBUF);
  console_uart_out.init(console_uart_outbuf, UART_CONSOLE_OUTBUF);
  console::initialize();
  // ay::AY_Init();
  // WS_Init();
}

/// - Main ---
int main()
{
  clk::initialize();
  initializeRuntime();
  initializePeripherals();
  initializeInterrupts();
  AU_TIM::startCounter();

  // main cycle, events processing
  while (1)
  {
    // Console UART input
    if (console_uart_in.used())
      console::uart_input(console_uart_in.get_byte());

    // Console UART output
    if (!console::is_sending && console_uart_out.used())
      console::processSend();

    // Console menus
    if (!console_uart_out.used())
      console::disp();
  }
}
