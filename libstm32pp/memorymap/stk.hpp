
#pragma once

#include "common.hpp"

namespace stk
{
  enum
  {
    ADDRESS = alias::PPB + 0x10
  };

  struct Registers
  {
      __RW u32 CTRL;    // 0x00: Control and status
      __RW u32 RELOAD;  // 0x04: Reload value
      __RW u32 VAL;     // 0x08: Current value
      __RW u32 CALIB;   // 0x0C: Calibration value
  };

  namespace cr
  {
    enum 
    {
      OFFSET = 0x00
    };

    namespace enable
    {
      enum
      {
        POSITION = 0,
        MASK = 0b1 << POSITION
      };

      enum States
      {
        SYSTICK_DISABLED = 0 << POSITION,
        SYSTICK_ENABLED = 1 << POSITION
      };
    }  // namespace enable

    namespace tickint
    {
      enum
      {
        POSITION = 1,
        MASK = 0b1 << POSITION
      };

      enum States
      {
        TICKINT_DISABLED = 0 << POSITION,
        TICKINT_ENABLED = 1 << POSITION
      };
    }  // namespace tickint

    namespace clksource
    {
      enum
      {
        POSITION = 2,
        MASK = 0b1 << POSITION
      };

      enum States
      {
        EXTERNAL_CLOCK = 0 << POSITION,
        CORE_CLOCK = 1 << POSITION
      };
    }  // namespace clksource

    namespace countflag
    {
      enum
      {
        POSITION = 16,
        MASK = 0b1 << POSITION
      };

      enum States
      {
        ZERO_REACHED = 1 << POSITION
      };
    }  // namespace countflag
  }; // namespace cr
} // namespace stk
