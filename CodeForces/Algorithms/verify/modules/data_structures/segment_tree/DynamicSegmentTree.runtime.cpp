#include "modules/data_structures/segment_tree/DynamicSegmentTree.hpp"

#include <cassert>

struct SumMonoid {
  using value_type = I64;

  static auto identity() -> value_type { return 0; }
  static auto combine(const value_type& a, const value_type& b) -> value_type {
    return a + b;
  }
};

struct MaxMonoid {
  using value_type = I64;
  static auto identity() -> value_type { return neg_infinity<I64>; }
  static auto combine(const value_type& a, const value_type& b) -> value_type {
    return std::max(a, b);
  }
};

int main() {
  DynamicSegmentTree<SumMonoid, true> seg(0, 8);
  auto root0 = seg.new_root();
  auto root1 = seg.set(root0, 3, 5);
  auto root2 = seg.multiply(root1, 3, 2);
  auto root3 = seg.set(root2, 6, 4);

  assert(seg.query(root0, 0, 8) == 0);
  assert(seg.query(root1, 0, 8) == 5);
  assert(seg.query(root2, 3, 4) == 7);
  assert(seg.query(root3, 0, 8) == 11);

  DynamicSegmentTree<MaxMonoid, true> max_seg(0, 8);
  auto mx0 = max_seg.new_root();
  mx0 = max_seg.set(mx0, 3, 7);
  mx0 = max_seg.set(mx0, 6, 4);
  assert(max_seg.max_right(mx0, 0, [](I64 x) { return x < 7; }) == 3);
  assert(max_seg.min_left(mx0, 8, [](I64 x) { return x < 7; }) == 4);
  return 0;
}
