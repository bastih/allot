project(allot-tests C CXX)

include_directories(${allot_INCLUDE_DIRS} ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(units
  generic_allocator.cpp)

target_link_libraries(units allot gtest_main)

enable_testing()
add_test(units units)

