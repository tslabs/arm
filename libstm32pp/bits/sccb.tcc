/*******************************************************************************
 *
 * Copyright (C) 2012 Jorge Aparicio <jorge.aparicio.r@gmail.com>
 *
 * This file is part of libstm32pp.
 *
 * libstm32pp is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * libstm32pp is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libstm32pp. If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#pragma once

namespace sccb {
  /**
   * @brief Initializes the peripherals needed for the SCCB emulation.
   */
  template<
      gpio::Address SDIO_C_PORT,
      u8 SDIO_C_PIN,
      gpio::Address SDIO_D_PORT,
      u8 SDIO_D_PIN,
      tim::Address DELAY_TIMER_ADDRESS,
      u32 FREQUENCY
  >
  void Functions<
      SDIO_C_PORT,
      SDIO_C_PIN,
      SDIO_D_PORT,
      SDIO_D_PIN,
      DELAY_TIMER_ADDRESS,
      FREQUENCY
  >::initialize()
  {
    TIMER::enableClock();

    TIMER::configureCounter(
        tim::cr1::cen::COUNTER_DISABLED,
        tim::cr1::udis::UPDATE_EVENT_ENABLED,
        tim::cr1::urs::UPDATE_REQUEST_SOURCE_OVERFLOW_UNDERFLOW,
        tim::cr1::opm::DONT_STOP_COUNTER_AT_NEXT_UPDATE_EVENT,
        tim::cr1::arpe::AUTO_RELOAD_UNBUFFERED);

    TIMER::setMicroSecondResolution();

    SDIO_C::enableClock();
    SDIO_D::enableClock();

    SDIO_C::setHigh();
    SDIO_D::setHigh();

    SDIO_C::setPullMode(gpio::pupdr::PULL_UP);
    SDIO_D::setPullMode(gpio::pupdr::PULL_UP);

    SDIO_C::setMode(gpio::moder::OUTPUT);

    SDIO_D::setMode(gpio::moder::OUTPUT);
  }

  /**
   * @brief Sends start condition.
   */
  template<
      gpio::Address SDIO_C_PORT,
      u8 SDIO_C_PIN,
      gpio::Address SDIO_D_PORT,
      u8 SDIO_D_PIN,
      tim::Address DELAY_TIMER_ADDRESS,
      u32 FREQUENCY
  >
  void Functions<
      SDIO_C_PORT,
      SDIO_C_PIN,
      SDIO_D_PORT,
      SDIO_D_PIN,
      DELAY_TIMER_ADDRESS,
      FREQUENCY
  >::sendStart()
  {
    SDIO_D::setHigh();
    TIMER::delay(DELAY);

    SDIO_C::setHigh();
    TIMER::delay(DELAY);

    SDIO_D::setLow();
    TIMER::delay(DELAY);

    SDIO_C::setLow();
    TIMER::delay(DELAY);
  }

  /**
   * @brief Sends stop condition.
   */
  template<
      gpio::Address SDIO_C_PORT,
      u8 SDIO_C_PIN,
      gpio::Address SDIO_D_PORT,
      u8 SDIO_D_PIN,
      tim::Address DELAY_TIMER_ADDRESS,
      u32 FREQUENCY
  >
  void Functions<
      SDIO_C_PORT,
      SDIO_C_PIN,
      SDIO_D_PORT,
      SDIO_D_PIN,
      DELAY_TIMER_ADDRESS,
      FREQUENCY
  >::sendStop()
  {
    SDIO_D::setLow();
    TIMER::delay(DELAY);

    SDIO_C::setHigh();
    TIMER::delay(DELAY);

    SDIO_D::setHigh();
    TIMER::delay(DELAY);
  }

  /**
   * @brief Sends NACK condition.
   */
  template<
      gpio::Address SDIO_C_PORT,
      u8 SDIO_C_PIN,
      gpio::Address SDIO_D_PORT,
      u8 SDIO_D_PIN,
      tim::Address DELAY_TIMER_ADDRESS,
      u32 FREQUENCY
  >
  void Functions<
      SDIO_C_PORT,
      SDIO_C_PIN,
      SDIO_D_PORT,
      SDIO_D_PIN,
      DELAY_TIMER_ADDRESS,
      FREQUENCY
  >::sendNACK()
  {
    SDIO_D::setHigh();
    TIMER::delay(DELAY);

    SDIO_C::setHigh();
    TIMER::delay(DELAY);

    SDIO_C::setLow();
    TIMER::delay(DELAY);

    SDIO_D::setLow();
    TIMER::delay(DELAY);
  }

  /**
   * @brief Sends byte via SCCB.
   */
  template<
      gpio::Address SDIO_C_PORT,
      u8 SDIO_C_PIN,
      gpio::Address SDIO_D_PORT,
      u8 SDIO_D_PIN,
      tim::Address DELAY_TIMER_ADDRESS,
      u32 FREQUENCY
  >
  bool Functions<
      SDIO_C_PORT,
      SDIO_C_PIN,
      SDIO_D_PORT,
      SDIO_D_PIN,
      DELAY_TIMER_ADDRESS,
      FREQUENCY
  >::sendByte(u8 data)
  {
    bool sucess;

    for (u8 i = 0; i < 8; i++) {
      if ((data << i) & 0x80)
        SDIO_D::setHigh();
      else
        SDIO_D::setLow();
      TIMER::delay(DELAY);

      SDIO_C::setHigh();
      TIMER::delay(DELAY);

      SDIO_C::setLow();
      TIMER::delay(DELAY);
    }

    SDIO_D::setMode(gpio::moder::INPUT);
    TIMER::delay(DELAY);

    SDIO_C::setHigh();
    TIMER::delay(DELAY);

    if (SDIO_D::isHigh())
      sucess = false;
    else
      sucess = true;

    SDIO_C::setLow();
    TIMER::delay(DELAY);

    SDIO_D::setMode(gpio::moder::OUTPUT);

    return sucess;
  }

  /**
   * @brief Gets byte via SCCB.
   */
  template<
      gpio::Address SDIO_C_PORT,
      u8 SDIO_C_PIN,
      gpio::Address SDIO_D_PORT,
      u8 SDIO_D_PIN,
      tim::Address DELAY_TIMER_ADDRESS,
      u32 FREQUENCY
  >
  u8 Functions<
      SDIO_C_PORT,
      SDIO_C_PIN,
      SDIO_D_PORT,
      SDIO_D_PIN,
      DELAY_TIMER_ADDRESS,
      FREQUENCY
  >::getByte()
  {
    u8 data = 0;

    SDIO_D::setMode(gpio::moder::INPUT);
    TIMER::delay(DELAY);

    for (u8 i = 8; i > 0; i--) {
      SDIO_C::setHigh();
      TIMER::delay(DELAY);

      data = data << 1;
      if (SDIO_D::isHigh())
        data++;

      SDIO_C::setLow();
      TIMER::delay(DELAY);
    }

    SDIO_D::setMode(gpio::moder::OUTPUT);

    return data;
  }

  /**
   * @brief Writes <value> in <registerAddress> on <slaveAddress> device.
   * @note  Returns true if the operation was successful.
   */
  template<
      gpio::Address SDIO_C_PORT,
      u8 SDIO_C_PIN,
      gpio::Address SDIO_D_PORT,
      u8 SDIO_D_PIN,
      tim::Address DELAY_TIMER_ADDRESS,
      u32 FREQUENCY
  >
  bool Functions<
      SDIO_C_PORT,
      SDIO_C_PIN,
      SDIO_D_PORT,
      SDIO_D_PIN,
      DELAY_TIMER_ADDRESS,
      FREQUENCY
  >::writeSlaveRegister(
      u8 const slaveAddress,
      u8 const registerAddress,
      u8 const value)
  {
    sendStart();

    if (sendByte(2 * slaveAddress)) {
      if (sendByte(registerAddress)) {
        if (sendByte(value)) {
          sendStop();
          return true;
        } else {
          sendStop();
          return false;
        }
      } else {
        sendStop();
        return false;
      }
    } else {
      sendStop();
      return false;
    }
  }

  /**
   * @brief Reads <registerAddress> from <slaveAddress> device.
   * @note  Returns true if the operation was successful.
   */
  template<
      gpio::Address SDIO_C_PORT,
      u8 SDIO_C_PIN,
      gpio::Address SDIO_D_PORT,
      u8 SDIO_D_PIN,
      tim::Address DELAY_TIMER_ADDRESS,
      u32 FREQUENCY
  >
  bool Functions<
      SDIO_C_PORT,
      SDIO_C_PIN,
      SDIO_D_PORT,
      SDIO_D_PIN,
      DELAY_TIMER_ADDRESS,
      FREQUENCY
  >::readSlaveRegister(
      u8 const slaveAddress,
      u8 const registerAddress,
      u8& value)
  {
    sendStart();

    if (sendByte(2 * slaveAddress)) {
      if (sendByte(registerAddress)) {
        sendStop();
        sendStart();

        if (sendByte(2 * slaveAddress + 1)) {
          value = getByte();

          sendNACK();

          sendStop();
          return true;
        } else {
          sendStop();
          return false;
        }
      } else {
        sendStop();
        return false;
      }
    } else {
      sendStop();
      return false;
    }
  }
}  // namespace sccb
