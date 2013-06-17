cmake_minimum_required(VERSION 2.8)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR})

project(allot C CXX)

add_subdirectory(deps/gtest)

set(allot_INCLUDE_DIRS ${allot_SOURCE_DIR}/include ${allot_BINARY_DIR}/include)

find_library(HWLOC NAMES hwloc)

if (NOT HWLOC) 
   MESSAGE(FATAL_ERROR "could not find hwloc - install libhwloc-dev")
endif ()

set(LIBS ${HWLOC})

ADD_DEFINITIONS(
    -std=c++11
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -g2
)

include_directories(${allot_INCLUDE_DIRS} /usr/local/include)

set(ALLOT_HEADERS
  include/allot/config.h.in
  include/allot/config.h
  include/allot/Allocator.h
  include/allot/LinearAllocator.h
  include/allot/MallocAllocator.h
  include/allot/NumaNodeAllocator.h
  include/allot/TracingAllocator.h)

add_library(allot STATIC 
  ${ALLOT_HEADERS}
  src/Allocator.cpp
  src/LinearAllocator.cpp
  src/MallocAllocator.cpp
  src/NumaNodeAllocator.cpp
  src/TracingAllocator.cpp)

target_link_libraries(allot ${LIBS})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/allot/config.h.in 
               ${CMAKE_CURRENT_BINARY_DIR}/include/allot/config.h @ONLY)


add_subdirectory(test)
