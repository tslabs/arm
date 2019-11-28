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

FW_TOOL = fw_tool$(EXE)

.PHONY:	clean all

all: $(FW_TOOL)

$(FW_TOOL): posix/fw_tool.c posix/fw_tool.h
	$(CC) $< -o $@

clean:
	# $(RM) $(FW_TOOL)
