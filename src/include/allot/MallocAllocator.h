#ifndef ALLOT_MALLOCALLOCATOR_H

#include "allot/Allocator.h"

namespace allot {

/// malloc()/free() based allocator
class MallocAllocator : public Allocator {
  virtual void* allocate(std::size_t size) override;
  
  virtual void deallocate(void* address, std::size_t) override;
};

}

#endif
