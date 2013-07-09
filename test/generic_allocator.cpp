#include "allot/map.h"
#include "allot/vector.h"
#include "allot/set.h"
#include "allot/unordered_map.h"

#include "allot/alloc_adapter.h"
#include "allot/MallocAllocator.h"
#include "allot/LinearAllocator.h"
#include "allot/NumaNodeAllocator.h"
#include "allot/TracingAllocator.h"
#include "gtest/gtest.h"

using ::testing::TestWithParam;
using ::testing::ValuesIn;

class AllocatorTest : public ::testing::TestWithParam<allot::Allocator*> {
 public:
  allot::TracingAllocator* _alloc;

  void SetUp() {
    _alloc = new allot::TracingAllocator(GetParam());
  }

  void TearDown() {
    EXPECT_NE(0, _alloc->getAllocations()) << "Allocations are supposed to happen";
    EXPECT_EQ(_alloc->getAllocations(), _alloc->getDeallocations()) << "Must deallocate all the memory";
    delete _alloc;
  }
};

static const std::size_t SIZE = 1000;
char scratchspace [SIZE];
allot::MallocAllocator ma;
allot::LinearAllocator la(scratchspace, SIZE);
allot::NumaNodeAllocator na(0);


std::vector<allot::Allocator*> GetAllocators() {
  return { &ma, &la, &na };
}

INSTANTIATE_TEST_CASE_P(AllocatorTests, AllocatorTest, ValuesIn(GetAllocators()));

const char* data = "012345678\0";
const char* long_data = "01234567891000000000000000000000000000000000000000000000000000000\0";

TEST_P(AllocatorTest, basic_allocation) {
  void* p = _alloc->allocate(10);
  strcpy(static_cast<char*>(p), data);
  _alloc->deallocate(p, 10);
}

TEST_P(AllocatorTest, vector) {
  allot::vector<int> vec(_alloc);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(4);
  vec.clear();
  vec.shrink_to_fit();
}

TEST_P(AllocatorTest, map) {
  allot::map<int, int> mp(_alloc);
  mp[0] = 10;
  mp[10] = 2;
  ASSERT_EQ(mp[0], 10);
  ASSERT_EQ(mp[10], 2);
}

TEST_P(AllocatorTest, unordered_map) {
  allot::unordered_map<int, int> mp(_alloc);
  mp[0] = 10;
  mp[10] = 2;
  ASSERT_EQ(mp[0], 10);
  ASSERT_EQ(mp[10], 2);
  mp.clear();
}

TEST_P(AllocatorTest, set) {
  allot::set<int> s(_alloc);
  s.insert(10);
  s.insert(20);
  ASSERT_EQ(1u, s.count(10));
  ASSERT_EQ(0u, s.count(11));
}
