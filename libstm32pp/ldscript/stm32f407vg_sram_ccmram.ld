/*******************************************************************************
 *
 * Copyright (C) 2012 Jorge Aparicio <jorge.aparicio.r@gmail.com>
 *
 * This file is part of bareCortexM.
 *
 * bareCortexM is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * bareCortexM is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with bareCortexM. If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

MEMORY
{
  FLASH  (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
  MAPRAM (rwx) : ORIGIN = 0x00000000, LENGTH = 128K
  SRAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
  SRAM1  (rwx) : ORIGIN = 0x20000000, LENGTH = 112K
  SRAM2  (rwx) : ORIGIN = 0x2001C000, LENGTH = 16K
  CCMRAM (rwx) : ORIGIN = 0x10000000, LENGTH = 64K
}

/* Config Libraries */
GROUP(libgcc.a libc.a libm.a libnosys.a)

ENTRY(resetHandler)

SECTIONS
{
  .text :
  {
    KEEP(*(.exception_vector))
    KEEP(*(.interrupt_vector))
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
  } > SRAM

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > SRAM

  __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > SRAM
  __exidx_end = .;

  __etext = .;

  .data : AT (__etext)
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
  
  .sram :
  {
    . = ALIGN(4);
    _ssram = .;
    *(.sram)
    . = ALIGN(4);     
    _esram = .;
  } > SRAM

  .sram1 :
  {
    . = ALIGN(4);
    _ssram1 = .;
    *(.sram1)
    . = ALIGN(4);     
    _esram1 = .;
  } > SRAM1

  .sram2 :
  {
    . = ALIGN(4);
    _ssram2 = .;
    *(.sram2)
    . = ALIGN(4);     
    _esram2 = .;
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