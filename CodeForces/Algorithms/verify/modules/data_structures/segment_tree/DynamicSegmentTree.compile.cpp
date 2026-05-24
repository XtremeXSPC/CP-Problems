#include "modules/data_structures/segment_tree/DynamicSegmentTree.hpp"

struct PairMinMonoid {
  using value_type = Pair<I32, I32>;

  static auto identity() -> value_type { return {INF32, INF32}; }
  static auto combine(const value_type& a, const value_type& b) -> value_type {
    return (a.first < b.first ? a : b);
  }
};

int main() {
  DynamicSegmentTree<PairMinMonoid, false> seg(0, 4);
  auto root = seg.new_root();
  root = seg.set(root, 2, {3, 7});
  root = seg.set(root, 1, {1, 9});
  return seg.query(root, 0, 4).second == 9 ? 0 : 1;
}
