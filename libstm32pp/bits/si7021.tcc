
/*******************************************************************************
 *
 * Copyright (C) 2019 TSL
 *
 * Licenced with GNU General Public License
 * <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

namespace si7021
{
  template<typename I2C>
  bool Functions<I2C>::Initialize()
  {
  }
  
  // measure humidity and temperature
  template<typename I2C>
  bool Functions<I2C>::Measure(u16 &hum, u16 &tem)
  {
    I2C::SendStart();
    if (!I2C::SendByte(DEVICE_ADDR << 1)) goto err;
    if (!I2C::SendByte(CMD_MS_HUM)) goto err;

    for (u8 i = 1; i < 255; i++)
    {
      I2C::SendStart();
      if (I2C::SendByte((DEVICE_ADDR << 1) | 1)) goto read1;
    }
    goto err;

  read1:
    hum = (I2C::RecvByte(true)) << 8;
    hum |= I2C::RecvByte(false);
    I2C::SendStop();

    I2C::SendStart();
    if (!I2C::SendByte(DEVICE_ADDR << 1)) goto err;
    if (!I2C::SendByte(CMD_MS_TEM)) goto err;

    for (u16 i = 1; i < 1000; i++)
    {
      I2C::SendStart();
      if (I2C::SendByte((DEVICE_ADDR << 1) | 1)) goto read2;
    }
    goto err;

  read2:
    tem = (I2C::RecvByte(true)) << 8;
    tem |= I2C::RecvByte(false);
    I2C::SendStop();
    return true;

  err:
    I2C::SendStop();
    return false;
  }

  // read unique serial number
  template<typename I2C>
  bool Functions<I2C>::ReadSn(u8 *sn)
  {
    I2C::SendStart();
    if (!I2C::SendByte(DEVICE_ADDR << 1)) goto err;
    if (!I2C::SendByte(CMD_RD_ID1_1)) goto err;
    if (!I2C::SendByte(CMD_RD_ID1_2)) goto err;
    I2C::SendStart();
    if (!I2C::SendByte((DEVICE_ADDR << 1) | 1)) goto err;
    for (u8 i = 0; i < 4; i++)
    {
      sn[i] = I2C::RecvByte(true);
      I2C::RecvByte(i != 3);
    }
    I2C::SendStop();

    I2C::SendStart();
    if (!I2C::SendByte(DEVICE_ADDR << 1)) goto err;
    if (!I2C::SendByte(CMD_RD_ID2_1)) goto err;
    if (!I2C::SendByte(CMD_RD_ID2_2)) goto err;
    I2C::SendStart();
    if (!I2C::SendByte((DEVICE_ADDR << 1) | 1)) goto err;
    for (u8 i = 4; i < 8;)
    {
      sn[i++] = I2C::RecvByte(true);
      sn[i++] = I2C::RecvByte(true);
      I2C::RecvByte(i != 8);
    }
    I2C::SendStop();
    return true;

  err:
    I2C::SendStop();
    return false;
  }

  // returns normalized humidity value in 8.8 fixed point format: bits 8..15 - integer part, bits 0..7 - fractal part
  u16 CalculateHumidity(u16 val)
  {
    return ((125 * (u32)val) >> 8) - (6 << 8);   // %RH = ((125 * val) / 65536) - 6
  }

  // returns normalized temperature value in 8.8 fixed point format: bits 8..15 - integer part, bits 0..7 - fractal part
  u16 CalculateTemperature(u16 val)
  {
    return ((44984 * (u32)val) >> 16) - 11993;   // 'C = ((175.72 * val) / 65536) - 46.85
  }
}