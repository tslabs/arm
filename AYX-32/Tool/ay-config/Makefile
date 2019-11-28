# Treat NT and non-NT windows the same
ifeq ($(OS),Windows_NT)
    OS = Windows
endif

ifeq ($(OS),Windows)
    EXE = .exe
    ifeq ($(OSTYPE),cygwin)
        RM = rm
    else
        RM = del
    endif
else # assume Linux/Unix
    EXE =
    RM = rm -f
endif

TOOLS_PATH = ..
FIRMWARE_PATH = ../../Firm
FIRMWARE = $(FIRMWARE_PATH)/obj/fw.h

HEX2BIN = $(TOOLS_PATH)/hex2bin/hex2bin$(EXE)
BIN2TAP = $(TOOLS_PATH)/bin2tap/bin2tap$(EXE)
MCTRD = $(TOOLS_PATH)/mctrd/mctrd$(EXE)

SDASZ80 = sdasz80
SDCC = sdcc
SDCC_OPTS = -mz80 --std-sdcc11 --no-std-crt0 --opt-code-speed -I$(FIRMWARE_PATH)/obj
SDCC_LOPTS = -Wl-b_HEADER=0x6000 -Wl-b_HOME=0x6010 -Wl-b_CODE=0x6200 -Wl-b_DATA=0x5B00

TAP = ayx32_fw.tap
TRD = ayx32_fw.trd

all: $(TAP) $(TRD)

tap: $(TAP)

trd: $(TRD)

obj/crt0.rel: src/crt0.s
	$(SDASZ80) -o $@ $<

obj/out.hex: src/main.c obj/crt0.rel $(FIRMWARE) src/*.c src/*.h
	$(SDCC) $(SDCC_OPTS) $(SDCC_LOPTS) obj/crt0.rel $< -o $@

obj/out.bin: obj/out.hex $(HEX2BIN)
	$(HEX2BIN) $<

$(TAP): obj/out.bin $(BIN2TAP)
	$(BIN2TAP) -b -c 24575 -a 24576 -r 24576 -o $@ $<

$(TRD): obj/out.bin $(MCTRD)
	$(MCTRD) new $@
	$(MCTRD) --hobeta add 'res/boot.$$B' $@
	$(MCTRD) add $< $@
	$(MCTRD) rename $@ out.bin code.C

$(HEX2BIN):
	$(MAKE) --directory=$(TOOLS_PATH)/hex2bin

$(BIN2TAP):
	$(MAKE) --directory=$(TOOLS_PATH)/bin2tap

$(MCTRD):
	$(MAKE) --directory=$(TOOLS_PATH)/mctrd

$(FIRMWARE):
	$(MAKE) --directory=$(FIRMWARE_PATH)

clean:
	$(RM) obj/* $(TAP) $(TRD)
	$(MAKE) clean --directory=$(TOOLS_PATH)/hex2bin
	$(MAKE) clean --directory=$(TOOLS_PATH)/bin2tap
	$(MAKE) clean --directory=$(TOOLS_PATH)/mctrd
