# Makefile for Raylib project on macOS M1 with pkg-config

# Compiler
CC = clang

# Use pkg-config to get the correct flags for Raylib
CFLAGS = $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib)

# Source files
SRCS = main.c

# Output executable
TARGET = raylib_app

# Default target: build and run
all: $(TARGET)

# Build the executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)

# Run the application
run: $(TARGET)
	./$(TARGET)

# Clean up
clean:
	rm -f $(TARGET)

# Rebuild the project
rebuild: clean all

.PHONY: all run clean rebuild
