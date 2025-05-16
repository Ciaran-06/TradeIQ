.PHONY: all build run tests retest clean

BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin

all: build

build:
	@cmake -S . -B $(BUILD_DIR)
	@cmake --build $(BUILD_DIR)

run:
	@$(BIN_DIR)/main_exec

tests:
	@cd $(BUILD_DIR) && ctest --output-on-failure -T test --no-compress-output

retest:
	@cd $(BUILD_DIR) && ctest --rerun-failed --output-on-failure -T test --no-compress-output

clean:
	@rm -rf $(BUILD_DIR)
