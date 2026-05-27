#include "modules/graph/tree/TreeCore.hpp"

int main() {
  Graph<> g(1);
  TreeCore tree(g);
  return tree.lca(0, 0) == 0 ? 0 : 1;
}
