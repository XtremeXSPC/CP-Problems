#include "modules/data_structures/fenwick/DualFenwickTree.hpp"
#include "modules/algebra/monoid/AddGroup.hpp"

#include <cassert>

int main() {
  DualFenwickTree<AddGroup<I64>> bit(VecI64{3, 1, 4, 1, 5});
  assert(bit.get(0) == 3);
  assert(bit.get(2) == 4);

  bit.apply(1, 4, 2);
  assert(bit.get_all() == VecI64({3, 3, 6, 3, 5}));

  bit.set(0, 9);
  assert(bit.query(0) == 9);
  assert(bit.get_all() == VecI64({9, 3, 6, 3, 5}));
  return 0;
}
