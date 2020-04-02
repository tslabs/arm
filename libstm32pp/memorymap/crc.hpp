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

namespace crc {
  struct Registers {
      __RW
      u32 DR;  // 0x00: Data
      __RW
      u32 IDR;  // 0x04: Independent data
      __RW
      u32 CR;  // 0x08: Control
  };

#ifdef STM32F1XX
  enum {
    ADDRESS = alias::AHB + 0x3000
  };
#else
  enum {
    ADDRESS = alias::AHB1 + 0x3000
  };
#endif

  namespace dr {
    enum {
      OFFSET = 0x00
    };
  }  // namespace dr

  namespace idr {
    enum {
      OFFSET = 0x04
    };
  }  // namespace idr

  namespace cr {
    enum {
      OFFSET = 0x08
    };
  }  // namespace cr
}  // namespace crc
