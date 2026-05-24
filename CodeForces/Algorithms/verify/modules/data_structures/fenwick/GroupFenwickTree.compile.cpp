#include "modules/data_structures/fenwick/GroupFenwickTree.hpp"

int main() {
  GroupFenwickTree<AddGroup<I32>> bit(4, [](I32 i) { return i + 1; });
  return bit.query(0, 4) == 10 ? 0 : 1;
}
