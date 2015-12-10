
#pragma once

namespace soft_i2c
{
  // init hardware: GPIO, timer
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  void Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::initialize()
  {
    TIMER::enableClock();
    TIMER::configureBasicCounter(
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
    SCL::pullUp();
    SCL::setMode(gpio::cr::GP_OPEN_DRAIN_2MHZ);
    SDA::pullUp();
    SDA::setMode(gpio::cr::GP_OPEN_DRAIN_2MHZ);
#else
    SCL::setPullMode(gpio::pupdr::PULL_UP);
    SCL::setOutputMode(gpio::otyper::OPEN_DRAIN);
    SCL::setMode(gpio::moder::OUTPUT);
    SDA::setPullMode(gpio::pupdr::PULL_UP);
    SDA::setOutputMode(gpio::otyper::OPEN_DRAIN);
    SDA::setMode(gpio::moder::OUTPUT);
#endif
  }

  // check if SCL line is free or the slave holds it
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  void Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::waitSCL()
  {
    u16 t;
    while (!SCL::getInput())
      if (++t == 50000) return;  // !!! add here BUS error indication and time-out option
  }

  // send I2C START condition
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  void Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::sendStart()
  {
    SDA::setHigh();
    TIMER::delay(DELAY);
    SCL::setHigh();
    TIMER::delay(DELAY);
    waitSCL();
    SDA::setLow();
    TIMER::delay(DELAY);
    SCL::setLow();
  }

  // send I2C STOP condition
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  void Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::sendStop()
  {
    SDA::setLow();
    TIMER::delay(DELAY);
    SCL::setHigh();
    TIMER::delay(DELAY);
    waitSCL();
    SDA::setHigh();
  }

  // send byte
  // returns ACK status: 0 - NACK, 1 - ACK
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  bool Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::sendByte(u8 d)
  {
    for (u8 i = 0; i < 8; i++)
    {
      if (d & 0x80) SDA::setHigh(); else SDA::setLow();
      TIMER::delay(DELAY);
      SCL::setHigh();
      d <<= 1;
      TIMER::delay(DELAY);
      waitSCL();
      SCL::setLow();
    }

    SDA::setHigh();
    TIMER::delay(DELAY);
    SCL::setHigh();
    TIMER::delay(DELAY);
    waitSCL();
    bool rc = !SDA::getInput();
    SCL::setLow();

    return rc;
  }

  // receive byte
  // input: return 0 - NACK, 1 - ACK
  template<gpio::Address SCL_PORT, u8 SCL_PIN, gpio::Address SDA_PORT, u8 SDA_PIN, tim::Address DELAY_TIMER_ADDRESS, u32 FREQUENCY>
  u8 Functions<SCL_PORT, SCL_PIN, SDA_PORT, SDA_PIN, DELAY_TIMER_ADDRESS, FREQUENCY>::recvByte(bool ack)
  {
    u8 d = 0;
    SDA::setHigh();

    for (u8 i = 0; i < 8; i++)
    {
      TIMER::delay(DELAY);
      SCL::setHigh();
      TIMER::delay(DELAY);
      waitSCL();
      d <<= 1; if (SDA::getInput()) d++;
      SCL::setLow();
    }

    // send I2C ACK/NACK condition
    if (ack) SDA::setLow(); else SDA::setHigh();
    TIMER::delay(DELAY);
    SCL::setHigh();
    TIMER::delay(DELAY);
    waitSCL();
    SCL::setLow();

    return d;
  }
}
