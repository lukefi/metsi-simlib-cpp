cmake_minimum_required(VERSION 3.2)
project(proj_build)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(METSI_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(METSI_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(METSI_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set(METSI_SIMLIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/metsi-simlib)
set(METSI_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/metsi-app)
set(CSV_SINGLE_HEADER_DIR ${PROJECT_SOURCE_DIR}/deps/csv-parser/single_include)

include_directories(${METSI_INSTALL_INCLUDE_DIR})
include_directories(${METSI_SIMLIB_INCLUDE_DIR})
include_directories(${METSI_HEADERS_DIR})

add_subdirectory(src/metsi-app)
add_subdirectory(src/metsi-simlib)
add_subdirectory(tests)
add_subdirectory(deps/csv-parser EXCLUDE_FROM_ALL)
add_subdirectory(deps/yaml-cpp EXCLUDE_FROM_ALL)