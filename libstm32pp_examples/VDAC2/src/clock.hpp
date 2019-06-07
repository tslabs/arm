
#include "device_select.hpp"
#include "peripheral/rcc.hpp"
#include "peripheral/flash.hpp"

#pragma once

namespace clk 
{
  enum 
  {
    SYSTEM      = 72000000,
    AHB         = 72000000,
    APB1        = 36000000, 
    APB2        = 72000000, 
    APB1_TIMERS = 72000000, 
    APB2_TIMERS = 72000000, 
  };
}
