cmake_minimum_required(VERSION 3.21)
if(${APPLE})
    set(CMAKE_OSX_ARCHITECTURES x86_64)
    set(VCPKG_TARGET_TRIPLET x64-osx CACHE STRING "" FORCE)
endif()
project(metsi)

set(CMAKE_CXX_STANDARD 20)

set(METSI_SIMLIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/metsi-simlib/include)
set(LUAJIT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/luajit/src)
set(WITH_MOTTI4 OFF CACHE BOOL "Build with Motti 4 library wrapper")
if(${WITH_MOTTI4})
    add_subdirectory(deps/motti4)
endif()

add_subdirectory(deps/luajit EXCLUDE_FROM_ALL)
add_subdirectory(metsi-app)
add_subdirectory(metsi-simlib)
