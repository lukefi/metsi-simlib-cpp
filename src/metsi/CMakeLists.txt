cmake_minimum_required(VERSION 3.2)
project(metsi)

set(SOURCE_FILES main.cpp)
add_executable(metsi ${SOURCE_FILES})
include_directories(${SIM_HEADERS_DIR})
find_package(yaml-cpp)

install(TARGETS metsi DESTINATION ${METSI_INSTALL_BIN_DIR})