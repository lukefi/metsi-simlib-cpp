cmake_minimum_required(VERSION 3.2)
project(proj_build)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

set(METSI_INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(METSI_INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
set(METSI_INSTALL_LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)

set(METSI_SIMLIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/metsi-simlib/include)
set(CSV_SINGLE_HEADER_DIR ${PROJECT_SOURCE_DIR}/deps/csv-parser/single_include)
set(YAML_CPP_HEADER_DIR ${PROJECT_SOURCE_DIR}/deps/yaml-cpp/include)

add_subdirectory(src/metsi-app)
add_subdirectory(src/metsi-simlib)
add_subdirectory(deps/csv-parser EXCLUDE_FROM_ALL)
add_subdirectory(deps/yaml-cpp EXCLUDE_FROM_ALL)