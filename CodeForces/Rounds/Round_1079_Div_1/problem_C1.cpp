#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Type Aliases */

using Edge  = Pair<I32, I32>;
using Path  = Vec<I32>;
using MPath = std::optional<Path>;

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);

  auto lex_compare = [&](const Path& a, const Path& b) -> I32 {
    const I32 common = std::min<I32>(a.size(), b.size());
    FOR(i, common) {
      if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    if (a.size() == b.size()) return 0;
    return a.size() < b.size() ? -1 : 1;
  };

  auto cmp_path_vs_seq = [&](const MPath& path, const Path& seq) -> I32 {
    if (!path.has_value()) return 1;
    return lex_compare(path.value(), seq);
  };

  std::map<I32, MPath> cache;
  auto ask = [&](I32 k) -> const MPath& {
    if (cache.contains(k)) return cache[k];

    OUT("?", k);
    FLUSH();

    I32 len = 0;
    if (!(std::cin >> len)) std::exit(0);
    if (len == -1) std::exit(0);
    if (len == 0) {
      cache[k] = std::nullopt;
      return cache[k];
    }

    Path path(len);
    FOR(i, len) IN(path[i]);
    cache[k] = path;
    return cache[k];
  };

  auto find_path_count = [&]() -> I32 {
    I32 lo = 1;
    I32 hi = (1 << 30) + 1;
    while (lo + 1 < hi) {
      const I32 mid = lo + (hi - lo) / 2;
      if (!ask(mid).has_value()) hi = mid;
      else lo = mid;
    }
    return lo;
  };

  auto lower_bound_path = [&](I32 path_count, const Path& seq) -> I32 {
    I32 lo = 1;
    I32 hi = path_count + 1;
    while (lo < hi) {
      const I32 mid = lo + (hi - lo) / 2;
      if (cmp_path_vs_seq(ask(mid), seq) < 0) lo = mid + 1;
      else hi = mid;
    }
    return lo;
  };

  const I32 path_count = find_path_count();
  Vec<I32> start(n + 2, 0), end(n + 2, 0);

  start[1] = 1;
  FOR(u, 1, n) {
    end[u] = lower_bound_path(path_count, Path{as<I32>(u + 1)});
    start[u + 1] = end[u];
  }
  end[n] = path_count + 1;

  const I32 sentinel = n + 1;
  Vec<Edge> edges;
  Vec<Vec<bool>> seen(n + 1, Vec<bool>(n + 1, false));

  FOR(u, 1, n + 1) {
    I32 cur = start[u] + 1; // Skip singleton path [u].
    while (cur < end[u]) {
      const MPath& path_opt = ask(cur);
      if (!path_opt.has_value()) std::exit(0);
      const Path& path = path_opt.value();
      if (path.size() < 2) std::exit(0);

      const I32 v = path[1];
      if (!seen[u][v]) {
        seen[u][v] = true;
        edges.eb(u, v);
      }

      cur = lower_bound_path(path_count, Path{as<I32>(u), v, sentinel});
    }
  }

  OUT("!", static_cast<I32>(edges.size()));
  for (const auto& [u, v] : edges) OUT(u, v);
  FLUSH();
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  I32 T = 0;
  if (!(std::cin >> T)) return 0;
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
