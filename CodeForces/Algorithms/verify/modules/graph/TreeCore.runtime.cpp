#include "modules/graph/tree/TreeCore.hpp"

#include <cassert>

int main() {
  Graph<I64> g(7);
  g.add_edge(0, 1, 2);
  g.add_edge(1, 2, 3);
  g.add_edge(1, 3, 5);
  g.add_edge(3, 4, 7);
  g.add_edge(5, 6, 11);

  TreeCore tree(g);
  assert(tree.lca(2, 4) == 1);
  assert(tree.lca(2, 6) == -1);
  assert(tree.dist(2, 4) == 3);
  assert(tree.dist_weighted(2, 4) == 15);
  assert(tree.jump(2, 4, 0) == 2);
  assert(tree.jump(2, 4, 1) == 1);
  assert(tree.jump(2, 4, 2) == 3);
  assert(tree.jump(2, 4, 3) == 4);
  assert(tree.jump(2, 4, 4) == -1);
  assert(tree.kth_ancestor(4, 2) == 1);
  assert(tree.in_subtree(4, 1));
  assert(!tree.in_subtree(6, 1));

  const auto path = tree.restore_path(2, 4);
  assert((path == VecI32{2, 1, 3, 4}));

  const auto [l, r] = tree.subtree_range(1);
  assert(r - l == 4);
  assert(tree.edge_child(0) == 1);
  assert(tree.edge_child(1) == 2);
  return 0;
}
