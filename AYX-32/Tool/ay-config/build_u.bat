@echo off

set path=%path%;res;c:\Tools\PROG\SDCC\bin\

if not exist obj md obj
del /q /s obj >nul

sdasz80 -o obj/crt0.rel src/crt0.s
sdcc -mz80 --std-sdcc11 --no-std-crt0 --opt-code-speed -Wl-b_HEADER=0x6000 -Wl-b_HOME=0x6010 -Wl-b_CODE=0x6200 -Wl-b_DATA=0x5B00 obj/crt0.rel src/main.c -o obj/out.hex
hex2bin obj/out.hex >nul
ren obj\out.bin code.C

trdtool # ayx32_fw.trd >nul
trdtool + ayx32_fw.trd res/boot.$b >nul
trdtool + ayx32_fw.trd obj/code.C >nul

unreal ayx32_fw.trd
