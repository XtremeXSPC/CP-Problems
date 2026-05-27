#include "modules/graph/tree/TreeCenter.hpp"

int main() {
  Graph<> g(1);
  const auto c = tree_center(g);
  return c.first == 0 && c.second == 0 ? 0 : 1;
}
