cmake_minimum_required(VERSION 2.8)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR})

project(allot C CXX)

#set(CMAKE_BUILD_TYPE RELEASE)

add_subdirectory(deps/gtest)
add_subdirectory(deps/bencho)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(bench)

