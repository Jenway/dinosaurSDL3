# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O3 -I sdl2/include

# Library flags
LDFLAGS = -L sdl2/lib -lSDL2main -lSDL2 -lSDL2_image

# Source files
SRCS = main.c

# Object files
OBJS = $(patsubst %.c, build/%.o, $(SRCS))

TARGET_DIR = build

# Executable
TARGET = $(TARGET_DIR)/dinosaur

# Default target
dinosaur: $(TARGET) copy_dlls

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to build object files
build/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# Clean rule
clean:
	rm -rf build

# .PHONY rule to prevent conflicts with file named 'all' and 'clean'
.PHONY: dinosaur clean copy_dlls
