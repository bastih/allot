cmake_minimum_required(VERSION 2.8)

project(allot-bench C CXX)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${bencho_CXX_FLAGS} -std=c++11")
message(STATUS ${bencho_INCLUDE_DIRS})
include_directories(${bencho_INCLUDE_DIRS} ${allot_INCLUDE_DIRS})

add_executable(benchmark base.cpp)
target_link_libraries(benchmark bencho allot)

