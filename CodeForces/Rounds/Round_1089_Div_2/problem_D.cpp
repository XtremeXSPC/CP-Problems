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
  auto leaf_cnt = [](const String& s) -> I32 {
    I32 cnt = 0;
    FOR(i, sz(s) - 1) cnt += (s[i] == '(' && s[i + 1] == ')');
    return cnt;
  };

  auto neck = [](const String& s) -> I32 {
    const I32 n = sz(s);
    VecI32 st, match(n, -1);
    st.reserve(n / 2);
    FOR(i, n) {
      if (s[i] == '(') {
        st.eb(i);
      } else {
        const I32 j = st.back();
        st.pop_back();
        match[j] = i;
      }
    }

    I32 l = 0;
    I32 r = n - 1;
    I32 dep = 0;
    while (l < r && match[l] == r) {
      ++dep;
      ++l;
      --r;
    }
    return dep;
  };

  INT(n);
  STR(s);
  STR(t);

  YES(leaf_cnt(s) == leaf_cnt(t) && neck(s) == neck(t));
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
