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

constexpr I32 LOG = 21;

auto first_block_cost(const I32 len) -> I64 {
  const I64 k = len - 1;
  return (k / 2) * (k - k / 2);
}

auto block_cost(const I32 pref_len, const I32 len) -> I64 {
  if (len <= pref_len) return 1LL * pref_len * len;

  const I64 all = pref_len + len;
  return (all / 2) * (all - all / 2);
}

void solve() {
  INT(n, q);
  VecI32 p(n + 1);
  FOR(i, 1, n + 1) IN(p[i]);

  VecI32 next_smaller(n + 2, n + 1);
  VecI32 block_len(n + 2, 0);
  VecI32 st;
  st.reserve(n);

  ROF(i, 1, n + 1) {
    while (!st.empty() && p[st.back()] > p[i]) st.pop_back();
    next_smaller[i] = st.empty() ? n + 1 : st.back();
    block_len[i]    = next_smaller[i] - i;
    st.eb(i);
  }

  Vec2D<I32> up(LOG, VecI32(n + 2, n + 1));
  Vec2D<I64> sum_pos_len(LOG, VecI64(n + 2, 0));
  Vec2D<I32> mx_need(LOG, VecI32(n + 2, Limits<I32>::min() / 4));

  FOR(i, 1, n + 1) {
    up[0][i] = next_smaller[i];
    sum_pos_len[0][i] = 1LL * i * block_len[i];
    mx_need[0][i]     = as<I32>(block_len[i] - i);
  }

  up[0][n + 1] = n + 1;

  FOR(k, 1, LOG) {
    FOR(i, 1, n + 2) {
      const I32 mid = up[k - 1][i];
      up[k][i] = up[k - 1][mid];
      sum_pos_len[k][i] = sum_pos_len[k - 1][i] + sum_pos_len[k - 1][mid];
      mx_need[k][i]     = std::max(mx_need[k - 1][i], mx_need[k - 1][mid]);
    }
  }

  auto jump_light = [&](I32 cur, const I32 l, const I32 lim, I64& ans) -> I32 {
    FORD(k, LOG - 1, 0) {
      const I32 to = up[k][cur];
      if (to > lim) continue;
      if (mx_need[k][cur] > -l) continue;

      ans += sum_pos_len[k][cur] - 1LL * l * (to - cur);
      cur = to;
    }
    return cur;
  };

  FOR(q) {
    INT(l, r);

    const I32 first_end = std::min(next_smaller[l], r + 1);
    I64 ans = first_block_cost(first_end - l);
    I32 cur = first_end;

    while (cur <= r) {
      const I32 pref_len = cur - l;
      const I32 full_end = next_smaller[cur];

      if (full_end > r + 1) {
        ans += block_cost(pref_len, r + 1 - cur);
        break;
      }

      if (block_len[cur] > pref_len) {
        ans += block_cost(pref_len, block_len[cur]);
        cur = full_end;
        continue;
      }

      cur = jump_light(cur, l, r + 1, ans);
    }

    OUT(ans);
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
