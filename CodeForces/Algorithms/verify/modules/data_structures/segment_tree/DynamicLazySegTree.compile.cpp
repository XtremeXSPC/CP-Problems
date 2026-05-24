#include "modules/data_structures/segment_tree/DynamicLazySegTree.hpp"

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

struct LocalChmaxAction {
  static auto apply(const I32& value, const I32& bound, I32) -> I32 {
    return std::max(value, bound);
  }
};

int main() {
  using Seg = DynamicLazySegTree<ActedMonoid<LocalMaxMonoid, LocalChmaxMonoid, LocalChmaxAction>, false>;
  Seg seg(0, 4);
  auto root = seg.new_root();
  root = seg.apply(root, 0, 3, 4);
  return seg.query(root, 0, 4) == 4 ? 0 : 1;
}
