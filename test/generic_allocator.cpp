#include "gtest/gtest.h"

#include <vector>
#include <map>

#include "allot/alloc_adapter.h"
#include "allot/MallocAllocator.h"
#include "allot/LinearAllocator.h"
#include "allot/TracingAllocator.h"

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

std::vector<Allocator*> GetAllocators() {
  return { &ma, &la, &tla };
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

TEST_P(AllocatorTest, vector) {
  Allocator& alloc = *GetParam();
  std::vector<int, alloc_adapter<int>> vec(alloc);
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
}


TEST_P(AllocatorTest, map) {
  Allocator& alloc = *GetParam();
  alloc_adapter< std::pair<const int, int> > aa(alloc);
  std::less<const int> less;
  std::map<const int, int, std::less<const int>, alloc_adapter< std::pair<const int, int> > > mp(less, alloc);
  mp[0] = 10;
}
