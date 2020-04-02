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

namespace nvic
{
  /**
   * @brief Enables an interrupt request.
   */
  template<irqn::E I>
  void Functions::enableIrq(void)
  {
    NVIC_REGS->ISER[I >> 5] = 1 << (I % 32);
  }

  /**
   * @brief Disables an interrupt request.
   */
  template<irqn::E I>
  void Functions::disableIrq(void)
  {
    NVIC_REGS->ICER[I >> 5] = 1 << (I % 32);
  }

  /**
   * @brief Sets a pending interrupt request.
   */
  template<irqn::E I>
  void Functions::setPendingIrq(void)
  {
    NVIC_REGS->ISPR[I >> 5] = 1 << (I % 32);
  }

  /**
   * @brief Clears a pending interrupt request.
   */
  template<irqn::E I>
  void Functions::clearPendingIrq(void)
  {
    NVIC_REGS->ICPR[I >> 5] = 1 << (I % 32);
  }

  /**
   * @brief Sets the interrupt priority level.
   * @note  A lower priority number, means higher priority.
   */
  template<irqn::E I, u8 P>
  void Functions::setPriority()
  {
    static_assert(P < 16, "The priority must be 0-15.");

    u32 s = NVIC_REGS->IPR[I >> 2];
    s &= ~(irqn::MASK << (8 * (I % 4)));
    s |= P << ((8 * (I % 4)) + 4);
    NVIC_REGS->IPR[I >> 2] = s;
  }

  /**
   * @brief Triggers an interrupt request.
   */
  template<irqn::E I>
  void Functions::triggerIrq()
  {
    NVIC_REGS->STIR = I;
  }
}  // namespace nvic
