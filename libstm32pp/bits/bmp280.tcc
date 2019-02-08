
/*******************************************************************************
 *
 * Copyright (C) 2019 TSL
 *
 * Licenced with GNU General Public License
 * <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

namespace bmp280
{
  template<typename I2C>
  bool Functions<I2C>::Initialize(CALIB& calib)
  {
    u8 *c = (u8*)&calib;
    u8 reg;

/*
    // check ID
    if (!I2C::ReadReg(DEVICE_ADDR, REG_ID, reg)) return false;
    if (reg != REG_ID_VAL) return false;

    // read calibration
    for (u8 i = 0; i <= (REG_CALIB_21 - REG_CALIB_00); i++)
      if (!I2C::ReadReg(DEVICE_ADDR, REG_CALIB_00 + i, c[i ^ 1])) return false;
*/
    return true;
  }

  template<typename I2C>
  bool Functions<I2C>::Measure(s32 &ut, s32 &up)
  {
    u8 reg;
/*
    // read raw temperature
    if (!I2C::WriteReg(DEVICE_ADDR, REG_CTRL_MEAS, MEAS_TEMPER)) return false;
    delay_ms(5);
    if (!I2C::ReadReg(DEVICE_ADDR, REG_OUT_LSB, reg)) return false;
    ut = reg;
    if (!I2C::ReadReg(DEVICE_ADDR, REG_OUT_MSB, reg)) return false;
    ut |= ((u16)reg << 8);

    // read raw pressure
    if (!I2C::WriteReg(DEVICE_ADDR, REG_CTRL_MEAS, MEAS_PRES_8X)) return false;
    delay_ms(26);
    if (!I2C::ReadReg(DEVICE_ADDR, REG_OUT_XLSB, reg)) return false;
    up = reg;
    if (!I2C::ReadReg(DEVICE_ADDR, REG_OUT_LSB, reg)) return false;
    up |= ((u32)reg << 8);
    if (!I2C::ReadReg(DEVICE_ADDR, REG_OUT_MSB, reg)) return false;
    up |= ((u32)reg << 16);
    up >>= 5;
*/
    return true;
  }

  void CalculatePressure(s32 ut, s32 up, s32 &t, s32 &p, CALIB &calib)
  {
    #define _pow2(a) ((s32)((u32)1 << (a)))
/*
    s32 x1, x2, x3, b3, b5, b6;
    u32 b4, b7;

    x1 = (ut - (u32)calib.ac6) * (u32)calib.ac5 / _pow2(15);
    x2 = (s32)calib.mc * _pow2(11) / (x1 + (s32)calib.md);
    b5 = x1 + x2;
    t = (b5 + 8) / _pow2(4);

    b6 = b5 - 4000;
    x1 = ((s32)calib.b2 * (b6 * b6 / _pow2(12))) / _pow2(11);
    x2 = (s32)calib.ac2 * b6 / _pow2(11);
    x3 = x1 + x2;
    b3 = ((((s32)calib.ac1 * _pow2(2) + x3) << 3) + 2) / _pow2(2);
    x1 = (s32)calib.ac3 * b6 / _pow2(13);
    x2 = ((s32)calib.b1 * (b6 * b6 / _pow2(12))) / _pow2(16);
    x3 = ((x1 + x2) + 2) / _pow2(2);
    b4 = (s32)calib.ac4 * (u32)(x3 + 32768) / _pow2(15);
    b7 = ((u32)up - b3) * (s32)(50000 >> 3);
    p = (b7 & 0x80000000) ? ((b7 / b4) * 2) : ((b7 * 2) / b4);
    x1 = p / _pow2(8);
    x1 *= x1;
    x1 = (x1 * 3038) / _pow2(16);
    x2 = ((s32)(-7357) * p) / _pow2(16);
    p = p + (x1 + x2 + 3791) / _pow2(4);
*/
    #undef _pow2
  }
}