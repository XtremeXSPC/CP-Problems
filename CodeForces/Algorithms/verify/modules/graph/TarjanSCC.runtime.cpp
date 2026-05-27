#include "modules/graph/TarjanSCC.hpp"

#include <cassert>

int main() {
  TarjanSCC scc(8);
  scc.add_edge(0, 1);
  scc.add_edge(1, 2);
  scc.add_edge(2, 0);
  scc.add_edge(2, 3);
  scc.add_edge(3, 4);
  scc.add_edge(4, 5);
  scc.add_edge(5, 3);
  scc.add_edge(5, 6);
  scc.add_edge(6, 7);
  scc.add_edge(7, 6);

  const auto res = scc.decompose();
  assert(res.count == 3);
  assert(res.comp[0] == res.comp[1]);
  assert(res.comp[1] == res.comp[2]);
  assert(res.comp[3] == res.comp[4]);
  assert(res.comp[4] == res.comp[5]);
  assert(res.comp[6] == res.comp[7]);
  assert(res.comp[2] != res.comp[3]);
  assert(res.comp[5] != res.comp[6]);

  assert(res.components.size() == 3);
  for (I32 from = 0; from < res.count; ++from) {
    for (I32 to : res.dag[from]) {
      assert(from > to);
    }
  }

  assert(res.dag[res.comp[2]].size() == 1);
  assert(res.dag[res.comp[2]][0] == res.comp[3]);
  assert(res.dag[res.comp[5]].size() == 1);
  assert(res.dag[res.comp[5]][0] == res.comp[6]);
  assert(res.dag[res.comp[6]].empty());

  TarjanSCC isolated(3);
  const auto comp = isolated.build();
  assert(comp.size() == 3);
  assert(isolated.condensation_dag().size() == 3);
  return 0;
}
