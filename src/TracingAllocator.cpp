#include "allot/TracingAllocator.h"

#include <iostream>

namespace allot {

TracingAllocator::TracingAllocator(Allocator& w) : _wrapped(w) {}

void* TracingAllocator::allocate(std::size_t size) {
  //std::cout << "Asked to allocate "<< size << std::endl;
  return _wrapped.allocate(size);
}

void TracingAllocator::deallocate(void* address, std::size_t size) {
  //std::cout << "Asked to de-allocate "<< address << " sized " << size << std::endl;
  return _wrapped.deallocate(address, size);
}

}
