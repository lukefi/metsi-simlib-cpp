cmake_minimum_required(VERSION 3.22)
project(sim CXX)

set(SOURCE_FILES event_graph.hpp branching_generators.hpp operation_util.hpp operation_util.cpp)
add_library(sim SHARED STATIC ${SOURCE_FILES})
install(TARGETS sim DESTINATION ${METSI_INSTALL_LIB_DIR})
install(FILES event_graph.hpp branching_generators.hpp operation_util.hpp DESTINATION ${METSI_INSTALL_INCLUDE_DIR})

