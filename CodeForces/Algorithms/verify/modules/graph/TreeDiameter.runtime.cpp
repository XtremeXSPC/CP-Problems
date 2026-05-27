#include "modules/graph/tree/TreeDiameter.hpp"

#include <cassert>

int main() {
  Graph<I64> g(5);
  g.add_edge(0, 1, 2);
  g.add_edge(1, 2, 3);
  g.add_edge(1, 3, 4);
  g.add_edge(3, 4, 5);

  const auto d = tree_diameter(g);
  assert(d.from == 4);
  assert(d.to == 2);
  assert(d.dist == 12);
  assert((d.path == VecI32{4, 3, 1, 2}));

  Graph<I64> forest(4);
  forest.add_edge(0, 1, 1);
  forest.add_edge(2, 3, 10);
  const auto c = tree_diameter(forest, 2);
  assert(c.dist == 10);
  assert((c.path == VecI32{3, 2} || c.path == VecI32{2, 3}));
  return 0;
}
