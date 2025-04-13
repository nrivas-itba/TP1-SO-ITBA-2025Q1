GCC = gcc
GCC_FLAGS = -Wall -pedantic -g

view_O = $(BIN_DIR)/view.o
PLAYER10_O = $(BIN_DIR)/player10.o
MASTER_ORIGINAL = $(BIN_DIR)/originales/ChompChampsAMD-v4_4_2025
MASTER_ORIGINAL_ARM = $(BIN_DIR)/originales/ChompChampsARM-v4_4_2025
MASTER_CUSTOM = $(BIN_DIR)/master.o

SOURCES = *.c

BIN_DIR = bin
view_DIR = src/view
PLAYER10_DIR = src/player10
MASTER_DIR = src/master
UTILS_DIR = utils