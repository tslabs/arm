@echo off

set path=%PATH%;c:\Tools\PROG\ARM-GCC\bin

if exist obj rd /s /q obj
md obj

set targ=-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
set incl=-I ../../libstm32pp/include -I common
set opts=-Os -std=c++11 -nostartfiles -nostdlib -nodefaultlibs -fno-exceptions -flto -fpermissive

set srcs=-x c++ boot.cpp
set defs=
set link=-T boot.ld -Wl,--gc-sections
set outp=-o obj/boot.elf

arm-none-eabi-c++ %srcs% %targ% %incl% %defs% %opts% -gdwarf-4 %link% %outp%
if errorlevel 1 goto exit
arm-none-eabi-objdump -x -S obj/boot.elf >obj/boot.lst
arm-none-eabi-g++ -E -P %srcs% %targ% %incl% %defs% %opts% %link% >obj/boot_pp.cpp

arm-none-eabi-c++ %srcs% %targ% %incl% %defs% %opts% %link% %outp%
arm-none-eabi-objdump -s obj/boot.elf >obj/boot.sct
arm-none-eabi-objcopy -j .text -j .data -j .txram -O binary obj/boot.elf obj/boot.bin
arm-none-eabi-size obj/boot.elf
xxd -i obj/boot.bin obj/boot.h

set srcs=-x c++ main.cpp
set defs=
set link=-T main.ld -Wl,--gc-sections
set outp=-o obj/main.elf

arm-none-eabi-c++ %srcs% %targ% %incl% %defs% %opts% -gdwarf-4 %link% %outp%
if errorlevel 1 goto exit
arm-none-eabi-objdump -x -S obj/main.elf >obj/main.lst
arm-none-eabi-g++ -E -P %srcs% %targ% %incl% %defs% %opts% %link% >obj/main_pp.cpp

arm-none-eabi-c++ %srcs% %targ% %incl% %defs% %opts% %link% %outp%
arm-none-eabi-objdump -s obj/main.elf >obj/main.sct
arm-none-eabi-objcopy -j .boot -j .text -j .data -j .txram -O binary obj/main.elf obj/main.bin
arm-none-eabi-size obj/main.elf

:exit
