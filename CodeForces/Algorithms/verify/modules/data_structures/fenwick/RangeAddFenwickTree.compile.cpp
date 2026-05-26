#include "modules/data_structures/fenwick/RangeAddFenwickTree.hpp"
#include "modules/algebra/monoid/AddGroup.hpp"

int main() {
  RangeAddFenwickTree<AddGroup<I32>> bit(5, [](I32 i) { return i + 1; });
  bit.add(0, 5, 2);
  return bit.query(0, 5) == 25 ? 0 : 1;
}
