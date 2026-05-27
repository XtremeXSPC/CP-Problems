#include "modules/graph/tree/TreeDiameter.hpp"

int main() {
  Graph<> g(2);
  g.add_edge(0, 1);
  return tree_diameter(g).dist == 1 ? 0 : 1;
}
