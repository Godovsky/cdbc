.PHONY: all run lib

PROJECTNAME = example

CC = gcc

CFLAGS = -Wall -ansi -pedantic -s
INC = ./include
INCLUDES = -I./$(INC)
SRC = src
CFILES = $(SRC)/cDBC.c
HFILES = $(INC)/cDBC.h

ifeq ($(OS),Windows_NT)
	TARGET = $(PROJECTNAME).exe
    LIB = libcdbc.dll
    RM = del
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
        TARGET = $(PROJECTNAME)
        LIB = libcdbc.so
        RM = rm -fv
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

all: $(TARGET)

$(TARGET): $(SRC)/$(PROJECTNAME).c $(CFILES) $(HFILES)
	@echo "Building $(@F)"
	@$(CC) $(CFLAGS) $(INCLUDES) $(CFILES) $< -o $@

lib: $(LIB)

$(LIB): $(SRC)/cDBC.c $(INC)/cDBC.h
	@echo "Building $(@F)"
	@$(CC) $(CFLAGS) -shared $(INCLUDES) $< -o $@

run:
	@./$(TARGET)

clean:
	@$(RM) $(TARGET)
	@$(RM) $(LIB)
