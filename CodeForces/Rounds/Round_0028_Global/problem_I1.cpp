#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_FAST_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct Frame {
  I32 l;
  I32 r;
  I32 add;
  I32 state = 0;  // 0: enter, 1: resume after inner segment.
  I32 kind = 0;   // 1: LR, 2: RR, 3: LL.
  I32 child_max = 0;
  bool child_all = true;
};

[[nodiscard]] auto construct_good_array(const String& s, Vec<I32>& a) -> bool {
  const I32 n = as<I32>(s.size());
  a.assign(as<Size>(n), -1);

  bool feasible = true;
  Vec<Frame> stack;
  stack.reserve(as<Size>(n + 4));
  stack.push_back({0, n - 1, 0});

  auto resolve_current = [&](const I32 mx, const bool all_appear) {
    stack.pop_back();
    if (!stack.empty()) {
      stack.back().child_max = mx;
      stack.back().child_all = all_appear;
    }
  };

  while (!stack.empty()) {
    Frame& f = stack.back();

    if (f.state == 0) {
      if (f.l == f.r) {
        a[f.l] = f.add;
        resolve_current(f.add, true);
        continue;
      }

      const char cl = s[f.l];
      const char cr = s[f.r];

      if (cl == 'R' && cr == 'L') {
        for (I32 i = f.l; i <= f.r; ++i) a[i] = f.add + 1;
        resolve_current(f.add + 1, false);
        continue;
      }

      if (cl == 'L' && cr == 'R') {
        a[f.l] = f.add;
        a[f.r] = f.add;
        if (f.l + 1 == f.r) {
          resolve_current(f.add, true);
          continue;
        }
        f.state = 1;
        f.kind = 1;
        stack.push_back({f.l + 1, f.r - 1, f.add + 1});
        continue;
      }

      if (cl == 'R' && cr == 'R') {
        a[f.r] = f.add;
        if (f.l + 1 == f.r) {
          a[f.l] = f.add + 1;
          resolve_current(a[f.l], true);
          continue;
        }
        f.state = 1;
        f.kind = 2;
        stack.push_back({f.l + 1, f.r - 1, f.add + 1});
        continue;
      }

      a[f.l] = f.add;  // (cl, cr) == (L, L).
      if (f.l + 1 == f.r) {
        a[f.r] = f.add + 1;
        resolve_current(a[f.r], true);
        continue;
      }
      f.state = 1;
      f.kind = 3;
      stack.push_back({f.l + 1, f.r - 1, f.add + 1});
      continue;
    }

    const I32 inner_max = f.child_max;
    const bool inner_all = f.child_all;

    if (f.kind == 1) {  // LR
      resolve_current(inner_max, inner_all);
      continue;
    }
    if (f.kind == 2) {  // RR
      if (!inner_all) feasible = false;
      a[f.l] = inner_max + 1;
      resolve_current(a[f.l], true);
      continue;
    }

    if (!inner_all) feasible = false;  // LL
    a[f.r] = inner_max + 1;
    resolve_current(a[f.r], true);
  }

  return feasible;
}

void solve() {
  INT(n);
  STR(s);

  Vec<I32> a;
  if (!construct_good_array(s, a)) {
    OUT(-1);
    return;
  }

  OUT(a);
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
