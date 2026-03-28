CC ?= gcc
PKG_CONFIG ?= pkg-config

TARGET_DIR = build
TARGET = $(TARGET_DIR)/dinosaur.exe

SDL_CFLAGS := $(shell $(PKG_CONFIG) --cflags sdl3)
SDL_LIBS := $(shell $(PKG_CONFIG) --libs sdl3)

PROJECT_INCLUDES = -Iinclude -Ithird_party/stb
CFLAGS = -Wall -Wextra -O3 -MMD -MP $(PROJECT_INCLUDES) $(SDL_CFLAGS)
LDFLAGS = $(SDL_LIBS)
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,$(TARGET_DIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

SHELL := cmd
.SHELLFLAGS := /C
MKDIR_BUILD = if not exist "$(TARGET_DIR)" mkdir "$(TARGET_DIR)"
COPY_RUNTIME = for /f "delims=" %%I in ('where SDL3.dll') do @copy /Y "%%I" "$(TARGET_DIR)" >NUL
RM_BUILD = if exist "$(TARGET_DIR)" rmdir /S /Q "$(TARGET_DIR)"

.PHONY: dinosaur clean copy_dlls

-include $(DEPS)

dinosaur: $(TARGET) copy_dlls

$(TARGET): $(OBJS) | $(TARGET_DIR)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(TARGET_DIR)/%.o: src/%.c | $(TARGET_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET_DIR):
	$(MKDIR_BUILD)

copy_dlls: | $(TARGET_DIR)
	$(COPY_RUNTIME)

clean:
	$(RM_BUILD)
