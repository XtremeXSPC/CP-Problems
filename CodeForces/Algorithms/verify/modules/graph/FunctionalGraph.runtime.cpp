#include "modules/graph/functional/FunctionalGraph.hpp"

#include <cassert>

int main() {
  FunctionalGraph fg(VecI32{1, 2, 0, 2, 5, 4, 5});
  assert(fg.cycles.size() == 2);
  assert(fg.comp[0] == fg.comp[1]);
  assert(fg.comp[1] == fg.comp[2]);
  assert(fg.comp[4] == fg.comp[5]);
  assert(fg.comp[3] == fg.comp[0]);
  assert(fg.comp[6] == fg.comp[4]);
  assert(fg.depth[0] == 0);
  assert(fg.depth[3] == 1);
  assert(fg.depth[6] == 1);
  assert(fg.in_cycle(4));
  assert(!fg.in_cycle(6));
  assert(fg.root[3] == 2);
  assert(fg.root[6] == 5);
  assert(fg.next(1) == 2);
  return 0;
}
