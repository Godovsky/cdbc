.PHONY: all run lib

CC = gcc

CFLAGS = -Wall -ansi -pedantic -s
INC = ./include
INCLUDES = -I./$(INC)
SRC = src
BIN = bin
CFILES = $(SRC)/cDBC.c
HFILES = $(INC)/cDBC.h

ifeq ($(OS),Windows_NT)
	TARGETS = $(patsubst  examples/%.c,$(BIN)/%.exe,$(wildcard examples/*.c))
    EXT = .exe
    LIB = $(BIN)/libcdbc.dll
    RM = del
    MKDIR = mkdir
    ECHO = echo
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        TARGETS = $(patsubst  examples/%.c,$(BIN)/%,$(wildcard examples/*.c))
        EXT = 
        LIB = $(BIN)/libcdbc.so
        RM = rm -rfv
        MKDIR = mkdir -fv
        ECHO = echo
    endif
    ifeq ($(UNAME_S),Darwin)
        
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        
    endif
endif

all: $(TARGETS)

$(BIN)/%$(EXT): examples/%.c $(CFILES) $(HFILES) | $(BIN)
	@echo "Building $(@F)"
	@$(CC) $(CFLAGS) $(INCLUDES) $(CFILES) $< -o $@

$(BIN):
	@$(MKDIR) $(BIN)

lib: $(LIB)

$(LIB): $(SRC)/cDBC.c $(INC)/cDBC.h | $(BIN)
	@echo "Building $(@F)"
	@$(CC) $(CFLAGS) -shared $(INCLUDES) $< -o $@

%:
	@./$(patsubst %,$(BIN)/%,$@)

clean:
	@$(RM) $(BIN)
