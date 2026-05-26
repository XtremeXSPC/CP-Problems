#include "modules/data_structures/fenwick/DualFenwickTree.hpp"
#include "modules/algebra/monoid/AddGroup.hpp"

int main() {
  DualFenwickTree<AddGroup<I32>> bit(4, [](I32 i) { return i; });
  bit.add(1, 3, 5);
  return bit.get(2) == 7 ? 0 : 1;
}
