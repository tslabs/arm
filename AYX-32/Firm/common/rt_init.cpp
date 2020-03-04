
extern u32 __data__;
extern u32 __data_start__;
extern u32 __data_end__;
extern u32 __bss_start__;
extern u32 __bss_end__;
extern u32 __txram__;
extern u32 __txram_start__;
extern u32 __txram_end__;

void rt_init()
{
  SYSCFG_REGS->MEMRMP = 3;   // SRAM to 0x00000000
  
  // Copy initialized data from FLASH to RAM
  u32 *src = &__data__;
  u32 *dst = &__data_start__;

  while (dst < &__data_end__)
    *dst++ = *src++;

  // Clear uninitialized data
  dst = &__bss_start__;

  while (dst < &__bss_end__)
    *dst++ = 0;

  // Copy code from FLASH to RAM
  src = &__txram__;
  dst = &__txram_start__;

  while (dst < &__txram_end__)
    *dst++ = *src++;
}
