cmake_minimum_required(VERSION 3.22)
project(add C CXX)

set(SOURCE_FILES add.hpp add.cpp)

add_library(add SHARED STATIC ${SOURCE_FILES})

install(TARGETS add DESTINATION ${METSI_INSTALL_LIB_DIR})
install(FILES add.hpp DESTINATION ${METSI_INSTALL_INCLUDE_DIR})
