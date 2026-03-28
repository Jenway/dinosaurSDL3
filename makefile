CC ?= gcc
WEB_CC ?= emcc
PKG_CONFIG ?= pkg-config

TARGET_DIR = build
WEB_TARGET_DIR = build-web
TEST_TARGET_DIR = build-test
SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,$(TARGET_DIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)
WEB_OBJS = $(patsubst src/%.c,$(WEB_TARGET_DIR)/%.o,$(SRCS))
WEB_DEPS = $(WEB_OBJS:.o=.d)
TEST_OBJS = $(TEST_TARGET_DIR)/test_game.o $(TEST_TARGET_DIR)/game.o $(TEST_TARGET_DIR)/obstacles.o
TEST_DEPS = $(TEST_OBJS:.o=.d)

SDL_CFLAGS := $(shell $(PKG_CONFIG) --cflags sdl3)
SDL_LIBS := $(shell $(PKG_CONFIG) --libs sdl3)

PROJECT_INCLUDES = -Iinclude -Ithird_party/stb
CFLAGS = -Wall -Wextra -O3 -MMD -MP $(PROJECT_INCLUDES) $(SDL_CFLAGS)
LDFLAGS = $(SDL_LIBS)
WEB_CFLAGS = -Wall -Wextra -O3 -MMD -MP $(PROJECT_INCLUDES) -sUSE_SDL=3
WEB_LDFLAGS = -sUSE_SDL=3 -sALLOW_MEMORY_GROWTH=1 --preload-file Resources@/Resources
TEST_CFLAGS = -Wall -Wextra -O3 -MMD -MP $(PROJECT_INCLUDES)

ifeq ($(OS),Windows_NT)
SHELL := cmd
.SHELLFLAGS := /C
TARGET = $(TARGET_DIR)/dinosaur.exe
WEB_TARGET = $(WEB_TARGET_DIR)/dinosaur.html
TEST_TARGET = $(TEST_TARGET_DIR)/test_game.exe
WEB_ENV = set EM_NODE_JS=node &&
MKDIR_BUILD = if not exist "$(TARGET_DIR)" mkdir "$(TARGET_DIR)"
MKDIR_BUILD_RESOURCES = if not exist "$(TARGET_DIR)\\Resources" mkdir "$(TARGET_DIR)\\Resources"
MKDIR_WEB = if not exist "$(WEB_TARGET_DIR)" mkdir "$(WEB_TARGET_DIR)"
MKDIR_TEST = if not exist "$(TEST_TARGET_DIR)" mkdir "$(TEST_TARGET_DIR)"
COPY_RUNTIME = for /f "delims=" %%I in ('where SDL3.dll') do @copy /Y "%%I" "$(TARGET_DIR)" >NUL
COPY_RESOURCES = $(MKDIR_BUILD_RESOURCES) && copy /Y "Resources\\dino.png" "$(TARGET_DIR)\\Resources\\dino.png" >NUL
RM_BUILD = if exist "$(TARGET_DIR)" rmdir /S /Q "$(TARGET_DIR)"
RM_WEB = if exist "$(WEB_TARGET_DIR)" rmdir /S /Q "$(WEB_TARGET_DIR)"
RM_TEST = if exist "$(TEST_TARGET_DIR)" rmdir /S /Q "$(TEST_TARGET_DIR)"
RUN_TEST = "$(TEST_TARGET)"
else
TARGET = $(TARGET_DIR)/dinosaur
WEB_TARGET = $(WEB_TARGET_DIR)/dinosaur.html
TEST_TARGET = $(TEST_TARGET_DIR)/test_game
WEB_ENV = EM_NODE_JS=node
MKDIR_BUILD = mkdir -p "$(TARGET_DIR)"
MKDIR_BUILD_RESOURCES = mkdir -p "$(TARGET_DIR)/Resources"
MKDIR_WEB = mkdir -p "$(WEB_TARGET_DIR)"
MKDIR_TEST = mkdir -p "$(TEST_TARGET_DIR)"
COPY_RUNTIME = :
COPY_RESOURCES = $(MKDIR_BUILD_RESOURCES) && cp -f "Resources/dino.png" "$(TARGET_DIR)/Resources/dino.png"
RM_BUILD = rm -rf "$(TARGET_DIR)"
RM_WEB = rm -rf "$(WEB_TARGET_DIR)"
RM_TEST = rm -rf "$(TEST_TARGET_DIR)"
RUN_TEST = ./$(TEST_TARGET)
endif

.PHONY: dinosaur web test clean clean-web clean-test copy_dlls copy_resources

-include $(DEPS) $(WEB_DEPS) $(TEST_DEPS)

dinosaur: $(TARGET) copy_dlls copy_resources

web: $(WEB_TARGET)

test: $(TEST_TARGET)
	$(RUN_TEST)

$(TARGET): $(OBJS) | $(TARGET_DIR)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(WEB_TARGET): $(WEB_OBJS) | $(WEB_TARGET_DIR)
	$(WEB_ENV) $(WEB_CC) $(WEB_OBJS) -o $(WEB_TARGET) $(WEB_LDFLAGS)

$(TEST_TARGET): $(TEST_OBJS) | $(TEST_TARGET_DIR)
	$(CC) $(TEST_OBJS) -o $(TEST_TARGET)

$(TARGET_DIR)/%.o: src/%.c | $(TARGET_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(WEB_TARGET_DIR)/%.o: src/%.c | $(WEB_TARGET_DIR)
	$(WEB_ENV) $(WEB_CC) -c $(WEB_CFLAGS) $< -o $@

$(TEST_TARGET_DIR)/test_game.o: tests/test_game.c | $(TEST_TARGET_DIR)
	$(CC) -c $(TEST_CFLAGS) $< -o $@

$(TEST_TARGET_DIR)/game.o: src/game.c | $(TEST_TARGET_DIR)
	$(CC) -c $(TEST_CFLAGS) $< -o $@

$(TEST_TARGET_DIR)/obstacles.o: src/obstacles.c | $(TEST_TARGET_DIR)
	$(CC) -c $(TEST_CFLAGS) $< -o $@

$(TARGET_DIR):
	$(MKDIR_BUILD)

$(WEB_TARGET_DIR):
	$(MKDIR_WEB)

$(TEST_TARGET_DIR):
	$(MKDIR_TEST)

copy_dlls: | $(TARGET_DIR)
	$(COPY_RUNTIME)

copy_resources: | $(TARGET_DIR)
	$(COPY_RESOURCES)

clean:
	$(RM_BUILD)

clean-web:
	$(RM_WEB)

clean-test:
	$(RM_TEST)
