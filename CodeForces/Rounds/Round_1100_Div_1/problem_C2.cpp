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
#include "modules/data_structures/PrefixFlipTree.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

auto target_signs(const VecI64& a) -> VecI32 {
  const I32 n = as<I32>(a.size());
  VecI32 s(n + 1, 1);

  I64 best = 0;
  FOR(i, n) best += a[i];

  I32 best_k = -1;
  VecI64 pref_abs(n + 1, 0);
  FOR(i, n) pref_abs[i + 1] = pref_abs[i] + abs(a[i]);

  VecI64 suff(n + 1, 0);
  FOR_R(i, n) suff[i] = suff[i + 1] + a[i];

  FOR(i, n) {
    if (a[i] < 0) continue;
    const I64 cand = pref_abs[i] - a[i] + suff[i + 1];
    if (cand > best) {
      best = cand;
      best_k = as<I32>(i);
    }
  }

  if (best_k == -1) return s;

  FOR(i, best_k) s[i] = (a[i] > 0 ? 1 : -1);
  s[best_k] = -1;
  FOR(i, best_k + 1, n) s[i] = 1;
  return s;
}

void solve() {
  INT(n);
  VecI64 A(n);
  FOR(i, n) IN(A[i]);

  const VecI32 s   = target_signs(A);
  const VecI32 ops = prefix_flip_ops_for_signs_1based(A, s);

  OUT(ops.size());
  if (!ops.empty()) {
    const I32 k = as<I32>(ops.size());
    FOR(i, k) {
      cout << ops[i] << (i + 1 == k ? "" : " ");
    }
    cout << '\n';
  }
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
