@echo off
fw_tool obj/main.bin
copy /b *.fw fw.bin
xxd -i fw.bin >fw.h
del fw.bin
del *.fw
