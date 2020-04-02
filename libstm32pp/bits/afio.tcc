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

namespace afio
{
  void Functions::enableClock()
  {
    RCC::enableClocks<rcc::apb2enr::AFIO>();
  }

  void Functions::disableClock()
  {
    RCC::disableClocks<rcc::apb2enr::AFIO>();
  }

  void Functions::configureSWJ(mapr::swjcfg::States SWJCFG)
  {
    u32 s = AFIO_REGS->MAPR;
    s &= ~(mapr::swjcfg::MASK << (mapr::swjcfg::POSITION));
    s |= SWJCFG;
    AFIO_REGS->MAPR = s;
  }

  template<mapr::Bits... BITS>
  void Functions::configureRemap()
  {
    u32 s = AFIO_REGS->MAPR;
    s &= ~cSum<BITS...>::value;
    // RCC_REGS->APB1RSTR |= 
    // RCC_REGS->APB1RSTR &= ~cSum<APB1RSTR...>::value;
    AFIO_REGS->MAPR = s;
  }

  template <exticr::States PORT, u8 PIN>
  void Functions::configureExti()
  {
    u32 s = AFIO_REGS->EXTICR[PIN / 4];
    s &= ~(exticr::MASK << (exticr::POSITION * (PIN % 4)));
    s |= PORT << (exticr::POSITION * (PIN % 4));
    AFIO_REGS->EXTICR[PIN / 4] = s;
  }
} // namespace afio
