#include "modules/data_structures/segment_tree/DualSegmentTree.hpp"

#include <cassert>

struct SumMonoid {
  using value_type = I64;

  static auto identity() -> value_type { return 0; }
  static auto combine(const value_type& a, const value_type& b) -> value_type {
    return a + b;
  }
};

int main() {
  DualSegmentTree<SumMonoid> seg(VecI64{1, 2, 3, 4});
  seg.apply(1, 4, 5);
  assert(seg.get(0) == 1);
  assert(seg.get(1) == 7);
  assert(seg.get(3) == 9);

  seg.set(2, 0);
  seg.apply(0, 3, -2);
  const VecI64 all = seg.get_all();
  assert((all == VecI64{-1, 5, -2, 9}));
  return 0;
}
