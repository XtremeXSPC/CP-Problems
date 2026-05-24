#include "modules/data_structures/segment_tree/MonoidSegmentTree.hpp"

#include <cassert>

int main() {
  MonoidSegmentTree<MaxMonoid<I32>> seg(VecI32{3, 1, 4, 1, 5});
  assert(seg.query(0, 5) == 5);
  assert(seg.query(1, 4) == 4);

  seg.set(3, 9);
  assert(seg.get(3) == 9);
  assert(seg.prod_all() == 9);

  seg.multiply(1, 6);
  assert(seg.get(1) == 6);
  assert(seg.query(0, 3) == 6);

  const VecI32 ids = seg.prod_ids(1, 5);
  assert(!ids.empty());

  MonoidSegmentTree<AddMonoid<I32>> xor_seg(VecI32{1, 2, 4, 8});
  assert(xor_seg.xor_prod(0, 4, 1) == 15);

  const I32 mr = seg.max_right(0, [](I32 x) { return x < 9; });
  assert(mr == 3);

  const I32 ml = seg.min_left(5, [](I32 x) { return x < 9; });
  assert(ml == 4);
  return 0;
}
