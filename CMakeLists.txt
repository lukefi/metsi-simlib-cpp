cmake_minimum_required(VERSION 3.22)
project(metsi)

set(CMAKE_CXX_STANDARD 20)

set(METSI_SIMLIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/metsi-simlib/include)

add_subdirectory(metsi-app)
add_subdirectory(metsi-simlib)
