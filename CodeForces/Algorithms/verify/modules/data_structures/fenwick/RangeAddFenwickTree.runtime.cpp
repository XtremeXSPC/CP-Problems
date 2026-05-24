#include "modules/data_structures/fenwick/RangeAddFenwickTree.hpp"

#include <cassert>

int main() {
  RangeAddFenwickTree<AddGroup<I64>> bit(VecI64{1, 2, 3, 4, 5});
  assert(bit.query(1, 4) == 9);

  bit.apply(1, 4, 10);
  assert(bit.get(2) == 13);
  assert(bit.prefix_query(4) == 40);

  bit.set(4, 7);
  bit.add_at(0, 5);
  assert(bit.get_all() == VecI64({6, 12, 13, 14, 7}));
  assert(bit.prod_all() == 52);
  return 0;
}
