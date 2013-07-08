#ifndef ALLOT_VECTOR_H
#define ALLOT_VECTOR_H

#include <vector>
#include "allot/alloc_adapter.h"

namespace allot {

template <typename T>
using vector = std::vector<T, alloc_adapter<T>>;

}

#endif 
