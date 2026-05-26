#include "modules/data_structures/segment_tree/ActedLazySegTree.hpp"
#include "modules/algebra/acted/RangeAddRangeSum.hpp"

#include <cassert>

int main() {
  ActedLazySegTree<RangeAddRangeSumActedMonoid<I64>> seg(VecI64{1, 2, 3, 4});
  assert(seg.query(0, 4) == 10);

  seg.apply(1, 4, 5);
  assert(seg.query(0, 4) == 25);
  assert(seg.get(2) == 8);

  seg.set(0, 7);
  seg.multiply(3, 2);
  assert(seg.get(3) == 11);
  assert(seg.prod_all() == 33);

  const VecI64 all = seg.get_all();
  assert((all == VecI64{7, 7, 8, 11}));

  const I32 mr = seg.max_right(0, [](I64 x) { return x <= 22; });
  assert(mr == 3);

  const I32 ml = seg.min_left(4, [](I64 x) { return x <= 19; });
  assert(ml == 2);

  ActedLazySegTree<RangeAddRangeSumActedMonoid<I64>> xor_seg(VecI64{1, 2, 3, 4});
  xor_seg.apply_xor_range(0, 2, 1, 10);
  const VecI64 all_xor = xor_seg.get_all();
  assert((all_xor == VecI64{11, 12, 3, 4}));
  return 0;
}
