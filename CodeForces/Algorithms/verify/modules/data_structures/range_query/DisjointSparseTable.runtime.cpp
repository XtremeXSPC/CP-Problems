#include "modules/algebra/monoid/AddMonoid.hpp"
#include "modules/algebra/monoid/AffineMonoid.hpp"
#include "modules/data_structures/range_query/DisjointSparseTable.hpp"

#include <cassert>

int main() {
  DisjointSparseTable<AddMonoid<I64>> sum(Vec<I64>{1, 2, 3, 4, 5});
  assert(sum.query(0, 0) == 0);
  assert(sum.query(0, 1) == 1);
  assert(sum.query(1, 4) == 9);
  assert(sum.query(2, 5) == 12);
  assert(sum.query(0, 5) == 15);

  using F = Pair<I64, I64>;
  Vec<F> affine{{2, 1}, {3, 4}, {5, 6}, {7, 8}, {11, 12}};
  DisjointSparseTable<AffineMonoid<I64>> comp(affine);
  const F mid = comp.query(1, 4);
  assert(AffineMonoid<I64>::eval(mid, 1) == 295);
  const F all = comp.query(0, 5);
  assert(AffineMonoid<I64>::eval(all, 1) == 5567);
  return 0;
}
