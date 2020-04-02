
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

namespace bmp280
{
  // calibration array
  typedef struct
  {
    u16 dig_t1;
    s16 dig_t2;
    s16 dig_t3;
    u16 dig_p1;
    s16 dig_p2;
    s16 dig_p3;
    s16 dig_p4;
    s16 dig_p5;
    s16 dig_p6;
    s16 dig_p7;
    s16 dig_p8;
    s16 dig_p9;
  } CALIB;

  // address
  enum
  {
    DEVICE_ADDR = 0x77
  };

  // registers
  enum
  {
    REG_OUT_XLSB   = 0xF8,
    REG_OUT_LSB    = 0xF7,
    REG_OUT_MSB    = 0xF6,
    REG_CTRL_MEAS  = 0xF4,
    REG_SOFT_RESET = 0xE0,
    REG_ID         = 0xD0,
    REG_CALIB_21   = 0xBF,
    REG_CALIB_00   = 0xAA
  };

  // REG_CTRL_MEAS
  enum
  {
    MEAS_TEMPER  = 0x2E,
    MEAS_PRES_1X = 0x34,
    MEAS_PRES_2X = 0x74,
    MEAS_PRES_4X = 0xB4,
    MEAS_PRES_8X = 0xF4
  };

  // REG_SOFT_RESET
  enum
  {
    SOFT_RES_VAL = 0xB6
  };

  // REG_ID
  enum
  {
    REG_ID_VAL = 0x58
  };

  template<typename>
  class Functions
  {
    public:
      static bool Initialize(CALIB&);
      static bool Measure(s32&, s32&);

    private:
  };

  void CalculatePressure(s32, s32, s32&, s32&, CALIB&);
}

#include "../../bits/bmp280.tcc"
