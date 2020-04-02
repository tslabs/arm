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

#include "common.hpp"

namespace dac
{
  enum
  {
    ADDRESS = alias::APB1 + 0x7400
  };

  struct Registers
  {
    __RW u32 CR;       // 0x00: Control
    __RW u32 SWTRIGR;  // 0x04: Software trigger
    struct
    {
      __RW u32 R12;  // 0x08, 0x14: 12-bit right-aligned
      __RW u32 L12;  // 0x0C, 0x18: 12-bit left-aligned
      __RW u32 R8;   // 0x10, 0x1C: 8-bit right-aligned
    } DHR[2];     // Channel data holding
    __RW u32 DHR12RD;  // 0x20: Dual 12-bit right-aligned data holding
    __RW u32 DHR12LD;  // 0x24: Dual 12-bit left-aligned data holding
    __RW u32 DHR8RD;   // 0x28: Dual 8-bit right-aligned data holding
    __RW u32 DOR[2];   // 0x2C: Channel data output

#if defined VALUE_LINE || \
    defined STM32F2XX || \
    defined STM32F4XX
    __RW u32 SR;      // 0x34: Status
#endif
  };

  namespace cr
  {
    enum
    {
      OFFSET = 0x00
    };

    namespace dac1
    {
      namespace enable
      {
        enum
        {
          POSITION = 0,
          MASK = 0b1 << POSITION
        };

        enum States
        {
          DISABLED = 0 << POSITION,
          ENABLED = 1 << POSITION
        };
      } // namespace enable

      namespace boff
      {
        enum
        {
          POSITION = 1,
          MASK = 0b1 << POSITION
        };

        enum States
        {
          ENABLED = 0 << POSITION,
          DISABLED = 1 << POSITION
        };
      } // namespace boff

      namespace ten
      {
        enum
        {
          POSITION = 2,
          MASK = 0b1 << POSITION
        };

        enum States
        {
          DISABLED = 0 << POSITION,
          ENABLED = 1 << POSITION
        };
      } // namespace ten

      namespace tsel
      {
        enum
        {
          POSITION = 3,
          MASK = 0b111 << POSITION
        };

        enum States
        {
          TIM6TRGO = 0 << POSITION,
          TIM8TRGO = 1 << POSITION,
          TIM7TRGO = 2 << POSITION,
          TIM5TRGO = 3 << POSITION,
          TIM2TRGO = 4 << POSITION,
          TIM4TRGO = 5 << POSITION,
          EXTI9 = 6 << POSITION,
          SWTRG = 7 << POSITION
        };
      } // namespace tsel

      namespace wave
      {
        enum
        {
          POSITION = 6,
          MASK = 0b11 << POSITION
        };

        enum States
        {
          DISABLED = 0 << POSITION,
          NOISE = 1 << POSITION,
          TRIANGLE = 2 << POSITION
        };
      } // namespace wave

      namespace mamp
      {
        enum
        {
          POSITION = 8,
          MASK = 0b1111 << POSITION
        };

        enum States
        {
          UMSK0 = 0 << POSITION,
          UMSK1_0 = 1 << POSITION,
          UMSK2_0 = 2 << POSITION,
          UMSK3_0 = 3 << POSITION,
          UMSK4_0 = 4 << POSITION,
          UMSK5_0 = 5 << POSITION,
          UMSK6_0 = 6 << POSITION,
          UMSK7_0 = 7 << POSITION,
          UMSK8_0 = 8 << POSITION,
          UMSK9_0 = 9 << POSITION,
          UMSK10_0 = 10 << POSITION,
          UMSK11_0 = 11 << POSITION
        };
      } // namespace mamp

      namespace dma
      {
        enum
        {
          POSITION = 12,
          MASK = 0b1 << POSITION
        };

        enum States
        {
          DISABLED = 0 << POSITION,
          ENABLED = 1 << POSITION
        };
      } // namespace dma

      namespace dmaudrie
      {
        enum
        {
          POSITION = 13,
          MASK = 0b1 << POSITION
        };

        enum States
        {
          DISABLED = 0 << POSITION,
          ENABLED = 1 << POSITION
        };
      } // namespace dmaudrie
    } // namespace dac1

    namespace dac2
    {
      namespace enable
      {
        enum
        {
          POSITION = 16,
          MASK = 0b1 << POSITION
        };

        enum States
        {
          DISABLED = 0 << POSITION,
          ENABLED = 1 << POSITION
        };
      } // namespace enable

      namespace boff
      {
        enum
        {
          POSITION = 17,
          MASK = 0b1 << POSITION
        };

        enum States
        {
          ENABLED = 0 << POSITION,
          DISABLED = 1 << POSITION
        };
      } // namespace boff

      namespace ten
      {
        enum
        {
          POSITION = 18,
          MASK = 0b1 << POSITION
        };

        enum States
        {
          DISABLED = 0 << POSITION,
          ENABLED = 1 << POSITION
        };
      } // namespace ten

      namespace tsel
      {
        enum
        {
          POSITION = 19,
          MASK = 0b111 << POSITION
        };

        enum States
        {
          TIM6TRGO = 0 << POSITION,
          TIM8TRGO = 1 << POSITION,
          TIM7TRGO = 2 << POSITION,
          TIM5TRGO = 3 << POSITION,
          TIM2TRGO = 4 << POSITION,
          TIM4TRGO = 5 << POSITION,
          EXTI9 = 6 << POSITION,
          SWTRG = 7 << POSITION
        };
      } // namespace tsel

      namespace wave
      {
        enum
        {
          POSITION = 22,
          MASK = 0b11 << POSITION
        };

        enum States
        {
          DISABLED = 0 << POSITION,
          NOISE = 1 << POSITION,
          TRIANGLE = 2 << POSITION
        };
      } // namespace wave

      namespace mamp
      {
        enum
        {
          POSITION = 24,
          MASK = 0b1111 << POSITION
        };

        enum States
        {
          UMSK0 = 0 << POSITION,
          UMSK1_0 = 1 << POSITION,
          UMSK2_0 = 2 << POSITION,
          UMSK3_0 = 3 << POSITION,
          UMSK4_0 = 4 << POSITION,
          UMSK5_0 = 5 << POSITION,
          UMSK6_0 = 6 << POSITION,
          UMSK7_0 = 7 << POSITION,
          UMSK8_0 = 8 << POSITION,
          UMSK9_0 = 9 << POSITION,
          UMSK10_0 = 10 << POSITION,
          UMSK11_0 = 11 << POSITION
        };
      } // namespace mamp

      namespace dma
      {
        enum
        {
          POSITION = 28,
          MASK = 0b1 << POSITION
        };

        enum States
        {
          DISABLED = 0 << POSITION,
          ENABLED = 1 << POSITION
        };
      } // namespace dma

      namespace dmaudrie
      {
        enum
        {
          POSITION = 29,
          MASK = 0b1 << POSITION
        };

        enum States
        {
          DISABLED = 0 << POSITION,
          ENABLED = 1 << POSITION
        };
      } // namespace dmaudrie
    } // namespace dac2
  }  // namespace cr

  namespace swtrigr
  {
    enum
    {
      OFFSET = 0x04
    };
  }  // namespace swtrigr

  namespace sr
  {
    enum
    {
      OFFSET = 0x34
    };
  }  // namespace sr
}  // namespace dac
