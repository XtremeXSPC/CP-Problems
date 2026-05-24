#include "modules/data_structures/range_query/MonoidSparseTable.hpp"

struct GcdMonoid {
  using value_type = I32;

  static auto identity() -> value_type { return 0; }
  static auto combine(const value_type& a, const value_type& b) -> value_type {
    return std::gcd(a, b);
  }
};

int main() {
  MonoidSparseTable<GcdMonoid> st(VecI32{12, 18, 24});
  return st.query(0, 3) == 6 ? 0 : 1;
}
