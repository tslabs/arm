
// AY-ARM main module
//
// AY-ARM project
// (c)2015 TS-Labs
//
// Demoscene is alive!!!

/// - Header includes ---
#include "string.h"
#include "clock.hpp"
#include "interrupt.hpp"
#include "core/stk.hpp"
#include "core/scb.hpp"
#include "peripheral/gpio.hpp"
#include "peripheral/tim.hpp"
#include "peripheral/usart.hpp"
#include "peripheral/devsig.hpp"
#include "peripheral/dac.hpp"
#include "../other/fifo.hpp"
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

/// - Code includes ---
#include "console/console.cpp"
#include "common/hw.cpp"
#include "common/interrupts.cpp"

/// - Functions ---
void initializeRuntime()
{
  console_uart_in.init(console_uart_inbuf, UART_CONSOLE_INBUF);
  console_uart_out.init(console_uart_outbuf, UART_CONSOLE_OUTBUF);
  console::initialize();
}

/// - Main ---
int main()
{
  clk::initialize();
  initializeRuntime();
  initializePeripherals();
  initializeInterrupts();

  // ay::AY_Init();
  // WS_Init();

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
