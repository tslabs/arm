
#pragma once

#include <device_select.hpp>
#include "../defs.hpp"

#include "../peripheral/gpio.hpp"
#include "../peripheral/tim.hpp"

namespace onewire
{
  // Delays in us for 1-Wire signaling, us
  enum Delay
  {
    D_RESET    = 480,
    D_RS_WAIT  = 60,
    D_RS_EPI   = 250,
    D_RW_EPI   = 60
  };

  template<gpio::Address DATA_PORT, u8 DATA_PIN, tim::Address DELAY_TIMER_ADDRESS>
  class Functions
  {
    typedef gpio::Pin<DATA_PORT, DATA_PIN> DATA;
    typedef tim::Functions<DELAY_TIMER_ADDRESS> TIMER;
    
    public:
      static inline void initializeTimer();
      static inline void initialize();
      static inline bool reset();
      static inline u8 readByte();
      static inline void writeByte(u8);

    private:
      Functions();
  };
}

#include "../../bits/1wire.tcc"
