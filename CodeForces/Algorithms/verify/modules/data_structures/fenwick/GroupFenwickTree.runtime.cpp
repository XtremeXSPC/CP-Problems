#include "modules/data_structures/fenwick/GroupFenwickTree.hpp"

#include <cassert>

int main() {
  GroupFenwickTree<AddGroup<I64>> bit(VecI64{2, 7, 1, 8, 2, 8});
  assert(bit.prefix_query(3) == 10);
  assert(bit.query(1, 4) == 16);
  assert(bit.max_right([](I64 x) { return x <= 18; }) == 4);
  assert(bit.min_left([](I64 x) { return x <= 11; }, 4) == 2);

  bit.apply(2, 5);
  assert(bit.get(2) == 6);
  assert(bit.query(0, 4) == 23);

  bit.set(1, 4);
  assert(bit.get(1) == 4);
  assert(bit.prod_all() == 30);

  const VecI64 values = bit.get_all();
  assert(values == VecI64({2, 4, 6, 8, 2, 8}));
  return 0;
}
