@echo off
cls

if exist obj rd /s /q obj
md obj

set srcs=-x c++ main.cpp ../../libstm32pp/include/interrupt_cpp.hpp
set targ=-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
set incl=-I ../../libstm32pp/include -I common -I ay
set defs=
set opts=-Os -std=c++11 -nostartfiles -nostdlib -flto -ffunction-sections -fdata-sections -fpermissive
set link=-L ../../libstm32pp/ldscript -T stm32f407vg_mapram_ccmram.ld -Wl,--gc-sections,-Map=obj/main.map
REM set link=-L ../../libstm32pp/ldscript -T stm32f407vg_flash_ccmram.ld -Wl,--gc-sections,-Map=obj/main.map
set outp=-o obj/main.elf

arm-none-eabi-c++ %srcs% %targ% %incl% %defs% %opts% -gdwarf-4 %link% %outp%
arm-none-eabi-objdump -d -S obj/main.elf >obj/main.lst

arm-none-eabi-c++ %srcs% %targ% %incl% %defs% %opts% %link% %outp%
arm-none-eabi-objcopy -j .text -j .data -j .ARM.exidx -O binary obj/main.elf obj/main.bin
arm-none-eabi-size obj/main.elf
