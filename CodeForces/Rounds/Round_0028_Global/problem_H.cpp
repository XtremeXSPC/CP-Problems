#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_FAST_IO

#include "templates/Base.hpp"
#include "modules/data_structures/FenwickTree.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  STR(s);
  const I32 n = as<I32>(s.size());

  // z[i] = number of consecutive zeros ending at i.
  VI z(n, 0);
  FOR(i, n) {
    if (s[i] == '1') continue;
    z[i] = (i == 0 ? 1 : z[i - 1] + 1);
  }

  auto norm = [](I64 x) -> I64 {
    x %= MOD2;
    if (x < 0) x += MOD2;
    return x;
  };

  // W is maintained through a difference-array Fenwick:
  // diff[0] = 1, diff[i>0] = 0  <=>  W[v] = 1 for all v.
  FenwickTree<I64> fen(n);
  fen.add(0, 1);

  I64 answer = 0;

  // Process k from n-1 down to 0 (equivalently 'j = k + 1' from n to 1).
  FOR_R(k, n) {
    const I32 zi = z[k];
    if (zi > 0 && zi < n) {
      const I64 add_val = norm(fen.sum(zi - 1));
      fen.add(zi, add_val);
    }

    answer += norm(fen.sum(k));
    if (answer >= MOD2) answer -= MOD2;
  }

  OUT(answer);
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
