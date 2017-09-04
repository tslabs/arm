
// Note: only normal SPI (1 bit duplex) commands are supported by now

#pragma once

#include <device_select.hpp>
#include "../defs.hpp"
#include "../peripheral/gpio.hpp"
#include "../peripheral/spi.hpp"
#include "../../memorymap/spiflash.hpp"

namespace spiflash
{
  template <spi::Address S, gpio::Address SS_PORT, u8 SS_PIN>
  class Functions
  {
    typedef spi::Functions<S> SPI;
    typedef gpio::Pin<SS_PORT, SS_PIN> SS;

    public:
      static inline void initialize();
      static inline void ssLow();
      static inline void ssHigh();
      static u8 sendByte(u8);
      static void sendDummy(int);
      static u32 detectSize();
      static void readBlock(u32, u8*, u32);
      static bool verifyBlock(u32, u8*, u32);
      static void writeBlock(u32, u8*, u32);
      static void eraseSector(u32);
      static bool checkBlank(u32, u32);
      static void eraseChip();
      static u8 readStatus();
      static void waitBusy();
      static bool isBusy();

    private:
      Functions();
  };
};

#include "../../bits/spiflash.tcc"
