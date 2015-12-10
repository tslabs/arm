
#pragma once

#include <device_select.hpp>
#include "../defs.hpp"

#include "../peripheral/gpio.hpp"
#include "../peripheral/tim.hpp"

namespace soft_i2c
{
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  class Functions
  {
    typedef gpio::Pin<SCL_PORT, SCL_PIN> SCL;
    typedef gpio::Pin<SDA_PORT, SDA_PIN> SDA;
    typedef tim::Functions<DELAY_TIMER_ADDRESS> TIMER;

    enum
    {
      DELAY = TIMER::FREQUENCY / (2 * FREQUENCY)
    };

    public:
      static inline void initialize();
      static inline void sendStart();
      static inline void sendStop();
      static inline void waitSCL();
      static bool sendByte(u8);
      static u8 recvByte(bool);

    private:
      Functions();
  };
};

#include "../../bits/soft_i2c.tcc"
