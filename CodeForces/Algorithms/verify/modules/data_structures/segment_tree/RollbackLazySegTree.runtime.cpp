#include "modules/data_structures/segment_tree/RollbackLazySegTree.hpp"

#include <cassert>

int main() {
  RollbackLazySegTree<RangeAddRangeSumActedMonoid<I64>> seg(VecI64{1, 2, 3, 4});
  const auto snap0 = seg.snapshot();

  seg.apply(1, 4, 5);
  assert(seg.query(0, 4) == 25);
  assert(seg.query(1, 3) == 15);
  assert(seg.max_right(0, [](I64 x) { return x <= 10; }) == 2);

  const auto snap1 = seg.snapshot();
  seg.set(0, 7);
  seg.multiply(3, 2);
  assert(seg.query(0, 4) == 33);
  assert(seg.min_left(4, [](I64 x) { return x <= 11; }) == 3);

  seg.rollback(snap1);
  assert(seg.query(0, 4) == 25);
  assert(seg.get(0) == 1);

  seg.rollback(snap0);
  const VecI64 all = seg.get_all();
  assert((all == VecI64{1, 2, 3, 4}));
  return 0;
}
