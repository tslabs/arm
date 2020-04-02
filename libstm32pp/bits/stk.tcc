
#pragma once

namespace stk
{
  template<
    u32 RELOAD_VALUE,
    stk::cr::clksource::States CS,
    stk::cr::tickint::States TI,
    stk::cr::enable::States EN>
  void Functions::configure()
  {
    enum 
    {
      RLD = ((CS ? clk::SYSTEM : (clk::AHB / 8)) * (u64)RELOAD_VALUE) / 1000000 - 1
    };
    
    static_assert(RLD < (1 << 24), "Period is too long for selected clock!");

    STK_REGS->CTRL = 0;
    STK_REGS->RELOAD = RLD;
    STK_REGS->CTRL = CS + TI + EN;
  }
} // namespace stk
