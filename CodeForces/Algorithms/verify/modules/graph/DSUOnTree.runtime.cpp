#include "modules/graph/tree/DSUOnTree.hpp"

#include <cassert>

int main() {
  Vec2D<I32> tree(7);
  auto add_edge = [&](I32 a, I32 b) {
    tree[a].push_back(b);
    tree[b].push_back(a);
  };
  add_edge(0, 1);
  add_edge(0, 2);
  add_edge(1, 3);
  add_edge(1, 4);
  add_edge(2, 5);
  add_edge(2, 6);

  VecI32 color{1, 2, 1, 2, 3, 1, 3};
  VecI32 cnt(4, 0);
  VecI32 distinct(7, 0);
  I32 cur = 0;

  auto add = [&](I32 v) {
    if (cnt[color[v]]++ == 0) ++cur;
  };
  auto query = [&](I32 v) { distinct[v] = cur; };
  auto clear = [&] {
    std::ranges::fill(cnt, 0);
    cur = 0;
  };

  dsu_on_tree(tree, 0, add, query, clear);
  assert((distinct == VecI32{3, 2, 2, 1, 1, 1, 1}));
  return 0;
}
