# If the GCC ARM tools are already on the path, use them. Otherwise, use
# the local version in gcc-arm/bin
GCC_ARM_VERSION := $(shell arm-none-eabi-gcc --version 2>/dev/null)
ifdef GCC_ARM_VERSION
    GCCDIR=
else
    GCCDIR=gcc-arm/bin/
endif

# Treat NT and non-NT windows the same
ifeq ($(OS),Windows_NT)
    OS = Windows
endif

ifeq ($(OS),Windows)
    SERIAL_PORT = com8:
    EXE = .exe
    ifeq ($(OSTYPE),cygwin)
        RM = rm
    else
        RM = del
    endif
else # assume Linux/Unix
    SERIAL_PORT = /dev/ttyUSB0
    EXE =
    RM = rm -f
endif

TOOLS_PATH = ../Tool

STLINK = st-link_cli
OPENOCD = openocd
FW_UART = $(TOOLS_PATH)/fw_uart/fw_uart$(EXE)
FW_TOOL = $(TOOLS_PATH)/fw_tool/fw_tool$(EXE)
BIN2C = $(TOOLS_PATH)/bin2c/bin2c$(EXE)

GPP = $(GCCDIR)arm-none-eabi-g++
CPP = $(GCCDIR)arm-none-eabi-c++
AR = $(GCCDIR)arm-none-eabi-ar
SIZE = $(GCCDIR)arm-none-eabi-size
OBJCOPY = $(GCCDIR)arm-none-eabi-objcopy
OBJDUMP = $(GCCDIR)arm-none-eabi-objdump
OPTS = -Os -std=c++11 -nostartfiles -nostdlib -nodefaultlibs \
	-flto -fno-exceptions -fpermissive -gdwarf-4
TARGET = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
INCL = -I ../../libstm32pp/include -I common
CFLAGS = -x c++ $(TARGET) $(OPTS) $(INCL)
LDFLAGS = -Wl,--gc-sections

.PHONY:	clean all flash firmware prog openocd

all: firmware

firmware: obj/fw.h

obj/%.elf: %.cpp common/* console/* boot/* bus/* sound/*
	$(CPP) $(CFLAGS) $(LDFLAGS) -T $(basename $<).ld $< -o $@
	$(SIZE) $@

obj/%.lst: obj/%.elf
	$(OBJDUMP) -x -S $< >$@

obj/%.sct: obj/%.elf
	$(OBJDUMP) -s $< >$@

obj/boot.h: obj/boot.bin $(BIN2C)
	$(BIN2C) obj_boot_bin $< >$@

obj/fw.h: obj/fw.bin $(BIN2C)
	$(BIN2C) fw_bin $< >$@

obj/fw.bin: obj/main.bin $(FW_TOOL)
	$(FW_TOOL) $< $@

obj/boot.bin: obj/boot.elf obj/boot.lst obj/boot.sct
	$(OBJCOPY) -j .text -j .data -j .txram -O binary $< $@

obj/main.bin: obj/boot.h obj/main.elf obj/main.lst obj/main.sct
	$(OBJCOPY) -j .boot -j .text -j .data -j .txram -O binary obj/main.elf $@

$(FW_TOOL):
	$(MAKE) --directory=$(TOOLS_PATH)/fw_tool

$(BIN2C):
	$(MAKE) --directory=$(TOOLS_PATH)/bin2c

flash: obj/main.bin
	$(STLINK) -p $< 0x08000000 -rst

openocd: obj/main.bin
	$(OPENOCD) -c "program $< verify reset exit 0x08000000"

prog: obj/fw.bin $(FW_UART)
	$(FW_UART) $< $(SERIAL_PORT)

clean:
	$(RM) obj/*
	$(MAKE) clean --directory=$(TOOLS_PATH)/bin2c
	# $(MAKE) clean --directory=$(TOOLS_PATH)/fw_tool
	# $(MAKE) clean --directory=$(TOOLS_PATH)/fw_uart
