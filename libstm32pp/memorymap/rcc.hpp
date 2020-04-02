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

namespace rcc
{
  struct Registers
  {
#ifdef STM32F1XX
      __RW
      u32 CR;  // 0x00: Clock control
      __RW
      u32 CFGR;  // 0x04: Clock configuration
      __RW
      u32 CIR;  // 0x08: Clock interrupt
      __RW
      u32 APB2RSTR;  // 0x0C: APB2 peripheral reset
      __RW
      u32 APB1RSTR;  // 0x10: APB1 peripheral reset
      __RW
      u32 AHBENR;  // 0x14: AHB peripheral clock enable
      __RW
      u32 APB2ENR;  // 0x18: APB2 peripheral clock enable
      __RW
      u32 APB1ENR;  // 0x1C: APB1 peripheral clock enable
      __RW
      u32 BDCR;  // 0x20: Backup domain control
      __RW
      u32 CSR;  // 0x24: Control/status
      __RW
      u32 AHBRSTR;  // 0x28: AHB peripheral clock reset
      __RW
      u32 CFGR2;  // 0x2C: Clock configuration 2
#else // STM32F1XX
      __RW
      u32 CR;// 0x00: Clock control
      __RW
      u32 PLLCFGR;// 0x04: PLL configuration
      __RW
      u32 CFGR;// 0x08: Clock configuration
      __RW
      u32 CIR;// 0x0C: Clock interrupt
      __RW
      u32 AHB1RSTR;// 0x10: AHB1 peripheral reset
      __RW
      u32 AHB2RSTR;// 0x14: AHB2 peripheral reset
      __RW
      u32 AHB3RSTR;// 0x18: AHB3 peripheral reset
      u32 _RESERVED0;
      __RW
      u32 APB1RSTR;// 0x20: APB1 peripheral reset
      __RW
      u32 APB2RSTR;// 0x24: APB2 peripheral reset
      u32 _RESERVED1[2];
      __RW
      u32 AHB1ENR;// 0x30: AHB1 peripheral clock enable
      __RW
      u32 AHB2ENR;// 0x34: AHB2 peripheral clock enable
      __RW
      u32 AHB3ENR;// 0x38: AHB3 peripheral clock enable
      u32 _RESERVED2;
      __RW
      u32 APB1ENR;// 0x40: APB1 peripheral clock enable
      __RW
      u32 APB2ENR;// 0x44: APB2 peripheral clock enable
      u32 _RESERVED3[2];
      __RW
      u32 AHB1LPENR;// 0x50: AHB1 peripheral clock enable in low power
      __RW
      u32 AHB2LPENR;// 0x54: AHB2 peripheral clock enable in low power
      __RW
      u32 AHB3LPENR;// 0x58: AHB3 peripheral clock enable in low power
      u32 _RESERVED4;
      __RW
      u32 APB1LPENR;// 0x60: APB1 peripheral clock enable in low power
      __RW
      u32 APB2LPENR;// 0x64: APB2 peripheral clock enable in low power
      u32 _RESERVED5[2];
      __RW
      u32 BDCR;// 0x70: Backup domain control
      __RW
      u32 CSR;// 0x74: Clock control and status
      u32 _RESERVED6[2];
      __RW
      u32 SSCGR;// 0x80: Spread spectrum clock generation
      __RW
      u32 PLLI2SCFGR;// 0x84: PLLI2S configuration
#endif // STM32F1XX
  };

  enum
  {
#ifdef STM32F1XX
    ADDRESS = alias::AHB + 0x1000
#else
    ADDRESS = alias::AHB1 + 0x3800
#endif
  };

namespace cr
{
  enum
  {
    OFFSET = 0x00
  };

  namespace hsion
  {
    enum
    {
      POSITION = 0,
      MASK = 1U << POSITION
    };

    enum States
    {
      HSI_OSCILLATOR_OFF = 0U << POSITION,
      HSI_OSCILLATOR_ON = 1U << POSITION,
    };
  }  // namespace hsion

  namespace hsirdy
  {
    enum
    {
      POSITION = 1,
      MASK = 1U << POSITION
    };

    enum States
    {
      HSI_OSCILLATOR_NOT_READY = 0U << POSITION,
      HSI_OSCILLATOR_READY = 1U << POSITION,
    };
  }  // namespace hsirdy

  namespace hseon
  {
    enum
    {
      POSITION = 16,
      MASK = 1U << POSITION
    };
    enum States {
      HSE_OSCILLATOR_OFF = 0U << POSITION,
      HSE_OSCILLATOR_ON = 1U << POSITION,
    };
  }  // namespace hseon

  namespace hserdy {
    enum {
      POSITION = 17,
      MASK = 1U << POSITION
    };
    enum States {
      HSE_OSCILLATOR_NOT_READY = 0U << POSITION,
      HSE_OSCILLATOR_READY = 1U << POSITION,
    };
  }  // namespace hserdy

  namespace hsebyp {
    enum {
      POSITION = 18,
      MASK = 1U << POSITION
    };
    enum States {
      HSE_OSCILLATOR_NOT_BYPASSED = 0U << POSITION,
      HSE_OSCILLATOR_BYPASSED_WITH_EXTERNAL_CLOCK = 1U << POSITION,
    };
  }  // namespace hsebyp

  namespace csson {
    enum {
      POSITION = 19,
      MASK = 1U << POSITION
    };
    enum States {
      CLOCK_DETECTOR_OFF = 0U << POSITION,
      CLOCK_DETECTOR_ON = 1U << POSITION,
    };
  }  // namespace csson

  namespace pllon {
    enum {
      POSITION = 24,
      MASK = 1U << POSITION
    };
    enum States {
      PLL_OFF = 0U << POSITION,
      PLL_ON = 1U << POSITION,
    };
  }  // namespace pllon

  namespace pllrdy {
    enum {
      POSITION = 25,
      MASK = 1U << POSITION
    };
    enum States {
      PLL_UNLOCKED = 0U << POSITION,
      PLL_LOCKED = 1U << POSITION,
    };
  }  // namespace pllrdy
#ifndef STM32F1XX
  namespace plli2son {
    enum {
      POSITION = 26,
      MASK = 1U << POSITION
    };
    enum States {
      PLLI2S_OFF = 0U << POSITION,
      PLLI2S_ON = 1U << POSITION,
    };
  }  // namespace plli2son

  namespace plli2srdy {
    enum {
      POSITION = 27,
      MASK = 1U << POSITION
    };
    enum States {
      PLLI2S_UNLOCKED = 0U << POSITION,
      PLLI2S_LOCKED = 1U << POSITION,
    };
  }  // namespace plli2srdy
#elif defined CONNECTIVITY_LINE
  namespace pll2on {
    enum {
      POSITION = 26,
      MASK = 1U << POSITION
    };
    enum States {
      PLL2_OFF = 0U << POSITION,
      PLL2_ON = 1U << POSITION,
    };
  }  // namespace pll2on

  namespace pll2rdy {
    enum {
      POSITION = 27,
      MASK = 1U << POSITION
    };
    enum States {
      PLL2_UNLOCKED = 0U << POSITION,
      PLL2_LOCKED = 1U << POSITION,
    };
  }  // namespace pll2rdy

  namespace pll3on {
    enum {
      POSITION = 28,
      MASK = 1U << POSITION
    };
    enum States {
      PLL3_OFF = 0U << POSITION,
      PLL3_ON = 1U << POSITION,
    };
  }  // namespace pll3on

  namespace pll3rdy {
    enum {
      POSITION = 29,
      MASK = 1U << POSITION
    };
    enum States {
      PLL3_UNLOCKED = 0U << POSITION,
      PLL3_LOCKED = 1U << POSITION,
    };
  }  // namespace pll3rdy
#endif // CONNECTIVITY_LINE
}  // namespace cr

#ifndef STM32F1XX
namespace pllcfgr {
  enum {
    OFFSET = 0x04
  };
  namespace pllm {
    enum {
      POSITION = 0,
      MASK = 0b111111U << POSITION
    };
  }  // namespace pllm

  namespace plln {
    enum {
      POSITION = 6,
      MASK = 0b111111111U << POSITION
    };
  }  // namespace plln

  namespace pllp {
    enum {
      POSITION = 16,
      MASK = 0b11U << POSITION
    };
  }  // namespace pllp

  namespace pllsrc {
    enum {
      POSITION = 22,
      MASK = 1U << POSITION
    };
    enum States {
      USE_HSI_CLOCK_AS_PLL_CLOCK_SOURCE = 0U << POSITION,
      USE_HSE_CLOCK_AS_PLL_CLOCK_SOURCE = 1U << POSITION,
    };
  }  // namespace pllsrc

  namespace pllq {
    enum {
      POSITION = 24,
      MASK = 0b1111U << POSITION
    };
  }  // namespace pllq
}  // namespace pllcfgr
#endif // !STM32F1XX
namespace cfgr {
  enum {
#ifdef STM32F1XX
    OFFSET = 0x04
#else // STM32F1XX
  OFFSET = 0x08
#endif // STM32F1XX
};
namespace sw {
  enum {
    POSITION = 0,
    MASK = 0b11U << POSITION
  };
  enum States {
    HSI_OSCILLATOR_SELECTED_AS_SYSTEM_CLOCK = 0U << POSITION,
    HSE_OSCILLATOR_SELECTED_AS_SYSTEM_CLOCK = 1U << POSITION,
    PLL_SELECTED_AS_SYSTEM_CLOCK = 2U << POSITION,
  };
}  // namespace sw

namespace sws {
  enum {
    POSITION = 2,
    MASK = 0b11U << POSITION
  };
  enum States {
    HSI_OSCILLATOR_USED_AS_SYSTEM_CLOCK = 0U << POSITION,
    HSE_OSCILLATOR_USED_AS_SYSTEM_CLOCK = 1U << POSITION,
    PLL_USED_AS_SYSTEM_CLOCK = 2U << POSITION,
  };
}  // namespace sws

namespace hpre {
  enum {
    POSITION = 4,
    MASK = 0b1111U << POSITION
  };
  enum States {
    SYSCLK_NOT_DIVIDED = 0U << POSITION,
    SYSCLK_DIVIDED_BY_2 = 8U << POSITION,
    SYSCLK_DIVIDED_BY_4 = 9U << POSITION,
    SYSCLK_DIVIDED_BY_8 = 10U << POSITION,
    SYSCLK_DIVIDED_BY_16 = 11U << POSITION,
    SYSCLK_DIVIDED_BY_64 = 12U << POSITION,
    SYSCLK_DIVIDED_BY_128 = 13U << POSITION,
    SYSCLK_DIVIDED_BY_256 = 14U << POSITION,
    SYSCLK_DIVIDED_BY_512 = 15U << POSITION,
  };
}  // namespace hpre

namespace ppre1 {
  enum {
#ifdef STM32F1XX
    POSITION = 8,
    #else // STM32F1XX
    POSITION = 10,
#endif // STM32F1XX
    MASK = 0b111U << POSITION
  };
  enum States {
    HCLK_NOT_DIVIDED = 0U << POSITION,
    HCLK_DIVIDED_BY_2 = 4U << POSITION,
    HCLK_DIVIDED_BY_4 = 5U << POSITION,
    HCLK_DIVIDED_BY_8 = 6U << POSITION,
    HCLK_DIVIDED_BY_16 = 7U << POSITION,
  };
}  // namespace ppre1

namespace ppre2 {
  enum {
#ifdef STM32F1XX
    POSITION = 11,
    #else // STM32F1XX
    POSITION = 13,
#endif // STM32F1XX
    MASK = 0b111U << POSITION
  };
  enum States {
    HCLK_NOT_DIVIDED = 0U << POSITION,
    HCLK_DIVIDED_BY_2 = 4U << POSITION,
    HCLK_DIVIDED_BY_4 = 5U << POSITION,
    HCLK_DIVIDED_BY_8 = 6U << POSITION,
    HCLK_DIVIDED_BY_16 = 7U << POSITION,
  };
}  // namespace ppre2

namespace adcpre {
  enum {
    POSITION = 14,
    MASK = 0b11U << POSITION
  };
  enum States {
    P2CLK_DIVIDED_BY_2 = 0U << POSITION,
    P2CLK_DIVIDED_BY_4 = 1U << POSITION,
    P2CLK_DIVIDED_BY_6 = 2U << POSITION,
    P2CLK_DIVIDED_BY_8 = 3U << POSITION,
  };
}  // namespace adcpre
#ifdef STM32F1XX
namespace pllsrc {
  enum {
    POSITION = 16,
    MASK = 1U << POSITION
  };
  enum States {
    USE_HSI_CLOCK_DIV_2_AS_PLL_SOURCE = 0U << POSITION,
    USE_PREDIV1_OUTPUT_AS_PLL_SOURCE = 1U << POSITION,
  };
}  // namespace pllsrc

namespace pllxtpre {
  enum {
    POSITION = 17,
    MASK = 1U << POSITION
  };
  enum States {
    HSE_SELECTED_AS_PREDIV1_SOURCE = 0U << POSITION,
    HSE_SELECTED_AS_PREDIV1_SOURCE_DIV_2 = 1U << POSITION,
  };
}  // namespace pllxtpre

namespace pllmul {
  enum {
    POSITION = 18,
    MASK = 0b111U << POSITION
  };
  enum States {
#ifndef CONNECTIVITY_LINE
    PLL_INPUT_CLOCK_X_2 = 0U << POSITION,
    PLL_INPUT_CLOCK_X_3 = 1U << POSITION,
    #endif // CONNECTIVITY_LINE
    PLL_INPUT_CLOCK_X_4 = 2U << POSITION,
    PLL_INPUT_CLOCK_X_5 = 3U << POSITION,
    PLL_INPUT_CLOCK_X_6 = 4U << POSITION,
    PLL_INPUT_CLOCK_X_7 = 5U << POSITION,
    PLL_INPUT_CLOCK_X_8 = 6U << POSITION,
    PLL_INPUT_CLOCK_X_9 = 7U << POSITION,
    #ifndef CONNECTIVITY_LINE
    PLL_INPUT_CLOCK_X_10 = 8U << POSITION,
    PLL_INPUT_CLOCK_X_11 = 9U << POSITION,
    PLL_INPUT_CLOCK_X_12 = 10U << POSITION,
    PLL_INPUT_CLOCK_X_13 = 11U << POSITION,
    PLL_INPUT_CLOCK_X_14 = 12U << POSITION,
    PLL_INPUT_CLOCK_X_15 = 13U << POSITION,
    PLL_INPUT_CLOCK_X_16 = 14U << POSITION,
#else // !CONNECTIVITY_LINE
  PLL_INPUT_CLOCK_X_6_DOT_5 = 13U << POSITION,
#endif // !CONNECTIVITY_LINE
};
}  // namespace pllmul

namespace usbpre {
enum {
  POSITION = 22,
  MASK = 1U << POSITION
};
enum States {
  PLL_CLOCK_DIVIDED_BY_1_DOT_5 = 0U << POSITION,
  PLL_CLOCK_NOT_DIVIDED = 1U << POSITION,
};
}
#ifdef CONNECTIVITY_LINE
namespace mco {
enum {
  POSITION = 24,
  MASK = 0b1111U << POSITION
};
enum States {
  NO_CLOCK_OUTPUT = 0b0000U << POSITION,
  OUTPUT_SYSTEM_CLOCK = 0b0100U << POSITION,
  OUTPUT_HSI_CLOCK = 0b0101U << POSITION,
  OUTPUT_HSE_CLOCK = 0b0110U << POSITION,
  OUTPUT_PLL_CLOCK_OVER_2 = 0b0111U << POSITION,
  OUTPUT_PLL2_CLOCK = 0b1000U << POSITION,
  OUTPUT_XT1_CLOCK = 0b1010U << POSITION,
  OUTPUT_PLL3_CLOCK = 0b1011U << POSITION,
};
}  // namespace mco
#else // CONNECTIVITY_LINE
namespace mco {
enum {
  POSITION = 24,
  MASK = 0b111U << POSITION
};
enum States {
  NO_CLOCK_OUTPUT = 0b000U << POSITION,
  OUTPUT_SYSTEM_CLOCK = 0b100U << POSITION,
  OUTPUT_HSI_CLOCK = 0b101U << POSITION,
  OUTPUT_HSE_CLOCK = 0b110U << POSITION,
  OUTPUT_PLL_CLOCK_OVER_2 = 0b111U << POSITION,
};
}  // namespace mco
#endif // CONNECTIVITY_LINE
#else // STM32F1XX
namespace rtcpre {
enum {
  POSITION = 16,
  MASK = 0b11111U << POSITION
};
}  // namespace rtcpre

namespace mco1 {
enum {
  POSITION = 21,
  MASK = 0b11U << POSITION
};
enum States {
  OUTPUT_HSI_CLOCK = 0U << POSITION,
  OUTPUT_LSE_CLOCK = 1U << POSITION,
  OUTPUT_HSE_CLOCK = 2U << POSITION,
  OUTPUT_PLL_CLOCK = 3U << POSITION,
};
}  // namespace mco1

namespace i2ssrc {
enum {
  POSITION = 23,
  MASK = 1U << POSITION
};
enum States {
  PLLI2S_USED_AS_I2S_CLOCK_SOURCE = 0U << POSITION,
  I2S_CKIN_USED_AS_I2S_CLOCK_SOURCE = 1U << POSITION,
};
}  // namespace i2ssrc

namespace mco1pre {
enum {
  POSITION = 24,
  MASK = 0b111U << POSITION
};
}  // namespace mco1pre

namespace mco2pre {
enum {
  POSITION = 27,
  MASK = 0b111U << POSITION
};
}  // namespace mco2pre

namespace mco2 {
enum {
  POSITION = 30,
  MASK = 0b11U << POSITION
};
enum States {
  OUTPUT_SYSTEM_CLOCK = 0U << POSITION,
  OUTPUT_PLLI2S_CLOCK = 1U << POSITION,
  OUTPUT_HSE_CLOCK = 2U << POSITION,
  OUTPUT_PLL_CLOCK = 3U << POSITION,
};
}  // namespace mco2
#endif // STM32F1XX
}  // namespace cfgr
namespace cir {
enum {
#ifdef STM32F1XX
OFFSET = 0x08
#else // STM32F1XX
OFFSET = 0x0C
#endif // STM32F1XX
};
  // TODO RCC CIR bits
} // namespace cir

namespace apb2rstr {
enum {
#ifdef STM32F1XX
OFFSET = 0x0C
#else // STM32F1XX
OFFSET = 0x24
#endif // STM32F1XX
};
enum Bits {
#ifdef STM32F1XX
AFIO = 1 << 0,
IOPA = 1 << 2,
IOPB = 1 << 3,
IOPC = 1 << 4,
IOPD = 1 << 5,
IOPE = 1 << 6,
IOPF = 1 << 7,
IOPG = 1 << 8,
ADC1 = 1 << 9,
ADC2 = 1 << 10,
TIM1 = 1 << 11,
SPI1 = 1 << 12,
TIM8 = 1 << 13,
USART1 = 1 << 14,
ADC3 = 1 << 15,
TIM15 = 1 << 16,
TIM16 = 1 << 17,
TIM17 = 1 << 18,
TIM9 = 1 << 19,
TIM10 = 1 << 20,
TIM11 = 1 << 21,
ALL = AFIO + IOPA + IOPB + IOPC + IOPD + IOPE + IOPF + IOPG + ADC1 + ADC2 +
TIM1 + SPI1 + TIM8 + USART1 + ADC3 + TIM15 + TIM16 + TIM17 + TIM9 + TIM10 +
TIM11,
#else // STM32F1XX
TIM1 = 1 << 0,
TIM8 = 1 << 1,
USART1 = 1 << 4,
USART6 = 1 << 5,
ADC = 1 << 8,
SDIO = 1 << 11,
SPI1 = 1 << 12,
SYSCFG = 1 << 14,
TIM9 = 1 << 16,
TIM10 = 1 << 17,
TIM11 = 1 << 18,
ALL = TIM1 + TIM8 + USART1 + USART6 + ADC + SDIO + SPI1 + SYSCFG + TIM9 +
TIM10 + TIM11,
#endif // STM32F1XX
};
} // namespace apb2rstr

namespace apb1rstr {
enum {
#ifdef STM32F1XX
OFFSET = 0x10
#else // STM32F1XX
OFFSET = 0x20
#endif // STM32F1XX
};
enum Bits {
#ifdef STM32F1XX
TIM2 = 1 << 0,
TIM3 = 1 << 1,
TIM4 = 1 << 2,
TIM5 = 1 << 3,
TIM6 = 1 << 4,
TIM7 = 1 << 5,
TIM12 = 1 << 6,
TIM13 = 1 << 7,
TIM14 = 1 << 8,
WWDG = 1 << 11,
SPI2 = 1 << 14,
SPI3 = 1 << 15,
USART2 = 1 << 17,
USART3 = 1 << 18,
UART4 = 1 << 19,
UART5 = 1 << 20,
I2C1 = 1 << 21,
I2C2 = 1 << 22,
USB = 1 << 23,
#ifdef CONNECTIVITY_LINE
CAN1 = 1 << 25,
CAN2 = 1 << 26,
#else // CONNECTIVITY_LINE
CAN = 1 << 25,
#endif // CONNECTIVITY_LINE
BKP = 1 << 27,
PWR = 1 << 28,
DAC = 1 << 29,
CEC = 1 << 30,
#ifdef CONNECTIVITY_LINE
ALL = TIM2 + TIM3 + TIM4 + TIM5 + TIM6 + TIM7 + TIM12 + TIM13 + TIM14 + WWDG +
SPI2 + SPI3 + USART2 + USART3 + UART4 + UART5 + I2C1 + I2C2 + USB + CAN1 +
CAN2 + BKP + PWR + DAC + CEC,
#else // CONNECTIVITY_LINE
ALL = TIM2 + TIM3 + TIM4 + TIM5 + TIM6 + TIM7 + TIM12 + TIM13 + TIM14 + WWDG +
SPI2 + SPI3 + USART2 + USART3 + UART4 + UART5 + I2C1 + I2C2 + USB + CAN + BKP +
PWR + DAC + CEC,
#endif // CONNECTIVITY_LINE
#else // STM32F1XX
TIM2 = 1 << 0,
TIM3 = 1 << 1,
TIM4 = 1 << 2,
TIM5 = 1 << 3,
TIM6 = 1 << 4,
TIM7 = 1 << 5,
TIM12 = 1 << 6,
TIM13 = 1 << 7,
TIM14 = 1 << 8,
WWDG = 1 << 11,
SPI2 = 1 << 14,
SPI3 = 1 << 15,
USART2 = 1 << 17,
USART3 = 1 << 18,
UART4 = 1 << 19,
UART5 = 1 << 20,
I2C1 = 1 << 21,
I2C2 = 1 << 22,
I2C3 = 1 << 23,
CAN1 = 1 << 25,
CAN2 = 1 << 26,
PWR = 1 << 28,
DAC = 1 << 29,
ALL = TIM2 + TIM3 + TIM4 + TIM5 + TIM6 + TIM7 + TIM12 + TIM13 + TIM14 + WWDG +
SPI2 + SPI3 + USART2 + USART3 + UART4 + UART5 + I2C1 + I2C2 + I2C3 + CAN1 +
CAN2 + PWR + DAC,
#endif // STM32F1XX
};
}
 // namespace apb1rstr

#ifdef STM32F1XX
namespace ahbenr {
enum {
OFFSET = 0x14
};
enum Bits {
DMA1 = 1 << 0,
DMA2 = 1 << 1,
SRAM = 1 << 2,
FLITF = 1 << 4,
CRC = 1 << 6,
FSMC = 1 << 8,
SDIO = 1 << 10,
USB_OTG_FS = 1 << 12,
ETH_MAC = 1 << 14,
ETH_MAC_TX = 1 << 15,
ETH_MAC_RX = 1 << 16,
};
}  // namespace ahbenr
#endif // STM32F1XX

  namespace apb2enr
  {
    enum
    {
#ifdef STM32F1XX
      OFFSET = 0x18
#else // STM32F1XX
      OFFSET = 0x44
#endif // STM32F1XX
    };

    enum Bits
    {
#ifdef STM32F1XX
      AFIO = 1 << 0,
      IOPA = 1 << 2,
      IOPB = 1 << 3,
      IOPC = 1 << 4,
      IOPD = 1 << 5,
      IOPE = 1 << 6,
      IOPF = 1 << 7,
      IOPG = 1 << 8,
      ADC1 = 1 << 9,
      ADC2 = 1 << 10,
      TIM1 = 1 << 11,
      SPI1 = 1 << 12,
      TIM8 = 1 << 13,
      USART1 = 1 << 14,
      ADC3 = 1 << 15,
      TIM15 = 1 << 16,
      TIM16 = 1 << 17,
      TIM17 = 1 << 18,
      TIM9 = 1 << 19,
      TIM10 = 1 << 20,
      TIM11 = 1 << 21,
#else // STM32F1XX
      TIM1 = 1 << 0,
      TIM8 = 1 << 1,
      USART1 = 1 << 4,
      USART6 = 1 << 5,
      ADC1 = 1 << 8,
      ADC2 = 1 << 9,
      ADC3 = 1 << 10,
      SDIO = 1 << 11,
      SPI1 = 1 << 12,
      SYSCFG = 1 << 14,
      TIM9 = 1 << 16,
      TIM10 = 1 << 17,
      TIM11 = 1 << 18,
#endif // STM32F1XX
    };
  }
  // namespace apb2enr

namespace apb1enr {
enum {
#ifdef STM32F1XX
OFFSET = 0x1C
#else // STM32F1XX
OFFSET = 0x40
#endif // STM32F1XX
};
enum Bits {
#ifdef STM32F1XX
TIM2 = 1 << 0,
TIM3 = 1 << 1,
TIM4 = 1 << 2,
TIM5 = 1 << 3,
TIM6 = 1 << 4,
TIM7 = 1 << 5,
TIM12 = 1 << 6,
TIM13 = 1 << 7,
TIM14 = 1 << 8,
WWDG = 1 << 11,
SPI2 = 1 << 14,
SPI3 = 1 << 15,
USART2 = 1 << 17,
USART3 = 1 << 18,
UART4 = 1 << 19,
UART5 = 1 << 20,
I2C1 = 1 << 21,
I2C2 = 1 << 22,
USB = 1 << 23,
#ifdef CONNECTIVITY_LINE
CAN1 = 1 << 25,
CAN2 = 1 << 26,
#else // CONNECTIVITY_LINE
CAN = 1 << 25,
#endif // CONNECTIVITY_LINE
BKP = 1 << 27,
PWR = 1 << 28,
DAC = 1 << 29,
CEC = 1 << 30,
#else // STM32F1XX
TIM2 = 1 << 0,
TIM3 = 1 << 1,
TIM4 = 1 << 2,
TIM5 = 1 << 3,
TIM6 = 1 << 4,
TIM7 = 1 << 5,
TIM12 = 1 << 6,
TIM13 = 1 << 7,
TIM14 = 1 << 8,
WWDG = 1 << 11,
SPI2 = 1 << 14,
SPI3 = 1 << 15,
USART2 = 1 << 17,
USART3 = 1 << 18,
UART4 = 1 << 19,
UART5 = 1 << 20,
I2C1 = 1 << 21,
I2C2 = 1 << 22,
I2C3 = 1 << 23,
CAN1 = 1 << 25,
CAN2 = 1 << 26,
PWR = 1 << 28,
DAC = 1 << 29,
#endif // STM32F1XX
};
}
  // namespace apb1enr

namespace bdcr {
enum {
#ifdef STM32F1XX
OFFSET = 0x20
#else // STM32F1XX
OFFSET = 0x70
#endif // STM32F1XX
};
namespace lseon {
enum {
POSITION = 0,
MASK = 1U << POSITION
};
enum States {
LSE_CLOCK_OFF = 0U << POSITION,
LSE_CLOCK_ON = 1U << POSITION,
};
}  // namespace lseon

namespace lserdy {
enum {
POSITION = 1,
MASK = 1U << POSITION
};
enum States {
LSE_CLOCK_NOT_READY = 0U << POSITION,
LSE_CLOCK_READY = 1U << POSITION,
};
}  // namespace lserdy

namespace lsebyp {
enum {
POSITION = 2,
MASK = 1U << POSITION
};
enum States {
LSE_OSCILLATOR_NOT_BYPASSED = 0U << POSITION,
LSE_OSCILLATOR_BYPASSED = 1U << POSITION,
};
}  // namespace lsebyp

namespace rtcsel {
enum {
POSITION = 8,
MASK = 0b11U << POSITION
};
enum States {
NO_RTC_CLOCK_SOURCE = 0U << POSITION,
LSE_CLOCK_AS_RTC_SOURCE = 1U << POSITION,
LSI_CLOCK_AS_RTC_SOURCE = 2U << POSITION,
HSE_CLOCK_AS_RTC_SOURCE = 3U << POSITION,
};
}  // namespace rtcsel

namespace rtcen {
enum {
POSITION = 15,
MASK = 1U << POSITION
};
enum States {
RTC_CLOCK_DISABLED = 0U << POSITION,
RTC_CLOCK_ENABLED = 1U << POSITION,
};
}  // namespace rtcen

namespace bdrst {
enum {
POSITION = 16,
MASK = 1U << POSITION
};
enum States {
BACKUP_DOMAIN_NOT_RESETTED = 0U << POSITION,
RESET_THE_BACKUP_DOMAIN = 1U << POSITION,
};
}  // namespace bdrst
}  // namespace bdcr

namespace csr {
enum {
#ifdef STM32F1XX
OFFSET = 0x24
#else // STM32F1XX
OFFSET = 0x74
#endif // STM32F1XX
};
namespace lsion {
enum {
POSITION = 0,
MASK = 1U << POSITION
};
enum States {
LSI_OSCILLATOR_OFF = 0U << POSITION,
LSI_OSCILLATOR_ON = 1U << POSITION,
};
}  // namespace lsion

namespace lsirdy {
enum {
POSITION = 1,
MASK = 1U << POSITION
};
enum States {
LSI_OSCILLATOR_NOT_READY = 0U << POSITION,
LSI_OSCILLATOR_READY = 1U << POSITION,
};
}  // namespace lsirdy

namespace rmvf {
enum {
POSITION = 24,
MASK = 1U << POSITION
};
enum States {
NO_EFFECT = 0U << POSITION,
CLEAR_THE_RESET_FLAGS = 1U << POSITION,
};
}  // namespace rmvf

namespace borrstf {
enum {
POSITION = 25,
MASK = 1U << POSITION
};
enum States {
NO_POR_PDR_BOR_RESET_OCCURRED = 0U << POSITION,
POR_PDR_BOR_RESET_OCCURRED = 1U << POSITION,
};
}  // namespace borrstf

namespace pinrstf {
enum {
POSITION = 26,
MASK = 1U << POSITION
};
enum States {
NO_NRST_PIN_RESET_OCCURRED = 0U << POSITION,
NRST_PIN_RESET_OCCURRED = 1U << POSITION,
};
}  // namespace pinrstf

namespace porrstf {
enum {
POSITION = 27,
MASK = 1U << POSITION
};
enum States {
NO_POR_PDR_RESET_OCCURRED = 0U << POSITION,
POR_PDR_RESET_OCCURRED = 1U << POSITION,
};
}  // namespace porrstf

namespace sftrstf {
enum {
POSITION = 28,
MASK = 1U << POSITION
};
enum States {
NO_SOFTWARE_RESET_OCCURRED = 0U << POSITION,
SOFTWARE_RESET_OCCURRED = 1U << POSITION,
};
}  // namespace sftrstf

namespace iwdgrstf {
enum {
POSITION = 29,
MASK = 1U << POSITION
};
enum States {
NO_WATCHDOG_RESET_OCCURRED = 0U << POSITION,
WATCHDOG_RESET_OCCURRED = 1U << POSITION,
};
}  // namespace iwdgrstf

namespace wwdgrstf {
enum {
POSITION = 30,
MASK = 1U << POSITION
};
enum States {
NO_WINDOW_WATCHDOG_RESET_OCCURRED = 0U << POSITION,
WINDOW_WATCHDOG_RESET_OCCURRED = 1U << POSITION,
};
}  // namespace wwdgrstf

namespace lpwrrstf {
enum {
POSITION = 31,
MASK = 1U << POSITION
};
enum States {
NO_LOW_POWER_MANAGEMENT_RESET_OCCURRED = 0U << POSITION,
LOW_POWER_MANAGEMENT_RESET_OCCURRED = 1U << POSITION,
};
}  // namespace lpwrrstf
}  // namespace csr

#ifdef CONNECTIVITY_LINE
namespace ahbrstr {
enum {
OFFSET = 0x28
};
enum Bits {
USB_OTG_FS = 1 << 12,
ETH_MAC = 1 << 14,
ALL = USB_OTG_FS + ETH_MAC,
};
}  // namespace ahbrstr
#endif // CONNECTIVITY_LINE
#if defined CONNECTIVITY_LINE || defined VALUE_LINE
namespace cfgr2 {
enum {
#ifdef CONNECTIVITY_LINE
OFFSET = 0x2C
#else // CONNECTIVITY_LINE
OFFSET = 0x2C
#endif // CONNECTIVITY_LINE
};
#ifdef CONNECTIVITY_LINE
namespace prediv1 {
enum {
POSITION = 0,
MASK = 0b1111U << POSITION
};
}  // namespace prediv1

namespace prediv2 {
enum {
POSITION = 4,
MASK = 0b1111U << POSITION
};
}  // namespace prediv2

namespace pll2mul {
enum {
POSITION = 8,
MASK = 0b1111U << POSITION
};
}  // namespace pll2mul

namespace pll3mul {
enum {
POSITION = 12,
MASK = 0b1111U << POSITION
};
}  // namespace pll3mul

namespace prediv1src {
enum {
POSITION = 16,
MASK = 1U << POSITION
};
enum States {
USE_HSE_OSCILLATOR_AS_PREDIV1_INPUT = 0U << POSITION,
USE_PLL2_AS_PREDIV1_INPUT = 1U << POSITION
};
}  // namespace prediv1src

namespace i2s2src {
enum {
POSITION = 17,
MASK = 1U << POSITION
};
enum States {
USE_SYSTEM_CLOCK_AS_I2S2_CLOCK = 0U << POSITION,
USE_PLL3_CLOCK_AS_I2S2_CLOCK = 1U << POSITION
};
}  // namespace i2s2src

namespace i2s3src {
enum {
POSITION = 18,
MASK = 1U << POSITION
};
enum States {
USE_SYSTEM_CLOCK_AS_I2S3_CLOCK = 0U << POSITION,
USE_PLL3_CLOCK_AS_I2S3_CLOCK = 1U << POSITION
};
}  // namespace i2s3src
#else // CONNECTIVITY_LINE
namespace prediv1 {
enum {
POSITION = 0,
MASK = 0b1111U << POSITION
};
}  // namespace prediv1
#endif // CONNECTIVITY_LINE
}  // namespace cfgr2
#endif // CONNECTIVITY_LINE || VALUE_LINE
#ifndef STM32F1XX
namespace ahb1rstr {
enum {
OFFSET = 0x10
};
enum Bits {
GPIOA = 1 << 0,
GPIOB = 1 << 1,
GPIOC = 1 << 2,
GPIOD = 1 << 3,
GPIOE = 1 << 4,
GPIOF = 1 << 5,
GPIOG = 1 << 6,
GPIOH = 1 << 7,
GPIOI = 1 << 8,
CRC = 1 << 12,
DMA1 = 1 << 21,
DMA2 = 1 << 22,
ETH_MAC = 1 << 25,
OTG_HS = 1 << 29,
ALL = GPIOA + GPIOB + GPIOC + GPIOD + GPIOE + GPIOF + GPIOG + GPIOH + GPIOI +
CRC + DMA1 + DMA2 + ETH_MAC + OTG_HS,
};
}  // namespace ahb1rstr

namespace ahb2rstr {
enum {
OFFSET = 0x14
};
enum Bits {
DCMI = 1 << 0,
CRYP = 1 << 4,
HASH = 1 << 5,
RNG = 1 << 6,
OTGFS = 1 << 7,
ALL = DCMI + CRYP + HASH + RNG + OTGFS,
};
}  // namespace ahb2rstr

namespace ahb3rstr {
enum {
OFFSET = 0x18
};
enum Bits {
FSMC = 1 << 0,
ALL = FSMC,
};
}  // namespace ahb3rstr

namespace ahb1enr {
enum {
OFFSET = 0x30
};
enum Bits {
GPIOA = 1 << 0,
GPIOB = 1 << 1,
GPIOC = 1 << 2,
GPIOD = 1 << 3,
GPIOE = 1 << 4,
GPIOF = 1 << 5,
GPIOG = 1 << 6,
GPIOH = 1 << 7,
GPIOI = 1 << 8,
CRC = 1 << 12,
BKPSRAM = 1 << 18,
#ifdef STM32F4XX
CCMDATARAM = 1 << 20,
#endif // STM32F4XX
DMA1 = 1 << 21,
DMA2 = 1 << 22,
ETH_MAC = 1 << 25,
ETH_MAC_TX = 1 << 26,
ETH_MAC_RX = 1 << 27,
ETH_MAC_PTP = 1 << 28,
OTG_HS = 1 << 29,
OTG_HS_ULPI = 1 << 30,
};
}  // namespace ahb1enr

namespace ahb2enr {
enum {
OFFSET = 0x34
};
enum Bits {
DCMI = 1 << 0,
CRYP = 1 << 4,
HASH = 1 << 5,
RNG = 1 << 6,
USB_OTG_FS = 1 << 7,
};
}  // namespace ahb2enr

namespace ahb3enr {
enum {
OFFSET = 0x38
};
enum Bits {
FSMC = 1 << 0,
};
}  // namespace ahb3enr

namespace ahb1lpenr {
enum {
OFFSET = 0x50
};
}  // namespace ahb1lpenr

namespace ahb2lpenr {
enum {
OFFSET = 0x54
};
}  // namespace ahb2lpenr

namespace ahb3lpenr {
enum {
OFFSET = 0x58
};
}  // namespace ahb3lpenr

namespace apb1lpenr {
enum {
OFFSET = 0x60
};
}  // namespace apb1lpenr

namespace apb2lpenr {
enum {
OFFSET = 0x64
};
}  // namespace apb2lpenr

namespace sscgr {
enum {
OFFSET = 0x80
};
  // TODO RCC SSCGR bits
}  // namespace sscgr

namespace plli2scfgr {
enum {
OFFSET = 0x84
};
namespace plli2sn {
enum {
POSITION = 6,
MASK = 0b111111111U << POSITION
};
}  // namespace plli2sn

namespace plli2sr {
enum {
POSITION = 28,
MASK = 0b111U << POSITION
};
}  // namespace plli2sr
}  // namespace plli2scfgr
#endif // !STM32F1XX
}
  // namespace rcc
