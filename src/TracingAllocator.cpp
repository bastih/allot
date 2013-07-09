#include "allot/TracingAllocator.h"

#include <iostream>

namespace allot {

TracingAllocator::TracingAllocator(Allocator* w) : _wrapped(w) {}

void* TracingAllocator::allocate(std::size_t size) {
  _allocated += size;
  return _wrapped->allocate(size);
}

void TracingAllocator::deallocate(void* address, std::size_t size) {
  _deallocated += size;
  return _wrapped->deallocate(address, size);
}

void TracingAllocator::report() const {
  std::cout << "Allocated " << _allocated << "\n"
            << "Deallocated " << _deallocated << std::endl;
}

}
