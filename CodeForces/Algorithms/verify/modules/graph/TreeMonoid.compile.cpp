#include "modules/algebra/monoid/AddMonoid.hpp"
#include "modules/graph/tree/TreeMonoid.hpp"

int main() {
  Graph<> g(2);
  g.add_edge(0, 1);
  TreeCore tree(g);
  TreeMonoid<TreeCore<I64>, AddMonoid<I64>> tm(tree);
  return tm.path_prod(0, 1) == 0 ? 0 : 1;
}
