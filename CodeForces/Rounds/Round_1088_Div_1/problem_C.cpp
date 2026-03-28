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
  INT(n, k);
  VecI32 a(n), b(n);
  IN(a, b);

  Vec<char> is_mono(k, true);
  FOR(i, n - k) {
    if (a[i] != a[i + k]) {
      is_mono[i % k] = false;
    }
  }

  VecI32 forced(k, -1);
  FOR(i, n) {
    const I32 r = i % k;
    if (!is_mono[r]) {
      if (b[i] != -1 && b[i] != a[i]) {
        OUT("NO");
        return;
      }
      continue;
    }

    if (b[i] == -1) {
      continue;
    }

    if (forced[r] == -1) {
      forced[r] = b[i];
    } elif (forced[r] != b[i]) {
      OUT("NO");
      return;
    }
  }

  VecI32 freq(n + 1, 0);
  FOR(r, k) {
    if (is_mono[r]) {
      ++freq[a[r]];
    }
  }

  FOR(r, k) {
    if (!is_mono[r] || forced[r] == -1) {
      continue;
    }

    --freq[forced[r]];
    if (freq[forced[r]] < 0) {
      OUT("NO");
      return;
    }
  }

  OUT("YES");
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
