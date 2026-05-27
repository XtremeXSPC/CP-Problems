#include "modules/algebra/monoid/AddMonoid.hpp"
#include "modules/graph/tree/StaticTreeMonoid.hpp"

int main() {
  Graph<> g(2);
  g.add_edge(0, 1);
  TreeCore tree(g);
  StaticTreeMonoid<TreeCore<I64>, AddMonoid<I64>> tm(tree);
  return tm.path_prod(0, 1) == 0 ? 0 : 1;
}
