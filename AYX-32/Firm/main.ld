
MEMORY
{
  BOOT   (rx)  : ORIGIN = 0x08000000, LENGTH = 16K
  LUT    (rx)  : ORIGIN = 0x08004000, LENGTH = 16K
  FLASH  (rx)  : ORIGIN = 0x08008000, LENGTH = 992K
  MAPRAM (rwx) : ORIGIN = 0x00000000, LENGTH = 112K
  SRAM2  (rwx) : ORIGIN = 0x2001C000, LENGTH = 16K
  CCMRAM (rwx) : ORIGIN = 0x10000000, LENGTH = 64K
}

/* Config Libraries */
GROUP(libgcc.a libc.a libm.a libnosys.a)

ENTRY(resetHandler)

SECTIONS
{
  .boot :
  {
    KEEP(*(.boot*))
  } > BOOT

  .text :
  {
    __main_vect = .;
    KEEP(*(.irq_vect))
    *(.text*)

    KEEP(*(.init))
    KEEP(*(.fini))

    /* .ctors */
    *crtbegin.o(.ctors)
    *crtbegin?.o(.ctors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
    *(SORT(.ctors.*))
    *(.ctors)

    /* .dtors */
     *crtbegin.o(.dtors)
     *crtbegin?.o(.dtors)
     *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
     *(SORT(.dtors.*))
     *(.dtors)

    *(.rodata*)

    KEEP(*(.eh_frame*))
  } > FLASH

  . = ALIGN(4);
  __txram__ = .;

  .irqram :
  {
    KEEP(*(.ram_irq_vect))
    __irqram_end__ = .;
  } > MAPRAM

  .txram : AT (__txram__)
  {
    __txram_start__ = .;
    *(.ram_text*)
    *(.ram_rodata*)
    __txram_end__ = .;
  } > MAPRAM

  . = __txram_end__ - __txram_start__ + __txram__;
  . = ALIGN(4);
  __data__ = .;

  .data : AT (__data__)
  {
    __data_start__ = .;
    *(vtable)
    *(.data*)

    . = ALIGN(4);
    /* preinit data */
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP(*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);

    . = ALIGN(4);
    /* init data */
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);


    . = ALIGN(4);
    /* finit data */
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);

    . = ALIGN(4);
    /* All data end */
    __data_end__ = .;

  } > CCMRAM

  .bss :
  {
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    __bss_end__ = .;
  } > CCMRAM

  .heap :
  {
    __end__ = .;
    end = __end__;
    *(.heap*)
    __HeapLimit = .;
  } > CCMRAM

  __sram1_start__ = __txram_end__ - __txram_start__ + __irqram_end__;
  
  .sram1 : AT (__sram1_start__)
  {
    . = ALIGN(4);
    *(.sram1)
  } > MAPRAM

  .sram2 :
  {
    . = ALIGN(4);
    *(.sram2)
  } > SRAM2

  /* .stack_dummy section doesn't contains any symbols. It is only
   * used for linker to calculate size of stack sections, and assign
   * values to stack symbols later */
  .stack_dummy :
  {
    *(.stack*)
  } > CCMRAM

  /* Set stack top to end of RAM, and stack limit move down by
   * size of stack_dummy section */
  __StackTop = ORIGIN(CCMRAM) + LENGTH(CCMRAM);
  __StackLimit = __StackTop - SIZEOF(.stack_dummy);
  PROVIDE(__stack = __StackTop);

  /* Check if data + heap + stack exceeds RAM limit */
  ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}