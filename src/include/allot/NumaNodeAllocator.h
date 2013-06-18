#ifndef ALLOT_NUMANODEALLOCATOR_H
#define ALLOT_NUMANODEALLOCATOR_H

#include "allot/Allocator.h"

namespace allot {
    
class NumaNodeAllocator : public Allocator {
 public:
  NumaNodeAllocator(std::size_t node);
  void * allocate(std::size_t size);
  void deallocate(void* address, std::size_t size);
  std::size_t max_size() const;
 private:
  const std::size_t _node;
  const std::size_t _size;
};


}

#endif
