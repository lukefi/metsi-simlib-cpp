cmake_minimum_required(VERSION 3.2)
project(metsi-app)

set(SOURCE_FILES src/main.cpp src/sim_configuration.cpp src/file_io.cpp src/state_model.cpp)
set(LIBLIKE_SOURCE_FILES src/sim_configuration.cpp src/file_io.cpp src/state_model.cpp)

# metsi files are librarized for Boost unit testability
add_library(metsi-applib STATIC ${LIBLIKE_SOURCE_FILES})
target_link_libraries(metsi-applib metsi-simlib yaml-cpp)

add_executable(metsi ${SOURCE_FILES})
target_link_libraries(metsi metsi-applib metsi-simlib yaml-cpp)

install(TARGETS metsi DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(Lua51 REQUIRED)

include_directories(src)
include(CTest)
include(BoostTestHelpers.cmake)

include_directories(${CSV_SINGLE_HEADER_DIR})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${METSI_SIMLIB_INCLUDE_DIR})

set(METSI_LIBS metsi-applib yaml-cpp metsi-simlib)

add_boost_test(tests/csv_parser_test.cpp)
add_boost_test(tests/overlaid_object_test.cpp)
add_boost_test_with_lib(tests/lua_test.cpp luajit-5.1)
add_boost_test_with_lib(tests/yaml_cpp_test.cpp ${METSI_LIBS})
add_boost_test_with_lib(tests/sim_configuration_test.cpp ${METSI_LIBS})
add_boost_test_with_lib(tests/file_io_test.cpp ${METSI_LIBS})
add_boost_test_with_lib(tests/state_model_test.cpp ${METSI_LIBS})

# TODO: need to make this work without specific TARGET..
# OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources doesn't do anything
add_custom_command(TARGET yaml_cpp_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
        )

