#include "modules/data_structures/segment_tree/LazySegTree.hpp"

#include <cassert>

int main() {
  LazySegTree<I64, I64> seg(VecI64{1, 2, 3, 4});
  assert(seg.query(0, 4) == 10);

  seg.update(1, 4, 3);
  seg.update(0, 2, -1);
  assert(seg.query(0, 4) == 17);
  assert(seg.get(0) == 0);
  assert(seg.get(1) == 4);

  const VecI64 all = seg.get_all();
  assert((all == VecI64{0, 4, 6, 7}));

  const I32 mr = seg.max_right(0, [](I64 x) { return x <= 10; });
  assert(mr == 3);

  const I32 ml = seg.min_left(4, [](I64 x) { return x <= 11; });
  assert(ml == 3);
  return 0;
}
