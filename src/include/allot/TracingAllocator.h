#ifndef ALLOT_TRACINGALLOCATOR_H
#define ALLOT_TRACINGALLOCATOR_H

#include "allot/Allocator.h"

namespace allot {

/// A simple tracing allocator facility
class TracingAllocator : public Allocator {
 public:
  explicit TracingAllocator(Allocator& w);
  virtual void* allocate(std::size_t size) override;
  virtual void deallocate(void* address, std::size_t size) override;
 private:
  Allocator& _wrapped;
};

}

#endif
