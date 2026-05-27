#include "modules/data_structures/utility/Cumsum2D.hpp"
#include "modules/algebra/monoid/AddGroup.hpp"

#include <cassert>

int main() {
  Cumsum2D<AddGroup<I64>> pref(Vec2D<I64>{{1, 2, 3}, {4, 5, 6}});
  assert(pref.prefix(2, 3) == 21);
  assert(pref.query(0, 0, 1, 2) == 3);
  assert(pref.query(0, 1, 2, 3) == 16);
  assert(pref.sum(1, 1, 2, 2) == 5);

  Cumsum2D<AddGroup<I32>> generated(3, 2, [](I32 i, I32 j) { return i * 10 + j; });
  assert(generated.query(1, 0, 3, 2) == 62);
  return 0;
}
