
  .globl  _main
  .area _HEADER (ABS)
init:
  di
  ld sp, #0x6000
  jp _main
