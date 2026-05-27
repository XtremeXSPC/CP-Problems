#include "modules/graph/tree/DSUOnTree.hpp"

int main() {
  Vec2D<I32> tree(1);
  I32 seen = 0;
  dsu_on_tree(tree, 0, [&](I32) { ++seen; }, [&](I32) {}, [&] {});
  return seen == 1 ? 0 : 1;
}
