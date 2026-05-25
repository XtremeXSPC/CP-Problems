#include "modules/data_structures/fenwick/GroupFenwickTree2D.hpp"

#include <cassert>

int main() {
  GroupFenwickTree2D<AddGroup<I64>> bit(3, 4, [](I32 x, I32 y) -> I64 { return x * 10 + y + 1; });
  assert(bit.prefix_query(0, 4) == 0);
  assert(bit.prefix_query(2, 3) == 42);
  assert(bit.query(1, 3, 1, 4) == 108);

  bit.apply(2, 1, 5);
  assert(bit.get(2, 1) == 27);
  assert(bit.query(2, 3, 0, 3) == 71);

  bit.set(0, 0, 100);
  assert(bit.get(0, 0) == 100);
  const Vec2D<I64> values = bit.get_all();
  assert(values[0][0] == 100);
  assert(values[2][1] == 27);
  return 0;
}
