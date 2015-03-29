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

#include "../include/peripheral/rcc.hpp"

namespace dac {
  void Functions::enableClock()
  {
    RCC::enableClocks<rcc::apb1enr::DAC>();
  }

  void Functions::disableClock()
  {
    RCC::disableClocks<rcc::apb1enr::DAC>();
  }

  void Functions::enablePeripheral()
  {
    DAC_REGS->CR |= cr::dac1::enable::ENABLED | cr::dac2::enable::ENABLED;
  }
  
  void Functions::disablePeripheral()
  {
    DAC_REGS->CR |= cr::dac1::enable::DISABLED | cr::dac2::enable::DISABLED;
  }
} // namespace dac
