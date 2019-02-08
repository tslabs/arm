
/*******************************************************************************
 *
 * Copyright (C) 2019 TSL
 *
 * Licenced with GNU General Public License
 * <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#pragma once

#include "soft_i2c.hpp"

namespace si7021
{
  enum
  {
    DEVICE_ADDR = 0x40,
    SIZE_OF_SN  = 8
  };

  // Si7021 commands
  enum
  {
    CMD_RS        = 0xFE,
    CMD_MS_HUM_H  = 0xE5,
    CMD_MS_HUM    = 0xF5,
    CMD_MS_TEM_H  = 0xE3,
    CMD_MS_TEM    = 0xF3,
    CMD_RD_TEM    = 0xE0,
    CMD_RD_UR1    = 0xE7,
    CMD_WR_UR1    = 0xE6,
    CMD_RD_HCR    = 0x11,
    CMD_WR_HCR    = 0x51,
    CMD_RD_ID1_1  = 0xFA,
    CMD_RD_ID1_2  = 0x0F,
    CMD_RD_ID2_1  = 0xFC,
    CMD_RD_ID2_2  = 0xC9,
    CMD_RD_FWR_1  = 0x84,
    CMD_RD_FWR_2  = 0xB8
  };

  // Si7021 registers
  enum
  {
    REG_UR1,
    REG_HCR
  };

  template<typename>
  class Functions
  {
    public:
      static bool Measure(u16&, u16&);
      static bool ReadSn(u8*);

    private:
  };

  u16 CalculateHumidity(u16);
  u16 CalculateTemperature(u16);
}

#include "../../bits/si7021.tcc"
