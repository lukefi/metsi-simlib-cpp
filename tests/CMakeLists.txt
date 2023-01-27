cmake_minimum_required(VERSION 3.2)
project(metsi_tests)

find_package(Boost COMPONENTS unit_test_framework REQUIRED) 

include(CTest)
include(BoostTestHelpers.cmake)

include_directories(${ADD_HEADERS_DIR})
add_boost_test(test_suite.cpp add)