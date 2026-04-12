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
  I32 AS;
  IN(AS);

  VecI32 ID(AS);
  FOR(idx, AS) IN(ID[idx]);

  I32 MaxXor = 0;
  FOR(i, AS) {
    FOR(j, i + 1, AS) {
      I32 CurXor = ID[i] ^ ID[j];
      MaxXor = max(MaxXor, CurXor);
    }
  }
  OUT(MaxXor);
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
