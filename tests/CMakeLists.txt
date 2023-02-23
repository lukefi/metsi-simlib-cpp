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

add_boost_test(sim/event_graph_test.cpp)
add_boost_test(sim/branching_generators_test.cpp)
add_boost_test_with_lib(app/yaml_cpp_test.cpp yaml-cpp)
add_boost_test_with_lib(app/lua_test.cpp luajit-5.1)
add_boost_test_with_lib(sim/framework_util_test.cpp sim)
add_boost_test(sim/class_typed_state_test.cpp)
add_boost_test_with_lib(app/sim_configuration_test.cpp ${METSI_LIBS})
add_boost_test_with_lib(app/file_io_test.cpp ${METSI_LIBS})

add_custom_command(TARGET yaml_cpp_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/app/test.yaml
        $<TARGET_FILE_DIR:yaml_cpp_test>
        )

add_custom_command(TARGET lua_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/app/test.lua
        $<TARGET_FILE_DIR:lua_test>
        )

add_custom_command(TARGET file_io_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/app/test.lua
        $<TARGET_FILE_DIR:file_io_test>
        )

add_custom_command(TARGET sim_configuration_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/app/event_aliasing.yaml
        $<TARGET_FILE_DIR:sim_configuration_test>
        )

add_custom_command(TARGET sim_configuration_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/app/simulation_events_non_nested.yaml
        $<TARGET_FILE_DIR:sim_configuration_test>
        )

add_custom_command(TARGET sim_configuration_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/app/simulation_events_nested.yaml
        $<TARGET_FILE_DIR:sim_configuration_test>
        )