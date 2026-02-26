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
    Array<U64, K_MAX + 1> pow_b {};

    for (I32 k = 0; k <= K_MAX; ++k) {
      U64 best = INF_REQ;
      for (I32 j = 0; j <= k; ++j) {
        const U64 req_l = (left[u] == -1 ? 1ULL : dp[left[u]][j]);
        const U64 req_r = (right[u] == -1 ? 1ULL : dp[right[u]][k - j]);
        const U64 cand = std::max(req_l, req_r);
        if (cand < best) best = cand;
      }
      merged[k] = best;
      base_req[k] = std::max(a[u], merged[k]);
    }

    pow_b[0] = 1;
    for (I32 c = 1; c <= K_MAX; ++c) {
      if (pow_b[c - 1] >= ceil_div_u64(REQ_CAP, b[u])) {
        pow_b[c] = REQ_CAP;
      } else {
        pow_b[c] = pow_b[c - 1] * b[u];
      }
    }

    auto& row = dp[u];
    row.fill(INF_REQ);

    for (I32 k = 0; k <= K_MAX; ++k) {
      for (I32 c = 0; c <= k; ++c) {
        const U64 req = ceil_div_u64(base_req[k - c], pow_b[c]);
        if (req < row[k]) row[k] = req;
      }
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
