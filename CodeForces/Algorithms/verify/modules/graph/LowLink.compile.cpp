#include "modules/graph/connectivity/LowLink.hpp"

int main() {
  LowLink g(2);
  g.add_edge(0, 1);
  g.build();
  return g.is_bridge[0] ? 0 : 1;
}
