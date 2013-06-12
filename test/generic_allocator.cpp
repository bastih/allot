#include "gtest/gtest.h"
#include "allot/MallocAllocator.h"
#include "allot/LinearAllocator.h"

using ::testing::TestWithParam;
using ::testing::ValuesIn;
using namespace allot;

class AllocatorTest : public ::testing::TestWithParam<allot::Allocator*> {};

char scratchspace [10];

std::vector<Allocator*> GetAllocators() {
  return { new MallocAllocator, new LinearAllocator(scratchspace, 10) };
}

INSTANTIATE_TEST_CASE_P(
    AllocatorT,
    AllocatorTest,
    ValuesIn(GetAllocators()));

const char* data = "012345678\0";
const char* long_data = "01234567891000000000000000000000000000000000000000000000000000000\0";

TEST_P(AllocatorTest, a1) {
  Allocator& a = *GetParam();
  void* p = a.allocate(10);
  strcpy(static_cast<char*>(p), data);
  a.deallocate(p, 10);
  std::cout << typeid(a).name() << std::endl;
}
