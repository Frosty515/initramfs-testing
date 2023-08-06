SHELL := /bin/bash

cxx_source_files := $(shell find src -name *.cpp)
cxx_object_files := $(patsubst src/%.cpp, bin-int/%.cpp.o, $(cxx_source_files))

asm_source_files := $(shell find src -name *.asm)
asm_object_files := $(patsubst src/%.asm, bin-int/%.asm.o, $(asm_source_files))

CXX = x86_64-pc-linux-gnu-g++
ASM = nasm
LD = x86_64-pc-linux-gnu-g++
CXXFLAGS = -m64 -std=c++20 -Wall -Wextra -Isrc -O2 -fno-exceptions -g
ASMFLAGS = -f elf64
LDFLAGS = 
TARGET = bin/fs-testing

.PHONY: all build clean clean-all test-img run

all: clean .WAIT test-img build
	@echo --------------
	@echo Build Complete
	@echo --------------

build: $(cxx_object_files) $(asm_object_files)
	@mkdir -p bin
	@echo -------
	@echo Linking
	@echo -------
	$(LD) $(asm_object_files) $(cxx_object_files) -o $(TARGET) $(LDFLAGS)

clean:
	@rm -fr bin bin-int

clean-all:
	@echo ------------
	@echo Cleaning all
	@echo ------------
	@rm -fr bin bin-int image

test-img:
ifeq ("$(wildcard test-img.tar)","")
	@tar -c --no-auto-compress -f test-img.tar test-files/*
endif

run: all
	@echo -------
	@echo Running
	@echo -------
	@$(TARGET) test-img.tar

$(cxx_object_files): bin-int/%.cpp.o : src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -o $@ -c $(patsubst bin-int/%.cpp.o, src/%.cpp, $@) $(CXXFLAGS)

$(asm_object_files): bin-int/%.asm.o : src/%.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $(patsubst bin-int/%.asm.o, src/%.asm, $@) -o $@
