cmake_minimum_required(VERSION 3.16)
project(${PROJECT_NAME} dinosaur_SDL2 VERSION 0.1.0 LANGUAGES C)

# set c standard
set(CMAKE_C_STANDARD 11)

aux_source_directory(src/. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

# include SDL2

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/SDL2)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include/SDL2_image)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/bin/SDL2.dll
    ${CMAKE_CURRENT_LIST_DIR}/bin/SDL2_image.dll
)
