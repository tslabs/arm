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
 *                              Servo motor driver
 *
 ******************************************************************************/

#pragma once

#include <device_select.hpp>
#include "../defs.hpp"
#include "../peripheral/tim.hpp"

// High-level functions
namespace servo {
  /**
   * This class implements a servo controller that can handle <N> servos at the
   * same time with a resolution of 1us.
   *
   * The controller will take over <N> pins for the control, the bit-band
   * address of these pins are stores in <pin[]>.
   *
   * The controller will generate digital signals with an ON time of:
   *
   * (<MidValue> + value[i]) us
   *
   * for each controller <pin[i]> at a rate of <Frequency> Hz
   *
   * This controller uses two timers:
   * + PeriodTimer: Whose address is <PeriodTimerAddress>
   * + DutyCycleTimer: Whose address is <DutyCycleTimerAddress>
   *
   * The user must configure every <pin[i]> as output, and must also enable
   * both timer's interrupts.
   *
   * Additionally the user must call the functions:
   *
   * + onPeriodTimerInterrupt
   * + onDutyCycleTimerInterrupt
   *
   * on their corresponding interrupts.
   *
   * See the demo folder for an example.
   *
   */
  template<
      tim::Address PeriodTimerAddress,
      u32 Frequency,
      tim::Address DutyCycleTimerAddress,
      u16 MidValue,
      u8 N
  >
  class Functions {
    public:
      Functions();

      typedef tim::Functions<PeriodTimerAddress> PeriodTimer;
      typedef tim::Functions<DutyCycleTimerAddress> DutyCycleTimer;

      inline void initialize();
      inline void setPin(u8 const, u32* const);
      inline void start();
      inline void stop();
      inline bool isActive();
      inline void load(s16 const (&)[N]);
      inline void onPeriodTimerInterrupt();
      inline void onDutyCycleTimerInterrupt();

    private:
      s16 buffer[N];
      u32* pin[N];
      s16 value[N];
      u8 sortedIndices[N];
      u8 servoIndex;
  };
}  // namespace servo

#include "../../bits/servo.tcc"
