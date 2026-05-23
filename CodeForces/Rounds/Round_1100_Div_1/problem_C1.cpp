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
  FOR(i, n) s[i] = (a[i] > 0 ? -1 : 1);
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
