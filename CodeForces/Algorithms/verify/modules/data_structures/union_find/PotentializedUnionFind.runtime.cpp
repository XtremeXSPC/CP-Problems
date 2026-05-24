#include "modules/data_structures/union_find/PotentializedUnionFind.hpp"

#include <cassert>

struct AddGroup {
  using value_type = I64;

  static constexpr value_type unit() { return 0; }
  static constexpr value_type op(value_type a, value_type b) { return a + b; }
  static constexpr value_type inverse(value_type x) { return -x; }
};

int main() {
  PotentializedUnionFind<AddGroup> dsu(5);
  assert(dsu.num_components() == 5);

  assert(dsu.unite(0, 1, 3));
  assert(dsu.unite(1, 2, 5));
  assert(dsu.unite(3, 4, -2));
  assert(!dsu.unite(0, 2, 8));

  auto diff_0_2 = dsu.diff(0, 2);
  assert(diff_0_2.has_value() && *diff_0_2 == 8);

  auto diff_2_0 = dsu.diff(2, 0);
  assert(diff_2_0.has_value() && *diff_2_0 == -8);

  auto diff_3_4 = dsu.diff(3, 4);
  assert(diff_3_4.has_value() && *diff_3_4 == -2);

  assert(!dsu.diff(0, 4).has_value());
  assert(dsu.same(0, 2));
  assert(!dsu.same(0, 4));
  assert(dsu.component_size(1) == 3);
  assert(dsu.num_components() == 2);
  return 0;
}
