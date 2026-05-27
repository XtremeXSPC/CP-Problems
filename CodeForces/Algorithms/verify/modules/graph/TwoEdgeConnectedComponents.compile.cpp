#include "modules/graph/connectivity/TwoEdgeConnectedComponents.hpp"

int main() {
  TwoEdgeConnectedComponents tecc(3);
  tecc.add_edge(0, 1);
  tecc.add_edge(1, 2);
  tecc.build();
  return tecc.comp_count == 3 ? 0 : 1;
}
