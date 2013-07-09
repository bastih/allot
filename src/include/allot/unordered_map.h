#ifndef ALLOT_UNORDERED_MAP_H
#define ALLOT_UNORDERED_MAP_H

#include <unordered_map>

namespace allot {

template<
  class Key,
  class T,
  class Hash = typename std::unordered_map<Key, T>::hasher,
  class KeyEqual = typename std::unordered_map<Key, T>::key_equal,
  class Allocator = alloc_adapter< typename std::unordered_map<Key, T>::value_type >
  >
#ifdef _LIBCPP_VERSION
using unordered_map = std::unordered_map<Key, T, Hash, KeyEqual, Allocator>; 
#else
class unordered_map : public std::unordered_map<Key, T, Hash, KeyEqual, Allocator> {
 public:
  explicit unordered_map(const Allocator& alloc) : std::unordered_map<Key, T, Hash, KeyEqual, Allocator>(10, Hash(), KeyEqual(), alloc) {}
};
#endif

}

#endif
