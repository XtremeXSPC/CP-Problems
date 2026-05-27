#include "modules/graph/functional/FunctionalGraph.hpp"

int main() {
  FunctionalGraph fg(VecI32{0});
  return fg.in_cycle(0) && fg.cycle(fg.comp[0]).size() == 1 ? 0 : 1;
}
