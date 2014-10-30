@cls

@set srcs=-x c++ led_wheel.cpp ../include/interrupt_cpp.hpp ../startup/exception.cpp ../startup/startup.cpp
@set targ=-mthumb -mcpu=cortex-m4
@set incl=-I ../include -I ../bits
@set defs=
@set opts=-Os -flto -gdwarf-2 -std=c++11 -nostdlib
@set link=-L ../ldscript -T stm32f407vg_sram.ld -Wl,--gc-sections,-Map=main.map
@set outp=-o main.elf

@arm-none-eabi-g++ %srcs% %targ% %incl% %defs% %opts% %link% %outp%
@arm-none-eabi-objdump -d main.elf >main.lst
@arm-none-eabi-objcopy.exe -j .text -j .data -O binary main.elf main.bin

pause
