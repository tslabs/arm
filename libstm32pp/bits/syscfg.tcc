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

#include "../include/peripheral/syscfg.hpp"

namespace syscfg {
  /**
   * @brief Enables the System Configuration Controller clock.
   * @note  Registers can't be written when the clock is disabled.
   */
  void Functions::enableClock()
  {
    RCC::enableClocks<
        rcc::apb2enr::SYSCFG
    >();
  }

  /**
   * @brief Disables the System Configuration Controller clock.
   * @note  Registers can't be written when the clock is disabled.
   */
  void Functions::disableClock()
  {
    RCC::disableClocks<
        rcc::apb2enr::SYSCFG
    >();
  }

  /**
   * @brief Maps the selected pin to select EXTI line.
   */
  template<
      u8 LINE,
      exticr::States EXTICR
  >
  void Functions::selectExtiPin()
  {
    static_assert(LINE < 16, "Only the first 16 EXTI lines can be mapped to GPIO pins");

    u32 s = SYSCFG_REGS->EXTICR[LINE >> 2];
    s &= ~(exticr::MASK << ((LINE & 3) << 2));
    s |= EXTICR << ((LINE & 3) << 2);
    SYSCFG_REGS->EXTICR[LINE >> 2] = s;
  }
}  // namespace syscfg

