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
  INT(r, g, b);

  Array<Pair<I32, char>, 3> freq = {
      Pair<I32, char>{r, 'R'},
      Pair<I32, char>{g, 'G'},
      Pair<I32, char>{b, 'B'},
  };

  std::sort(all(freq), [](const Pair<I32, char>& lhs, const Pair<I32, char>& rhs) {
    if (lhs.first != rhs.first) {
      return lhs.first > rhs.first;
    }
    return lhs.second < rhs.second;
  });

  const I32 v1  = freq[0].first;
  const I32 v2  = freq[1].first;
  const I32 v3  = freq[2].first;
  const char c1 = freq[0].second;
  const char c2 = freq[1].second;
  const char c3 = freq[2].second;

  const I32 total = r + g + b;
  const I32 len   = std::min(total, 2 * (v2 + v3) + 1);

  const I32 even_slots = (len + 1) / 2;
  const I32 odd_slots  = len / 2;

  const I32 use_c1 = std::min(v1, even_slots);
  const I32 use_c2 = std::min(v2, len - use_c1);

  String answer(len, '?');

  const I32 c3_even_prefix = even_slots - use_c1;
  FOR(j, even_slots) {
    answer[2 * j] = (j < c3_even_prefix ? c3 : c1);
  }

  FOR(j, odd_slots) {
    answer[2 * j + 1] = (j < use_c2 ? c2 : c3);
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
