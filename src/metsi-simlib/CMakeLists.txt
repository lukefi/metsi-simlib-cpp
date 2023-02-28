cmake_minimum_required(VERSION 3.22)
project(metsi-simlib CXX)

set(SOURCE_FILES src/framework_util.cpp)
set(INCLUDE_FILES
        include/metsi-simlib/event_graph.hpp
        include/metsi-simlib/branching_generators.hpp
        include/metsi-simlib/framework_util.hpp
        include/metsi-simlib/core_types.hpp)
add_library(metsi-simlib SHARED ${SOURCE_FILES})

install(TARGETS metsi-simlib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include_directories(include)
include(CTest)
include(BoostTestHelpers.cmake)

add_boost_test(tests/event_graph_test.cpp)
add_boost_test(tests/branching_generators_test.cpp)
add_boost_test(tests/class_typed_state_test.cpp)
add_boost_test_with_lib(tests/framework_util_test.cpp metsi-simlib)