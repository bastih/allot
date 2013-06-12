#include "allot/LinearAllocator.h"

#include <stdexcept>

namespace allot {

LinearAllocator::LinearAllocator(char* data, std::size_t size)
      : _data(data), _next_free(data), _end(data + size) {}

void* LinearAllocator::allocate(std::size_t size) {
    if (_next_free + size >  _end) {
      throw std::bad_alloc();
    }
    void * result = _next_free;
    _next_free += size;
    return result;
}


void LinearAllocator::deallocate(void*, std::size_t) {}

std::size_t LinearAllocator::max_size() const {
  return std::distance(static_cast<const char*>(_next_free), _end);
}

}
