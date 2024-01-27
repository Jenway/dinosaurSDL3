cmake_minimum_required(VERSION 3.16)
project(${PROJECT_NAME} dinosaur_SDL3 VERSION 0.1.0 LANGUAGES CXX)

# Create an option to switch between a system sdl library and a vendored SDL library
option(MYGAME_VENDORED "Use vendored libraries" OFF)

# set c++ standard
set(CMAKE_CXX_STANDARD 17)

# Create compile_commands.json for clangd
set (CMAKE_EXPORT_COMPILECOMMANDS True)

if(MYGAME_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
    find_package(SDL3_image REQUIRED CONFIG REQUIRED COMPONENTS SDL3_image-shared)
endif()

# add include directories
include_directories(include)

aux_source_directory(src/. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST} main.cc)

# Link to the actual SDL3 library.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_image::SDL3_image)
