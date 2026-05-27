#include "modules/graph/tree/VirtualTree.hpp"

int main() {
  Graph<> g(2);
  g.add_edge(0, 1);
  HLD hld(g);
  VirtualTree vt(hld);
  vt.build(VecI32{1});
  return vt.vertex(0) == 1 ? 0 : 1;
}
