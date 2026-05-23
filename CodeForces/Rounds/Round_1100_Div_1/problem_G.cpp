#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_MACROS
#define NEED_TIMER
#define NEED_CORE
#define NEED_IO
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, k, x);
  --x;

  VecI64 H(n), D(n - 1);
  FOR(i, n) IN(H[i]);
  FOR(i, n - 1) IN(D[i]);

  VecI64 T(n, 0);
  T[x] = 1;
  FOR(i, x, n - 1) {
    T[i + 1] = max(T[i] + 1, D[i]);
    if (i == x && D[i] == 1) T[i + 1] = 1;
  }
  for (I64 i = x - 1; i >= 0; --i) {
    T[i] = max(T[i + 1] + 1, D[i]);
    if (i == x - 1 && D[i] == 1) T[i] = 1;
  }

  VecI64 L(n), R(n);
  Vec<PairI64> st;
  st.push_back({n, I64(2'000'000'000)});
  for (I64 i = n - 1; i >= 0; --i) {
    while (st.back().second <= H[i]) st.pop_back();
    R[i] = st.back().first;
    st.push_back({i, H[i]});
  }

  st.clear();
  st.push_back({-1, I64(2'000'000'000)});
  FOR(i, n) {
    while (st.back().second <= H[i]) st.pop_back();
    L[i] = st.back().first;
    st.push_back({i, H[i]});
  }

  VecI64 pref(n + 1, 0);
  FOR(i, n) pref[i + 1] = pref[i] + H[i];

  auto between = [&](I64 a, I64 b) -> I64 {
    if (a > b) swap(a, b);
    return pref[b] - pref[a + 1];
  };

  Vec<PairI64> ord;
  ord.reserve(n);
  FOR(i, n) ord.push_back({T[i], i});
  sort(all(ord));

  VecI64 dp(n, 0);
  I64 ans = 0;

  for (auto [_, i] : ord) {
    if (T[i] > k) continue;
    ans = max(ans, dp[i] + (k - T[i] + 1) * H[i]);

    for (I64 j : {L[i], R[i]}) {
      if (j < 0 || j >= n) continue;
      const I64 dist = abs(j - i);
      if (dist > T[j] - T[i]) continue;

      const I64 stay = T[j] - T[i] - dist + 1;
      dp[j] = max(dp[j], dp[i] + stay * H[i] + between(i, j));
    }
  }

  OUT(ans);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Stopwatch timer;
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
