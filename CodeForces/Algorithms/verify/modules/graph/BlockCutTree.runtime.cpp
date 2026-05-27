#include "modules/graph/connectivity/BlockCutTree.hpp"

#include <cassert>

int main() {
  BlockCutTree bct(7);
  bct.add_edge(0, 1);
  bct.add_edge(1, 2);
  bct.add_edge(2, 0);
  bct.add_edge(1, 3);
  bct.add_edge(3, 4);
  bct.add_edge(4, 5);
  bct.add_edge(5, 3);
  bct.build();

  assert(bct.block_count == 4);
  assert(bct.tree.size() == 11);
  assert(bct.is_articulation[1]);
  assert(bct.is_articulation[3]);
  assert(!bct.is_articulation[0]);
  assert(bct.vertex_blocks[1].size() == 2);
  assert(bct.vertex_blocks[3].size() == 2);
  assert(bct.vertex_blocks[6].size() == 1);

  bool has_triangle = false;
  bool has_bridge = false;
  bool has_isolated = false;
  for (VecI32 block : bct.blocks) {
    std::ranges::sort(block);
    has_triangle = has_triangle || block == VecI32{0, 1, 2};
    has_bridge = has_bridge || block == VecI32{1, 3};
    has_isolated = has_isolated || block == VecI32{6};
  }
  assert(has_triangle);
  assert(has_bridge);
  assert(has_isolated);
  return 0;
}
