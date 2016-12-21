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
 *                               FLASH Registers
 *
 ******************************************************************************/

#pragma once

#include <device_select.hpp>

#include "../defs.hpp"
#include "../../memorymap/flash.hpp"

// Low-level access to the registers
#define FLASH_REGS reinterpret_cast<flash::Registers*>(flash::ADDRESS)

// High-level functions
namespace flash {
  class Functions {
    public:
#ifndef VALUE_LINE
      static inline void setLatency(acr::latency::States);
#endif // VALUE_LINE
#ifdef STM32F1XX
#ifndef VALUE_LINE
      static inline void enablePrefetch();
      static inline void disablePrefetch();
#endif // !VALUE_LINE
      static inline void enableHalfCycleFlashAccess();
      static inline void disableHalfCycleFlashAccess();
#ifdef VALUE_LINE
      static inline void configure(acr::hlfcya::States);
#else // VALUE_LINE
      static inline void configure(
        acr::latency::States,
        acr::hlfcya::States,
        acr::prftbe::States);
#endif // VALUE_LINE
#else // STM32F1XX
      static inline void enablePrefetch();
      static inline void disablePrefetch();
      static inline void enableDataCache();
      static inline void disableDataCache();
      static inline void resetDataCache();
      static inline void enableInstructionCache();
      static inline void disableInstructionCache();
      static inline void resetInstructionCache();

      static inline void configure(
        acr::latency::States,
        acr::prften::States,
        acr::dcen::States,
        acr::icen::States);
#endif // STM32F1XX

      static inline bool isBusy();
      static inline void waitBusy();

      static inline void unlock();
      static inline void lock();
      static inline void eraseFlash();
#ifdef STM32F1XX
      static inline void erasePage(u32);
      static inline void unlockOptionBytes();
      static inline void lockOptionBytes();
      static inline void eraseOptionBytes();
      static inline void startProgram();
      static inline void startProgramOptionBytes();
#else // STM32F1XX
      static inline void eraseSector(cr::snb::States);
      static inline void startProgram(cr::psize::States);
#endif // STM32F1XX

    private:
      Functions();
  };
}  // namespace flash

// High-level access to the peripheral
typedef flash::Functions FLASH;

#include "../../bits/flash.tcc"
