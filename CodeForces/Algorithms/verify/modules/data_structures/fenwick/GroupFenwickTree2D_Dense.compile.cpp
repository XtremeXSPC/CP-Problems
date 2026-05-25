#include "modules/data_structures/fenwick/GroupFenwickTree2D_Dense.hpp"

int main() {
  GroupFenwickTree2D_Dense<AddGroup<I32>> bit(1, 1);
  return bit.query(0, 1, 0, 1) == 0 ? 0 : 1;
}
