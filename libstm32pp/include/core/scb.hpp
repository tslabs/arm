
/*******************************************************************************
 *
 *                             System Control Block
 *
 ******************************************************************************/

#pragma once

#include <device_select.hpp>

#include "../defs.hpp"
#include "../../memorymap/scb.hpp"

// Low-level access to the registers
#define SCB_REGS reinterpret_cast<scb::Registers*>(scb::ADDRESS)

// High-level functions
namespace scb
{
  class Functions
  {
    public:
      template<scb::irqn::E I, u8 P>
      static inline void setPriority();

    private:
      Functions();
  };
}

// High-level access to the perihperal
typedef scb::Functions SCB;

#include "../../bits/scb.tcc"
