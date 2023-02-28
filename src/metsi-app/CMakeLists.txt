cmake_minimum_required(VERSION 3.2)
project(metsi-app)

set(SOURCE_FILES main.cpp sim_configuration.cpp file_io.cpp)
set(LIBLIKE_SOURCE_FILES sim_configuration.cpp file_io.cpp)

# metsi files are librarized for Boost unit testability
add_library(metsi-applib STATIC ${LIBLIKE_SOURCE_FILES})
target_link_libraries(metsi-applib metsi-simlib yaml-cpp)

add_executable(metsi ${SOURCE_FILES})
target_link_libraries(metsi metsi-applib metsi-simlib yaml-cpp)

install(TARGETS metsi DESTINATION ${METSI_INSTALL_BIN_DIR})
install(FILES file_io.hpp sim_configuration.hpp DESTINATION ${METSI_INSTALL_INCLUDE_DIR})
