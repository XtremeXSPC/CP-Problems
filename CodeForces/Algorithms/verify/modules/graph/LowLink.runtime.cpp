#include "modules/graph/connectivity/LowLink.hpp"

#include <cassert>

int main() {
  LowLink g(6);
  g.add_edge(0, 1);
  g.add_edge(1, 2);
  g.add_edge(2, 0);
  const I32 bridge_a = g.add_edge(1, 3);
  const I32 bridge_b = g.add_edge(3, 4);
  g.add_edge(3, 5);
  g.add_edge(4, 5);
  g.build();

  assert(g.is_articulation[1]);
  assert(g.is_articulation[3]);
  assert(!g.is_articulation[0]);
  assert(g.is_bridge[bridge_a]);
  assert(!g.is_bridge[bridge_b]);
  assert(g.bridges.size() == 1);
  assert((g.bridges[0] == PairI32(1, 3)));

  LowLink multigraph(2);
  multigraph.add_edge(0, 1);
  multigraph.add_edge(0, 1);
  multigraph.build();
  assert(multigraph.bridges.empty());
  return 0;
}
