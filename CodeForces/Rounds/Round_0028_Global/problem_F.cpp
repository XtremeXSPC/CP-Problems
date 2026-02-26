#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_FAST_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

constexpr I32 K_MAX = 60;
constexpr U64 REQ_CAP = 1'000'000'000'000'000'000ULL;
constexpr U64 INF_REQ = 4'000'000'000'000'000'000ULL;

[[nodiscard]] inline auto ceil_div_u64(const U64 x, const U64 y) -> U64 {
  return x / y + (x % y != 0);
}

[[nodiscard]] auto solve_one(const I32 n, const Vec<U64>& a, const Vec<U64>& b) -> I32 {
  Vec<I32> left(n, -1), right(n, -1), parent(n, -1), st;
  st.reserve(as<Size>(n));

  // Min-cartesian tree on b (stable with '>' tie handling).
  for (I32 i = 0; i < n; ++i) {
    I32 last = -1;
    while (!st.empty() && b[st.back()] > b[i]) {
      last = st.back();
      st.pop_back();
    }
    if (last != -1) {
      left[i] = last;
      parent[last] = i;
    }
    if (!st.empty()) {
      right[st.back()] = i;
      parent[i] = st.back();
    }
    st.push_back(i);
  }

  Vec<Array<U64, K_MAX + 1>> dp(as<Size>(n));
  Vec<I32> waiting_children(as<Size>(n), 0);
  Vec<I32> queue;
  queue.reserve(as<Size>(n));

  for (I32 u = 0; u < n; ++u) {
    waiting_children[u] = (left[u] != -1) + (right[u] != -1);
    if (waiting_children[u] == 0) queue.push_back(u);
  }

  I32 q_head = 0;
  I32 root = -1;

  while (q_head < sz(queue)) {
    const I32 u = queue[q_head++];
    root = u;

    Array<U64, K_MAX + 1> merged {};
    Array<U64, K_MAX + 1> base_req {};
    Array<U64, K_MAX + 1> left_req {};
    Array<U64, K_MAX + 1> right_req {};

    if (left[u] == -1) {
      left_req.fill(1);
    } else {
      left_req = dp[left[u]];
    }
    if (right[u] == -1) {
      right_req.fill(1);
    } else {
      right_req = dp[right[u]];
    }

    auto& row = dp[u];
    // Child merge in O(K log K): for fixed k, objective max(L[j], R[k-j])
    // is minimized around the crossing point of decreasing/increasing curves.
    for (I32 k = 0; k <= K_MAX; ++k) {
      I32 lo = 0;
      I32 hi = k;
      while (lo < hi) {
        const I32 mid = (lo + hi) >> 1;
        if (left_req[mid] <= right_req[k - mid]) {
          hi = mid;
        } else {
          lo = mid + 1;
        }
      }

      U64 best = std::max(left_req[lo], right_req[k - lo]);
      if (lo > 0) {
        const U64 cand = std::max(left_req[lo - 1], right_req[k - (lo - 1)]);
        if (cand < best) best = cand;
      }
      merged[k] = best;
      base_req[k] = std::max(a[u], merged[k]);
    }

    // Local transition in O(K).
    row[0] = base_req[0];
    for (I32 k = 1; k <= K_MAX; ++k) {
      const U64 using_more_here = ceil_div_u64(row[k - 1], b[u]);
      row[k] = std::min(base_req[k], using_more_here);
    }

    const I32 p = parent[u];
    if (p != -1) {
      --waiting_children[p];
      if (waiting_children[p] == 0) queue.push_back(p);
    }
  }

  for (I32 k = 0; k <= K_MAX; ++k) {
    if (dp[root][k] == 1) return k;
  }
  return K_MAX;
}

void solve() {
  INT(n);

  Vec<U64> a(as<Size>(n));
  Vec<U64> b(as<Size>(n));
  FOR(i, n) IN(a[i]);
  FOR(i, n) IN(b[i]);

  OUT(solve_one(n, a, b));
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
