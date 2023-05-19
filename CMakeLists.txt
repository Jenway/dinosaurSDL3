cmake_minimum_required(VERSION 3.16)
project(${PROJECT_NAME} dinosaur_SDL2 VERSION 0.1.0 LANGUAGES C_STANDARD)

# set c standard
set(CMAKE_C_STANDARD 11)

include(CTest)
enable_testing()

aux_source_directory(src/. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
