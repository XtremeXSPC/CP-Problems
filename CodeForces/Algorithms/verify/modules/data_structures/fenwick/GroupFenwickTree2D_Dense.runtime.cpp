#include "modules/data_structures/fenwick/GroupFenwickTree2D_Dense.hpp"

#include <cassert>
#include <type_traits>

int main() {
  static_assert(std::is_same_v<GroupFenwickTree2D_Dense<AddGroup<I64>>, GroupFenwickTree2D<AddGroup<I64>>>);
  GroupFenwickTree2D_Dense<AddGroup<I64>> bit(Vec2D<I64>{{2, 1}, {3, 4}});
  assert(bit.query(0, 2, 0, 2) == 10);
  bit.apply(0, 1, 5);
  assert(bit.get(0, 1) == 6);
  return 0;
}
