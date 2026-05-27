#include "modules/algebra/monoid/AddMonoid.hpp"
#include "modules/algebra/monoid/AffineMonoid.hpp"
#include "modules/graph/tree/StaticTreeMonoid.hpp"

#include <cassert>

int main() {
  Graph<I64> g(5);
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(3, 4);

  TreeCore tree(g);

  Vec<I64> val{10, 20, 30, 40, 50};
  StaticTreeMonoid<TreeCore<I64>, AddMonoid<I64>> sum(tree, val);
  assert(sum.path_prod(2, 4) == 140);
  assert(sum.subtree_prod(3) == 90);

  using F = Pair<I64, I64>;
  Vec<F> affine{{2, 1}, {3, 4}, {5, 6}, {7, 8}, {11, 12}};
  StaticTreeMonoid<TreeCore<I64>, AffineMonoid<I64>> comp(tree, affine);
  const F path = comp.path_prod(2, 4);
  assert(AffineMonoid<I64>::eval(path, 1) == 2949);
  const F rev_path = comp.path_prod(4, 2);
  assert(AffineMonoid<I64>::eval(rev_path, 1) == 2561);

  StaticTreeMonoid<TreeCore<I64>, AddMonoid<I64>, true> edge_sum(tree, [&](I32 id) {
    return I64(id + 1);
  });
  assert(edge_sum.path_prod(2, 4) == 2 + 3 + 4);
  assert(edge_sum.subtree_prod(1) == 2 + 3 + 4);
  return 0;
}
