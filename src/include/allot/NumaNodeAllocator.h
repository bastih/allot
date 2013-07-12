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
  std::size_t node() const { return _node; }
 private:
  const std::size_t _node;
  const std::size_t _size;
};


namespace numa {

void try_bind_close_to_allocator(const Allocator* alloc);
namespace detail {
void bind_to_node(std::size_t node);
}
}

}

#endif
