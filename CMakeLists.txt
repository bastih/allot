cmake_minimum_required(VERSION 2.8)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR})

project(allot C CXX)

find_library(HWLOC_LIB NAMES hwloc PATHS)
set(INCLUDE_INSTALL_DIR include)
set(BIN_INSTALL_DIR bin)
set(LIBS ${HWLOC_LIB})

ADD_DEFINITIONS(
    -std=c++11
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -g
)

include_directories(${allot_SOURCE_DIR}/include)

set(ALLOT_HEADERS
  include/allot/config.h.in
  include/allot/config.h
  include/allot/Allocator.h
  include/allot/LinearAllocator.h
  include/allot/MallocAllocator.h
  include/allot/TracingAllocator.h)

add_library(allot STATIC 
  src/Allocator.cpp
  src/LinearAllocator.cpp
  src/MallocAllocator.cpp
  src/TracingAllocator.cpp)

configure_file(include/allot/config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)

link_libraries(allot)
link_libraries(pthread)
link_libraries(${LIBS})

add_executable(units
  test/gtest/gtest-all.cpp
  test/main.cpp
  test/generic_allocator.cpp)

add_custom_target(test)
macro(run_test test_target)
  add_custom_target(${test_target}_runtest
      COMMAND ${test_target} #cmake 2.6 required
      DEPENDS ${test_target}
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  add_dependencies(test ${test_target}_runtest)
endmacro()
run_test(units)