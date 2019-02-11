
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
  enum APP_REGS
  {
    // Application Mode and common for both modes regs
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
    REG_SW_RESET        = 0xFF,  // If the correct 4 bytes (0x11 0xE5 0x72 0x8A) are written to this register in a single sequence the device will reset and return to BOOT mode

    // Boot Mode regs
    REG_APP_ERASE       = 0xF1,  // If the correct (0xE7 0xA7 0xE6 0x09) are written to this register in a single sequence the device will start the application erase
    REG_APP_DATA        = 0xF2,  // Transmit flash code for the bootloader to write to the application flash code space.
    REG_APP_VERIFY      = 0xF3,  // Starts the process of the bootloader checking though the application to make sure a full image is valid.
    REG_APP_START       = 0xF4,  // Application start. Used to transition the CCS811 state from boot to application mode, a write with no data is required. Before performing a write to APP_START the Status register should be accessed to check if there is a valid application present.
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

  enum
  {
    MODE_0 = 0,
    MODE_1 = 1,
    MODE_2 = 2,
    MODE_3 = 3,
    MODE_4 = 4,
  };

  typedef struct
  {
    u8 hw;
    u16 fw_boot;
    u16 fw_app;
  } VERSION;

  typedef struct
  {
    union
    {
      struct
      {
        u8 error : 1;
        u8 _pad0 : 2;
        u8 data_ready : 1;
        u8 app_valid : 1;
        u8 _pad1 : 2;
        u8 fw_mode : 1;
      };
      u8 byte;
    };
  } STATUS;

  typedef struct
  {
    union
    {
      struct
      {
        u8 wreg_inv : 1;
        u8 rred_inv : 1;
        u8 mmod_inv : 1;
        u8 max_res : 1;
        u8 heat_flt : 1;
        u8 heat_supl : 1;
        u8 _pad0 : 2;
      };
      u8 byte;
    };
  } ERROR;

  typedef struct
  {
    union
    {
      struct
      {
        u8 _pad0 : 2;
        u8 int_thresh : 1;
        u8 int_datardy : 1;
        u8 drive_mode : 3;
        u8 _pad1 : 1;
      };
      u8 byte;
    };
  } MEAS_MODE;

  typedef struct
  {
    union
    {
      struct
      {
        u16 eco2;   // 400..8192ppm
        u16 tvoc;   // 0..1187ppb
      };
      u8 bytes[4];
    };
  } RES_DATA;

  const u8 RESET_SEQ[] = {0x11, 0xE5, 0x72, 0x8A};

  template<typename>
  class Functions
  {
    public:
      static bool Initialize();
      static bool ReadVer(VERSION&);
      static bool ReadStatus(STATUS&);
      static bool ReadError(ERROR&);
      static bool ReadData(RES_DATA&);
      static bool RunApplication();
      static bool SoftwareReset();
      static bool SetMeasurementMode(MEAS_MODE&);
      static bool SetEnvironment(s16_16&, s16_16&);
    private:
  };
}

#include "../../bits/ccs811.tcc"
