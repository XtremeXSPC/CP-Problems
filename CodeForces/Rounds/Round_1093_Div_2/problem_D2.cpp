#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_TIMER
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  constexpr I32 LIM = 33;

  INT(N);
  const I32 N2 = 2 * N + 1;

#ifdef LOCAL
  VecI32 A(N2 + 1);
  FOR(i, 1, N2 + 1) IN(A[i]);

  VecI32 want;
  Vec2D<I32> P(N + 1);
  FOR(i, 1, N2 + 1) {
    P[A[i]].eb(i);
    if (sz(P[A[i]]) == 3) want = P[A[i]];
  }
#endif

  I32 used = 0;
  auto ask = [&](I32 mid, const VecI32& extra) -> I32 {
    ++used;
    my_assert(used <= LIM);

#ifdef LOCAL
    VecI32 CNT(N + 1, 0);
    FOR(i, 1, mid + 1) ++CNT[A[i]];
    for (I32 j : extra) ++CNT[A[j]];

    I32 Res = 0;
    FOR(v, 1, N + 1) Res += (CNT[v] == 1);
#else
    cout << "? " << mid + as<I32>(extra.size());
    FOR(i, 1, mid + 1) cout << ' ' << i;
    for (I32 j : extra) cout << ' ' << j;
    cout << '\n';
    FLUSH();

    I32 Res;
    cin >> Res;
    if (Res == -1) std::exit(0);
#endif

    return (mid + as<I32>(extra.size()) - Res) & 1;
  };

  auto find_pos = [&](I32 r, const VecI32& extra) -> I32 {
    I32 L = 1;
    while (L < r) {
      const I32 mid = (L + r) / 2;
      if (ask(mid, extra)) {
        r = mid;
      } else {
        L = mid + 1;
      }
    }
    return L;
  };

  const I32 Z = find_pos(N2, {});
  const I32 Y = find_pos(Z - 1, {Z});
  const I32 X = find_pos(Y - 1, {Y, Z});

#ifdef LOCAL
  VecI32 got = {X, Y, Z};
  my_assert(got == want);
  OUT(X, Y, Z);
#else
  cout << "! " << X << ' ' << Y << ' ' << Z << '\n';
  FLUSH();
#endif
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  I32 T;
  if (!(cin >> T)) return 0;
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
