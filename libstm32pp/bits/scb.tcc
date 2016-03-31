
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

  // execute a system reset
  // refer to: Cortex-M3 Technical Reference Manual_(DDI0337E_cortex_m3_r1p1_trm).pdf, p.166
  void Functions::generateReset()
  {
    SCB_REGS->AIRCR = (0x5FA << 16) | 2 | 1;
  }

  // Vector Table Offset must be aligned to 128 bytes
  void Functions::setVectorTableOffset(u32 ofs)
  {
    SCB_REGS->VTOR = ofs;
  }
}
