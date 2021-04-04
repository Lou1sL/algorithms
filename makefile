
# Envirmental Settings -------------

COMPILER_DIR = /usr/bin

# ----------------------------------

CXX     = $(COMPILER_DIR)/x86_64-linux-gnu-g++
ASM     = $(COMPILER_DIR)/x86_64-linux-gnu-as
LINK    = $(COMPILER_DIR)/x86_64-linux-gnu-g++
OBJCOPY = $(COMPILER_DIR)/x86_64-linux-gnu-objcopy

# Libraries ------------------------

# ----------------------------------

# Project Settings -----------------

APP_NAME     = app

OBJ_DIR      = ./obj
SRC_DIR      = ./src
INCLUDE_DIR  = ./src

# ----------------------------------

# Compiling Flags ------------------

CXXFLAGS  = -O2 -std=c++17
LINKFLAGS = -O2 -std=c++17

# ----------------------------------

.PHONY: all clean

all: $(APP_NAME)

$(APP_NAME) : $(OBJ_DIR)/main.o
	$(LINK) $< $(LINKFLAGS) -o $(APP_NAME) -Wl,-Map,$(OBJ_DIR)/$(APP_NAME).map,--cref -lm

$(OBJ_DIR)/main.o : $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ -I$(INCLUDE_DIR) $<

GARBAGE_TYPES         = *.o *.elf *.map *.bin *.hex
DIRECTORIES_TO_CLEAN  = $(shell find "./obj" -type d)
GARBAGE_TYPED_FOLDERS = $(foreach DIR, $(DIRECTORIES_TO_CLEAN), $(addprefix $(DIR)/,$(GARBAGE_TYPES)))

clean:
	$(RM) -rf $(GARBAGE_TYPED_FOLDERS)
	$(RM) $(APP_NAME)