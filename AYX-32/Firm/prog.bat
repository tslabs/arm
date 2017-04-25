@echo off
fw_tool obj/main.bin
copy /b *.fw fw.bin
xxd -i fw.bin >fw.h
del fw.bin
fw_uart.exe ayx32_1_1.fw com4
pause
