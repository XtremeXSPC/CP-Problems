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
  INT(n, m);

  Vec2D<I32> graph(n + 1);
  FOR(_, m) {
    INT(u, v);
    graph[u].eb(v);
    graph[v].eb(u);
  }

  VecI32 color(n + 1, -1);
  I64 answer = 0;

  FOR(start, 1, n + 1) {
    if (color[start] != -1) {
      continue;
    }

    Queue<I32> q;
    q.push(start);
    color[start] = 0;

    I32 cnt0 = 1;
    I32 cnt1 = 0;
    bool is_bipartite = true;

    while (!q.empty()) {
      const I32 u = q.front();
      q.pop();

      for (I32 v : graph[u]) {
        if (color[v] == -1) {
          color[v] = color[u] ^ 1;
          if (color[v] == 0) {
            ++cnt0;
          } else {
            ++cnt1;
          }
          q.push(v);
        } else if (color[v] == color[u]) {
          is_bipartite = false;
        }
      }
    }

    if (is_bipartite) {
      answer += std::max(cnt0, cnt1);
    }
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
