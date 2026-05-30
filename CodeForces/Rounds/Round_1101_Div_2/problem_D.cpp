#ifndef CP_TEMPLATE_PROFILE_STRICT
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
  I32 n;
  IN(n);
  VecI32 a(n + 1);
  bool VL = true;
  FOR(i, 1, n + 1) {
    IN(a[i]);
    if (a[i] >= i) VL = false;
  }

  if (!VL) {
    OUT("NO");
    return;
  }

  OUT("YES");

  Vec<Array<I32, 3>> ans;
  VecI32 p(n + 1, 1);

  auto dfs = [&](auto& self, I32 k, const VecI32& g) -> void {
    if (k == 0) return;
    if (p[k] == g[k]) {
      self(self, k - 1, g);
    } else {
      I32 u = p[k];
      I32 v = g[k];
      I32 w = 6 - u - v;

      VecI32 ng = g;
      for (I32 i = k - a[k]; i < k; ++i) ng[i] = u;
      for (I32 i = 1; i < k - a[k]; ++i) ng[i] = w;

      self(self, k - 1, ng);

      ans.push_back({k, u, v});
      p[k] = v;

      self(self, k - 1, g);
    }
  };

  VecI32 init_g(n + 1, 3);
  dfs(dfs, n, init_g);

  OUT(ans.size());
  for (auto& m : ans) {
    OUT(m[0], m[1], m[2]);
  }
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Stopwatch timer;
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
