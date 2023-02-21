cmake_minimum_required(VERSION 3.2)
project(metsi)

find_package(yaml-cpp REQUIRED)
set(SOURCE_FILES main.cpp sim_configuration.cpp file_io.cpp)
add_executable(metsi ${SOURCE_FILES})
include_directories(${SIM_HEADERS_DIR})
add_library(metsi-lib SHARED STATIC ${SOURCE_FILES})
target_link_libraries(metsi-lib sim yaml-cpp)
target_link_libraries(metsi sim metsi-lib yaml-cpp)

install(TARGETS metsi DESTINATION ${METSI_INSTALL_BIN_DIR})
install(FILES file_io.hpp sim_configuration.hpp DESTINATION ${METSI_INSTALL_INCLUDE_DIR})
