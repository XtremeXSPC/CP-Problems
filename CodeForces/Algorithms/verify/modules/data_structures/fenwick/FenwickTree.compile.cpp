#include "modules/data_structures/fenwick/FenwickTree.hpp"

int main() {
  FenwickTree<I32> bit(5, [](I32 i) { return i + 1; });
  return bit.prefix_sum(5) == 15 && bit.lower_bound(7) == 4 ? 0 : 1;
}
