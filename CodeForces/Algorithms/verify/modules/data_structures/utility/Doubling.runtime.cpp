#include "modules/data_structures/utility/Doubling.hpp"
#include "modules/algebra/monoid/AddMonoid.hpp"

#include <cassert>

int main() {
  Doubling<AddMonoid<I64>> db(5, 4);
  db.add(0, 1, 3);
  db.add(1, 2, 4);
  db.add(2, 3, 5);
  db.add(3, -1, 6);
  db.add(4, 4, 10);
  db.build();

  assert((db.jump(0, 0) == Pair<I32, I64>(0, 0)));
  assert((db.jump(0, 2) == Pair<I32, I64>(2, 7)));
  assert((db.jump(0, 4) == Pair<I32, I64>(-1, 18)));
  assert((db.jump(4, 3) == Pair<I32, I64>(4, 30)));

  const I64 steps = db.max_step(0, [](I32 v, I64 sum) { return v != -1 && sum <= 7; });
  assert(steps == 2);
  return 0;
}
