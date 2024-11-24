# Compiler
CC = clang

# Use pkg-config to get the correct flags for Raylib
CFLAGS = $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib)

# Source directory and target directory
SRC_DIR = src
BUILD_DIR = build

# Find all source files and extract project names
SRCS := $(wildcard $(SRC_DIR)/*.c)
PROJECTS := $(notdir $(basename $(SRCS)))

# Output executables
TARGETS := $(addprefix $(BUILD_DIR)/, $(PROJECTS))

# Default target: build all projects
all: $(TARGETS)

# Compile each project
$(BUILD_DIR)/%: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@
	@echo "Built $@"

# Build and run a specific project
run: $(BUILD_DIR)/$(PROJECT)
	./$(BUILD_DIR)/$(PROJECT)

# Clean up
clean:
	rm -rf $(BUILD_DIR)

# Rebuild all projects
rebuild: clean all

# Help message
help:
	@echo "Usage:"
	@echo "  make               - Build all projects"
	@echo "  make PROJECT=name  - Build and run a specific project in src/"
	@echo "  make run PROJECT=name - Run a specific project after building"
	@echo "  make clean         - Remove all built files"
	@echo "  make rebuild       - Clean and rebuild all projects"

.PHONY: all run clean rebuild help
