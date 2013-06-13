#ifndef ALLOT_ALLOC_ADAPTER_H
#define ALLOT_ALLOC_ADAPTER_H

#include <cstddef>

#include "allot/Allocator.h"

namespace allot {

template <typename T>
class alloc_adapter {
 public:
  // Allocator requirements
  typedef std::size_t size_type;
  typedef T value_type;
  typedef T *pointer;
  typedef std::ptrdiff_t difference_type;
  typedef const T *const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  
  // Disallow default construction, we always expect to have a backing
  // instance of Allocator at hand.
  alloc_adapter() = delete;

  // Explicitly allow implicit conversion from allocator instance
  alloc_adapter(Allocator& alloc) noexcept :
    _alloc(alloc)  {}

  template <class Other>
  alloc_adapter(const alloc_adapter<Other>& other) :
    _alloc(other.getAllocator()) {}

  template<class Other>
  struct rebind {
    typedef alloc_adapter<Other> other;
  };
  
  pointer allocate(const size_type n, __attribute__((__unused__)) void* hint = nullptr) {
    return static_cast<pointer>(_alloc.allocate(sizeof(T) * n));
  }

  void deallocate(pointer ptr, size_type n) throw () {
    _alloc.deallocate(ptr, sizeof(T) * n);
  }

  size_type max_size() const throw() {
    return _alloc.max_size() / sizeof(T);
  }

  Allocator& getAllocator() const {
    return _alloc;
  }

  pointer address(reference val) const throw () {
    return std::addressof(val);
  }

  const_pointer address(const_reference val) const throw () {
    return std::addressof(val);
  }

  void construct(pointer ptr, const_reference val) {
    new (ptr) value_type(val);
  }

  void destroy(pointer ptr) {
    ptr->~value_type();
  }
  
 private:
  Allocator& _alloc;
};

template<typename A, typename B>
inline bool operator==(const alloc_adapter<A>& a, const alloc_adapter<B>& b) {
  return a.getAllocator() == b.getAllocator();
}

template<typename A, typename B>
inline bool operator!=(const alloc_adapter<A>& a, const alloc_adapter<B>& b) {
  return !(a==b);
}

}

#endif
