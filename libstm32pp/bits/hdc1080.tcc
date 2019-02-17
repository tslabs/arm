
/*******************************************************************************
 *
 * Copyright (C) 2019 TSL
 *
 * Licenced with GNU General Public License
 * <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

namespace hdc1080
{
# define SWAP_BYTES(a) \
  { \
    u8 *b = (u8*)&a; \
    u8 c = b[1]; \
    b[1] = b[0]; \
    b[0] = c; \
  }

  template<typename I2C, tim::Address DELAY_TIMER_ADDRESS>
  bool Functions<I2C, DELAY_TIMER_ADDRESS>::Initialize()
  {
    u8 data[2];

    // check ID
    if (!I2C::ReadReg(DEVICE_ADDR, REG_MF_ID, data, 2)) return false;
    if (*(u16*)data != REG_MF_ID_VAL) return false;

    if (!I2C::ReadReg(DEVICE_ADDR, REG_DEV_ID, data, 2)) return false;
    if (*(u16*)data != REG_DEV_ID_VAL) return false;

    CONFIG cfg = {};
    cfg.mode = 1;   // Temperature and humidity
    cfg.tres = 0;   // 14 bits
    cfg.hres = 0;   // 14 bits
    if (!Configure(cfg)) return false;

    return true;
  }

  template<typename I2C, tim::Address DELAY_TIMER_ADDRESS>
  bool Functions<I2C, DELAY_TIMER_ADDRESS>::Configure(CONFIG cfg)
  {
    SWAP_BYTES(cfg.word);
    return I2C::WriteReg(DEVICE_ADDR, REG_CONF_STAT, &cfg.word, 2);
  }

  template<typename I2C, tim::Address DELAY_TIMER_ADDRESS>
  bool Functions<I2C, DELAY_TIMER_ADDRESS>::ReadId(void *data)
  {
    u8 *_data = (u8*)data;

    if (!I2C::ReadReg(DEVICE_ADDR, REG_SER_ID0, &_data[0], 2)) return false;
    if (!I2C::ReadReg(DEVICE_ADDR, REG_SER_ID1, &_data[2], 2)) return false;
    if (!I2C::ReadReg(DEVICE_ADDR, REG_SER_ID2, &_data[4], 2)) return false;
    SWAP_BYTES(_data[0]);
    SWAP_BYTES(_data[2]);
    SWAP_BYTES(_data[4]);

    return true;
  }

  template<typename I2C, tim::Address DELAY_TIMER_ADDRESS>
  u8 Functions<I2C, DELAY_TIMER_ADDRESS>::Measure(u16 &hum, u16 &tem)
  {
    if (!I2C::WriteReg(DEVICE_ADDR, REG_TEMP, 0, 0)) return 2;

    TIMER::delay(6350);   // temperature 14 bit
    TIMER::delay(6500);   // humidity 14 bit

    I2C::SendStart();
    if (!I2C::SendByte((DEVICE_ADDR << 1) | 1))
    {
      I2C::SendStop();
      return 3;
    }
    
    u8 data[4];
    u8 *ptr = data;
    u8 len = sizeof(data);
    while (len--) *ptr++ = I2C::RecvByte(len != 0);
    I2C::SendStop();

    SWAP_BYTES(data[0]);
    SWAP_BYTES(data[2]);
    tem = *(u16*)&data[0];
    hum = *(u16*)&data[2];

    return true;
  }

  // returns normalized humidity value in 8.8 fixed point format: bits 8..15 - integer part, bits 0..7 - fractal part
  u16 CalculateHumidity(u16 val)
  {
    return ((100 * (u32)val) >> 8);   // hum, %RH = (val / 2^16) * 100
  }

  // returns normalized temperature value in 8.8 fixed point format: bits 8..15 - integer part, bits 0..7 - fractal part
  u16 CalculateTemperature(u16 val)
  {
    return ((165 * (u32)val) >> 8) - (40 << 8);   // temp, 'C = (val / 2^16 * 165) - 40
  }

#undef SWAP_BYTES
}
