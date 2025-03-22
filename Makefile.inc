GCC = gcc
GCC_FLAGS = -Wall -pedantic # todo falta el -std=c99

VISTA_O = $(BIN_DIR)/vista.o
PLAYER10_O = $(BIN_DIR)/player10.o
MASTER_O = $(BIN_DIR)/originales/ChompChamps

SOURCES = *.c

BIN_DIR = bin
VISTA_DIR = src/vista
PLAYER10_DIR = src/player10
UTILS_DIR = utils