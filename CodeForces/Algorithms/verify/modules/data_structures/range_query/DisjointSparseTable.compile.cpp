#include "modules/algebra/monoid/AddMonoid.hpp"
#include "modules/data_structures/range_query/DisjointSparseTable.hpp"

int main() {
  DisjointSparseTable<AddMonoid<I32>> st(VecI32{1, 2});
  return st.prod(0, 2) == 3 ? 0 : 1;
}
