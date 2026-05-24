#include "modules/data_structures/fenwick/FenwickTree.hpp"

#include <cassert>

int main() {
  FenwickTree<I64> bit(VecI64{2, 3, 5, 7});
  assert(bit.prefix_sum(0) == 0);
  assert(bit.prefix_sum(3) == 10);
  assert(bit.sum(1, 4) == 15);
  assert(bit.lower_bound(1) == 1);
  assert(bit.lower_bound(9) == 3);
  assert(bit.upper_bound(9) == 3);

  bit.add(1, 4);
  assert(bit.get(1) == 7);
  assert(bit.sum(0, 4) == 21);

  bit.set(3, 10);
  assert(bit.get(3) == 10);
  assert(bit.sum(2, 4) == 15);
  return 0;
}
