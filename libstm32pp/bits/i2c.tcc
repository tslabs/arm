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

#include "bitband.hpp"
#include "../include/core/nvic.hpp"

namespace i2c {
  /**
   * @brief Configures the I2C in standard mode.
   * @brief Overrides the old configuration.
   */
  template<Address I>
  void Standard<I>::configure(
      cr1::smbus::States SMBUS,
      cr1::smbtype::States SMBTYPE,
      cr1::enarp::States ENARP,
      cr1::enpec::States ENPEC,
      cr1::engc::States ENGC,
      cr1::nostretch::States NOSTRETCH,
      cr2::iterren::States ITERREN,
      cr2::itevten::States ITVEN,
      cr2::itbufen::States ITBUFEN,
      cr2::dmaen::States DMAEN)
  {
    static_assert((FREQUENCY >= 2000000) || (FREQUENCY <= 42000000),
        "I2C Frequency must be between 2 MHz and 42 MHz (inclusive)");

    reinterpret_cast<Registers*>(I)->CR1 =
        SMBUS + SMBTYPE + ENARP + ENPEC + ENGC + NOSTRETCH;

    reinterpret_cast<Registers*>(I)->CR2 =
        ITERREN + ITVEN + ITBUFEN + DMAEN +
            ((FREQUENCY / 1000000) << cr2::freq::POSITION);
  }

  /**
   * @brief Configures the I2C clock.
   * @note  In standard mode, CCR >= 4 and in fast mode, CCR  >= 1
   *                           APB1
   * FREQ = -----------------------------------------
   *        CCR *(NORMAL:2, FAST:2 + 1, FAST: 16 + 9)
   */
  template<Address I>
  template<
      ccr::f_s::States F_S,
      ccr::duty::States DUTY,
      u32 FREQUENCY_HZ
  >
  void Standard<I>::configureClock()
  {
    enum {
      CCR = FREQUENCY / (FREQUENCY_HZ * (F_S == ccr::f_s::STANDARD_MODE ? 2 : (DUTY == ccr::duty::T_LOW_16_T_HIGH_9 ? 25 : 3)))
    };

    static_assert(CCR < 2048,
        "This frequency can't be achieved with this configuration.");
    static_assert((CCR >= 1) ||
        (F_S == ccr::f_s::STANDARD_MODE),
        "This frequency can't be achieved with this configuration.");
    static_assert((CCR >= 4) ||
        (F_S == ccr::f_s::FAST_MODE),
        "This frequency can't be achieved with this configuration.");

    reinterpret_cast<Registers*>(I)->CCR = F_S + DUTY + CCR;
  }

  /**
   * @brief Sets own address register 1 in 7 bit address mode.
   */
  template<Address I>
  void Standard<I>::configureOwnAddress7(u8 addr)
  {
    reinterpret_cast<Registers*>(I)->OAR1 =
      (addr << oar1::add7::POSITION) |
      oar1::addmode::MODE_7_BIT_SLAVE_ADDRESS;
    reinterpret_cast<Registers*>(I)->OAR2 =
      oar2::endual::DUAL_ADDRESSING_MODE_DISABLE;
  }

  /**
   * @brief Sets own address registers 1 and 2 in 7 bit address mode.
   */
  template<Address I>
  void Standard<I>::configureOwnAddress7Dual(u8 addr1, u8 addr2)
  {
    reinterpret_cast<Registers*>(I)->OAR1 =
      (addr1 << oar1::add7::POSITION) |
      oar1::addmode::MODE_7_BIT_SLAVE_ADDRESS;
    reinterpret_cast<Registers*>(I)->OAR2 =
      (addr2 << oar2::add7::POSITION) |
      oar2::endual::DUAL_ADDRESSING_MODE_ENABLE;
  }

  /**
   * @brief Sets own address register 1 in 10 bit address mode.
   */
  template<Address I>
  void Standard<I>::configureOwnAddress10(u16 addr)
  {
    reinterpret_cast<Registers*>(I)->OAR1 =
      (addr << oar1::add10::POSITION) |
      oar1::addmode::MODE_10_BIT_SLAVE_ADDRESS;
    reinterpret_cast<Registers*>(I)->OAR2 =
      oar2::endual::DUAL_ADDRESSING_MODE_DISABLE;
  }

  /**
   * @brief Enables the I2C's clock.
   * @note  Registers can't be written when the clock is disabled.
   */
  template<Address I>
  void Standard<I>::enableClock()
  {
    switch (I) {
      case I2C1:
        RCC::enableClocks<rcc::apb1enr::I2C1>();
        break;
      case I2C2:
        RCC::enableClocks<rcc::apb1enr::I2C2>();
        break;
#ifndef STM32F1XX
      case I2C3:
        RCC::enableClocks<rcc::apb1enr::I2C3>();
        break;
#endif // !STM32F1XX
    }
  }

  /**
   * @brief Disables the I2C's clock.
   * @note  Registers can't be written when the clock is disabled.
   */
  template<Address I>
  void Standard<I>::disableClock()
  {
    switch (I) {
      case I2C1:
        RCC::disableClocks<rcc::apb1enr::I2C1>();
        break;
      case I2C2:
        RCC::disableClocks<rcc::apb1enr::I2C2>();
        break;
#ifndef STM32F1XX
      case I2C3:
        RCC::disableClocks<rcc::apb1enr::I2C3>();
        break;
#endif // !STM32F1XX
    }
  }

  /**
   * @brief Turns on the I2C peripheral.
   */
  template<Address I>
  void Standard<I>::enablePeripheral()
  {
    *(u32 volatile*) (bitband::peripheral<
        I + cr1::OFFSET,
        cr1::pe::POSITION
    >()) = 1;
  }

  /**
   * @brief Turns off the I2C peripheral.
   */
  template<Address I>
  void Standard<I>::disablePeripheral()
  {
    *(u32 volatile*) (bitband::peripheral<
        I + cr1::OFFSET,
        cr1::pe::POSITION
    >()) = 0;
  }

  /**
   * @brief Sends Start condition.
   */
  template<Address I>
  void Standard<I>::sendStart()
  {
    *(u32 volatile*) (bitband::peripheral<
        I + cr1::OFFSET,
        cr1::start::POSITION
    >()) = 1;
  }

  /**
   * @brief Sends Stop condition.
   */
  template<Address I>
  void Standard<I>::sendStop()
  {
    *(u32 volatile*) (bitband::peripheral<
        I + cr1::OFFSET,
        cr1::stop::POSITION
    >()) = 1;
  }

  /**
   * @brief Resets I2C controller.
   */
  template<Address I>
  void Standard<I>::reset()
  {
    *(u32 volatile*) (bitband::peripheral<
        I + cr1::OFFSET,
        cr1::swrst::POSITION
    >()) = 1;
    *(u32 volatile*) (bitband::peripheral<
        I + cr1::OFFSET,
        cr1::swrst::POSITION
    >()) = 0;
  }
  /**
   * @brief Unmasks Event I2C interrupts.
   */
  template<Address I>
  void Standard<I>::unmaskEventInterrupts()
  {
    switch (I) {
      case I2C1:
        NVIC::enableIrq<
          nvic::irqn::I2C1_EV
        >();
      break;

      case I2C2:
        NVIC::enableIrq<
          nvic::irqn::I2C2_EV
        >();
      break;

      case I2C3:
        NVIC::enableIrq<
          nvic::irqn::I2C3_EV
        >();
      break;
    }
  }

  /**
   * @brief Unmasks Error I2C interrupts.
   */
  template<Address I>
  void Standard<I>::unmaskErrorInterrupts()
  {
    switch (I) {
      case I2C1:
        NVIC::enableIrq<
          nvic::irqn::I2C1_ER
        >();
      break;

      case I2C2:
        NVIC::enableIrq<
          nvic::irqn::I2C2_ER
        >();
      break;

      case I2C3:
        NVIC::enableIrq<
          nvic::irqn::I2C3_ER
        >();
      break;
    }
  }

  /**
   * @brief Clear Pending Event I2C interrupts.
   */
  template<Address I>
  void Standard<I>::clearPendingEventInterrupts()
  {
    switch (I) {
      case I2C1:
        NVIC::clearPendingIrq<
          nvic::irqn::I2C1_EV
        >();
      break;

      case I2C2:
        NVIC::clearPendingIrq<
          nvic::irqn::I2C2_EV
        >();
      break;

      case I2C3:
        NVIC::clearPendingIrq<
          nvic::irqn::I2C3_EV
        >();
      break;
    }
  }

  /**
   * @brief Clear Pending Error I2C interrupts.
   */
  template<Address I>
  void Standard<I>::clearPendingErrorInterrupts()
  {
    switch (I) {
      case I2C1:
        NVIC::clearPendingIrq<
          nvic::irqn::I2C1_ER
        >();
      break;

      case I2C2:
        NVIC::clearPendingIrq<
          nvic::irqn::I2C2_ER
        >();
      break;

      case I2C3:
        NVIC::clearPendingIrq<
          nvic::irqn::I2C3_ER
        >();
      break;
    }
  }

  /**
   * @brief Sends data.
   */
  template<Address I>
  void Standard<I>::sendData(u8 const data)
  {
    reinterpret_cast<Registers*>(I)->DR = data;
  }

  /**
   * @brief Reads data.
   */
  template<Address I>
  u8 Standard<I>::getData()
  {
    return reinterpret_cast<Registers*>(I)->DR;
  }

  /**
   * @brief Reads SR1 and SR2.
   */
  template<Address I>
  u32 Standard<I>::getLastEvent()
  {
    u16 sr1 = reinterpret_cast<Registers*>(I)->SR1;
    u16 sr2 = reinterpret_cast<Registers*>(I)->SR2;
    return (sr2 << 16) | sr1;
  }

  /**
   * @brief Sends the slave's address.
   */
  template<Address I>
  void Standard<I>::sendAddress(
      u8 const add,
      operation::E op)
  {
    reinterpret_cast<Registers*>(I)->DR = (add << 1) + op;
  }

  /**
   * @brief Send acknowledge after byte reception.
   */
  template<Address I>
  void Standard<I>::enableACK()
  {
    *(u32 volatile*) (bitband::peripheral<
        I + cr1::OFFSET,
        cr1::ack::POSITION
    >()) = 1;
  }

  /**
   * @brief Don't send acknowledge after byte reception.
   */
  template<Address I>
  void Standard<I>::disableACK()
  {
    *(u32 volatile*) (bitband::peripheral<
        I + cr1::OFFSET,
        cr1::ack::POSITION
    >()) = 0;
  }

  /**
   * @brief Clears AF.
   */
  template<Address I>
  void Standard<I>::clearAF()
  {
    *(u32 volatile*) (bitband::peripheral<
        I + sr1::OFFSET,
        sr1::af::POSITION
    >()) = 0;
  }

  /**
   * @brief returns true if a start condition has been sent.
   */
  template<Address I>
  bool Standard<I>::hasSentStart()
  {
    return *(bool volatile*) bitband::peripheral<
        I + sr1::OFFSET,
        sr1::sb::POSITION
    >();
  }

  /**
   * @brief returns true when a stop condition has been sent.
   */
  template<Address I>
  bool Standard<I>::hasSentStop()
  {
    return *(bool volatile*) bitband::peripheral<
        I + sr1::OFFSET,
        sr1::stopf::POSITION
    >();
  }

  /**
   * @brief returns true when the slave address has been transmitted.
   */
  template<Address I>
  bool Standard<I>::hasAddressTransmitted()
  {
    return *(bool volatile*) bitband::peripheral<
        I + sr1::OFFSET,
        sr1::addr::POSITION
    >();
  }

  /**
   * @brief returns true if data has been received.
   */
  template<Address I>
  bool Standard<I>::hasReceivedData()
  {
    return *(bool volatile*) bitband::peripheral<
        I + sr1::OFFSET,
        sr1::rxne::POSITION
    >();
  }

  /**
   * @brief 0 - Can't send data, 1 - Can send data.
   */
  template<Address I>
  bool Standard<I>::canSendData()
  {
    return *(bool volatile*) bitband::peripheral<
        I + sr1::OFFSET,
        sr1::txe::POSITION
    >();
  }

  /**
   * @brief 0 - Transfer hasn't finished, 1 - Transfer has finished.
   */
  template<Address I>
  bool Standard<I>::hasTranferFinished()
  {
    return *(bool volatile*) bitband::peripheral<
        I + sr1::OFFSET,
        sr1::btf::POSITION
    >();
  }

  /**
   * @brief  0 - No communication on the bus,
   *          1 - Communication ongoing on the bus.
   */
  template<Address I>
  bool Standard<I>::isTheBusBusy()
  {
    return *(bool volatile*) (bitband::peripheral<
        I + sr2::OFFSET,
        sr2::busy::POSITION
    >());
  }

  /**
   * @brief Writes a value to a slave device register.
   */
  template<Address I>
  void Standard<I>::writeSlaveRegister(
      u8 const slaveAddress,
      u8 const registerAddress,
      u8 const value)
  {
    sendStart();
    while (!hasSentStart());
    Standard<I>::sendAddress(slaveAddress, operation::WRITE);
    while (!hasAddressTransmitted());
    reinterpret_cast<Registers*>(I)->SR2;     // a dummy read: EV1: ADDR=1, cleared by reading SR1 followed by reading SR2
    sendData(registerAddress);
    while (!canSendData());
    sendData(value);
    while (!hasTranferFinished());
    sendStop();
    while (isTheBusBusy());
  }

  /**
   * @brief Reads a value from a slave device register.
   */
  template<Address I>
  u8 Standard<I>::readSlaveRegister(
      u8 const slaveAddress,
      u8 const registerAddress)
  {
    sendStart();
    while (!hasSentStart());
    sendAddress(slaveAddress, operation::WRITE);
    while (!hasAddressTransmitted());
    reinterpret_cast<Registers*>(I)->SR2;
    sendData(registerAddress);
    while (!canSendData());
    sendStart();
    while (!hasSentStart());
    sendAddress(slaveAddress, operation::READ);
    disableACK();
    while (!hasAddressTransmitted());
    reinterpret_cast<Registers*>(I)->SR2;
    while (!hasReceivedData());
    sendStop();
    while (isTheBusBusy());
    return getData();
  }
}  // namespace i2c
