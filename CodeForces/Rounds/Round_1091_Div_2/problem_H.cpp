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

struct Rule {
  I32 x;
  I32 y;
  I64 w;

  auto operator<(const Rule& other) const -> bool {
    if (x != other.x) return x < other.x;
    return y < other.y;
  }
};

auto state_id(const Vec2D<I32>& ch, const VecI32& par, I32 u, I32 x) -> I32 {
  if (x == 0) return 0;
  if (par[u] != 0 && x == par[u]) return 1;

  const auto& cur = ch[u];
  const auto  it  = lower_bound(cur.begin(), cur.end(), x);

#ifdef LOCAL
  my_assert(it != cur.end() && *it == x);
#endif

  return as<I32>(it - cur.begin()) + 1 + (par[u] != 0);
}

auto small_path(I32 u, I32 v, const VecI32& par, const VecI32& dep) -> VecI32 {
  VecI32 left, right;
  I32    x = u;
  I32    y = v;
  I32    d = 0;

  while (dep[x] > dep[y]) {
    left.push_back(x);
    x = par[x];
    ++d;
    if (d > 3) return {};
  }

  while (dep[y] > dep[x]) {
    right.push_back(y);
    y = par[y];
    ++d;
    if (d > 3) return {};
  }

  while (x != y) {
    left.push_back(x);
    right.push_back(y);
    x = par[x];
    y = par[y];
    d += 2;
    if (d > 3) return {};
  }

  left.push_back(x);
  for (auto it = right.rbegin(); it != right.rend(); ++it) {
    left.push_back(*it);
  }
  return left;
}

void solve() {
  INT(n);
  const I32 m = 2 * n;

  VecI64 w(n);
  IN(w);

  Vec<PairI32> pos(n + 1, {0, 0});
  for (I32 i = 1; i <= m; ++i) {
    INT(x);
    if (pos[x].first == 0) {
      pos[x].first = i;
    } else {
      pos[x].second = i;
    }
  }

  Vec2D<I32> adj(m + 1);
  for (I32 i = 1; i < m; ++i) {
    INT(u, v);
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  VecI32 par(m + 1), dep(m + 1), ord;
  ord.reserve(m);

  VecI32 que = {1};
  for (I32 head = 0; head < as<I32>(que.size()); ++head) {
    const I32 u = que[head];
    ord.push_back(u);
    for (const I32 v : adj[u]) {
      if (v == par[u]) continue;
      par[v] = u;
      dep[v] = dep[u] + 1;
      que.push_back(v);
    }
  }

  Vec2D<I32> ch(m + 1);
  for (I32 v = 2; v <= m; ++v) ch[par[v]].push_back(v);
  for (I32 u = 1; u <= m; ++u) sort(ch[u].begin(), ch[u].end());

  Vec2D<Rule> raw(m + 1);

  const auto add_rule = [&](I32 u, I32 v, I32 su, I32 sv, I64 val) {
    if (par[v] == u) {
      raw[v].push_back({su, sv, val});
      return;
    }

#ifdef LOCAL
    my_assert(par[u] == v);
#endif

    raw[u].push_back({sv, su, val});
  };

  for (I32 i = 1; i <= n; ++i) {
    const I32 u   = pos[i].first;
    const I32 v   = pos[i].second;
    const I64 val = w[i - 1];

    const VecI32 path = small_path(u, v, par, dep);
    if (path.empty()) continue;

    const I32 len = as<I32>(path.size()) - 1;
    if (len == 1) {
      add_rule(path[0], path[1], 0, 0, val);
      add_rule(path[0], path[1], path[1], path[0], val);
    } else if (len == 2) {
      add_rule(path[1], path[0], path[2], 0, val);
      add_rule(path[1], path[2], path[0], 0, val);
    } else if (len == 3) {
      add_rule(path[1], path[2], path[0], path[3], val);
    }
  }

  for (I32 u = 2; u <= m; ++u) {
    auto& cur = raw[u];
    sort(cur.begin(), cur.end());

    Vec<Rule> next;
    for (const auto& rule : cur) {
      if (!next.empty() && next.back().x == rule.x && next.back().y == rule.y) {
        next.back().w += rule.w;
      } else {
        next.push_back(rule);
      }
    }
    cur.swap(next);
  }

  Vec2D<I64> dp(m + 1);

  for (I32 it = m - 1; it >= 0; --it) {
    const I32 u = ord[it];
    const I32 s = 1 + as<I32>(ch[u].size()) + (par[u] != 0);

    I64    sum = 0;
    VecI64 add(s);

    for (const I32 v : ch[u]) {
      const auto& dv = dp[v];

      I64 base = 0;
      for (I32 i = 0; i < as<I32>(dv.size()); ++i) {
        if (i == 1) continue;
        base = max(base, dv[i]);
      }

      I64 take = dv[1];

      const auto& cur = raw[v];
      for (I32 i = 0; i < as<I32>(cur.size());) {
        const I32 x = cur[i].x;
        I64 best    = base;

        while (i < as<I32>(cur.size()) && cur[i].x == x) {
          const I32 y = cur[i].y;
          if (x == v && y == u) {
            take += cur[i].w;
          } else {
            const I32 id = state_id(ch, par, v, y);
            best = max(best, dv[id] + cur[i].w);
          }
          ++i;
        }

        if (x != v) {
          const I32 id = state_id(ch, par, u, x);
          add[id] += best - base;
        }
      }

      sum += base;
      add[state_id(ch, par, u, v)] += take - base;
    }

    dp[u].assign(s, sum);
    for (I32 i = 0; i < s; ++i) dp[u][i] += add[i];
  }

  OUT(*max_element(dp[1].begin(), dp[1].end()));
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
