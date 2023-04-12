cmake_minimum_required(VERSION 3.21)
project(metsi)

set(CMAKE_CXX_STANDARD 20)

set(METSI_SIMLIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/metsi-simlib/include)
set(LUAJIT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/luajit/src)
set(WITH_MOTTI4 OFF CACHE BOOL "Build with Motti 4 library wrapper")
add_subdirectory(deps/luajit EXCLUDE_FROM_ALL)
add_subdirectory(metsi-app)
add_subdirectory(metsi-simlib)
