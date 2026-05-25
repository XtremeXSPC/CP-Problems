#include "modules/data_structures/fenwick/FenwickTree2D_Dense.hpp"

int main() {
  FenwickTree2D_Dense<I32> bit(1, 2);
  return bit.sum(0, 1, 0, 2) == 0 ? 0 : 1;
}
