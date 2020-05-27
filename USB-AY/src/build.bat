@echo off

set OPENCM3_DIR=d:\ARM\libopencm3
make clean all
c:\Tools\PROG\ARM-GCC\bin\arm-none-eabi-objcopy -j .text -j .data -j .ARM.exidx -O binary usb-ay.elf usb-ay.bin
arm-none-eabi-objdump -d -S usb-ay.elf >usb-ay.lst

if errorlevel 1 pause