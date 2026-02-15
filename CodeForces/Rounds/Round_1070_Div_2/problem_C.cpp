#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  LL(n);
  VEC(I64, a, n);

  VLL odds;
  VLL evens;
  for (auto x : a) {
    if (x & 1) odds.pb(x);
    else evens.pb(x);
  }

  sort(rall(evens));

  auto m = sz(odds);
  auto p = sz(evens);
  I64 max_odd = m > 0 ? *max_element(all(odds)) : 0;

  // Prefix sums of even coins (sorted descending).
  VLL pre(p + 1, 0);
  FOR(i, p) pre[i + 1] = pre[i] + evens[i];

  FOR(k, 1, n + 1) {
    if (m == 0) { fast_io::write(0LL); }
    else {
      I64 min_raw = _max(1LL, k - p);
      I64 odd_used = (min_raw % 2 == 1) ? min_raw : min_raw + 1;

      if (odd_used > _min(k, m)) {
        fast_io::write(0LL);
      } else {
        I64 num_even = k - odd_used;
        fast_io::write(max_odd + pre[num_even]);
      }
    }
    fast_io::write_char(k < n ? ' ' : '\n');
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
