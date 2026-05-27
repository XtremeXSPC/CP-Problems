#include "modules/graph/connectivity/TwoEdgeConnectedComponents.hpp"

#include <cassert>

int main() {
  TwoEdgeConnectedComponents tecc(7);
  tecc.add_edge(0, 1);
  tecc.add_edge(1, 2);
  tecc.add_edge(2, 0);
  tecc.add_edge(1, 3);
  tecc.add_edge(3, 4);
  tecc.add_edge(4, 5);
  tecc.add_edge(5, 3);
  tecc.build();

  assert(tecc.comp_count == 3);
  assert(tecc.comp[0] == tecc.comp[1]);
  assert(tecc.comp[1] == tecc.comp[2]);
  assert(tecc.comp[3] == tecc.comp[4]);
  assert(tecc.comp[4] == tecc.comp[5]);
  assert(tecc.comp[1] != tecc.comp[3]);
  assert(tecc.comp[6] != tecc.comp[0]);

  const I32 a = tecc.comp[1];
  const I32 b = tecc.comp[3];
  assert(std::ranges::find(tecc.tree[a], b) != tecc.tree[a].end());
  assert(tecc.tree[tecc.comp[6]].empty());
  return 0;
}
