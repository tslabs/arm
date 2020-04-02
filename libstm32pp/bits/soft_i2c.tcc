
/*******************************************************************************
 *
 * Copyright (C) 2019 TSL
 *
 * Licenced with GNU General Public License
 * <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

namespace soft_i2c
{
  // init hardware: GPIO, timer
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  void Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::Initialize()
  {
#ifndef SOFT_I2C_DUMMY
    TIMER::enableClock();
    TIMER::configureCounter(
      tim::cr1::cen::COUNTER_DISABLED,
      tim::cr1::udis::UPDATE_EVENT_ENABLED,
      tim::cr1::urs::UPDATE_REQUEST_SOURCE_OVERFLOW_UNDERFLOW,
      tim::cr1::opm::DONT_STOP_COUNTER_AT_NEXT_UPDATE_EVENT,
      tim::cr1::arpe::AUTO_RELOAD_UNBUFFERED);
    TIMER::setMaxResolution();

    SCL::enableClock();
    SDA::enableClock();

    SCL::setHigh();
    SDA::setHigh();

#ifdef STM32F1XX
    SCL::setMode(gpio::cr::GP_OPEN_DRAIN_2MHZ);
    SDA::setMode(gpio::cr::GP_OPEN_DRAIN_2MHZ);
#else // STM32F1XX
    SCL::setPullMode(gpio::pupdr::PULL_UP);
    SCL::setOutputMode(gpio::otyper::OPEN_DRAIN);
    SCL::setMode(gpio::moder::OUTPUT);
    SDA::setPullMode(gpio::pupdr::PULL_UP);
    SDA::setOutputMode(gpio::otyper::OPEN_DRAIN);
    SDA::setMode(gpio::moder::OUTPUT);
#endif  // STM32F1XX
#endif  // SOFT_I2C_DUMMY
  }

  // check if SCL line is free or the slave holds it
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  void Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::waitScl()
  {
#ifndef SOFT_I2C_DUMMY
    u16 t;
    while (!SCL::getInput())
      if (++t == 50000) return;  // !!! add here BUS error indication and time-out option
#endif  // SOFT_I2C_DUMMY
  }

  // send I2C START condition
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  void Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::SendStart()
  {
#ifndef SOFT_I2C_DUMMY
    SDA::setHigh();
    TIMER::delay(DELAY);
    SCL::setHigh(); waitScl();
    TIMER::delay(DELAY);
    SDA::setLow();
    TIMER::delay(DELAY);
    SCL::setLow();
#endif  // SOFT_I2C_DUMMY
  }

  // send I2C STOP condition
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  void Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::SendStop()
  {
#ifndef SOFT_I2C_DUMMY
    SDA::setLow();
    TIMER::delay(DELAY);
    SCL::setHigh(); waitScl();
    TIMER::delay(DELAY);
    SDA::setHigh();
    TIMER::delay(DELAY);
#endif  // SOFT_I2C_DUMMY
  }

  // send byte
  // returns ACK status: 0 - NACK, 1 - ACK
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  bool Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::SendByte(u8 d)
  {
#ifndef SOFT_I2C_DUMMY
    for (u8 i = 0; i < 8; i++)
    {
      if (d & 0x80) SDA::setHigh(); else SDA::setLow();
      d <<= 1;
      TIMER::delay(DELAY);
      SCL::setHigh(); waitScl();
      TIMER::delay(DELAY);
      SCL::setLow();
    }

    SDA::setHigh();
    TIMER::delay(DELAY);
    SCL::setHigh(); waitScl();
    TIMER::delay(DELAY);
    bool rc = !SDA::getInput();
    SCL::setLow();

    return rc;
#else  // SOFT_I2C_DUMMY
    return false;
#endif  // SOFT_I2C_DUMMY
  }

  // receive byte
  // input: return 0 - NACK, 1 - ACK
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  u8 Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::RecvByte(bool ack)
  {
#ifndef SOFT_I2C_DUMMY
    u8 d = 0;
    SDA::setHigh();

    for (u8 i = 0; i < 8; i++)
    {
      TIMER::delay(DELAY);
      SCL::setHigh(); waitScl();
      TIMER::delay(DELAY);
      d <<= 1; if (SDA::getInput()) d++;
      SCL::setLow();
    }

    // send I2C ACK/NACK condition
    if (ack) SDA::setLow(); else SDA::setHigh();
    TIMER::delay(DELAY);
    SCL::setHigh(); waitScl();
    TIMER::delay(DELAY);
    SCL::setLow();

    return d;
#else  // SOFT_I2C_DUMMY
    return 0;
#endif  // SOFT_I2C_DUMMY
  }

  // read register from device: <S><SLAW><reg><R><SLAR><data><P>
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  bool Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::ReadReg(u8 addr, u8 reg, u8 &data)
  {
    return ReadReg(addr, reg, &data, 1);
  }

  // read register from device: <S><SLAW><reg><R><SLAR><data><P>
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  bool Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::ReadReg(u8 addr, u8 reg, void *data, u8 len)
  {
    u8 *_data = (u8*)data;

    SendStart();
    if (!SendByte(addr << 1)) goto err;
    if (!SendByte(reg)) goto err;
    SendStart();
    if (!SendByte((addr << 1) | 1)) goto err;

    while (len--)
      *_data++ = RecvByte(len != 0);

    SendStop();
    return true;

  err:
    SendStop();
    return false;
  }

  // write register to device: <S><SLAW><reg><data><P>
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  bool Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::WriteReg(u8 addr, u8 reg, u8 data)
  {
    return WriteReg(addr, reg, &data, 1);
  }

  // write register to device: <S><SLAW><reg><data><P>
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  bool Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::WriteReg(u8 addr, u8 reg, const void *data, u8 len)
  {
    u8 *_data = (u8*)data;
    
    SendStart();
    if (!SendByte(addr << 1)) goto err;
    if (!SendByte(reg)) goto err;

    while (len--)
      if (!SendByte(*_data++)) goto err;

    SendStop();
    return true;

  err:
    SendStop();
    return false;
  }
}
