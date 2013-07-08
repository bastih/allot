#ifndef ALLOT_MAP_H
#define ALLOT_MAP_H

#include <map>
#include "allot/alloc_adapter.h"

namespace allot {

#ifdef _LIBCPP_VERSION
template <
  typename Key,
  typename T,
  typename Compare=typename std::map<Key, T>::key_compare,
  typename Allocator=alloc_adapter<typename std::map<Key, T>::value_type>
>
using map = std::map<Key, T, Compare, Allocator>;

#else

//c++11-ify map re allocation
template <
  class Key,
  class T,
  typename Compare=typename std::map<Key, T>::key_compare,
  typename Allocator=alloc_adapter<typename std::map<Key, T>::value_type>
>
class map : public std::map<Key, T, Compare, Allocator> {
 public:
  inline explicit map(const Allocator& alloc) : std::map<Key, T, Compare, Allocator>(Compare(), alloc) {}  
};

#endif
}

#endif
