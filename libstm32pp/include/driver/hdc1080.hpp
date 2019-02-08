
/*******************************************************************************
 *
 * Copyright (C) 2019 TSL
 *
 * Licenced with GNU General Public License
 * <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#pragma once

#include "defs.hpp"
#include "gpio.hpp"
#include "softi2c.hpp"

namespace hdc1080
{
  // registers
  enum
  {
    REG_TEMP      = 0x00,  // Temperature 0x0000 Temperature measurement output
    REG_HUM       = 0x01,  // Humidity 0x0000 Relative Humidity measurement output
    REG_CONF_STAT = 0x02,  // Configuration 0x1000 HDC1080 configuration and status
    REG_SER_ID0   = 0xFB,  // Serial ID device dependent First 2 bytes of the serial ID of the part
    REG_SER_ID1   = 0xFC,  // Serial ID device dependent Mid 2 bytes of the serial ID of the part
    REG_SER_ID2   = 0xFD,  // Serial ID device dependent Last byte bit of the serial ID of the part
    REG_MF_ID     = 0xFE,  // Manufacturer ID 0x5449 ID of Texas Instruments
    REG_DEV_ID    = 0xFF   // Device ID 0x1050 ID of the device
  };

  // address
  enum
  {
    DEVICE_ADDR = 0x40
  };

  // IDs (LSB/MSB are swapped)
  enum
  {
    REG_MF_ID_VAL  = 0x4954,
    REG_DEV_ID_VAL = 0x5010
  };

  template<typename>
  class Functions
  {
    public:
      static bool Initialize();
      static bool ReadId(void*);
    private:
  };
}

#include "../../bits/hdc1080.tcc"
