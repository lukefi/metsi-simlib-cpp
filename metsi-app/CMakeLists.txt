cmake_minimum_required(VERSION 3.2)
project(metsi-app)

#find_package(PkgConfig REQUIRED)
find_package(Boost 1.81.0 COMPONENTS program_options REQUIRED)
find_package(yaml-cpp 0.7.0 REQUIRED)
#pkg_check_modules(LuaJIT REQUIRED luajit)
include_directories(${Boost_INCLUDE_DIR})

set(LIB_SOURCE_FILES src/sim_configuration.cpp src/file_io.cpp src/state_model.cpp src/app_io.cpp src/sim_events.cpp)
set(SOURCE_FILES src/main.cpp ${LIB_SOURCE_FILES})
set(METSI_EXTERNAL_DEPS yaml-cpp metsi-simlib ${Boost_LIBRARIES})
set(METSI_ALL_DEPS metsi-applib ${METSI_EXTERNAL_DEPS})

# metsi files are librarized for Boost unit testability
add_library(metsi-applib STATIC ${LIB_SOURCE_FILES})
target_link_libraries(metsi-applib ${METSI_EXTERNAL_DEPS})

add_executable(metsi ${SOURCE_FILES})
target_link_libraries(metsi ${METSI_ALL_DEPS})

install(TARGETS metsi DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY resources DESTINATION ${CMAKE_INSTALL_PREFIX})

include_directories(src)
find_package(Boost 1.81.0 COMPONENTS unit_test_framework REQUIRED)
include(CTest)
include(BoostTestHelpers.cmake)

#include_directories(${LUA_INCLUDE_DIR})
include_directories(${METSI_SIMLIB_INCLUDE_DIR})

add_boost_test(tests/csv_parser_test.cpp ${METSI_ALL_DEPS})
add_boost_test(tests/overlaid_object_test.cpp ${METSI_ALL_DEPS})
#add_boost_test(tests/lua_test.cpp luajit-5.1 ${METSI_ALL_DEPS})
add_boost_test(tests/app_io_test.cpp ${METSI_ALL_DEPS})
add_boost_test(tests/yaml_cpp_test.cpp ${METSI_ALL_DEPS})
add_boost_test(tests/sim_configuration_test.cpp ${METSI_ALL_DEPS})
add_boost_test(tests/simulation_run_test.cpp ${METSI_ALL_DEPS})
add_boost_test(tests/file_io_test.cpp ${METSI_ALL_DEPS})
add_boost_test(tests/state_model_test.cpp ${METSI_ALL_DEPS})


#Running things from CLion defaults to PWD as the binary dir. These command is a time-saving helper for that.
#Running tests with CTest, working directory is configured to be the source directory.
add_custom_command(TARGET yaml_cpp_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_resources
        $<TARGET_FILE_DIR:yaml_cpp_test>/test_resources
        )

add_custom_command(TARGET metsi POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/forest_data.csv
        $<TARGET_FILE_DIR:metsi>/
        )

add_custom_command(TARGET metsi POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/control.yaml
        $<TARGET_FILE_DIR:metsi>/
        )