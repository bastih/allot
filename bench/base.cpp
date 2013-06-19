#include "bencho.h"
#include "allot/allot.h"
#include "allot/MallocAllocator.h"
#include "allot/LinearAllocator.h"
#include "allot/alloc_adapter.h"
#include <iostream>
#include <cmath>

using namespace std;

class Search: public AbstractBenchmark
{
 public:
  std::size_t _length;
  
  static std::string name() { return "Search"; }

  enum allocator_types {
    default_vector = 0,
    malloc_vector = 1,
    linear_vector = 2
  };
  
  void initialize() {
    setName("Search");
   
    setWarmUpRuns(0);
    setMaxRuns(10);
        
    addPerformanceCounter("PAPI_TOT_CYC");

    Parameter *length = new Parameter("length", {1, 2, 4, 8, 16, 64, 128, 256, 512, 1024});
    setSequenceId("length");
    addParameter(length);
        
    addTestSeries(allocator_types::default_vector, "default_vector");
    addTestSeries(allocator_types::malloc_vector, "malloc_vector");
    addTestSeries(allocator_types::linear_vector, "linear_vector");
    setAggregatingFunction(AggregationType::Average);
  }
    
  void prepareStart() {}
    
  void finalize() {}
    
  void prepareCombination(map<string, int> parameters, int combination) {}
    
  void finishCombination(map<string, int> parameters, int combination) {}
    
  void prepareRun(map<string, int> parameters, int combination, int test_series_id, int run) {
    _length = parameters.at("length");
  }
    
  void finishRun(map<string, int> parameters, int combination, int test_series_id, int run) {}

  void benchDefaultVector(std::size_t length) {
    std::vector<int> vec;
    for (auto i = 0u; i < length; ++i) {
      vec.push_back(i);
    }
  }

  void benchMallocVector(std::size_t length) {
    allot::MallocAllocator ma;
    std::vector<int, allot::alloc_adapter<int>> vec(ma);
    for (auto i = 0u; i < length; ++i) {
      vec.push_back(i);
    }
  }

  void benchLinearVector(std::size_t length) {
    char space[int(length * sizeof(int) * (log2(length)+1))]; // probably too much
    allot::LinearAllocator la(space, sizeof(space));
    std::vector<int, allot::alloc_adapter<int>> vec(la);
    for (auto i = 0u; i < length; ++i) {
      vec.push_back(i);
    }
  }
  
  void doTheTest(map<string, int> parameters, int combination, int test_series_id, int run) {
    switch (test_series_id) {
      case default_vector: benchDefaultVector(_length); break;
      case malloc_vector: benchMallocVector(_length); break;
      case linear_vector: benchLinearVector(_length); break;
    }
  }
};  


MAIN(Search)

