#include "modules/graph/tree/VirtualTree.hpp"

#include <cassert>

int main() {
  Graph<> g(7);
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(1, 3);
  g.add_edge(1, 4);
  g.add_edge(2, 5);
  g.add_edge(2, 6);

  HLD hld(g);
  VirtualTree vt(hld);
  vt.build(VecI32{3, 4, 5});

  assert((vt.nodes == VecI32{0, 1, 3, 4, 5}));
  assert(vt.tree.size() == 5);
  assert(vt.tree[0].size() == 2);
  assert(vt.tree[1].size() == 2);
  assert(vt.tree[0][0].to == 1);
  assert(vt.tree[0][0].dist == 1);
  assert(vt.tree[0][1].to == 4);
  assert(vt.tree[0][1].dist == 2);
  assert(vt.tree[1][0].to == 2);
  assert(vt.tree[1][0].dist == 1);
  assert(vt.vertex(4) == 5);
  return 0;
}
