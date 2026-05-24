#include "modules/data_structures/segment_tree/ActedLazySegTree.hpp"

struct LocalMaxMonoid {
  using value_type = I32;

  static auto identity() -> value_type { return -INF32; }
  static auto combine(const value_type& a, const value_type& b) -> value_type {
    return std::max(a, b);
  }
};

struct LocalChmaxMonoid {
  using value_type = I32;

  static auto identity() -> value_type { return -INF32; }
  static auto combine(const value_type& a, const value_type& b) -> value_type {
    return std::max(a, b);
  }
};

struct ChmaxAction {
  static auto apply(const I32& value, const I32& bound, I32) -> I32 {
    return std::max(value, bound);
  }
};

int main() {
  using Seg = ActedLazySegTree<ActedMonoid<LocalMaxMonoid, LocalChmaxMonoid, ChmaxAction>>;
  Seg seg(VecI32{1, 5, 2, 3});
  seg.apply(0, 3, 4);
  return seg.query(0, 4) == 5 ? 0 : 1;
}
