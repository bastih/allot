#include "allot/MallocAllocator.h"

#include <cstdlib>
#include <cerrno>

#include <stdexcept>

namespace allot {

void* MallocAllocator::allocate(const std::size_t size) {
  void* result = std::malloc(size);
  if (errno == ENOMEM) {
    throw std::bad_alloc();
  }
  return result;
}

void MallocAllocator::deallocate(void* address, std::size_t) {
  free(address);
}

}
