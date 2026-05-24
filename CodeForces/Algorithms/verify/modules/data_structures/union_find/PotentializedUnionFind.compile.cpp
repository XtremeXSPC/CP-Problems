#include "modules/data_structures/union_find/PotentializedUnionFind.hpp"

struct XorGroup {
  using value_type = U32;

  static constexpr value_type unit() { return 0; }
  static constexpr value_type op(value_type a, value_type b) { return a ^ b; }
  static constexpr value_type inverse(value_type x) { return x; }
};

int main() {
  PotentializedUnionFind<XorGroup> dsu(3);
  dsu.unite(0, 1, 7);
  dsu.unite(1, 2, 4);
  auto diff_0_2 = dsu.diff(0, 2);
  return diff_0_2.has_value() && *diff_0_2 == 3 ? 0 : 1;
}
