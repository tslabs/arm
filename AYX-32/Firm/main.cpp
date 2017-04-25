
// AYX-32 main module
//
// AYX-32 project
// (c) TS-Labs
//
// Demoscene is alive!

// To do:
// - add IOA to PC[7:0] (rr_(), wr_(), mode programming on MUX)
// - use DMA for DAC buf zeroing
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
#include "bus/command.hpp"
#include "bus/bus.hpp"
}

namespace snd
{
#include "sound/sound.hpp"
}

namespace console
{
#include "console/terminal.h"
}

__attribute__((used, section(".boot")))
#include "obj/boot.h"

/// - Variables ---
u8 console_uart_inbuf[UART_CONSOLE_INBUF];
u8 console_uart_outbuf[UART_CONSOLE_OUTBUF];
FIFO console_uart_in;
FIFO console_uart_out;

volatile bool req_snd_buf;

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
#include "bus/vectors.cpp"
#include "bus/bus.cpp"
}

namespace snd
{
#include "sound/config.cpp"
#include "sound/psg.cpp"
#include "sound/ws.cpp"
#include "sound/sound.cpp"
#include "sound/events.cpp"
#include "sound/vectors.cpp"
}

namespace console
{
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
  snd::initialize();
  bus::initialize();

  time_ms = 0;
  time_s = 0;
#ifndef BOOT
  cpu_load = 0;
  cpu_load_cnt = 0;
  rq_cpu_load_cnt_res = false;
#endif
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
      snd::render_snd_buffer();
    }

    // Background task
    if (is_bg_task)
    {
      bg_task();
      clear_bg_task();
    }

    // CPU load measurement
    // TEST::setHigh();
    u16 t = EVT_TIM::getCounter();
    asm ("wfi \n");
    t = EVT_TIM::getCounter() - t;
    // TEST::setLow();
    if (rq_cpu_load_cnt_res)
    {
      rq_cpu_load_cnt_res = false;
      cpu_load_cnt = t;
    }
    else
      cpu_load_cnt += t;
  }
}
