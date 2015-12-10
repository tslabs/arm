
#pragma once

#include <device_select.hpp>
#include "../defs.hpp"

#include <clock.hpp>
#include "../../memorymap/stk.hpp"

// Low-level access to the registers
#define STK_REGS reinterpret_cast<stk::Registers*>(stk::ADDRESS)

// High-level functions
namespace stk
{
  class Functions
  {
    public:
      template<u32 RELOAD_VALUE,
        stk::cr::clksource::States,
        stk::cr::tickint::States,
        stk::cr::enable::States>
      static inline void configure();

    private:
      Functions();
  };
} // namespace stk

// High-level access to the peripheral
// TODO STK high-level access

typedef stk::Functions SYSTICK;

#include "../../bits/stk.tcc"
