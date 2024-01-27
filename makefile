ifeq ($(OS),Windows_NT)
    SHELL := pwsh.exe
else
   SHELL := pwsh
endif
.SHELLFLAGS := -NoProfile -Command 

# 设置变量
CMAKE = cmake
BUILD_DIR = ./build
FLAG = -G=Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=True
TARGET = dinosaur_SDL3.exe

.clang-format:
	clang-format --dump-config --style=webkit > .clang-format

# 默认目标
all: run

# Config
config:
	@$(CMAKE) -S . -B $(BUILD_DIR) $(FLAG)

#  build
build: config
	@$(CMAKE) --build $(BUILD_DIR)

# run
run: build
	@$(BUILD_DIR)/$(TARGET)

# 清理目标
clean:
	rm $(BUILD_DIR) -r

.PHONY: all config build clean run
