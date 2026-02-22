#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

// Type aliases:
using Path = Vec<I32>;
using Edge = Pair<I32, I32>;

void solve() {
  INT(n);

  constexpr I32 MAXK = 1 << 30;
  UnorderedMap<I32, Path> cache; // k -> path (empty vec = nonexistent).

  auto ask = [&](I32 k) -> const Path& {
    if (auto it = cache.find(k); it != cache.end()) return it->second;
    OUT("?", k);
    FLUSH();

    I32 len = 0;
    if (!(cin >> len)) exit(0);
    if (len == -1) {
      return cache[k]; // insert empty = nonexistent.
    }
    Path& path = cache[k];
    path.resize(len);
    FOR(i, len) IN(path[i]);
    return path;
  };

  auto is_prefix = [](const Path& path, const Path& pref) -> bool {
    if (path.empty() || path.size() < pref.size()) return false;
    FOR(i, as<I32>(pref.size())) {
      if (path[i] != pref[i]) return false;
    }
    return true;
  };

  Vec<bool> done(n + 1, false);
  Vec<I32> cnt(n + 1, 0);
  Vec<Vec<bool>> has_edge(n + 1, Vec<bool>(n + 1, false));
  Vec<Edge> edges;

  struct Frame {
    I32 v;
    Path pref;
    I32 cursor;
    I32 total;
  };

  // Iterative DFS to avoid deep recursion.
  auto dfs = [&](I32 root, const Path& root_pref, I32 start_idx) -> I32 {
    Vec<Frame> stk;
    stk.push_back({root, root_pref, start_idx, 1});

    while (!stk.empty()) {
      auto& fr = stk.back();

      if (done[fr.v]) {
        I32 res = cnt[fr.v];
        stk.pop_back();
        if (!stk.empty()) {
          stk.back().total += res;
          stk.back().cursor += res;
        }
        continue;
      }

      I32 nxt = fr.cursor + 1;
      if (nxt <= MAXK) {
        const Path& path = ask(nxt);
        if (is_prefix(path, fr.pref)) {
          I32 to = path[as<I32>(fr.pref.size())];
          if (!has_edge[fr.v][to]) {
            has_edge[fr.v][to] = true;
            edges.eb(fr.v, to);
          }
          Path child_pref = fr.pref;
          child_pref.push_back(to);
          stk.push_back({to, std::move(child_pref), nxt, 1});
          continue;
        }
      }

      done[fr.v] = true;
      cnt[fr.v] = fr.total;
      I32 res = fr.total;
      stk.pop_back();
      if (!stk.empty()) {
        stk.back().total += res;
        stk.back().cursor += res;
      }
    }

    return cnt[root];
  };

  cache[1] = {1}; // Path(1) is always [1].
  I32 pos = 1;
  FOR(u, 1, n + 1) {
    if (!done[u]) {
      dfs(u, Path{as<I32>(u)}, pos);
    }
    pos += cnt[u];
  }

  OUT("!", as<I32>(edges.size()));
  for (const auto& [a, b] : edges) OUT(a, b);
  FLUSH();
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
#endif

  I32 T = 0;
  if (!(cin >> T)) return 0;
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
