#include "allot/NumaNodeAllocator.h"

#include "allot/config.h"

#include <stdexcept>
#include <iostream>
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

/// Tries to retrieve NumaNode and falls back to retrieving the
/// current machine.
inline hwloc_obj_t getNodeOrMachine(const hwloc_topology_t& topo, std::size_t node) {
  if (auto o = hwloc_get_obj_by_type(topo, HWLOC_OBJ_NODE, node)) { return o; }
  else if (auto o = hwloc_get_obj_by_type(topo, HWLOC_OBJ_MACHINE, 0)) { return o; }
  // fallback to the complete machine - if there is no machine, we
  // should error out.
  else { throw std::runtime_error("could not find machine"); }
}

std::size_t getAvailableMemory(std::size_t node) {
  const hwloc_topology_t& topo = getTopology();
  if (auto obj = getNodeOrMachine(topo, node)) {
     return obj->memory.local_memory;
  }
  throw std::runtime_error("No available memory");
}

NumaNodeAllocator::NumaNodeAllocator(std::size_t node) :
    _node(node), _size(getAvailableMemory(node)) {}

void* NumaNodeAllocator::allocate(std::size_t size) {
  const hwloc_topology_t& topo = getTopology();
  if (auto obj = getNodeOrMachine(topo, _node)) {
    void* memory = hwloc_alloc_membind_nodeset(topo,
                                            size,
                                            obj->nodeset,
                                            HWLOC_MEMBIND_BIND,
                                            HWLOC_MEMBIND_NOCPUBIND);
    if (!memory) {
      throw std::runtime_error(strerror(errno));
    }
    return memory;
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


namespace numa {

namespace detail {

void bind_to_node(std::size_t node) {
  auto topo = getTopology();
  auto obj = getNodeOrMachine(topo, node);
  int r = hwloc_set_cpubind(topo, obj->cpuset, HWLOC_CPUBIND_THREAD | HWLOC_CPUBIND_STRICT);
  if (r == -1) { throw std::runtime_error("binding failed"); }
}

}

void try_bind_close_to_allocator(const Allocator* alloc) {
  const NumaNodeAllocator* numa_allocator = dynamic_cast<const NumaNodeAllocator*>(alloc);
  if (!numa_allocator) { return; }
  std::size_t node = numa_allocator->node();
  detail::bind_to_node(node);
}

}

}


