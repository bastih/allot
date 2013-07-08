

#include <vector>
#include <map>

#include "allot/alloc_adapter.h"
#include "allot/MallocAllocator.h"
#include "allot/LinearAllocator.h"
#include "allot/NumaNodeAllocator.h"
#include "allot/TracingAllocator.h"
#include "gtest/gtest.h"
#include "allot/map.h"
#include "allot/vector.h"

using ::testing::TestWithParam;
using ::testing::ValuesIn;
using namespace allot;


class AllocatorTest : public ::testing::TestWithParam<Allocator*> {};


static const std::size_t SIZE = 1000;
char scratchspace [SIZE];
char scratchspace2 [SIZE];

MallocAllocator ma;
LinearAllocator la(scratchspace, SIZE);
LinearAllocator la2(scratchspace2, SIZE);
TracingAllocator tla(la2);
NumaNodeAllocator na(0);

std::vector<Allocator*> GetAllocators() {
  return { &ma, &la, &tla, &na };
}

INSTANTIATE_TEST_CASE_P(
    AllocatorT,
    AllocatorTest,
    ValuesIn(GetAllocators()));

const char* data = "012345678\0";
const char* long_data = "01234567891000000000000000000000000000000000000000000000000000000\0";

TEST_P(AllocatorTest, basic_allocation) {
  Allocator& a = *GetParam();
  std::cout << typeid(a).name() << std::endl;
  void* p = a.allocate(10);
  strcpy(static_cast<char*>(p), data);
  a.deallocate(p, 10);
}





/*template <
  typename kT,
  typename comp=typename std::set<kT>::key_compare,
  typename alloc=alloc_adapter<typename std::set<kT>::value_type>
>
using set = std::set<kT, comp, alloc>;*/


TEST_P(AllocatorTest, vector) {
  Allocator& alloc = *GetParam();
  vector<int> vec(alloc);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
}

TEST_P(AllocatorTest, map) {
  Allocator& alloc = *GetParam();
  map<int, int> mp(alloc);
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
