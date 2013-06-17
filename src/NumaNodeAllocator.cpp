#include "allot/NumaNodeAllocator.h"

#include "allot/config.h"

#include <stdexcept>

#include "hwloc.h"

namespace allot {

static hwloc_topology_t& getTopology() {
  static hwloc_topology_t topology = [] () {
    hwloc_topology_t t;
    hwloc_topology_init(&t);
    hwloc_topology_load(t);
    return t;
  }();
  return topology;
}

std::size_t getAvailableMemory(std::size_t node) {
  const hwloc_topology_t& topo = getTopology();
  if (hwloc_obj_t obj = hwloc_get_obj_by_type(topo, HWLOC_OBJ_NODE, node)) {
    return obj->memory.local_memory;
  }
  return 0;
}

NumaNodeAllocator::NumaNodeAllocator(std::size_t node) :
    _node(node), _size(getAvailableMemory(node)) {}

void* NumaNodeAllocator::allocate(std::size_t size) {
  const hwloc_topology_t& topo = getTopology();
  if (hwloc_obj_t obj = hwloc_get_obj_by_type(topo, HWLOC_OBJ_NODE, _node)) {
    void* res = hwloc_alloc_membind_nodeset(topo,
                                            size,
                                            obj->nodeset,
                                            HWLOC_MEMBIND_BIND,
                                            HWLOC_MEMBIND_NOCPUBIND);
    if (!res) {
      throw std::runtime_error(strerror(errno));
    }
    return res;
  } else {
    throw std::runtime_error("Could not get numa node");
  }
}

void NumaNodeAllocator::deallocate(void* address, std::size_t size) {
  const hwloc_topology_t& topo = getTopology();
  hwloc_free(topo, address, size);
}

std::size_t NumaNodeAllocator::max_size() const {
  return _size;
}

   

}
