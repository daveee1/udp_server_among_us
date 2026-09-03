# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Directories
SRC_DIR = src
BIN_DIR = bin

# Executables to produce
PROGRAMS = $(BIN_DIR)/client $(BIN_DIR)/server

# Default target
all: $(PROGRAMS)

# Pattern rule: build executables in bin/ from C files in src/
$(BIN_DIR)/%: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Create the bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean built executables and the bin directory
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean