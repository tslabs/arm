/*******************************************************************************
 *
 * Copyright (C) 2019 TSL
 *
 * Licenced with GNU General Public License
 * <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

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
      static inline void Initialize();
      static inline void SendStart();
      static inline void SendStop();
      static inline bool SendByte(u8);
      static inline u8   RecvByte(bool);
      static inline bool ReadReg(u8, u8, u8&);
      static inline bool ReadReg(u8, u8, void*, u8);
      static inline bool WriteReg(u8, u8, u8);
      static inline bool WriteReg(u8, u8, const void*, u8);

    private:
      static inline void waitScl();
  };
};

#include "../../bits/soft_i2c.tcc"
