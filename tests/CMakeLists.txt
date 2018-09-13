cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(bbcodetests)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})

file(GLOB UTIL_SOURCES test_*.cpp)

add_executable(TestBBCPP
    main.cpp
    ${UTIL_SOURCES}
)

target_link_libraries(TestBBCPP
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    bbcppstatic
)

add_test(NAME TestBBCPP
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/TestBBCPP
)
