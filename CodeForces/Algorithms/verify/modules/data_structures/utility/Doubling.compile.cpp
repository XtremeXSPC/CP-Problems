#include "modules/data_structures/utility/Doubling.hpp"
#include "modules/algebra/monoid/AddMonoid.hpp"

int main() {
  Doubling<AddMonoid<I32>> db(2, 3);
  db.set_next(0, 1, 1);
  db.set_next(1, -1, 2);
  db.build();
  return db.jump(0, 2).second == 3 ? 0 : 1;
}
