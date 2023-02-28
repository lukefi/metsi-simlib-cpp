cmake_minimum_required(VERSION 3.2)
project(metsi_tests)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(Lua51 REQUIRED)

include(CTest)
include(BoostTestHelpers.cmake)

include_directories(${CSV_SINGLE_HEADER_DIR})
include_directories(${LUA_INCLUDE_DIR})

set(METSI_LIBS metsi-applib yaml-cpp metsi-simlib)

add_boost_test(sim/event_graph_test.cpp)
add_boost_test(sim/branching_generators_test.cpp)
add_boost_test(sim/class_typed_state_test.cpp)
add_boost_test(app/csv_parser_test.cpp)
add_boost_test_with_lib(app/lua_test.cpp luajit-5.1)
add_boost_test_with_lib(app/yaml_cpp_test.cpp ${METSI_LIBS})
add_boost_test_with_lib(sim/framework_util_test.cpp ${METSI_LIBS})
add_boost_test_with_lib(app/sim_configuration_test.cpp ${METSI_LIBS})
add_boost_test_with_lib(app/file_io_test.cpp ${METSI_LIBS})

# TODO: need to make this work without specific TARGET..
# OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources doesn't do anything
add_custom_command(TARGET yaml_cpp_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/app/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
        )
