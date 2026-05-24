#include "modules/data_structures/segment_tree/MonoidSegmentTree.hpp"

struct PairMinMonoid {
  using value_type = Pair<I32, I32>;

  static auto identity() -> value_type { return {INF32, INF32}; }
  static auto combine(const value_type& a, const value_type& b) -> value_type {
    return a.first < b.first ? a : b;
  }
};

int main() {
  MonoidSegmentTree<PairMinMonoid> seg(Vec<Pair<I32, I32>>{{3, 0}, {1, 1}, {2, 2}});
  return seg.query(0, 3).second == 1 ? 0 : 1;
}
