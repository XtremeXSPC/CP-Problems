#include "modules/data_structures/fenwick/FenwickTree2D.hpp"

#include <cassert>

int main() {
  FenwickTree2D<I64> bit(Vec2D<I64>{{1, 2, 3}, {4, 5, 6}});
  assert(bit.prefix_sum(2, 2) == 12);
  assert(bit.sum(0, 2, 1, 3) == 16);

  bit.add(1, 0, 10);
  assert(bit.get(1, 0) == 14);
  assert(bit.sum(1, 2, 0, 2) == 19);
  return 0;
}
