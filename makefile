.PHONY: all build run tests clean

BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin

all: build

build:
	@cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR)

run:
	@$(BIN_DIR)/main

tests:
	@$(BIN_DIR)/tests

clean:
	@rm -rf $(BUILD_DIR)
