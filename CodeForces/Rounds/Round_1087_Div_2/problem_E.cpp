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
#include "modules/strings/ZAlgorithm.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, q);
  STR(s);

  static VecI32 dp;
  static VecI32 count;
  static VecI32 remove_head;
  static VecI32 remove_next;
  static VecI32 remove_val;

  const I32 need_size = n + 5;
  if (sz(dp) < need_size) {
    dp.resize(need_size);
    count.resize(need_size);
    remove_head.resize(need_size);
    remove_next.resize(need_size);
    remove_val.resize(need_size);
  }

  FOR(_, q) {
    INT(l, r);
    const I32 len = r - l + 1;

    const String sub = s.substr(l - 1, len);
    const VecI32 z   = z_algorithm(sub);

    std::fill(count.begin(), count.begin() + len + 1, 0);
    std::fill(remove_head.begin(), remove_head.begin() + len + 2, -1);

    dp[0] = 0;
    I32 max_active = 0;
    I32 pool_size  = 0;
    I64 answer     = 0;

    FOR(i, 1, len + 1) {
      const I32 j   = i - 1;
      const I32 end = j + z[j];
      if (end >= i) {
        const I32 value = dp[j];
        count[value]++;
        chmax(max_active, value);

        ++pool_size;
        remove_val[pool_size]  = value;
        remove_next[pool_size] = remove_head[end + 1];
        remove_head[end + 1]   = pool_size;
      }

      I32 node = remove_head[i];
      while (node != -1) {
        count[remove_val[node]]--;
        node = remove_next[node];
      }

      while (max_active > 0 && count[max_active] == 0) {
        --max_active;
      }

      dp[i] = max_active + 1;
      answer += dp[i];
    }

    OUT(answer);
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
