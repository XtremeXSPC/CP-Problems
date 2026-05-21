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

struct Entry {
  I32 val;
  I32 dist;
  bool operator<(const Entry& o) const {
    return val < o.val;
  }
};

void solve() {
  INT(n);
  VecI32 a(n);
  FOR(i, n) IN(a[i]);

  Vec<Entry> elem;
  elem.reserve(70 * n);

  FOR(i, n) {
    I32 x = a[i];
    if (x == 1) {
      elem.push_back({1, 0});
      elem.push_back({2, 1});
    } else {
      I32 dist = 0;
      while (x != 1) {
        elem.push_back({x, dist});
        if (x % 2 == 0) {
          x /= 2;
        } else {
          x += 1;
        }
        dist++;
      }
      elem.push_back({1, dist});
    }
  }

  sort(ALL(elem));

  I32 min_total = Limits<I32>::max();

  for (I32 i = 0; i < sz(elem); ) {
    I32 j = i;
    I32 cur_val = elem[i].val;
    I32 cur_sum = 0;
    while (j < sz(elem) && elem[j].val == cur_val) {
      cur_sum += elem[j].dist;
      j++;
    }
    if (j - i == n) {
      min_total = min(min_total, cur_sum);
    }
    i = j;
  }

  OUT(min_total);
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
