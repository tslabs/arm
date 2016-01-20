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

namespace tim {
  enum Address {
    TIM2 = alias::APB1 + 0x0000,
    TIM3 = alias::APB1 + 0x0400,
    TIM4 = alias::APB1 + 0x0800,
    TIM5 = alias::APB1 + 0x0C00,
    TIM6 = alias::APB1 + 0x1000,
    TIM7 = alias::APB1 + 0x1400,
    TIM12 = alias::APB1 + 0x1800,
    TIM13 = alias::APB1 + 0x1C00,
    TIM14 = alias::APB1 + 0x2000,
    #ifdef STM32F1XX
    TIM1 = alias::APB2 + 0x2C00,
    TIM8 = alias::APB2 + 0x3400,
    TIM9 = alias::APB2 + 0x4C00,
    TIM10 = alias::APB2 + 0x5000,
    TIM11 = alias::APB2 + 0x5400,
#else
    TIM1 = alias::APB2 + 0x0000,
    TIM8 = alias::APB2 + 0x0400,
    TIM9 = alias::APB2 + 0x4000,
    TIM10 = alias::APB2 + 0x4400,
    TIM11 = alias::APB2 + 0x4800,
#endif
#ifdef VALUE_LINE
  TIM15 = alias::APB2 + 0x4000,
  TIM16 = alias::APB2 + 0x4400,
  TIM17 = alias::APB2 + 0x4800,
#endif
  };

  struct Registers {
      __RW
      u32 CR1;  // 0x00: Control 1
      __RW
      u32 CR2;  // 0x04: Control 2
      __RW
      u32 SMCR;  // 0x08: Slave mode control
      __RW
      u32 DIER;  // 0x0C: DMA/Interrupt enable
      __RW
      u32 SR;  // 0x10: Status
      __RW
      u32 EGR;  // 0x14: Event generation
      __RW
      u32 CCMR1;  // 0x18: Capture/Compare mode 1
      __RW
      u32 CCMR2;  // 0x1C: Capture/Compare mode 2
      __RW
      u32 CCER;  // 0x20: Capture/Compare enable
      __RW
      u32 CNT;  // 0x24: Counter
      __RW
      u32 PSC;  // 0x28: Prescaler
      __RW
      u32 ARR;  // 0x2C: Auto-reload
      __RW
      u32 RCR;  // 0x30: Repetition counter
      __RW
      u32 CCR1;  // 0x34: Capture/Compare 1
      __RW
      u32 CCR2;  // 0x38: Capture/Compare 2
      __RW
      u32 CCR3;  // 0x3C: Capture/Compare 3
      __RW
      u32 CCR4;  // 0x40: Capture/Compare 4
      __RW
      u32 BDTR;  // 0x44: Break and dead-time
      __RW
      u32 DCR;  // 0x48: DMA control
      __RW
      u32 DMAR;  // 0x4C: DMA address for full transfer
      union {
          __RW
          u32 TIM2_OR;  // 0x50: Timer 2 option
          __RW
          u32 TIM5_OR;  // 0x50: Timer 5 option
          __RW
          u32 TIM11_OR;  // 0x50: Timer 11 option
      };
  };

  namespace cr1 {
    enum {
      OFFSET = 0x00
    };

    namespace cen {
      enum {
        POSITION = 0,
        MASK = 1 << POSITION
      };
      enum States {
        COUNTER_DISABLED = 0 << POSITION,
        COUNTER_ENABLED = 1 << POSITION,
      };
    }  // namespace cen

    namespace udis {
      enum {
        POSITION = 1,
        MASK = 1 << POSITION
      };
      enum States {
        UPDATE_EVENT_ENABLED = 0 << POSITION,
        UPDATE_EVENT_DISABLED = 1 << POSITION,
      };
    }  // namespace udis

    namespace urs {
      enum {
        POSITION = 2,
        MASK = 1 << POSITION
      };
      enum States {
        UPDATE_REQUEST_SOURCE_ALL = 0 << POSITION,
        UPDATE_REQUEST_SOURCE_OVERFLOW_UNDERFLOW = 1 << POSITION,
      };
    }  // namespace urs

    namespace opm {
      enum {
        POSITION = 3,
        MASK = 1 << POSITION
      };
      enum States {
        DONT_STOP_COUNTER_AT_NEXT_UPDATE_EVENT = 0 << POSITION,
        STOP_COUNTER_AT_NEXT_UPDATE_EVENT = 1 << POSITION,
      };
    }  // namespace opm

    namespace arpe {
      enum {
        POSITION = 7,
        MASK = 1 << POSITION
      };
      enum States {
        AUTO_RELOAD_UNBUFFERED = 0 << POSITION,
        AUTO_RELOAD_BUFFERED = 1 << POSITION,
      };
    }  // namespace arpe
  }  // namespace cr1

  namespace cr2 {
    enum {
      OFFSET = 0x04
    };

    namespace mms {
      enum {
        POSITION = 4,
        MASK = 0b111 << POSITION
      };
      enum States {
        RESET = 0 << POSITION,
        ENABLE = 1 << POSITION,
        UPDATE = 2 << POSITION,
      };
    }  // namespace mms
  }  // namespace cr2

  namespace smcr {
    enum {
      OFFSET = 0x08
    };
  // TODO TIM SMCR bits
  }

  namespace dier {
    enum {
      OFFSET = 0x0C
    };

    namespace uie {
      enum {
        POSITION = 0,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace uie

    namespace cc1ie {
      enum {
        POSITION = 1,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace cc1ie

    namespace cc2ie {
      enum {
        POSITION = 2,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace cc2ie

    namespace cc3ie {
      enum {
        POSITION = 3,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace cc3ie

    namespace cc4ie {
      enum {
        POSITION = 4,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace cc4ie

    namespace comie {
      enum {
        POSITION = 5,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace comie

    namespace tie {
      enum {
        POSITION = 6,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace tie

    namespace bie {
      enum {
        POSITION = 7,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace bie

    namespace ude {
      enum {
        POSITION = 8,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace ude

    namespace cc1de {
      enum {
        POSITION = 9,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace cc1de

    namespace cc2de {
      enum {
        POSITION = 10,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace cc2de

    namespace cc3de {
      enum {
        POSITION = 11,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace cc3de

    namespace cc4de {
      enum {
        POSITION = 12,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace cc4de

    namespace tde {
      enum {
        POSITION = 14,
        MASK = 1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION,
      };
    }  // namespace tde
  }  // namespace dier

  namespace sr {
    enum {
      OFFSET = 0x10
    };

    namespace uif {
      enum {
        POSITION = 0
      };

      enum {
        MASK = 1 << POSITION
      };

      namespace states {
        enum E {
          NO_INTERRUPT_OCURRED = 0 << POSITION,
          INTERRUPT_PENDING = 1 << POSITION,
        };
      }  // namespace states
    }  // namespace uif

    namespace cc1if {
      enum {
        POSITION = 1
      };

      enum {
        MASK = 1 << POSITION
      };

      namespace states {
        enum E {
          NO_INTERRUPT_OCURRED = 0 << POSITION,
          INTERRUPT_PENDING = 1 << POSITION,
        };
      }  // namespace states
    }  // namespace cc1if

    namespace cc2if {
      enum {
        POSITION = 2
      };

      enum {
        MASK = 1 << POSITION
      };

      namespace states {
        enum E {
          NO_INTERRUPT_OCURRED = 0 << POSITION,
          INTERRUPT_PENDING = 1 << POSITION,
        };
      }  // namespace states
    }  // namespace cc2if

    namespace cc3if {
      enum {
        POSITION = 3
      };

      enum {
        MASK = 1 << POSITION
      };

      namespace states {
        enum E {
          NO_INTERRUPT_OCURRED = 0 << POSITION,
          INTERRUPT_PENDING = 1 << POSITION,
        };
      }  // namespace states
    }  // namespace cc3if

    namespace cc4if {
      enum {
        POSITION = 4
      };

      enum {
        MASK = 1 << POSITION
      };

      namespace states {
        enum E {
          NO_INTERRUPT_OCURRED = 0 << POSITION,
          INTERRUPT_PENDING = 1 << POSITION,
        };
      }  // namespace states
    }  // namespace cc4if

    namespace comif {
      enum {
        POSITION = 5
      };

      enum {
        MASK = 1 << POSITION
      };

      namespace states {
        enum E {
          NO_INTERRUPT_OCURRED = 0 << POSITION,
          INTERRUPT_PENDING = 1 << POSITION,
        };
      }  // namespace states
    }  // namespace comif

    namespace tif {
      enum {
        POSITION = 6
      };

      enum {
        MASK = 1 << POSITION
      };

      namespace states {
        enum E {
          NO_INTERRUPT_OCURRED = 0 << POSITION,
          INTERRUPT_PENDING = 1 << POSITION,
        };
      }  // namespace states
    }  // namespace tif

    namespace bif {
      enum {
        POSITION = 7
      };

      enum {
        MASK = 1 << POSITION
      };

      namespace states {
        enum E {
          NO_INTERRUPT_OCURRED = 0 << POSITION,
          INTERRUPT_PENDING = 1 << POSITION,
        };
      }  // namespace states
    }  // namespace bif

    namespace cc1of {
      enum {
        POSITION = 9
      };

      enum {
        MASK = 1 << POSITION
      };

      namespace states {
        enum E {
          NO_INTERRUPT_OCURRED = 0 << POSITION,
          INTERRUPT_PENDING = 1 << POSITION,
        };
      }  // namespace states
    }  // namespace cc1of

    namespace cc2of {
      enum {
        POSITION = 10
      };

      enum {
        MASK = 1 << POSITION
      };

      namespace states {
        enum E {
          NO_INTERRUPT_OCURRED = 0 << POSITION,
          INTERRUPT_PENDING = 1 << POSITION,
        };
      }  // namespace states
    }  // namespace cc2of

    namespace cc3of {
      enum {
        POSITION = 11
      };

      enum {
        MASK = 1 << POSITION
      };

      namespace states {
        enum E {
          NO_INTERRUPT_OCURRED = 0 << POSITION,
          INTERRUPT_PENDING = 1 << POSITION,
        };
      }  // namespace states
    }  // namespace cc3of

    namespace cc4of {
      enum {
        POSITION = 12
      };

      enum {
        MASK = 1 << POSITION
      };

      namespace states {
        enum E {
          NO_INTERRUPT_OCURRED = 0 << POSITION,
          INTERRUPT_PENDING = 1 << POSITION,
        };
      }  // namespace states
    }  // namespace cc4of
  }  // namespace sr

  namespace egr {
    enum {
      OFFSET = 0x14
    };
    namespace ug {
      enum {
        POSITION = 0
      };
      enum {
        MASK = 1 << POSITION
      };
      enum States {
        NO_ACTION = 0 << POSITION,
        GENERATE_AN_UPDATE = 1 << POSITION,
      };
    }  // namespace ug
  }  // namespace egr

  namespace ccmr1 {
    enum {
      OFFSET = 0x18
    };

    namespace cc1s {
      enum {
        POSITION = 0,
        MASK = 0b11 << POSITION
      };

      enum States {
        OUTPUT = 0 << POSITION,
        INPUT_IC1_TI1 = 1 << POSITION,
        INPUT_IC1_TI2 = 2 << POSITION,
        INPUT_IC1_TRC = 3 << POSITION
      };
    } // namespace cc1s

    namespace oc1fe {
      enum {
        POSITION = 2,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace oc1fe

    namespace oc1pe {
      enum {
        POSITION = 3,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace oc1pe

    namespace oc1m {
      enum {
        POSITION = 4,
        MASK = 0b111 << POSITION
      };

      enum States {
        FROZEN = 0 << POSITION,
        CH_ACT = 1 << POSITION,
        CH_INACT = 2 << POSITION,
        TOGGLE = 3 << POSITION,
        FORCE_INACT = 4 << POSITION,
        FORCE_ACT = 5 << POSITION,
        PWM_1 = 6 << POSITION,
        PWM_2 = 7 << POSITION
      };
    } // namespace oc1m

    namespace oc1ce {
      enum {
        POSITION = 7,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace oc1ce

    namespace ic1psc {
      enum {
        POSITION = 2,
        MASK = 0b11 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        PSC2 = 1 << POSITION,
        PSC4 = 2 << POSITION,
        PSC8 = 3 << POSITION
      };
    } // namespace ic1psc

    namespace ic1f {
      enum {
        POSITION = 4,
        MASK = 0b1111 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        FCK_INT_N2 = 1 << POSITION,
        FCK_INT_N4 = 2 << POSITION,
        FCK_INT_N8 = 3 << POSITION,
        FDTS2_N6 = 4 << POSITION,
        FDTS2_N8 = 5 << POSITION,
        FDTS4_N6 = 6 << POSITION,
        FDTS4_N8 = 7 << POSITION,
        FDTS8_N6 = 8 << POSITION,
        FDTS8_N8 = 9 << POSITION,
        FDTS16_N5 = 10 << POSITION,
        FDTS16_N6 = 11 << POSITION,
        FDTS16_N8 = 12 << POSITION,
        FDTS32_N5 = 13 << POSITION,
        FDTS32_N6 = 14 << POSITION,
        FDTS32_N8 = 15 << POSITION
      };
    } // namespace ic1f

    namespace cc2s {
      enum {
        POSITION = 0,
        MASK = 0b11 << POSITION
      };

      enum States {
        OUTPUT = 0 << POSITION,
        INPUT_IC2_TI2 = 1 << POSITION,
        INPUT_IC2_TI1 = 2 << POSITION,
        INPUT_IC2_TRC = 3 << POSITION
      };
    } // namespace cc2s

    namespace oc2fe {
      enum {
        POSITION = 2,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace oc2fe

    namespace oc2pe {
      enum {
        POSITION = 3,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace oc2pe

    namespace oc2m {
      enum {
        POSITION = 4,
        MASK = 0b111 << POSITION
      };

      enum States {
        FROZEN = 0 << POSITION,
        CH_ACT = 1 << POSITION,
        CH_INACT = 2 << POSITION,
        TOGGLE = 3 << POSITION,
        FORCE_INACT = 4 << POSITION,
        FORCE_ACT = 5 << POSITION,
        PWM_1 = 6 << POSITION,
        PWM_2 = 7 << POSITION
      };
    } // namespace oc2m

    namespace oc2ce {
      enum {
        POSITION = 7,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace oc2ce

    namespace ic2psc {
      enum {
        POSITION = 2,
        MASK = 0b11 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        PSC2 = 1 << POSITION,
        PSC4 = 2 << POSITION,
        PSC8 = 3 << POSITION
      };
    } // namespace ic2psc

    namespace ic2f {
      enum {
        POSITION = 4,
        MASK = 0b1111 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        FCK_INT_N2 = 1 << POSITION,
        FCK_INT_N4 = 2 << POSITION,
        FCK_INT_N8 = 3 << POSITION,
        FDTS2_N6 = 4 << POSITION,
        FDTS2_N8 = 5 << POSITION,
        FDTS4_N6 = 6 << POSITION,
        FDTS4_N8 = 7 << POSITION,
        FDTS8_N6 = 8 << POSITION,
        FDTS8_N8 = 9 << POSITION,
        FDTS16_N5 = 10 << POSITION,
        FDTS16_N6 = 11 << POSITION,
        FDTS16_N8 = 12 << POSITION,
        FDTS32_N5 = 13 << POSITION,
        FDTS32_N6 = 14 << POSITION,
        FDTS32_N8 = 15 << POSITION
      };
    } // namespace ic2f
  }  // namespace ccmr1

  namespace ccmr2 {
    enum {
      OFFSET = 0x1C
    };

    namespace cc3s {
      enum {
        POSITION = 0,
        MASK = 0b11 << POSITION
      };

      enum States {
        OUTPUT = 0 << POSITION,
        INPUT_IC3_TI3 = 1 << POSITION,
        INPUT_IC3_TI4 = 2 << POSITION,
        INPUT_IC3_TRC = 3 << POSITION
      };
    } // namespace cc3s

    namespace oc3fe {
      enum {
        POSITION = 2,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace oc3fe

    namespace oc3pe {
      enum {
        POSITION = 3,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace oc3pe

    namespace oc3m {
      enum {
        POSITION = 4,
        MASK = 0b111 << POSITION
      };

      enum States {
        FROZEN = 0 << POSITION,
        CH_ACT = 1 << POSITION,
        CH_INACT = 2 << POSITION,
        TOGGLE = 3 << POSITION,
        FORCE_INACT = 4 << POSITION,
        FORCE_ACT = 5 << POSITION,
        PWM_1 = 6 << POSITION,
        PWM_2 = 7 << POSITION
      };
    } // namespace oc3m

    namespace oc3ce {
      enum {
        POSITION = 7,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace oc3ce

    namespace ic3psc {
      enum {
        POSITION = 2,
        MASK = 0b11 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        PSC2 = 1 << POSITION,
        PSC4 = 2 << POSITION,
        PSC8 = 3 << POSITION
      };
    } // namespace ic3psc

    namespace ic3f {
      enum {
        POSITION = 4,
        MASK = 0b1111 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        FCK_INT_N2 = 1 << POSITION,
        FCK_INT_N4 = 2 << POSITION,
        FCK_INT_N8 = 3 << POSITION,
        FDTS2_N6 = 4 << POSITION,
        FDTS2_N8 = 5 << POSITION,
        FDTS4_N6 = 6 << POSITION,
        FDTS4_N8 = 7 << POSITION,
        FDTS8_N6 = 8 << POSITION,
        FDTS8_N8 = 9 << POSITION,
        FDTS16_N5 = 10 << POSITION,
        FDTS16_N6 = 11 << POSITION,
        FDTS16_N8 = 12 << POSITION,
        FDTS32_N5 = 13 << POSITION,
        FDTS32_N6 = 14 << POSITION,
        FDTS32_N8 = 15 << POSITION
      };
    } // namespace ic3f

    namespace cc4s {
      enum {
        POSITION = 0,
        MASK = 0b11 << POSITION
      };

      enum States {
        OUTPUT = 0 << POSITION,
        INPUT_IC4_TI4 = 1 << POSITION,
        INPUT_IC4_TI3 = 2 << POSITION,
        INPUT_IC4_TRC = 3 << POSITION
      };
    } // namespace cc4s

    namespace oc4fe {
      enum {
        POSITION = 2,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace oc4fe

    namespace oc4pe {
      enum {
        POSITION = 3,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace oc4pe

    namespace oc4m {
      enum {
        POSITION = 4,
        MASK = 0b111 << POSITION
      };

      enum States {
        FROZEN = 0 << POSITION,
        CH_ACT = 1 << POSITION,
        CH_INACT = 2 << POSITION,
        TOGGLE = 3 << POSITION,
        FORCE_INACT = 4 << POSITION,
        FORCE_ACT = 5 << POSITION,
        PWM_1 = 6 << POSITION,
        PWM_2 = 7 << POSITION
      };
    } // namespace oc4m

    namespace oc4ce {
      enum {
        POSITION = 7,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace oc4ce

    namespace ic4psc {
      enum {
        POSITION = 2,
        MASK = 0b11 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        PSC2 = 1 << POSITION,
        PSC4 = 2 << POSITION,
        PSC8 = 3 << POSITION
      };
    } // namespace ic4psc

    namespace ic4f {
      enum {
        POSITION = 4,
        MASK = 0b1111 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        FCK_INT_N2 = 1 << POSITION,
        FCK_INT_N4 = 2 << POSITION,
        FCK_INT_N8 = 3 << POSITION,
        FDTS2_N6 = 4 << POSITION,
        FDTS2_N8 = 5 << POSITION,
        FDTS4_N6 = 6 << POSITION,
        FDTS4_N8 = 7 << POSITION,
        FDTS8_N6 = 8 << POSITION,
        FDTS8_N8 = 9 << POSITION,
        FDTS16_N5 = 10 << POSITION,
        FDTS16_N6 = 11 << POSITION,
        FDTS16_N8 = 12 << POSITION,
        FDTS32_N5 = 13 << POSITION,
        FDTS32_N6 = 14 << POSITION,
        FDTS32_N8 = 15 << POSITION
      };
    } // namespace ic4f
  } // namespace ccmr2

  namespace ccer {
    enum {
      OFFSET = 0x20
    };

    namespace cc1e {
      enum {
        POSITION = 0,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace cc1e

    namespace cc1p {
      enum {
        POSITION = 1,
        MASK = 0b1 << POSITION
      };

      enum States {
        HIGH = 0 << POSITION,
        LOW = 1 << POSITION
      };
    } // namespace cc1p

    namespace cc1npp {
      enum {
        POSITION = 1,
        MASK = 0b101 << POSITION
      };

      enum States {
        NONINV_RISE = 0b000 << POSITION,
        INV_FALL = 0b001 << POSITION,
        NONINV_BOTH = 0b101 << POSITION
      };
    } // namespace cc1npp

    namespace cc2e {
      enum {
        POSITION = 4,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace cc2e

    namespace cc2p {
      enum {
        POSITION = 5,
        MASK = 0b1 << POSITION
      };

      enum States {
        HIGH = 0 << POSITION,
        LOW = 1 << POSITION
      };
    } // namespace cc2p

    namespace cc2npp {
      enum {
        POSITION = 5,
        MASK = 0b101 << POSITION
      };

      enum States {
        NONINV_RISE = 0b000 << POSITION,
        INV_FALL = 0b001 << POSITION,
        NONINV_BOTH = 0b101 << POSITION
      };
    } // namespace cc2npp

    namespace cc3e {
      enum {
        POSITION = 8,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace cc3e

    namespace cc3p {
      enum {
        POSITION = 9,
        MASK = 0b1 << POSITION
      };

      enum States {
        HIGH = 0 << POSITION,
        LOW = 1 << POSITION
      };
    } // namespace cc3p

    namespace cc3npp {
      enum {
        POSITION = 9,
        MASK = 0b101 << POSITION
      };

      enum States {
        NONINV_RISE = 0b000 << POSITION,
        INV_FALL = 0b001 << POSITION,
        NONINV_BOTH = 0b101 << POSITION
      };
    } // namespace cc3npp

    namespace cc4e {
      enum {
        POSITION = 12,
        MASK = 0b1 << POSITION
      };

      enum States {
        DISABLED = 0 << POSITION,
        ENABLED = 1 << POSITION
      };
    } // namespace cc4e

    namespace cc4p {
      enum {
        POSITION = 13,
        MASK = 0b1 << POSITION
      };

      enum States {
        HIGH = 0 << POSITION,
        LOW = 1 << POSITION
      };
    } // namespace cc4p

    namespace cc4npp {
      enum {
        POSITION = 13,
        MASK = 0b101 << POSITION
      };

      enum States {
        NONINV_RISE = 0b000 << POSITION,
        INV_FALL = 0b001 << POSITION,
        NONINV_BOTH = 0b101 << POSITION
      };
    } // namespace cc4npp
  } // namespace ccer

  namespace cnt {
    enum {
      OFFSET = 0x24
    };
  }  // namespace cnt

  namespace psc {
    enum {
      OFFSET = 0x28
    };
  } // namespace psc

  namespace arr {
    enum {
      OFFSET = 0x2C
    };
  } // namespace arr

  namespace rcr {
    enum {
      OFFSET = 0x30
    };
  } // namespace rcr

  namespace ccr1 {
    enum {
      OFFSET = 0x34
    };
  }  // namespace ccr1

  namespace ccr2 {
    enum {
      OFFSET = 0x38
    };
  }  // namespace ccr2

  namespace ccr3 {
    enum {
      OFFSET = 0x3C
    };
  }  // namespace ccr3

  namespace ccr4 {
    enum {
      OFFSET = 0x40
    };
  }  // namespace ccr4

  namespace bdtr {
    enum {
      OFFSET = 0x44
    };
// TODO TIM BDTR bits
  }// namespace bdtr

  namespace dcr {
    enum {
      OFFSET = 0x48
    };
// TODO TIM DCR bits
  }// namespace dcr

  namespace dmar {
    enum {
      OFFSET = 0x4C
    };
  }  // namespace dmar

  namespace tim2_or {
    enum {
      OFFSET = 0x50
    };
// TODO TIM TIM2_OR bits
  }// namespace tim2_or

  namespace tim5_or {
    enum {
      OFFSET = 0x50
    };
// TODO TIM TIM5_OR bits
  }// namespace tim5_or

  namespace tim11_or {
    enum {
      OFFSET = 0x50
    };
// TODO TIM TIM11_OR bits
  }// namespace tim11_or
}  // namespace tim
