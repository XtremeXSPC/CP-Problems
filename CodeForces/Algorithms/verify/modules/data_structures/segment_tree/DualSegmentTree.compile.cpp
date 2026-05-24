#include "modules/data_structures/segment_tree/DualSegmentTree.hpp"

struct PairAddMonoid {
  using value_type = Pair<I32, I32>;

  static auto identity() -> value_type { return {0, 0}; }
  static auto combine(const value_type& a, const value_type& b) -> value_type {
    return {a.first + b.first, a.second + b.second};
  }
};

int main() {
  DualSegmentTree<PairAddMonoid> seg(Vec<Pair<I32, I32>>{{1, 2}, {3, 4}});
  seg.apply(0, 2, {5, 6});
  return seg.get(1).second == 10 ? 0 : 1;
}
