#include "modules/data_structures/fenwick/GroupFenwickTree2D.hpp"
#include "modules/algebra/monoid/AddGroup.hpp"

int main() {
  GroupFenwickTree2D<AddGroup<I32>> bit(2, 3);
  bit.add(1, 2, 7);
  return bit.get(1, 2) == 7 ? 0 : 1;
}
