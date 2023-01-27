cmake_minimum_required(VERSION 3.2)
project(metsi)
enable_testing()

add_subdirectory(add)
set(SOURCE_FILES main.cpp)

add_executable(metsi ${SOURCE_FILES})
target_link_libraries(metsi add)
install(TARGETS metsi DESTINATION ${METSI_INSTALL_BIN_DIR})