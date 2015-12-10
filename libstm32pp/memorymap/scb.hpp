
#pragma once

namespace scb
{
  enum
  {
    ADDRESS = alias::PPB + 0xD00
  };

  struct Registers
  {
      __RW u32 CPUID;   // 0x00: CPU ID
      __RW u32 ICSR;    // 0x04: Interrupt control and state
      __RW u32 VTOR;    // 0x08: Vector table offset
      __RW u32 AIRCR;   // 0x0C: Application interrupt and reset control
      __RW u32 SCR;     // 0x10: System control
      __RW u32 CCR;     // 0x14: Configuration and control
      __RW u32 SHPR[3]; // 0x18..20: System handler priority
      __RW u32 SHCSR;   // 0x24: System handler control and state
      __RW u32 CFSR;    // 0x28: Configurable fault status
      __RW u32 HFSR;    // 0x2C: Hard fault status
      __RW u32 DFSR;    // 0x30: Debug Fault Status Register
      __RW u32 MMFAR;   // 0x34: Memory management fault address
      __RW u32 BFAR;    // 0x38: Bus fault address
      __RW u32 AFSR;    // 0x3C: Auxiliary fault status
  };

  namespace irqn
  {
    enum
    {
      MASK = 0xFF
    };
    
    enum E 
    {
      MemManage  = 0,
      BusFault   = 1,
      UsageFault = 2,
      SVCall     = 7,
      DebugMon   = 8,
      PendSV     = 10,
      SysTick    = 11
    };
  };
}// namespace scb
