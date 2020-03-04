@echo off

set path=%PATH%;c:\Tools\PROG\ARM-GCC\bin

if exist obj rd /s /q obj
md obj
if exist bin rd /s /q bin
md bin

set targ=-mthumb -mcpu=cortex-m3
set incl=-I ../../libstm32pp/include -I src
set opts=-Os -std=c++11 -nostartfiles -nostdlib -nodefaultlibs -fno-exceptions -flto -fpermissive

set srcs=-x c++ src/main.cpp ../../libstm32pp/include/interrupt.cpp
set defs=
set link=-T main.ld -Wl,--gc-sections,-Map=obj/main.map
set outp=-o obj/main.elf

arm-none-eabi-c++ %srcs% %targ% %incl% %defs% %opts% -gdwarf-4 %link% %outp%
if errorlevel 1 goto exit
set defs=%defs%
arm-none-eabi-objdump -d -S obj/main.elf >obj/main.lst
arm-none-eabi-c++ -E -P %srcs% %targ% %incl% %defs% %opts% %link% >obj/main_pp.cpp

arm-none-eabi-c++ %srcs% %targ% %incl% %defs% %opts% %link% %outp%
if errorlevel 1 goto exit
arm-none-eabi-objcopy -j .boot -j .text -j .data -j .ARM.exidx -O binary obj/main.elf bin/main.bin
arm-none-eabi-size obj/main.elf

:exit
