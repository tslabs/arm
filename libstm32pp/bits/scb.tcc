
#pragma once

namespace scb 
{
  template<scb::irqn::E I, u8 P>
  void Functions::setPriority()
  {
    static_assert(P < 16, "The priority must be 0-15.");

    u32 s = SCB_REGS->SHPR[I >> 2];
    s &= ~(irqn::MASK << (8 * (I % 4)));
    s |= P << ((8 * (I % 4)) + 4);
    SCB_REGS->SHPR[I >> 2] = s;
  }
}
