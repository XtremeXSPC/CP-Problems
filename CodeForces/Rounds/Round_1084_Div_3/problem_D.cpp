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

auto solve() -> void {
  INT(n, x, y);
  VecI32 p(n);
  IN(p);

  VecI32 outer;
  outer.reserve(n - (y - x));
  FOR(i, 0, x) outer.push_back(p[i]);
  FOR(i, y, n) outer.push_back(p[i]);

  I64 min_idx = x;
  FOR(i, x + 1, y) {
    if (p[i] < p[min_idx]) min_idx = i;
  }
  const I32 block_min = p[min_idx];

  I64 insert_pos = sz(outer);
  FOR(i, 0, sz(outer)) {
    if (outer[i] > block_min) {
      insert_pos = i;
      break;
    }
  }

  VecI32 answer;
  answer.reserve(n);

  FOR(i, 0, insert_pos) answer.push_back(outer[i]);
  FOR(i, min_idx, y) answer.push_back(p[i]);
  FOR(i, x, min_idx) answer.push_back(p[i]);
  FOR(i, insert_pos, sz(outer)) answer.push_back(outer[i]);

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
