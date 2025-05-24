# Compiler and flags
CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic -std=c99 -Iinclude -MMD -MP -MJ compile_commands.json
# CFLAGS := -Wall -Wextra -std=c99 -Iinclude

# Directories
SRC_DIR := src
OBJ_DIR := build
BIN := pngfilter

# Source & Object files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default rule
all: $(BIN)

# Link
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean
