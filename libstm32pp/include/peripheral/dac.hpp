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
 *                      Digital to Analog Converter
 *
 ******************************************************************************/

#pragma once

#include <device_select.hpp>

#include "../defs.hpp"
#include "../../memorymap/dac.hpp"

// Low-level access to the registers
#define DAC_REGS reinterpret_cast<dac::Registers *>(dac::ADDRESS)

// High-level functions
namespace dac {
  class Functions {
    public:
      static inline void enableClock();
      static inline void disableClock();
      static inline void enablePeripheral();
      static inline void disablePeripheral();
      static inline void configureBasic(
          dac::cr::dac1::enable::States,
          dac::cr::dac1::boff::States,
          dac::cr::dac2::enable::States,
          dac::cr::dac2::boff::States);

    private:
      Functions();
  };
}  // namespace dac

// High-level access to the peripheral
typedef dac::Functions DAC;

#include "../../bits/dac.tcc"
