#include "modules/data_structures/fenwick/FenwickTree2D.hpp"

int main() {
  FenwickTree2D<I32> bit(2, 2, [](I32 x, I32 y) { return x + y; });
  return bit.sum(0, 2, 0, 2) == 4 ? 0 : 1;
}
