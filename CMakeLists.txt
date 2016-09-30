cmake_minimum_required(VERSION 3.0)

project(bbcpp)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MACOSX_RPATH 1)

if (NOT LIB_INSTALL_DIR)
    set (LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
endif (NOT LIB_INSTALL_DIR)

add_subdirectory(lib)
# add_subdirectory(examples)
# add_subdirectory(tests)
