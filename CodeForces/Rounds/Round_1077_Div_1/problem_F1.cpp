#define NEED_CORE
#define NEED_FAST_IO

#include "templates/Base.hpp"
#include "modules/data_structures/SegmentTree.hpp"
#include "modules/data_structures/SegmentTreeMinAddSearch.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct MaxOp {
  [[nodiscard]] I32 operator()(I32 a, I32 b) const { return std::max(a, b); }
};

void solve() {
  INT(n);

  VI a(as<Size>(n + 1), 0);
  VI b(as<Size>(n + 1), 0);

  FOR(i, 1, n + 1) IN(a[as<Size>(i)]);
  FOR(i, 1, n + 1) IN(b[as<Size>(i)]);

  VI pos_b(as<Size>(n + 1), 0);
  FOR(i, n + 1) pos_b[as<Size>(b[as<Size>(i)])] = i;

  VI a_prime(as<Size>(n + 1), 0);
  FOR(i, n + 1) a_prime[as<Size>(i)] = pos_b[as<Size>(a[as<Size>(i)])];

  VI next_smaller(as<Size>(n + 1), n + 1);
  VI stack_min;
  stack_min.reserve(as<Size>(n + 1));
  FOR(i, n + 1) {
    while (!stack_min.empty() && a_prime[as<Size>(stack_min.back())] > a_prime[as<Size>(i)]) {
      next_smaller[as<Size>(stack_min.back())] = i;
      stack_min.pop_back();
    }
    stack_min.push_back(i);
  }

  VLL base_min(as<Size>(n + 1), 0);
  FOR(i, n + 1) base_min[as<Size>(i)] = -as<I64>(i);
  SegmentTreeMinAddSearch<I64> min_tree(base_min);
  SegmentTree<I32, MaxOp> dp_tree(n + 1, 0, MaxOp{});

  VI st;
  st.reserve(as<Size>(n + 1));

  for (I32 i = n; i >= 0; --i) {
    const I32 new_max = a_prime[as<Size>(i)];

    while (!st.empty() && a_prime[as<Size>(st.back())] < new_max) {
      const I32 j = st.back();
      st.pop_back();
      const I32 next_idx = st.empty() ? n : st.back() - 1;
      const I32 old_max = a_prime[as<Size>(j)];
      min_tree.range_add(j, next_idx + 1, as<I64>(new_max - old_max));
    }

    st.push_back(i);
    min_tree.range_add(i, i + 1, as<I64>(new_max));

    const I64 target = as<I64>(new_max - i);
    const I32 limit = next_smaller[as<Size>(i)] - 1;

    I32 r_i = i;
    if (i <= limit) {
      const I32 candidate = min_tree.find_rightmost_eq(i, limit + 1, target);
      if (candidate != -1) r_i = candidate;
    }

    const I32 best_child = dp_tree.query(i + 1, r_i + 1);
    const I32 dp_i = 1 + best_child;
    dp_tree.update(i, dp_i);

    if (i == 0) {
      OUT(dp_i - 1);
      return;
    }
  }
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
