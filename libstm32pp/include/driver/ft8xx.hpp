
#pragma once

#include <device_select.hpp>
#include "../defs.hpp"
#include "../peripheral/gpio.hpp"
#include "peripheral/spi.hpp"
#include "../../memorymap/ft8xx.hpp"

namespace ft8xx
{
  typedef struct
  {
    u8 f_mul;       // PLL multiplier
    u8 f_div;       // Pixel Clock divisor
    u16 h_fporch;   // Horizontal front porch size
    u16 h_sync;     // Horizontal sync size
    u16 h_bporch;   // Horizontal back porch size
    u16 h_visible;  // Horizontal visible area size
    u16 v_fporch;   // Vertical front porch size
    u16 v_sync;     // Vertical sync size
    u16 v_bporch;   // Vertical back porch size
    u16 v_visible;  // Vertical visible area size
  } MODE;

  template <spi::Address S, gpio::Address P, u8 N>
  class Functions
  {
    typedef spi::Functions<S> SPI;
    typedef gpio::Pin<P, N> SS;

  public:
    static void initialize(const MODE&);
    static void sendCommand(u8);
    static void sendCommand(u8, u8);
    static u32 readReg(u32);
    static void writeReg(u32, u32);
    static void readMem(u8*, u32, u32);
    static void writeMem(const u8*, u32, u32);
    static u8 sendByte(u8);
    static void cpWait();
    static void cpReset();
    static void ccmdWrite();
    static bool loadCfifo(void*, u16);

  private:
    Functions();
  };
}  // namespace ft8xx

#include "../../bits/ft8xx.tcc"
