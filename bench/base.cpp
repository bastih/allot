#include "bencho.h"
#include "allot/allot.h"
#include "allot/MallocAllocator.h"
#include "allot/LinearAllocator.h"
#include "allot/TracingAllocator.h"

#include "allot/alloc_adapter.h"
#include <iostream>
#include <cmath>

using namespace std;

std::size_t sumOfAllocations(std::size_t length, std::size_t element_length) {
  std::size_t sum = 0;
  for (auto i=length; i != 0; i /= 2) {
    sum += i;
  }
  return sum * element_length;
}

class Search: public AbstractBenchmark
{
 public:
  std::size_t _length;
  
  //static std::string name() { return "Search"; }

  enum allocator_types {
    default_vector = 0,
    malloc_vector = 1,
    linear_vector = 2,
    malloc_trace_vector = 3
  };
  
  void initialize() {
    //setName("Search");
   
    setWarmUpRuns(0);
    setMaxRuns(2);
        
    addPerformanceCounter("PAPI_TOT_CYC");

    Parameter *length = new Parameter("length", {256, 512, 1024, 2048, 4096, 8192});
    setSequenceId("length");
    addParameter(length);
        
    addTestSeries(allocator_types::default_vector, "default_vector");
    addTestSeries(allocator_types::malloc_vector, "malloc_vector");
    addTestSeries(allocator_types::malloc_trace_vector, "malloc_trace_vector");
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

  template <typename T>
  inline void add(T& vec, std::size_t length) {
    for (auto i = 0u; i < length; ++i) {
      vec.push_back(i);
    }
    vec.clear();
    vec.shrink_to_fit();
  }
  
  void benchDefaultVector(std::size_t length) {
    std::vector<int> vec;
    add(vec, length);
  }

  void benchMallocVector(std::size_t length) {
    allot::MallocAllocator ma;
    std::vector<int, allot::alloc_adapter<int>> vec(ma);
    add(vec, length);
  }

 void benchMallocTraceVector(std::size_t length) {
    allot::MallocAllocator ma;
    allot::TracingAllocator la(ma);
    std::vector<int, allot::alloc_adapter<int>> vec(la);
    add(vec, length);
  }
  
  void benchLinearVector(std::size_t length) {
    char space[sumOfAllocations(length, sizeof(int))]; // probably a
                                                       // tad too much
    allot::LinearAllocator la(space, sizeof(space));
    std::vector<int, allot::alloc_adapter<int>> vec(la);
    add(vec, length);
  }
  
  void doTheTest(map<string, int> parameters, int combination, int test_series_id, int run) {
    switch (test_series_id) {
      case default_vector: benchDefaultVector(_length); break;
      case malloc_vector: benchMallocVector(_length); break;
      case malloc_trace_vector: benchMallocTraceVector(_length); break;
      case linear_vector: benchLinearVector(_length); break;
    }
  }
};  


MAIN(Search)

