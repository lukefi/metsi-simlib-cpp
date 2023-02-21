cmake_minimum_required(VERSION 3.2)
project(metsi_tests)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(Lua51 REQUIRED)

include(CTest)
include(BoostTestHelpers.cmake)

include_directories(${SIM_HEADERS_DIR})
include_directories(${METSI_HEADERS_DIR})
include_directories(${LUA_INCLUDE_DIR})
set(METSI_LIBS metsi-lib yaml-cpp)

add_boost_test(sim/event_graph.cpp)
add_boost_test(sim/branching_generators.cpp)
add_boost_test_with_lib(app/yaml_tester.cpp yaml-cpp)
add_boost_test_with_lib(app/lua_tester.cpp luajit-5.1)
add_boost_test_with_lib(sim/framework_util_test.cpp sim)
add_boost_test(sim/class_type_operation_test.cpp)
add_boost_test_with_lib(app/sim_configuration.cpp ${METSI_LIBS})
add_boost_test_with_lib(app/file_io.cpp ${METSI_LIBS})

add_custom_command(TARGET yaml_tester POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/app/test.yaml
        $<TARGET_FILE_DIR:yaml_tester>
        )

add_custom_command(TARGET lua_tester POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/app/test.lua
        $<TARGET_FILE_DIR:lua_tester>
        )

add_custom_command(TARGET file_io POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/app/test.lua
        $<TARGET_FILE_DIR:file_io>
        )

add_custom_command(TARGET sim_configuration POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/app/test_control.yaml
        $<TARGET_FILE_DIR:sim_configuration>
        )