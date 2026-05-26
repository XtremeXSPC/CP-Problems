#include "modules/data_structures/range_query/MonoidSparseTable.hpp"
#include "modules/algebra/monoid/MinMonoid.hpp"

#include <cassert>

int main() {
  MonoidSparseTable<MinMonoid<I32>> st(VecI32{5, 1, 4, 2, 3});
  assert(st.query(0, 5) == 1);
  assert(st.query(2, 5) == 2);

  const I32 mr = st.max_right(0, [](I32 x) { return x >= 1; });
  assert(mr == 5);

  const I32 ml = st.min_left(5, [](I32 x) { return x >= 2; });
  assert(ml == 2);
  return 0;
}
