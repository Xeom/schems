#!/usr/bin/make

all_proxy: all

VERSION=0.0.1

# Toolchain programs.
CC=g++
LD=ld
AR=ar
LN=ln


# Directories.
SRC=src
INC=src/include

# Source and header files.
SRC_OBJS=test
INC_OBJS=

OBJ_PATHS=$(addprefix $(SRC)/, $(addsuffix .o, $(SRC_OBJS)))
SRC_PATHS=$(addprefix $(SRC)/, $(addsuffix .cpp, $(SRC_OBJS)))
INC_PATHS=$(addprefix $(INC)/, $(addsuffix .h, $(INC_OBJS)))



# Compiler flags.
CCFLAGS=-I$(INC) -g -Wall -Wextra -Wno-unused-parameter -Wformat -Wpedantic 

# Default rule for compiling object files.
%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< -o $@


# Rule for compiling main executable.
test: $(OBJ_PATHS)
	$(CC) -g $^ -o $@

# Main rule.
all: test

# Clean repo.
clean:
	rm -f $(OBJ_PATHS) test

.PHONY: all_proxy all clean
