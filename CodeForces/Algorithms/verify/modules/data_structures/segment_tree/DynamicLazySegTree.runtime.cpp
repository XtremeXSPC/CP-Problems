#include "modules/data_structures/segment_tree/DynamicLazySegTree.hpp"
#include "modules/algebra/acted/RangeAddRangeSum.hpp"

#include <cassert>

int main() {
  DynamicLazySegTree<RangeAddRangeSumActedMonoid<I64>, true> seg(0, 8);
  auto root0 = seg.new_root();
  auto root1 = seg.apply(root0, 2, 6, 3);
  auto root2 = seg.apply(root1, 4, 8, 2);
  auto root3 = seg.set(root2, 5, 10);

  assert(seg.query(root0, 0, 8) == 0);
  assert(seg.query(root1, 0, 8) == 12);
  assert(seg.query(root2, 0, 8) == 20);
  assert(seg.query(root2, 4, 8) == 14);
  assert(seg.query(root3, 0, 8) == 25);
  assert(seg.get(root3, 5) == 10);
  assert(seg.max_right(root2, 0, [](I64 x) { return x <= 11; }) == 5);
  assert(seg.min_left(root2, 8, [](I64 x) { return x <= 9; }) == 5);
  return 0;
}
