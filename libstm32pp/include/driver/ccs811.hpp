
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

namespace ccs811
{
  // registers
  enum
  {
    REG_STATUS          = 0x00,  // Status register
    REG_MEAS_MODE       = 0x01,  // Measurement mode and conditions register
    REG_ALG_RESULT_DATA = 0x02,  // Algorithm result. The most significant 2 bytes contain a ppm estimate of the equivalent CO2 (eCO2) level, and the next two bytes contain a ppb estimate of the total VOC level
    REG_RAW_DATA        = 0x03,  // Raw ADC data values for resistance and current source used
    REG_ENV_DATA        = 0x05,  // Temperature and Humidity data can be written to enable compensation
    REG_NTC             = 0x06,  // Provides the voltage across the reference resistor and the voltage across the NTC resistor – from which the ambient temperature can be determined
    REG_THRESHOLDS      = 0x10,  // Thresholds for operation when interrupts are only generated when eCO2 ppm crosses a threshold
    REG_BASELINE        = 0x11,  // The encoded current baseline value can be read. A previously saved encoded baseline can be written
    REG_HW_ID           = 0x20,  // Hardware ID. The value is 0x81
    REG_HW_VERSION      = 0x21,  // Hardware Version. The value is 0x1X
    REG_FW_BOOT_VERSION = 0x23,  // Firmware Boot Version. The first 2 bytes contain the firmware version number for the boot code
    REG_FW_APP_VERSION  = 0x24,  // Firmware Application Version. The first 2 bytes contain the firmware version number for the application code
    REG_ERROR_ID        = 0xE0,  // Error ID. When the status register reports an error its source is located in this register
    REG_SW_RESET        = 0xFF   // If the correct 4 bytes (0x11 0xE5 0x72 0x8A) are written to this register in a single sequence the device will reset and return to BOOT mode
  };

  // address
  enum
  {
    DEVICE_ADDR = 0x5A
  };

  // IDs
  enum
  {
    REG_HW_ID_VAL  = 0x81
  };

  typedef struct
  {
    u8 hw;
    u16 fw_boot;
    u16 fw_app;
  } VERSION;

  template<typename>
  class Functions
  {
    public:
      static bool Initialize();
      static bool ReadVer(VERSION&);
    private:
  };
}

#include "../../bits/ccs811.tcc"
