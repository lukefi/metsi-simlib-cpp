cmake_minimum_required(VERSION 3.2)
project(metsi)

set(SOURCE_FILES main.cpp sim_configuration.cpp file_io.cpp)
add_executable(metsi ${SOURCE_FILES})
add_library(metsi-lib SHARED STATIC ${SOURCE_FILES})
target_link_libraries(metsi-lib metsi-simlib yaml-cpp)
target_link_libraries(metsi metsi-simlib metsi-lib yaml-cpp)

install(TARGETS metsi DESTINATION ${METSI_INSTALL_BIN_DIR})
install(FILES file_io.hpp sim_configuration.hpp DESTINATION ${METSI_INSTALL_INCLUDE_DIR})
