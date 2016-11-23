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

namespace afio
{
  enum
  {
    ADDRESS = alias::APB2 + 0x0000
  };

  struct Registers
  {
      __RW u32 EVCR;       // 0x00: Event control
      __RW u32 MAPR;       // 0x04: Remap and debug configuration
      __RW u32 EXTICR[4];  // 0x08: External interrupt configuration
           u32 _RESERVED0;
      __RW u32 MAPR2;      // 0x1C: Remap and debug configuration 2
  };

#ifdef STM32F1XX
  namespace mapr
  {
    namespace spi1
    {
      enum
      {
        MASK = 0b1,
        POSITION = 0
      };

      enum States
      {
        // +++
      };
    }

    namespace i2c1
    {
      enum
      {
        MASK = 0b1,
        POSITION = 1
      };

      enum States
      {
        // +++
      };
    }

    namespace usart1
    {
      enum
      {
        MASK = 0b1,
        POSITION = 2
      };

      enum States
      {
        // +++
      };
    }

    namespace usart2
    {
      enum
      {
        MASK = 0b1,
        POSITION = 3
      };

      enum States
      {
        // +++
      };
    }

    namespace usart3
    {
      enum
      {
        MASK = 0b11,
        POSITION = 4
      };

      enum States
      {
        // +++
      };
    }

    namespace tim1
    {
      enum
      {
        MASK = 0b11,
        POSITION = 6
      };

      enum States
      {
        // +++
      };
    }

    namespace tim2
    {
      enum
      {
        MASK = 0b11,
        POSITION = 8
      };

      enum States
      {
        // +++
      };
    }

    namespace tim3
    {
      enum
      {
        MASK = 0b11,
        POSITION = 10
      };

      enum States
      {
        NO_REMAP      = 0 << POSITION,
        PARTIAL_REMAP = 2 << POSITION,
        FULL_REMAP    = 3 << POSITION
      };
    }

    namespace tim4
    {
      enum
      {
        MASK = 0b1,
        POSITION = 12
      };

      enum States
      {
        // +++
      };
    }

    namespace can1
    {
      enum
      {
        MASK = 0b11,
        POSITION = 13
      };

      enum States
      {
        // +++
      };
    }

    namespace pd01
    {
      enum
      {
        MASK = 0b1,
        POSITION = 15
      };

      enum States
      {
        // +++
      };
    }

    namespace tim5ch4
    {
      enum
      {
        MASK = 0b1,
        POSITION = 16
      };

      enum States
      {
        // +++
      };
    }

// +++ trim CONNECTIVITY LINE
    namespace adc1etrginj
    {
      enum
      {
        MASK = 0b1,
        POSITION = 17
      };

      enum States
      {
        // +++
      };
    }

    namespace adc1etrgreg
    {
      enum
      {
        MASK = 0b1,
        POSITION = 18
      };

      enum States
      {
        // +++
      };
    }

    namespace adc2etrginj
    {
      enum
      {
        MASK = 0b1,
        POSITION = 19
      };

      enum States
      {
        // +++
      };
    }

    namespace adc2etrgreg
    {
      enum
      {
        MASK = 0b1,
        POSITION = 20
      };

      enum States
      {
        // +++
      };
    }
// end trim CONNECTIVITY LINE

// +++ trim !(CONNECTIVITY LINE)
    namespace eth
    {
      enum
      {
        MASK = 0b1,
        POSITION = 21
      };

      enum States
      {
        // +++
      };
    }

    namespace can2
    {
      enum
      {
        MASK = 0b1,
        POSITION = 22
      };

      enum States
      {
        // +++
      };
    }

    namespace miirmii
    {
      enum
      {
        MASK = 0b1,
        POSITION = 23
      };

      enum States
      {
        // +++
      };
    }
// end trim !(CONNECTIVITY LINE)

    namespace swjcfg
    {
      enum
      {
        MASK = 0b111,
        POSITION = 24
      };

      enum States
      {
        JTAG_ENABLED_SW_ENABLED           = 0 << POSITION,
        JTAG_ENABLED_SW_ENABLED_NO_NJTRST = 1 << POSITION,
        JTAG_DISABLED_SW_ENABLED          = 2 << POSITION,
        JTAG_DISABLED_SW_DISABLED         = 3 << POSITION
      };
    }

// +++ trim !(CONNECTIVITY LINE)
    namespace spi3
    {
      enum
      {
        MASK = 0b1,
        POSITION = 28
      };

      enum States
      {
        // +++
      };
    }

    namespace tim2itr1
    {
      enum
      {
        MASK = 0b1,
        POSITION = 29
      };

      enum States
      {
        // +++
      };
    }

    namespace ptppps
    {
      enum
      {
        MASK = 0b1,
        POSITION = 30
      };

      enum States
      {
        // +++
      };
    }
// end trim !(CONNECTIVITY LINE)

    enum Bits
    {
      TIM3_NO_REMAP      = tim3::NO_REMAP      << tim3::POSITION,
      TIM3_PARTIAL_REMAP = tim3::PARTIAL_REMAP << tim3::POSITION,
      TIM3_FULL_REMAP    = tim3::FULL_REMAP    << tim3::POSITION
    };
  }  // namespace mapr
#endif // #ifdef STM32F1XX

  namespace exticr
  {
    enum
    {
      MASK = 0b1111,
      POSITION = 4
    };

    enum States
    {
      PA = 0,
      PB = 1,
      PC = 2,
      PD = 3,
      PE = 4,
      PF = 5,
      PG = 6
    };
  }  // namespace exticr
}// namespace afio
