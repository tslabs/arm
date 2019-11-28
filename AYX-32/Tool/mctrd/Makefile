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

MCTRD = mctrd$(EXE)

all: $(MCTRD)

$(MCTRD): mctrd.c
	$(CC) $< -o $@

clean:
	$(RM) $(MCTRD)
