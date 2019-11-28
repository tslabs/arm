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

HEX2BIN = hex2bin$(EXE)
MOT2BIN = mot2bin$(EXE)

all: $(HEX2BIN) $(MOT2BIN)

$(HEX2BIN): hex2bin.c
	$(CC) $< -o $@

$(MOT2BIN): mot2bin.c
	$(CC) $< -o $@

clean:
	$(RM) $(HEX2BIN) $(MOT2BIN)
