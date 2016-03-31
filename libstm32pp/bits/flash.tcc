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

namespace flash
{

#ifndef VALUE_LINE
  void Functions::setLatency(acr::latency::States LATENCY)
  {
    FLASH_REGS->ACR &= ~acr::latency::MASK;
    FLASH_REGS->ACR |= LATENCY;
  }
#endif // !VALUE_LINE

#ifdef STM32F1XX
#ifndef VALUE_LINE

  /**
   * @brief Configures the flash memory access.
   * @note  Overrides the old configuration.
   */
  void Functions::configure(
      acr::latency::States LATENCY,
      acr::hlfcya::States HLFCYA,
      acr::prftbe::States PRFTBE)
  {
    FLASH_REGS->ACR = LATENCY + HLFCYA + PRFTBE;
  }

  /**
   * @brief Enables the prefetch buffer.
   */
  void Functions::enablePrefetch()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + acr::OFFSET, acr::prftbe::POSITION>()) = 1;
  }

  /**
   * @brief Disables the prefetch buffer.
   */
  void Functions::disablePrefetch()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + acr::OFFSET, acr::prftbe::POSITION>()) = 0;
  }

  /**
   * @brief Erase whole Flash.
   */
  void Functions::eraseFlash()
  {
    FLASH_REGS->CR = cr::mer::MASS_ERASE_ACTIVATED;
    *(u32 volatile*)(bitband::peripheral<ADDRESS + cr::OFFSET, cr::strt::POSITION>()) = 1;
  }

  /**
   * @brief Erase page.
   */
  void Functions::erasePage(u32 addr)
  {
    FLASH_REGS->CR = cr::per::PAGE_ERASE_ACTIVATED;
    FLASH_REGS->AR = addr;
    *(u32 volatile*)(bitband::peripheral<ADDRESS + cr::OFFSET, cr::strt::POSITION>()) = 1;
  }

  /**
   * @brief Erase Option Bytes.
   */
  void Functions::eraseOptionBytes()
  {
    FLASH_REGS->CR = cr::opter::OPTION_BYTE_ERASE_ACTIVATED;
    *(u32 volatile*)(bitband::peripheral<ADDRESS + cr::OFFSET, cr::strt::POSITION>()) = 1;
  }

  /**
   * @brief Program Flash.
   */
  void Functions::startProgram()
  {
    FLASH_REGS->CR = cr::pg::FLASH_PROGRAMMING_ACTIVATED;
  }

  /**
   * @brief Program Option Bytes.
   */
  void Functions::startProgramOptionBytes()
  {
    FLASH_REGS->CR = cr::optpg::OPTION_BYTE_PROGRAMMING_ACTIVATED;
  }

#else // !VALUE_LINE

  /**
   * @brief Configures the flash memory access.
   * @note  Overrides the old configuration.
   */
  void Functions::configure(acr::hlfcya::States HLFCYA)
  {
    FLASH_REGS->ACR = HLFCYA;
  }

#endif // !VALUE_LINE

  /**
   * @brief Enables the half cycle flash access.
   */
  void Functions::enableHalfCycleFlashAccess()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + acr::OFFSET, acr::hlfcya::POSITION>()) = 1;
  }

  /**
   * @brief Disables the half cycle flash access.
   */
  void Functions::disableHalfCycleFlashAccess()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + acr::OFFSET, acr::hlfcya::POSITION>()) = 0;
  }

#else // STM32F1XX

  /**
   * @brief Configures the flash memory access.
   * @note  Overrides the old configuration.
   */
  void Functions::configure(
      acr::latency::States LATENCY,
      acr::prften::States PRFTEN,
      acr::dcen::States DCEN,
      acr::icen::States ICEN)
  {
    FLASH_REGS->ACR = LATENCY + PRFTEN + DCEN + ICEN;
  }

  /**
   * @brief Enables the prefetch buffer.
   */
  void Functions::enablePrefetch()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + acr::OFFSET, acr::prften::POSITION>()) = 1;
  }

  /**
   * @brief Disables the prefetch buffer.
   */
  void Functions::disablePrefetch()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + acr::OFFSET, acr::prften::POSITION>()) = 0;
  }

  /**
   * @brief Enables the data cache.
   */
  void Functions::enableDataCache()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + acr::OFFSET, acr::dcen::POSITION>()) = 1;
  }

  /**
   * @brief Disables the data cache.
   */
  void Functions::disableDataCache()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + acr::OFFSET, acr::dcen::POSITION>()) = 0;
  }

  /**
   * @brief Resets the data cache.
   */
  void Functions::resetDataCache()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + acr::OFFSET, acr::dcrst::POSITION>()) = 1;
  }

  /**
   * @brief Enables the instruction cache.
   */
  void Functions::enableInstructionCache()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + acr::OFFSET, acr::icen::POSITION>()) = 1;
  }

  /**
   * @brief Disables the instruction cache.
   */
  void Functions::disableInstructionCache()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + acr::OFFSET, acr::icen::POSITION>()) = 0;
  }

  /**
   * @brief Resets the instruction cache.
   */
  void Functions::resetInstructionCache()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + acr::OFFSET, acr::icrst::POSITION>()) = 1;
  }

  /**
   * @brief Erase whole Flash.
   */
  void Functions::eraseFlash()
  {
    FLASH_REGS->CR = cr::psize::PROGRAM_X32 + cr::mer::MASS_ERASE_ACTIVATED;
    *(u32 volatile*)(bitband::peripheral<ADDRESS + cr::OFFSET, cr::strt::POSITION>()) = 1;
  }

  /**
   * @brief Erase sector.
   */
  void Functions::eraseSector(cr::snb::States SEC)
  {
    FLASH_REGS->CR = cr::psize::PROGRAM_X32 + SEC + cr::ser::SECTOR_ERASE_ACTIVATED;
    *(u32 volatile*)(bitband::peripheral<ADDRESS + cr::OFFSET, cr::strt::POSITION>()) = 1;
  }

  /**
   * @brief Program Flash.
   */
  void Functions::startProgram()
  {
    FLASH_REGS->CR = cr::psize::PROGRAM_X32 + cr::pg::FLASH_PROGRAMMING_ACTIVATED;
  }
#endif // STM32F1XX

  /**
   * @brief Returns Flash Busy state.
   */
  bool Functions::isBusy()
  {
    return *(u32 volatile*)(bitband::peripheral<ADDRESS + sr::OFFSET, sr::bsy::POSITION>());
  }

  /**
   * @brief Wait while Flash Busy.
   */
  void Functions::waitBusy()
  {
    while (*(u32 volatile*)(bitband::peripheral<ADDRESS + sr::OFFSET, sr::bsy::POSITION>()));
  }

  /**
   * @brief Unlocks Flash.
   */
  void Functions::unlock()
  {
    FLASH_REGS->KEYR = keyr::KEY1;
    FLASH_REGS->KEYR = keyr::KEY2;
  }

  /**
   * @brief Locks Flash.
   */
  void Functions::lock()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + cr::OFFSET, cr::lock::POSITION>()) = 1;
  }
  
  /**
   * @brief Unlocks Option Bytes.
   */
  void Functions::unlockOptionBytes()
  {
    FLASH_REGS->OPTKEYR = optkeyr::OPTKEY1;
    FLASH_REGS->OPTKEYR = optkeyr::OPTKEY2;
  }
  
  /**
   * @brief Locks Option Bytes.
   */
  void Functions::lockOptionBytes()
  {
    *(u32 volatile*)(bitband::peripheral<ADDRESS + cr::OFFSET, cr::optwre::POSITION>()) = 0;
  }
}  // namespace flash
