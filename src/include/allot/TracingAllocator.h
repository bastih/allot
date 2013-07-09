#ifndef ALLOT_TRACINGALLOCATOR_H
#define ALLOT_TRACINGALLOCATOR_H

#include "allot/Allocator.h"

namespace allot {

/// A simple tracing allocator facility
class TracingAllocator : public Allocator {
 public:
  explicit TracingAllocator(Allocator* w);
  virtual void* allocate(std::size_t size) override;
  virtual void deallocate(void* address, std::size_t size) override;
  void report() const;
  std::size_t getAllocations() { return _allocated; };
  std::size_t getDeallocations() { return _deallocated; };
 private:
  Allocator* _wrapped;
  std::size_t _allocated {0};
  std::size_t _deallocated {0};
};


}

#endif
