#include "bencho.h"
#include "allot/allot.h"
#include "allot/MallocAllocator.h"
#include "allot/NumaNodeAllocator.h"
#include "allot/LinearAllocator.h"
#include "allot/TracingAllocator.h"

#include "allot/alloc_adapter.h"
#include "allot/vector.h"
#include <iostream>
#include <cmath>
#include <thread>
#include <algorithm>

/* Numa benchmark

   In this benchmark we try to measure the effect of different numa
   allocations (and default allocation). 

   We will use different types of work and different types of
   allocation to compare the effects of numa systems.

   # Setup
   
   Initialization:
   - n worker threads (ideally n = #numa nodes in system)
   - allocate n allot::vectors with X elements
   
   Types of allocation:
   * Allocate with default malloc
   * Allocate with numa allocator (vector n on node #n)

   Types of work: WORKLOAD 
   * sum up all elements, sequentially
   * randomly access elements (but still access all)

   Placement:
   * Random (leave decision to thread scheduler)
   * Explicitly bind to same cores as allocated
   * Bind to numa group only
   * Explicitly bind to different cores

   # Expectations

   Show superiority of same core allocation/work execution
   Show bad performance of allocation/execution mismatch
   Random
   
   # Extensions
   * Add more worker threads -> create more traffic between cores
   * 

   */

enum ALLOCATION {
  AL_malloc = 0,
  AL_numa = 1
};

enum WORKLOAD {
  WL_sequential = 2,
  WL_random = 3
};

enum PLACEMENT {
  PL_random = 4,
  PL_core_strict = 5,
  PL_numa_strict = 6,
  PL_numa_different = 7
};

class NumaBenchmark: public AbstractBenchmark {
 public:
  std::size_t _length;
  std::size_t _numa_groups = 2;
  std::size_t _worker_threads = 2;
  std::vector<std::thread> _threads;
  std::vector<allot::vector<std::uint32_t>* > _work_vectors;

  void initialize() {
    setName("Numa");
   
    setWarmUpRuns(0);
    setMaxRuns(2);
        
    addPerformanceCounter("PAPI_TOT_CYC");

    addParameter(new Parameter("length", {256, 512, 1024, 2048, 4096, 8192}));
    addParameter(new Parameter("workload", {WL_sequential /*, WL_random*/}));
    addParameter(new Parameter("placement", {PL_random, /*PL_core_strict, */ PL_numa_strict/*, PL_numa_different*/}));
    addParameter(new Parameter("allocation", {AL_malloc, AL_numa}));
    setSequenceId("length");
        
    addTestSeries(0, "general");
    setAggregatingFunction(AggregationType::Average);
  }
    
  void prepareStart() {}
    
  void finalize() {}
    
  void prepareCombination(std::map<std::string, int> p, int combination) {
    for (std::size_t worker = 0; worker < _worker_threads; ++worker) {
      allot::Allocator* allocator;
      switch (p["allocation"]) {
        case AL_malloc: allocator = new allot::MallocAllocator; break;
        case AL_numa: allocator = new allot::NumaNodeAllocator(worker); break;
        default: throw std::runtime_error("Invalid allocation parameter");
      }

      auto v = new allot::vector<std::uint32_t>(allocator);
      v->resize(p["length"], 0);
      std::iota(std::begin(*v), std::end(*v), 0);
      _work_vectors.push_back(v);
    }
  }
    
  void finishCombination(std::map<std::string, int> parameters, int combination) {
    for (auto& v: _work_vectors) {
      // get our hands on the allocator before we delete vector
      // so we can delete the allocator after the vector has been
      // deleted (and in the process used the allocator to deallocate
      // the memory
      auto allocator = v->get_allocator().getAllocator();
      delete v;
      delete allocator;
    }
    _work_vectors.clear();
  }
    
  void prepareRun(std::map<std::string, int> parameters, int combination, int test_series_id, int run) {}
    
  void finishRun(std::map<std::string, int> parameters, int combination, int test_series_id, int run) {}

  static void some_func(std::map<std::string, int>& p, allot::vector<std::uint32_t> const * v, int i) {
    try {
      std::size_t found = false;
      switch (p.at("placement")) {
        case PL_random: break;
        case PL_numa_strict: allot::numa::try_bind_close_to_allocator(v->get_allocator().getAllocator()); break;
        default: throw std::runtime_error("invalid thread placement");
      }
      switch (p.at("workload")) {
        case WL_sequential: {
          for (const auto& element: *v) {
            if (element == 10) {
              found++;
            }
          }
        } break;
        default: throw std::runtime_error("invalid workload");
      }
    } catch (...) {
      std::cout << "something failed" << std::endl;
    }
  }
  
  
  void doTheTest(std::map<std::string, int> p, int combination, int test_series_id, int run) {
    std::vector<std::thread> threads;
    for (std::size_t _i=0, e=_worker_threads; _i < e; ++_i) {
      //some_func(p, _work_vectors.at(_i), _i);
      std::thread t(some_func, std::ref(p), _work_vectors.at(_i), _i);
      t.join();
      
    }
    for (auto& t: threads) {
      t.join();
    }
  }
};  


MAIN(NumaBenchmark)

