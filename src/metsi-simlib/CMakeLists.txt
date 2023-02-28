cmake_minimum_required(VERSION 3.22)
project(metsi-simlib CXX)

set(SOURCE_FILES event_graph.hpp branching_generators.hpp framework_util.hpp framework_util.cpp)
add_library(metsi-simlib SHARED STATIC ${SOURCE_FILES})
install(TARGETS metsi-simlib DESTINATION ${METSI_INSTALL_LIB_DIR})
install(FILES event_graph.hpp branching_generators.hpp framework_util.hpp DESTINATION ${METSI_INSTALL_INCLUDE_DIR})

