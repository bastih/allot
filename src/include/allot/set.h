#ifndef ALLOT_SET_H
#define ALLOT_SET_H

#include <set>

namespace allot {

template <
  typename T,
  typename Compare = typename std::set<T>::key_compare,
  typename Allocator = alloc_adapter<T>
>
#ifdef _LIBCPP_VERSION
using set = std::set<T, Compare, Allocator>;
#else
class set : public std::set<T, Compare, Allocator> {
 public:
  explicit set(const Allocator& allocator) : std::set(Compare(), allocator) {}
}
#endif // _LIBCPP_VERSION

}

#endif
