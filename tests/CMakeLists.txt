cmake_minimum_required(VERSION 3.2)
project(metsi_tests)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(yaml-cpp)

include(CTest)
include(BoostTestHelpers.cmake)

include_directories(${SIM_HEADERS_DIR})
add_boost_test(event_graph.cpp)
add_boost_test(branching_generators.cpp)
add_boost_test_with_lib(yaml_tester.cpp yaml-cpp)

add_custom_command(TARGET yaml_tester POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/test.yaml
        $<TARGET_FILE_DIR:yaml_tester>
        )