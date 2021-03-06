#ifndef ALLOT_ALLOCATOR_H
#define ALLOT_ALLOCATOR_H

#include <cstddef>
#include <limits>
#include <ostream>
#include <typeinfo>

namespace allot {


/// Allocator base class interface
class Allocator {
 public:
  virtual ~Allocator();
  /// Allocate a number of bytes, need to be deallocated through
  /// matching call of deallocate()
  /// @param size number of bytes to allocate
  /// @return pointer to memory
  virtual void *allocate(std::size_t size) = 0;
  /// Deallocate memory
  /// @param address pointer to memory
  /// @param size bytes at address to deallocate
  virtual void deallocate(void *address, std::size_t size) = 0;
  /// @returns Maximum number of bytes that this allocator can
  /// allocate
  virtual std::size_t max_size() const {
    return std::numeric_limits<std::size_t>::max();
  }
};

inline void PrintTo(const allot::Allocator& value, ::std::ostream* os) {
   *os << typeid(value).name();
}


}


#endif
