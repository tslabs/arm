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

/*******************************************************************************
 *
 *                         Reset and Clock Control
 *
 ******************************************************************************/

#pragma once

#include <device_select.hpp>
#include "../defs.hpp"

#include "../../memorymap/rcc.hpp"

// Low-level access to the registers
#define RCC_REGS  reinterpret_cast<rcc::Registers *>(rcc::ADDRESS)

#include "cfunctions.hpp"

// High-level functions
namespace rcc {
  class Functions {
    public:
      static inline void enableHse();
      static inline void disableHse();
      static inline bool isHseStable();
      static inline void useHseOscillator();
      static inline void bypassHseOscillator();

      static inline void enableLse();
      static inline void disableLse();
      static inline bool isLseStable();
      static inline void useLseOscillator();
      static inline void bypassLseOscillator();

      static inline void enableHsi();
      static inline void disableHsi();
      static inline bool isHsiStable();

      static inline void enableLsi();
      static inline void disableLsi();
      static inline bool isLsiStable();

      static inline void enableRtc();
      static inline void disableRtc();

      static inline void enablePll();
      static inline void disablePll();
      static inline bool isPllStable();

#ifdef CONNECTIVITY_LINE
      static inline void enablePll2();
      static inline void disablePll2();
      static inline bool isPll2Stable();
      static inline void enablePll3();
      static inline void disablePll3();
      static inline bool isPll3Stable();
#endif // CONNECTIVITY_LINE
      static inline bool isSystemClockSourceStable();

      static inline void setSystemClockSource(rcc::cfgr::sw::States);
      static inline void setRtcClockSource(rcc::bdcr::rtcsel::States);

      template<
          rcc::apb1enr::Bits...
      >
      static inline void enableClocks();

      template<
      rcc::apb1enr::Bits ...
      >
      static inline void disableClocks();

      template<
      rcc::apb1rstr::Bits...
      >
      static inline void resetPeripherals();

      template<
      rcc::apb2enr::Bits ...
      >
      static inline void enableClocks();

      template<
      rcc::apb2enr::Bits ...
      >
      static inline void disableClocks();

      template<
      rcc::apb2rstr::Bits ...
      >
      static inline void resetPeripherals();

#ifdef STM32F1XX
      template<rcc::ahbenr::Bits ...> static inline void enableClocks();
      template<rcc::ahbenr::Bits ...> static inline void disableClocks();
      static inline void configureClockOutput(rcc::cfgr::mco::States);

#ifdef CONNECTIVITY_LINE
      template<
      rcc::ahbrstr::Bits ...
      >
      static inline void resetPeripherals();
#endif

#ifdef VALUE_LINE
      template<
      u8 HPRE,
      u8 PPRE1,
      u8 PPRE2,
      u8 ADCPRE
      >
      static inline void configureBusPrescalers();
#else // VALUE_LINE
      template<
      u8 HPRE,
      u8 PPRE1,
      u8 PPRE2,
      u8 ADCPRE,
      u8 USBPRE
      >
      static inline void configureBusPrescalers();
#endif // VALUE_LINE

#ifdef VALUE_LINE
      template<
      rcc::cfgr::pllsrc::States,
      u8 PLLMUL,
      u8 PREDIV1
      >
      static inline void configurePll();

#else // VALUE_LINE
#ifdef CONNECTIVITY_LINE
      template<
      rcc::cfgr::pllsrc::States,
      u8 PLLMUL,
      u8 PREDIV1,
      u8 PREDIV2,
      u8 PLL2MUL,
      u8 PLL3MUL,
      rcc::cfgr2::prediv1src::States,
      rcc::cfgr2::i2s2src::States,
      rcc::cfgr2::i2s3src::States
      >
      static inline void configurePll();

#else // CONNECTIVITY_LINE
      template <
      rcc::cfgr::sws::States,
      rcc::cfgr::pllsrc::States,
      rcc::cfgr::pllxtpre::States,
      rcc::cfgr::pllmul::States,
      rcc::cfgr::hpre::States,
      rcc::cfgr::ppre1::States,
      rcc::cfgr::ppre2::States,
      rcc::cfgr::usbpre::States,
      rcc::cfgr::adcpre::States
      >
      static inline void configurePll();

#endif // CONNECTIVITY_LINE
#endif // VALUE_LINE
#else // STM32F1XX
      template<
      rcc::ahb1enr::Bits ...
      >
      static inline void enableClocks();

      template<
      rcc::ahb1enr::Bits ...
      >
      static inline void disableClocks();

      template<
      rcc::ahb1rstr::Bits ...
      >
      static inline void resetPeripherals();

      template<
      rcc::ahb2enr::Bits ...
      >
      static inline void enableClocks();

      template<
      rcc::ahb2enr::Bits ...
      >
      static inline void disableClocks();

      template<
      rcc::ahb2rstr::Bits ...
      >
      static inline void resetPeripherals();

      template<
      rcc::ahb3enr::Bits ...
      >
      static inline void enableClocks();

      template<
      rcc::ahb3enr::Bits ...
      >
      static inline void disableClocks();

      template<
      rcc::ahb3rstr::Bits ...
      >
      static inline void resetPeripherals();

      template<
      rcc::pllcfgr::pllsrc::States,
      u8 PLLM,
      u16 PLLN,
      u8 PLLP,
      u8 PLLQ
      >
      static inline void configurePll();

      template<
      u16 PLLI2SN,
      u8 PLLI2SR
      >
      static inline void configureI2sPll();

      template<
      u8 HPRE,
      u8 PPRE1,
      u8 PPRE2,
      u8 RTCPRE
      >
      static inline void configurePrescalers();

      template<
      rcc::cfgr::mco1::States,
      rcc::cfgr::mco2::States,
      u8 MCOPRE1,
      u8 MCOPRE2
      >
      static inline void configureClockOutput();

      static inline void selectI2sSource(rcc::cfgr::i2ssrc::States);

#endif // STM32F1XX
      private:
      Functions();
    };
  }  // namespace rcc

#include "../../bits/rcc.tcc"

// High-level access to the peripheral
  typedef rcc::Functions RCC;
