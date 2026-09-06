# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.

# Directories
SRC_DIR = src
BIN_DIR = bin

# Helper object files
UTILS_OBJ = $(BIN_DIR)/utils.o

# Executables to produce
PROGRAMS = $(BIN_DIR)/client $(BIN_DIR)/server

# Default target
all: $(PROGRAMS)

# Rule to compile utils.c into a .o file
$(BIN_DIR)/utils.o: $(SRC_DIR)/utils.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to link executables with utils.o
$(BIN_DIR)/client: $(SRC_DIR)/client.c $(UTILS_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/server: $(SRC_DIR)/server.c $(UTILS_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Create the bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean built binaries
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean