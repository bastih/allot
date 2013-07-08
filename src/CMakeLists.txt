find_library(HWLOC NAMES hwloc)

if (NOT HWLOC) 
   MESSAGE(FATAL_ERROR "could not find hwloc - install libhwloc-dev")
endif ()

set(LIBS ${HWLOC})

set(allot_CXX_FLAGS "-std=c++11" CACHE STRING "allot compile flags")
set(allot_INCLUDE_DIRS 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/include CACHE STRING "allot include dirs")

set(internal_flags "-Wall -Wextra -Werror -pedantic -g2 -ggdb")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${allot_CXX_FLAGS} ${internal_flags}")

include_directories(${allot_INCLUDE_DIRS} /usr/local/include)

set(ALLOT_HEADERS
  include/allot/config.h.in
  include/allot/Allocator.h
  include/allot/LinearAllocator.h
  include/allot/MallocAllocator.h
  include/allot/NumaNodeAllocator.h
  include/allot/TracingAllocator.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/allot/config.h.in 
               ${CMAKE_CURRENT_BINARY_DIR}/include/allot/config.h @ONLY)

add_library(allot STATIC 
  ${ALLOT_HEADERS}
  Allocator.cpp
  LinearAllocator.cpp
  MallocAllocator.cpp
  NumaNodeAllocator.cpp
  TracingAllocator.cpp)

target_link_libraries(allot ${LIBS})

