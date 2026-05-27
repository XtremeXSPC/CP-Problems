#include "modules/graph/TarjanSCC.hpp"

int main() {
  Vec2D<I32> adj(2);
  adj[0].push_back(1);
  TarjanSCC scc(adj);
  const auto res = scc.decompose();
  return res.count == 2 ? 0 : 1;
}
