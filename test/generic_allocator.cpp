#include "allot/map.h"
#include "allot/vector.h"

#include "allot/alloc_adapter.h"
#include "allot/MallocAllocator.h"
#include "allot/LinearAllocator.h"
#include "allot/NumaNodeAllocator.h"
#include "allot/TracingAllocator.h"
#include "gtest/gtest.h"

using ::testing::TestWithParam;
using ::testing::ValuesIn;

class AllocatorTest : public ::testing::TestWithParam<allot::Allocator*> {};

static const std::size_t SIZE = 1000;
char scratchspace [SIZE];
char scratchspace2 [SIZE];

allot::MallocAllocator ma;
allot::LinearAllocator la(scratchspace, SIZE);
allot::LinearAllocator la2(scratchspace2, SIZE);
allot::TracingAllocator tla(la2);
allot::NumaNodeAllocator na(0);

std::vector<allot::Allocator*> GetAllocators() {
  return { &ma, &la, &tla, &na };
}

INSTANTIATE_TEST_CASE_P(
    AllocatorT,
    AllocatorTest,
    ValuesIn(GetAllocators()));

const char* data = "012345678\0";
const char* long_data = "01234567891000000000000000000000000000000000000000000000000000000\0";

TEST_P(AllocatorTest, basic_allocation) {
  allot::Allocator& a = *GetParam();
  std::cout << typeid(a).name() << std::endl;
  void* p = a.allocate(10);
  strcpy(static_cast<char*>(p), data);
  a.deallocate(p, 10);
}


TEST_P(AllocatorTest, vector) {
  allot::Allocator& alloc = *GetParam();
  allot::vector<int> vec(alloc);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(4);
}

TEST_P(AllocatorTest, map) {
  allot::Allocator& alloc = *GetParam();
  allot::map<int, int> mp(alloc);
  mp[0] = 10;
  mp[10] = 2;
  ASSERT_EQ(mp[0], 10);
  ASSERT_EQ(mp[10], 2);
}

/*
TEST_P(AllocatorTest, set) {
  Allocator& alloc = *GetParam();
  set<int> mp(alloc);
  mp.insert(10);
  mp.insert(20);
  }*/
