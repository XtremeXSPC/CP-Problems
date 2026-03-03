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

using namespace std;

void solve() {
  INT(n);

  Vec2D<I32> adj(n + 1);
  VecI32 deg(n + 1, 0);
  VecPairI32 edges;
  edges.reserve(n - 1);

  FOR(n - 1) {
    INT(u, v);
    adj[u].push_back(v);
    adj[v].push_back(u);
    ++deg[u];
    ++deg[v];
    edges.emplace_back(u, v);
  }

  I64 answer {0};
  for (const auto& [u, v] : edges) {
    answer = max(answer, as<I64>(deg[u]) + deg[v] - 2);
  }

  FOR (i, 1, n + 1) {
    ranges::sort(adj[i]);
  }

  VecI32 nodes(n);
  iota(nodes.begin(), nodes.end(), 1);
  ranges::sort(nodes, [&](const I32 lhs, const I32 rhs) {
    if (deg[lhs] != deg[rhs]) return deg[lhs] > deg[rhs];
    return lhs < rhs;
  });

  FOR(i, n) {
    const I32 u = nodes[i];
    if (i + 1 < n &&
        as<I64>(deg[u]) + deg[nodes[i + 1]] - 1 <= answer) {
      break;
    }

    FOR (j, i + 1, n) {
      const I32 v = nodes[j];
      const I64 candidate = as<I64>(deg[u]) + deg[v] - 1;

      if (candidate <= answer) break;
      if (binary_search(adj[u].begin(), adj[u].end(), v)) continue;

      answer = candidate;
      break;
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
