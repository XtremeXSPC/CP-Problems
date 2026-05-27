#include "modules/graph/tree/TreeCenter.hpp"

#include <cassert>

int main() {
  Graph<> odd_path(4);
  odd_path.add_edge(0, 1);
  odd_path.add_edge(1, 2);
  odd_path.add_edge(2, 3);
  const auto c1 = tree_center(odd_path);
  assert(c1.diameter_edges == 3);
  assert(c1.first == 2);
  assert(c1.second == 1);

  Graph<> even_path(5);
  even_path.add_edge(0, 1);
  even_path.add_edge(1, 2);
  even_path.add_edge(2, 3);
  even_path.add_edge(3, 4);
  const auto c2 = tree_center(even_path);
  assert(c2.diameter_edges == 4);
  assert(c2.first == 2);
  assert(c2.second == 2);
  return 0;
}
