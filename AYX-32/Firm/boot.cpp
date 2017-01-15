
// AYX-32 boot module
//
// AYX-32 project
// (c) TS-Labs
//
// Das boot!

// Hints:
// + mutexes for flasher UART vs. Z80

#define BOOT

/// - Header includes ---
#include <stdarg.h>
#include <limits.h>
#include "string.h"
#include "clock.hpp"
#include "core/stk.hpp"
#include "core/scb.hpp"
#include "peripheral/gpio.hpp"
#include "peripheral/tim.hpp"
#include "peripheral/usart.hpp"
#include "peripheral/exti.hpp"
#include "peripheral/syscfg.hpp"
#include "peripheral/flash.hpp"
#include "../other/fifo.hpp"
#include "types.hpp"
#include "main.hpp"

namespace interrupts
{
#include "common/interrupts.hpp"
}

namespace bus
{
#include "bus/registers.hpp"
#include "bus/bus.hpp"
#include "bus/command.hpp"
}

/// - Types ---

/// - Variables ---
extern TASK __StackTop;

u8 uart_inbuf[256];
u8 uart_outbuf[32];
FIFO uart_in;
FIFO uart_out;

__attribute__ ((section(".sram1"), aligned(8)))
u8 fw_buf[97 * 1024];  // FW buffer (1kB Header, 96kB FW)

const BOOT_HDR* boot_hdrp = (BOOT_HDR*)MAIN_ADDR;

/// - Code includes ---
#include "../other/crc.cpp"
#include "common/func.cpp"

namespace flasher
{
#include "boot/flasher.cpp"
}
#include "common/hw.cpp"

namespace uart
{
#include "boot/uart.cpp"
}

namespace bus
{
#include "bus/registers.cpp"
#include "bus/command.cpp"
#include "bus/vectors_wa.cpp"
#include "bus/vectors_wr.cpp"
#include "bus/vectors_rr.cpp"
#include "bus/vectors_cm.cpp"
}

namespace interrupts
{
#include "common/interrupts.cpp"
}

#include "common/rt_init.cpp"

/// - Functions ---
extern "C" __attribute__((used, naked))
void resetHandler();

__attribute__ ((used, section(".irq_vect")))
const void *bootVector[] =
{
  (void*)(&__StackTop),
  (void*)resetHandler
};

namespace clk
{
  void hseFailureHandler()
  {
    while (true);
  }
}

void initializeRuntime()
{
  rt_init();
  uart::initialize();
  bus::initialize();

  time_ms = 0;
}

/// - Main ---
void resetHandler()
{
  clk::initialize();

  // pull-up mode pin
  TEST::enableClock();
  TEST::setMode(gpio::moder::INPUT);
  TEST::setPullMode(gpio::pupdr::PULL_UP);

  // check mode pin
  u32 delay = 1000;
  while (delay--)
  {
    // main code start if pin is high
    if (TEST::isHigh())
      goto *(TASK*)(boot_hdrp->start_addr);
  }

  // boot start
  initializeRuntime();
  initializePeripherals();
  interrupts::initialize();
  flasher::initialize();

  while (1)
  {
    // Background task
    if (is_bg_task)
    {
      bg_task();
      clear_bg_task();
    }

    // UART input
    if (uart_in.used())
      uart::input_byte(uart_in.get_byte());

    // UART output
    if (!uart::is_sending && uart_out.used())
      uart::processSend();
  }
}
