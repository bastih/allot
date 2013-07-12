cmake_minimum_required(VERSION 2.8)

project(allot-bench C CXX)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${bencho_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-unused-parameter -ggdb -pthread -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 -D__GCC_HAVE_SYNC_COMPARE_AND_SWAP_8")
message(STATUS ${bencho_INCLUDE_DIRS})
include_directories(${bencho_INCLUDE_DIRS} ${allot_INCLUDE_DIRS})

add_executable(benchmark base.cpp)
add_executable(numa numa.cpp)
target_link_libraries(benchmark bencho allot)
target_link_libraries(numa bencho allot)

