#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, k);
  const I32 m = n - k + 1;
  const I32 full_chunks = m / k;
  const I32 rem = m % k;

  VecI32 insertion_pos;
  insertion_pos.reserve(as<Size>(full_chunks + (rem > 0 ? 1 : 0)));

  FOR (i, full_chunks) insertion_pos.push_back((i + 1) * k);
  if (rem > 0) insertion_pos.push_back((full_chunks + 1) * k);

  const I32 req_min_count = as<I32>(sz(insertion_pos));
  VecI32 p(n, 0);

  FOR (i, req_min_count) {
    p[as<Size>(insertion_pos[i] - 1)] = i + 1;
  }

  I32 cur = req_min_count + 1;
  FOR (i, n) {
    if (p[i] == 0) {
      p[i] = cur++;
    }
  }

  FOR (i, n) {
    std::cout << p[i] << (i + 1 == n ? '\n' : ' ');
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
