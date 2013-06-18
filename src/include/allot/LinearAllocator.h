#ifndef ALLOT_LINEARALLOCATOR_H
#define ALLOT_LINEARALLOCATOR_H

#include "allot/Allocator.h"

namespace allot {

/// A bring-your-own-memory allocator that doesn't deallocate
class LinearAllocator : public Allocator {
 public:
  LinearAllocator(char* data, std::size_t size);
  virtual void* allocate(std::size_t size) override;
  virtual void deallocate(void*, std::size_t) override;
  virtual std::size_t max_size() const override;
 private:
  /*const char* _data;*/
  char* _next_free;
  const char* _end;
};

}

#endif
