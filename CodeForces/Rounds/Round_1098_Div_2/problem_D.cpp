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
  INT(n);
  struct Point {
    I32 x, y;
  };
  Vec<Point> pts(n);
  VecI32 uniq_y;
  uniq_y.reserve(n);

  FOR(i, 0, n) {
    cin >> pts[i].x >> pts[i].y;
    uniq_y.push_back(pts[i].y);
  }

  sort(all(uniq_y));
  uniq_y.erase(unique(all(uniq_y)), uniq_y.end());

  sort(all(pts), [](const Point& a, const Point& b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
  });

  VecI32 min_y, max_y;
  min_y.reserve(n);
  max_y.reserve(n);

  for (I32 i = 0; i < n;) {
    I32 j = i;
    I32 cur_min = pts[i].y;
    I32 cur_max = pts[i].y;
    while (j < n && pts[j].x == pts[i].x) {
      if (pts[j].y < cur_min) cur_min = pts[j].y;
      if (pts[j].y > cur_max) cur_max = pts[j].y;
      j++;
    }
    min_y.push_back(cur_min);
    max_y.push_back(cur_max);
    i = j;
  }

  I32 Cx = min_y.size();
  if (Cx <= 1) {
    OUT(0);
    return;
  }

  VecI32 pref_min(Cx), pref_max(Cx);
  pref_min[0] = min_y[0];
  pref_max[0] = max_y[0];
  FOR(i, 1, Cx) {
    pref_min[i] = min(pref_min[i - 1], min_y[i]);
    pref_max[i] = max(pref_max[i - 1], max_y[i]);
  }

  VecI32 suff_min(Cx), suff_max(Cx);
  suff_min[Cx - 1] = min_y[Cx - 1];
  suff_max[Cx - 1] = max_y[Cx - 1];
  for (I32 i = Cx - 2; i >= 0; --i) {
    suff_min[i] = min(suff_min[i + 1], min_y[i]);
    suff_max[i] = max(suff_max[i + 1], max_y[i]);
  }

  I64 ans = 0;
  FOR(i, 0, Cx - 1) {
    I32 m1 = max(pref_min[i], suff_min[i + 1]);
    I32 m2 = min(pref_max[i], suff_max[i + 1]);
    if (m1 < m2) {
      auto it1 = lower_bound(all(uniq_y), m1);
      auto it2 = lower_bound(all(uniq_y), m2);
      ans += (it2 - it1);
    }
  }

  OUT(ans);
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
