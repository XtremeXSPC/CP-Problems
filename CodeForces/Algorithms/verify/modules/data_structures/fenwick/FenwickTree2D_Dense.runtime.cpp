#include "modules/data_structures/fenwick/FenwickTree2D_Dense.hpp"

#include <cassert>
#include <type_traits>

int main() {
  static_assert(std::is_same_v<FenwickTree2D_Dense<I64>, FenwickTree2D<I64>>);
  FenwickTree2D_Dense<I64> bit(2, 2, [](I32 x, I32 y) -> I64 { return x + y + 1; });
  assert(bit.prefix_sum(2, 2) == 8);
  bit.add(1, 1, 3);
  assert(bit.get(1, 1) == 6);
  return 0;
}
