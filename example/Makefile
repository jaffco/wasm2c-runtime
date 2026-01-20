# Makefile for WASM2C example
# Compiles app.cpp to WASM, converts to C with wasm2c, and builds final executable

# Directories
BUILD_DIR := build
APP_DIR := app
SRC_DIR := ../src
INCLUDE_DIR := ../include

# Tools
EMCC := emcc
WASM2C := wasm2c
CXX := g++
CC := gcc

# Files
APP_CPP := $(APP_DIR)/app.cpp
APP_WASM := $(BUILD_DIR)/app.wasm
APP_C := $(BUILD_DIR)/app.c
APP_H := $(BUILD_DIR)/app.h
MAIN_CPP := main.cpp
EXECUTABLE := $(BUILD_DIR)/example

# Runtime sources
RUNTIME_SOURCES := $(SRC_DIR)/wasm-rt-impl.c \
                   $(SRC_DIR)/wasm-rt-mem-impl.c \
                   $(SRC_DIR)/wasm-rt-exceptions-impl.c

# Compiler flags
EMCC_FLAGS := -O2 \
              -s STANDALONE_WASM \
              -s EXPORTED_FUNCTIONS='["_add"]' \
              -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
              --no-entry

CC_FLAGS := -std=c99 -Wall -Wno-incompatible-pointer-types -I$(INCLUDE_DIR) -I$(BUILD_DIR) -I. -include wasm-rt-compat.h
CXX_FLAGS := -std=c++11 -Wall -I$(INCLUDE_DIR) -I$(BUILD_DIR)
LDFLAGS := 

# Default target
.PHONY: all
all: $(EXECUTABLE)

# Create build directory
$(BUILD_DIR):
	@echo "Creating build directory..."
	@mkdir -p $(BUILD_DIR)

# Compile app.cpp to WASM
$(APP_WASM): $(APP_CPP) | $(BUILD_DIR)
	@echo "Compiling app.cpp to WebAssembly..."
	$(EMCC) $(EMCC_FLAGS) $(APP_CPP) -o $(APP_WASM)
	@echo "✓ Generated $(APP_WASM)"

# Convert WASM to C using wasm2c
$(APP_C) $(APP_H): $(APP_WASM)
	@echo "Converting WASM to C with wasm2c..."
	$(WASM2C) $(APP_WASM) -o $(APP_C)
	@echo "✓ Generated $(APP_C) and $(APP_H)"

# Build the final executable
$(EXECUTABLE): $(APP_C) $(APP_H) $(MAIN_CPP) $(RUNTIME_SOURCES)
	@echo "Building final executable..."
	$(CC) $(CC_FLAGS) -c $(SRC_DIR)/wasm-rt-impl.c -o $(BUILD_DIR)/wasm-rt-impl.o
	$(CC) $(CC_FLAGS) -c $(SRC_DIR)/wasm-rt-mem-impl.c -o $(BUILD_DIR)/wasm-rt-mem-impl.o
	$(CC) $(CC_FLAGS) -c $(SRC_DIR)/wasm-rt-exceptions-impl.c -o $(BUILD_DIR)/wasm-rt-exceptions-impl.o
	$(CC) $(CC_FLAGS) -c $(APP_C) -o $(BUILD_DIR)/app.o
	$(CXX) $(CXX_FLAGS) \
		$(MAIN_CPP) \
		$(BUILD_DIR)/wasm-rt-impl.o \
		$(BUILD_DIR)/wasm-rt-mem-impl.o \
		$(BUILD_DIR)/wasm-rt-exceptions-impl.o \
		$(BUILD_DIR)/app.o \
		$(LDFLAGS) \
		-o $(EXECUTABLE)
	@echo "✓ Built $(EXECUTABLE)"

# Run the example
.PHONY: run
run: $(EXECUTABLE)
	@echo ""
	@echo "Running example..."
	@echo ""
	@$(EXECUTABLE)

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)
	@echo "✓ Clean complete"

# Help target
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  make all    - Build everything (default)"
	@echo "  make run    - Build and run the example"
	@echo "  make clean  - Remove all build artifacts"
	@echo "  make help   - Show this help message"
