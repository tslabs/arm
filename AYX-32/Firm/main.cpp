
// AYX-32 main module
//
// AYX-32 project
// (c) TS-Labs
//
// Demoscene is alive!

// To do:
// - use DMA for DAC buf zeroing
// - make amp table load via 'data' registers (only initialized if table selected)
// - procedures for muted tone/noise/enveloper generators - only calculate generators, but do not write to memory
// - meander procedures to use duty cycle
// - buffer adders via saturated math
// - correct processing of registers readback/chip selects
// - optional disable of PSG/extra features (until reset)
// - move audio buffer renderer over BG tasks (software IRQ)

/// - Header includes ---
#include <stdarg.h>
#include <limits.h>
#include "string.h"
#include "clock.hpp"
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
#include "../other/text.hpp"

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

namespace snd
{
#include "sound/psg.hpp"
#include "sound/ws.hpp"
#include "sound/sound.hpp"
}

__attribute__((used, section(".boot")))
#include "obj/boot.h"

/// - Variables ---
u8 console_uart_inbuf[UART_CONSOLE_INBUF];
u8 console_uart_outbuf[UART_CONSOLE_OUTBUF];
FIFO console_uart_in;
FIFO console_uart_out;

volatile bool req_snd_buf;

u32 dac_buf[2][DAC_SAMPLES_COUNT] __attribute__((section(".sram2")));

extern char __StackTop;

extern "C" __attribute__((used, naked))
void resetHandler();

__attribute__ ((used, section(".irq_vect")))
const BOOT_HDR boot_hdr = {(void*)resetHandler, CPR_STRING, BLD_STRING, HW_VER, FW_VER, CF_VER};
const BOOT_HDR* boot_hdrp = &boot_hdr;

/// - Code includes ---
#include "common/hw.cpp"
#include "common/func.cpp"

namespace bus
{
#include "bus/registers.cpp"
#include "bus/command.cpp"
#include "bus/vectors_wa.cpp"
#include "bus/vectors_wr.cpp"
#include "bus/vectors_rr.cpp"
#include "bus/vectors_cm.cpp"
}

namespace snd
{
#include "sound/config.cpp"
#include "sound/sound.cpp"
#include "sound/events.cpp"
#include "sound/vectors_ev.cpp"
}

namespace console
{
#include "console/terminal.h"
#include "console/terminal.cpp"
#include "console/interrupts.cpp"
#include "console/console.cpp"
}

#include "common/rt_init.cpp"

namespace interrupts
{
#include "common/interrupts.cpp"
}

/// - Functions ---
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
  console::initialize();
  bus::initialize();
  snd::initialize();

  time_ms = 0;
}

/// - Main ---
void resetHandler()
{
  initializeRuntime();
  initializePeripherals();
  interrupts::initialize();
  EVT_TIM::startCounter();
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
      console::menu();

    // Sound
    if (req_snd_buf)
    {
      req_snd_buf = false;
      TEST::setHigh();
      snd::render_snd_buffer();
      TEST::setLow();
    }

    // Background task
    if (is_bg_task)
    {
      bg_task();
      clear_bg_task();
    }
  }
}
