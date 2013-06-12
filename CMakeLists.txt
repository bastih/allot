cmake_minimum_required(VERSION 2.8)
project(allot CXX)

ADD_DEFINITIONS(
    -std=c++11
    -Wall
    -Wextra
    -Wpedantic
    -g
)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(ALLOT_HEADERS
  include/allot/Allocator.h
  include/allot/MallocAllocator.h
  include/allot/TracingAllocator.h)

add_library(allot STATIC 
  src/Allocator.cpp
  src/MallocAllocator.cpp
  src/LinearAllocator.cpp
  src/TracingAllocator.cpp
)

link_libraries(allot)
link_libraries(pthread)
link_libraries(hwloc)

add_executable(units
  test/gtest/gtest-all.cpp
  test/main.cpp
  test/generic_allocator.cpp
  test/malloc_allocator.cpp)

add_custom_target(test)
macro(run_test test_target)
  add_custom_target(${test_target}_runtest
      COMMAND ${test_target} #cmake 2.6 required
      DEPENDS ${test_target}
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  add_dependencies(test ${test_target}_runtest)
endmacro()
run_test(units)