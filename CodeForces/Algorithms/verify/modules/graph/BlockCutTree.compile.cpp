#include "modules/graph/connectivity/BlockCutTree.hpp"

int main() {
  BlockCutTree bct(1);
  bct.build();
  return bct.block_count == 1 && bct.tree.size() == 2 ? 0 : 1;
}
