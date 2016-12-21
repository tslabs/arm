@echo off

set path=%path%;c:\Tools\PROG\STM32 ST-LINK Utility\ST-LINK Utility

ST-LINK_CLI.exe -p obj/main.bin 0x8000000 -rst
