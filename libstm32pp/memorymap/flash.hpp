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

namespace flash
{
  enum
  {
#ifdef STM32F1XX
  ADDRESS = alias::AHB + 0x2000
#else // STM32F1XX
  ADDRESS = alias::AHB1 + 0x3C00
#endif // STM32F1XX
  };

  struct Registers
  {
    __RW u32 ACR;     // 0x00: Access control
    __RW u32 KEYR;    // 0x04: Key
    __RW u32 OPTKEYR; // 0x08: Option key
    __RW u32 SR;      // 0x0C: Status
    __RW u32 CR;      // 0x10: Control
#ifdef STM32F1XX
    __RW u32 AR;      // 0x14: Address
    __RW u32 _RSVD1;  // 0x18: Reserved
    __RW u32 OBR;     // 0x1C: Option byte
    __RW u32 WRPR;    // 0x20: Write protection
#else // STM32F1XX
    __RW u32 OPTCR;   // 0x14: Option control
#endif // STM32F1XX
  };

  namespace acr
  {
    enum    {
      OFFSET = 0x00
    };

#ifdef STM32F1XX
#ifndef VALUE_LINE
    namespace latency
    {
      enum      {
        POSITION = 0,
        MASK = 0b111 << POSITION
      };

      enum States      {
        ZERO_WAIT_STATE = 0 << POSITION,
        ONE_WAIT_STATE = 1 << POSITION,
        TWO_WAIT_STATES = 2 << POSITION
      };
    }  // namespace latency
#endif
    namespace hlfcya
    {
      enum      {
        POSITION = 3,
        MASK = 1 << POSITION
      };

      enum States      {
        FLASH_HALF_CYCLE_ACCESS_DISABLED = 0 << POSITION,
        FLASH_HALF_CYCLE_ACCESS_ENABLED = 1 << POSITION
      };
    }  // namespace prftbe
#ifndef VALUE_LINE
    namespace prftbe
    {
      enum      {
        POSITION = 4,
        MASK = 1 << POSITION
      };

      enum States      {
        PREFETCH_DISABLED = 0 << POSITION,
        PREFETCH_ENABLED = 1 << POSITION
      };
    }  // namespace prftbe

    namespace prftbs
    {
      enum      {
        POSITION = 5,
        MASK = 1 << POSITION
      };

      enum States      {
        PREFETCH_DISABLED = 0 << POSITION,
        PREFETCH_ENABLED = 1 << POSITION
      };
    }  // namespace prftbs
#endif
#else /* STM32F2XX || STM32F4XX */
    namespace latency
    {
      enum      {
        POSITION = 0,
        MASK = 0b111 << POSITION
      };

      enum States      {
        ZERO_WAIT_STATE = 0 << POSITION,
        ONE_WAIT_STATE = 1 << POSITION,
        TWO_WAIT_STATES = 2 << POSITION,
        THREE_WAIT_STATES = 3 << POSITION,
        FOUR_WAIT_STATES = 4 << POSITION,
        FIVE_WAIT_STATES = 5 << POSITION,
        SIX_WAIT_STATES = 6 << POSITION,
        SEVEN_WAIT_STATES = 7 << POSITION
      };
    }  // namespace latency

    namespace prften
    {
      enum      {
        POSITION = 8,
        MASK = 1 << POSITION
      };

      enum States      {
        PREFETCH_DISABLED = 0 << POSITION,
        PREFETCH_ENABLED = 1 << POSITION
      };
    }  // namespace prften

    namespace icen
    {
      enum      {
        POSITION = 9,
        MASK = 1 << POSITION
      };

      enum States      {
        INSTRUCTION_CACHE_DISABLED = 0 << POSITION,
        INSTRUCTION_CACHE_ENABLED = 1 << POSITION
      };
    }  // namespace icen

    namespace dcen
    {
      enum      {
        POSITION = 10,
        MASK = 1 << POSITION
      };

      enum States      {
        DATA_CACHE_DISABLED = 0 << POSITION,
        DATA_CACHE_ENABLED = 1 << POSITION
      };
    }  // namespace dcen

    namespace icrst
    {
      enum      {
        POSITION = 11,
        MASK = 1 << POSITION
      };

      enum States      {
        INSTRUCTION_CACHE_NOT_RESET = 0 << POSITION,
        INSTRUCTION_CACHE_RESET = 1 << POSITION
      };
    }  // namespace dcen

    namespace dcrst
    {
      enum      {
        POSITION = 12,
        MASK = 1 << POSITION
      };

      enum States      {
        DATA_CACHE_NOT_RESET = 0 << POSITION,
        DATA_CACHE_RESET = 1 << POSITION
      };
    }  // namespace dcen
#endif  // #ifdef STM32F1XX
  }  // namespace acr

  namespace keyr
  {
    enum
    {
      OFFSET = 0x04
    };

    enum
    {
#ifdef STM32F1XX
      RDPRT = 0x00A5,
#endif
      KEY1 = 0x45670123,
      KEY2 = 0xCDEF89AB
    };
  }

  namespace optkeyr
  {
    enum
    {
      OFFSET = 0x08
    };

    enum
    {
#ifdef STM32F1XX
      OPTKEY1 = 0x45670123,
      OPTKEY2 = 0xCDEF89AB
#else
      OPTKEY1 = 0x08192A3B,
      OPTKEY2 = 0x4C5D6E7F
#endif
    };
  }

  namespace sr
  {
    enum
    {
      OFFSET = 0x0C
    };

#ifdef STM32F1XX
    namespace bsy
    {
      enum
      {
        POSITION = 0,
        MASK = 1 << POSITION
      };

      enum States
      {
        NO_FLASH_MEMORY_OPERATION_ONGOING = 0 << POSITION,
        FLASH_MEMORY_OPERATION_ONGOING = 1 << POSITION
      };
    }

    namespace pgerr
    {
      enum
      {
        POSITION = 2,
        MASK = 1 << POSITION
      };

      enum States
      {
        NO_ERROR = 0 << POSITION,
        ERROR = 1 << POSITION
      };
    }

    namespace wrprterr
    {
      enum
      {
        POSITION = 4,
        MASK = 1 << POSITION
      };

      enum States
      {
        NO_ERROR = 0 << POSITION,
        ERROR = 1 << POSITION
      };
    }

    namespace eop
    {
      enum
      {
        POSITION = 5,
        MASK = 1 << POSITION
      };

      enum States
      {
        OPERATION_IN_PROGRESS = 0 << POSITION,
        END_OF_OPERATION = 1 << POSITION
      };
    }
#else  // #ifdef STM32F1XX
    namespace eop
    {
      enum
      {
        POSITION = 0,
        MASK = 1 << POSITION
      };

      enum States
      {
        OPERATION_IN_PROGRESS = 0 << POSITION,
        END_OF_OPERATION = 1 << POSITION
      };
    }

    namespace operr
    {
      enum
      {
        POSITION = 1,
        MASK = 1 << POSITION
      };

      enum States
      {
        NO_ERROR = 0 << POSITION,
        ERROR = 1 << POSITION
      };
    }

    namespace wrperr
    {
      enum
      {
        POSITION = 4,
        MASK = 1 << POSITION
      };

      enum States
      {
        NO_ERROR = 0 << POSITION,
        ERROR = 1 << POSITION
      };
    }

    namespace pgaerr
    {
      enum
      {
        POSITION = 5,
        MASK = 1 << POSITION
      };

      enum States
      {
        NO_ERROR = 0 << POSITION,
        ERROR = 1 << POSITION
      };
    }

    namespace pgperr
    {
      enum
      {
        POSITION = 6,
        MASK = 1 << POSITION
      };

      enum States
      {
        NO_ERROR = 0 << POSITION,
        ERROR = 1 << POSITION
      };
    }

    namespace pgserr
    {
      enum
      {
        POSITION = 7,
        MASK = 1 << POSITION
      };

      enum States
      {
        NO_ERROR = 0 << POSITION,
        ERROR = 1 << POSITION
      };
    }

    namespace bsy
    {
      enum
      {
        POSITION = 16,
        MASK = 1 << POSITION
      };

      enum States
      {
        NO_FLASH_MEMORY_OPERATION_ONGOING = 0 << POSITION,
        FLASH_MEMORY_OPERATION_ONGOING = 1 << POSITION
      };
    }
#endif  // #ifdef STM32F1XX
  }

  namespace cr
  {
    enum
    {
      OFFSET = 0x10
    };

    namespace pg
    {
      enum
      {
        POSITION = 0,
        MASK = 1 << POSITION
      };

      enum States
      {
        FLASH_PROGRAMMING_ACTIVATED = 1 << POSITION
      };
    }

#ifdef STM32F1XX
    namespace per
    {
      enum
      {
        POSITION = 1,
        MASK = 1 << POSITION
      };

      enum States
      {
        PAGE_ERASE_ACTIVATED = 1 << POSITION
      };
    }
#else
    namespace ser
    {
      enum
      {
        POSITION = 1,
        MASK = 1 << POSITION
      };

      enum States
      {
        SECTOR_ERASE_ACTIVATED = 1 << POSITION
      };
    }
#endif

    namespace mer
    {
      enum
      {
        POSITION = 2,
        MASK = 1 << POSITION
      };

      enum States
      {
        MASS_ERASE_ACTIVATED = 1 << POSITION
      };
    }

#ifdef STM32F1XX
    namespace optpg
    {
      enum
      {
        POSITION = 4,
        MASK = 1 << POSITION
      };

      enum States
      {
        OPTION_BYTE_PROGRAMMING_ACTIVATED = 1 << POSITION
      };
    }

    namespace opter
    {
      enum
      {
        POSITION = 5,
        MASK = 1 << POSITION
      };

      enum States
      {
        OPTION_BYTE_ERASE_ACTIVATED = 1 << POSITION
      };
    }

    namespace strt
    {
      enum
      {
        POSITION = 6,
        MASK = 1 << POSITION
      };

      enum States
      {
        START = 1 << POSITION
      };
    }

    namespace lock
    {
      enum
      {
        POSITION = 7,
        MASK = 1 << POSITION
      };

      enum States
      {
        LOCK = 1 << POSITION
      };
    }

    namespace optwre
    {
      enum
      {
        POSITION = 9,
        MASK = 1 << POSITION
      };

      enum States
      {
        OPTION_BYTE_WRITE_DISABLE = 0 << POSITION,
        OPTION_BYTE_WRITE_ENABLE = 1 << POSITION
      };
    }

    namespace errie
    {
      enum
      {
        POSITION = 10,
        MASK = 1 << POSITION
      };

      enum States
      {
        INTERRUPT_DISABLED = 0 << POSITION,
        INTERRUPT_ENABLED = 1 << POSITION
      };
    }

    namespace eopie
    {
      enum
      {
        POSITION = 12,
        MASK = 1 << POSITION
      };

      enum States
      {
        INTERRUPT_DISABLED = 0 << POSITION,
        INTERRUPT_ENABLED = 1 << POSITION
      };
    }
#else   // #ifdef STM32F1XX
    namespace snb
    {
      enum
      {
        POSITION = 3,
        MASK = 0b1111 << POSITION
      };

      enum States
      {
        SECTOR_0 = 0 << POSITION,
        SECTOR_1 = 1 << POSITION,
        SECTOR_2 = 2 << POSITION,
        SECTOR_3 = 3 << POSITION,
        SECTOR_4 = 4 << POSITION,
        SECTOR_5 = 5 << POSITION,
        SECTOR_6 = 6 << POSITION,
        SECTOR_7 = 7 << POSITION,
        SECTOR_8 = 8 << POSITION,
        SECTOR_9 = 9 << POSITION,
        SECTOR_10 = 10 << POSITION,
        SECTOR_11 = 11 << POSITION
      };
    }

    namespace psize
    {
      enum
      {
        POSITION = 8,
        MASK = 0b11 << POSITION
      };

      enum States
      {
        PROGRAM_X8 = 0 << POSITION,
        PROGRAM_X16 = 1 << POSITION,
        PROGRAM_X32 = 2 << POSITION,
        PROGRAM_X64 = 3 << POSITION
      };
    }

    namespace strt
    {
      enum
      {
        POSITION = 16,
        MASK = 1 << POSITION
      };

      enum States
      {
        START = 1 << POSITION
      };
    }

    namespace eopie
    {
      enum
      {
        POSITION = 24,
        MASK = 1 << POSITION
      };

      enum States
      {
        INTERRUPT_DISABLED = 0 << POSITION,
        INTERRUPT_ENABLED = 1 << POSITION
      };
    }

    namespace errpie
    {
      enum
      {
        POSITION = 25,
        MASK = 1 << POSITION
      };

      enum States
      {
        INTERRUPT_DISABLED = 0 << POSITION,
        INTERRUPT_ENABLED = 1 << POSITION
      };
    }

    namespace lock
    {
      enum
      {
        POSITION = 31,
        MASK = 1 << POSITION
      };

      enum States
      {
        LOCK = 1 << POSITION
      };
    }
#endif  // #ifdef STM32F1XX
  }  // namespace cr

#ifdef STM32F1XX
  namespace ar
  {
    enum
    {
      OFFSET = 0x14
    };
  }  // namespace ar

  namespace obr
  {
    enum
    {
      OFFSET = 0x1C
    };

    namespace opterr
    {
      enum
      {
        POSITION = 0,
        MASK = 1 << POSITION
      };

      enum States
      {
        OPTION_BYTE_ERROR = 1 << POSITION
      };
    }

    namespace rdprt
    {
      enum
      {
        POSITION = 1,
        MASK = 1 << POSITION
      };

      enum States
      {
        READ_PROTECTION = 1 << POSITION
      };
    }

    namespace wdgsw
    {
      enum
      {
        POSITION = 2,
        MASK = 1 << POSITION
      };

      enum States
      {
        WDG_SW_OFF = 0 << POSITION,
        WDG_SW_ON = 1 << POSITION
      };
    }

    namespace nrststop
    {
      enum
      {
        POSITION = 3,
        MASK = 1 << POSITION
      };

      enum States
      {
        N_RST_STOP_ON = 0 << POSITION,
        N_RST_STOP_OFF = 1 << POSITION
      };
    }

    namespace nrstdby
    {
      enum
      {
        POSITION = 4,
        MASK = 1 << POSITION
      };

      enum States
      {
        N_RST_STDBY_ON = 0 << POSITION,
        N_RST_STDBY_OFF = 1 << POSITION
      };
    }

    namespace data0
    {
      enum
      {
        POSITION = 10,
        MASK = 0b11111111 << POSITION
      };
    }

    namespace data1
    {
      enum
      {
        POSITION = 18,
        MASK = 0b11111111 << POSITION
      };
    }
  }  // namespace obr

  namespace wrpr
  {
    enum
    {
      OFFSET = 0x20
    };
  }  // namespace wrpr
#else  // #ifdef STM32F1XX
  namespace optcr
  {
    enum
    {
      OFFSET = 0x14
    };

    namespace optlock
    {
      enum
      {
        POSITION = 0,
        MASK = 1 << POSITION
      };

      enum States
      {
        OPTLOCK = 1 << POSITION
      };
    }

    namespace optstrt
    {
      enum
      {
        POSITION = 1,
        MASK = 1 << POSITION
      };

      enum States
      {
        START = 1 << POSITION
      };
    }

    namespace bor_lev
    {
      enum
      {
        POSITION = 2,
        MASK = 0b11 << POSITION
      };

      enum States
      {
        VBOR3 = 0 << POSITION,  // 2.70 - 3.60V
        VBOR2 = 1 << POSITION,  // 2.40 - 2.70V
        VBOR1 = 2 << POSITION,  // 2.10 - 2.40V
        VBOR0 = 3 << POSITION   // 1.80 - 2.10V
      };
    }

    namespace wdg_sw
    {
      enum
      {
        POSITION = 5,
        MASK = 1 << POSITION
      };

      enum States
      {
        WDG_SW = 1 << POSITION
      };
    }

    namespace nrst_stop
    {
      enum
      {
        POSITION = 6,
        MASK = 1 << POSITION
      };

      enum States
      {
        NRST_STOP = 1 << POSITION
      };
    }

    namespace nrst_stdby
    {
      enum
      {
        POSITION = 7,
        MASK = 1 << POSITION
      };

      enum States
      {
        NRST_STDBY = 1 << POSITION
      };
    }

    namespace rdp
    {
      enum
      {
        POSITION = 8,
        MASK = 0b11111111 << POSITION
      };

      enum States
      {
        LEVEL0 = 0xAA << POSITION,  // read protection not active
        LEVEL1 = 0x00 << POSITION,  // read protection of memories active
        LEVEL2 = 0xCC << POSITION   // chip read protection active
      };
    }

    namespace nwrp
    {
      enum
      {
        POSITION = 16,
        MASK = 0b111111111111 << POSITION
      };
    }
  }  // namespace optcr
#endif  // #ifdef STM32F1XX
}  // namespace flash
