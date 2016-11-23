@echo off

"%IAR%\az80.exe" drawc.asm
"%IAR%\iccz80.exe" -ml -uua -s9 -P -xDFT -T -i -q -K -I"%IAR%/../inc/" main.c -L
"%IAR%\xlink.exe" -f main.xcl -l main.html -o main.bin main.r01 drawc.r01 "%IAR%\..\lib\clz80.r01"
REM "%IAR%\xlink.exe" -f main.xcl -l main.html -o main.hex main.r01 drawc.r01 "%IAR%\..\lib\clz80.r01"
REM objcopy -I ihex -O binary -j.sec2 main.hex main.bin
del *.r01

pause
