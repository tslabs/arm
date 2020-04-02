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

/*******************************************************************************
 *
 *                          Cryptographic Processor
 *
 ******************************************************************************/

#pragma once

#include <device_select.hpp>

#ifndef STM32F1XX

#include "../defs.hpp"
#include "../../memorymap/cryp.hpp"

// Low-level access to the registers
#define CRYP_REGS reinterpret_cast<cryp::Registers *>(cryp::ADDRESS)

// High-level functions
namespace cryp {
  class Functions {
    public:
      static inline void enableClock();
      static inline void disableClock();

    private:
      Functions();
  };
}  // namespace cryp

// High-level access to the peripheral
typedef cryp::Functions CRYP;

#include "../../bits/cryp.tcc"

#endif
