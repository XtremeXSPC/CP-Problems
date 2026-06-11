#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_MACROS
#define NEED_TIMER
#define NEED_CONTAINERS
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

struct Solver {
  I32 n = 0, m = 0;
  Vec<PairI32> edges;
  Vec2D<I32> adj;
  VecI32 X, Y, used;

  struct Arm {
    VecI32 v;
  };

  struct Side {
    I32 at   = 0;
    I32 root = 0;
    array<Arm, 2> arm;
    I32 arms = 0;
    I32 pick = -1;

    [[nodiscard]] PairI32 interval(I32 p) const {
      I32 up = 0, down = 0;
      if (arms == 0) {
        up = down = 0;
      } else if (arms == 1) {
        up   = (p == 0 ? arm[0].v.size() : 0);
        down = (p == 1 ? arm[0].v.size() : 0);
      } else if (arms == 2) {
        up   = arm[p].v.size();
        down = arm[1 - p].v.size();
      }
      return {at - down, at + up};
    }
  };

  bool valid() {
    Set<PairI32> seen;
    FOR(v, 1, n + 1) {
      if (X[v] < 0 || Y[v] < -200000 || Y[v] > 200000)
        return false;
      if (!seen.insert({X[v], Y[v]}).second)
        return false;
    }

    for (auto [u, v] : edges) {
      const I32 dx = abs(X[u] - X[v]);
      const I32 dy = abs(Y[u] - Y[v]);
      if (dx + dy != 1)
        return false;
    }
    return true;
  }

  bool run(I32 a, I32 b) {
    X.assign(n + 1, -1);
    Y.assign(n + 1, 0);
    used.assign(n + 1, 0);

    I32 cnt = 0;
    array<I32, 2> cur{a, b};
    array<I32, 2> prev{0, 0};
    I32 y = -200000;

    if (a) {
      X[a] = 0;
      Y[a] = y;
      used[a] = 1;
      ++cnt;
    }
    if (b) {
      X[b] = 1;
      Y[b] = y;
      used[b] = 1;
      ++cnt;
    }

    while (cnt < n) {
      if (++y > 200000)
        return false;

      array<I32, 2> nxt{0, 0};
      FOR(c, 2) {
        const I32 v = cur[c];
        if (!v)
          continue;

        VecI32 open;
        for (const I32 to : adj[v]) {
          if (to == prev[c])
            continue;
          if (to == cur[1 - c])
            continue;

          if (used[to])
            return false;
          open.push_back(to);
        }
        if (open.size() > 1)
          return false;
        if (!open.empty())
          nxt[c] = open[0];
      }

      if (!nxt[0] && !nxt[1])
        return false;

      FOR(c, 2) {
        const I32 u = nxt[c];
        if (!u || nxt[1 - c])
          continue;

        VecI32 cand;
        for (const I32 to : adj[u]) {
          if (to == cur[c])
            continue;
          if (used[to])
            return false;
          cand.push_back(to);
        }

        if (!cand.empty()) {
          sort(all(cand), [&](I32 p, I32 q) {
            return adj[p].size() < adj[q].size();
          });
          nxt[1 - c] = cand[0];
        }
      }

      FOR(c, 2) {
        const I32 v = nxt[c];
        if (!v)
          continue;
        if (used[v])
          return false;

        X[v] = c;
        Y[v] = y;
        used[v] = 1;
        ++cnt;
      }

      prev = cur;
      cur  = nxt;
    }

    return valid();
  }

  bool path_from(I32 v, I32 p, const VecI32& cyc, VecI32& out) {
    while (v) {
      if (cyc[v])
        return false;
      out.push_back(v);

      I32 nxt = 0;
      for (const I32 to : adj[v]) {
        if (to == p)
          continue;
        if (nxt)
          return false;
        nxt = to;
      }
      p = v;
      v = nxt;
    }
    return true;
  }

  bool collect_arm(I32 v, I32 p, const VecI32& on, VecI32& out) {
    while (v) {
      if (on[v])
        return false;
      out.push_back(v);

      I32 nxt = 0;
      for (const I32 to : adj[v]) {
        if (to == p || on[to])
          continue;
        if (nxt)
          return false;
        nxt = to;
      }
      p = v;
      v = nxt;
    }
    return true;
  }

  bool try_spine(const VecI32& path) {
    VecI32 on(n + 1, 0);
    FOR(i, path.size())
    on[path[i]] = 1;

    Vec<Side> side;
    FOR(i, path.size()) {
      const I32 v = path[i];
      Side s;
      s.at = i;

      for (const I32 to : adj[v]) {
        if (on[to])
          continue;
        if (s.root)
          return false;
        s.root = to;
      }
      if (!s.root)
        continue;

      VecI32 nexts;
      for (const I32 to : adj[s.root]) {
        if (to == v)
          continue;
        nexts.push_back(to);
      }
      if (nexts.size() > 2)
        return false;

      if (nexts.empty()) {
        s.arms = 0;
      } else {
        FOR(j, nexts.size()) {
          VecI32 tail;
          if (!collect_arm(nexts[j], s.root, on, tail))
            return false;
          s.arm[j].v = std::move(tail);
        }
        s.arms = nexts.size();
      }
      side.push_back(s);
    }

    sort(all(side), [](const Side& a, const Side& b) {
      return a.at < b.at;
    });

    I32 last = -1'000'000'000;
    I32 lo = 0, hi = path.size() - 1;
    for (Side& s : side) {
      I32 best   = -1;
      I32 best_r = 1'000'000'000;
      const I32 ways = (s.arms <= 1 ? 2 : 2);

      FOR(p, ways) {
        auto [l, r] = s.interval(p);
        if (l <= last)
          continue;
        if (r < best_r) {
          best_r = r;
          best   = p;
        }
      }
      if (best < 0)
        return false;

      s.pick      = best;
      auto [l, r] = s.interval(best);
      last = r;
      lo   = min(lo, l);
      hi   = max(hi, r);
    }

    const I32 base = -200000 - lo;
    if (base + hi > 200000)
      return false;

    X.assign(n + 1, -1);
    Y.assign(n + 1, 0);
    FOR(i, path.size()) {
      X[path[i]] = 0;
      Y[path[i]] = base + i;
    }

    auto put = [&](const VecI32& a, I32 y0, I32 dir) {
      FOR(i, a.size()) {
        X[a[i]] = 1;
        Y[a[i]] = y0 + dir * (i + 1);
      }
    };

    for (const Side& s : side) {
      const I32 y0 = base + s.at;
      X[s.root]    = 1;
      Y[s.root]    = y0;

      if (s.arms == 1) {
        put(s.arm[0].v, y0, s.pick == 0 ? 1 : -1);
      } else if (s.arms == 2) {
        put(s.arm[s.pick].v, y0, 1);
        put(s.arm[1 - s.pick].v, y0, -1);
      }
    }

    return valid();
  }

  bool try_tree_path() {
    if (m != n - 1)
      return false;

    VecI32 leaves;
    FOR(v, 1, n + 1) {
      if (adj[v].size() <= 1)
        leaves.push_back(v);
    }
    if (leaves.empty())
      return false;

    VecI32 par(n + 1), q;
    q.reserve(n);

    auto build_path = [&](I32 src, I32 dst) -> VecI32 {
      fill(all(par), -1);
      q.clear();
      q.push_back(src);
      par[src] = 0;
      for (I32 i = 0; i < q.size(); ++i) {
        const I32 v = q[i];
        for (const I32 to : adj[v]) {
          if (par[to] != -1)
            continue;
          par[to] = v;
          q.push_back(to);
        }
      }

      VecI32 path;
      if (par[dst] == -1)
        return path;
      for (I32 v = dst; v; v = par[v])
        path.push_back(v);
      reverse(all(path));
      return path;
    };

    auto far_from = [&](I32 src) -> I32 {
      fill(all(par), -1);
      q.clear();
      q.push_back(src);
      par[src] = 0;
      for (I32 i = 0; i < q.size(); ++i) {
        const I32 v = q[i];
        for (const I32 to : adj[v]) {
          if (par[to] != -1)
            continue;
          par[to] = v;
          q.push_back(to);
        }
      }
      return q.back();
    };

    Vec<PairI32> todo;
    if (n <= 2500 && leaves.size() <= 120) {
      FOR(i, leaves.size()) {
        FOR(j, i + 1, leaves.size())
        todo.push_back({leaves[i], leaves[j]});
      }
    } else {
      const I32 a = far_from(1);
      const I32 b = far_from(a);
      todo.push_back({a, b});

      constexpr I32 LIM    = 80;
      const I32 leaf_count = leaves.size();
      const I32 cnt = min(leaf_count, LIM);
      FOR(i, cnt) {
        todo.push_back({a, leaves[i]});
        todo.push_back({b, leaves[i]});
      }
    }

    for (auto [a, b] : todo) {
      if (a == b)
        continue;
      VecI32 path = build_path(a, b);
      if (!path.empty() && try_spine(path))
        return true;
    }

    return false;
  }

  bool try_unicyclic() {
    if (m != n)
      return false;

    VecI32 deg(n + 1), cyc(n + 1, 1);
    Queue<I32> q;
    FOR(v, 1, n + 1) {
      deg[v] = adj[v].size();
      if (deg[v] == 1)
        q.push(v);
    }

    while (!q.empty()) {
      const I32 v = q.front();
      q.pop();
      cyc[v] = 0;
      for (const I32 to : adj[v]) {
        if (--deg[to] == 1)
          q.push(to);
      }
    }

    VecI32 verts;
    FOR(v, 1, n + 1) {
      if (cyc[v])
        verts.push_back(v);
    }
    const I32 L = verts.size();
    if (L < 4 || L % 2)
      return false;

    VecI32 ord;
    ord.reserve(L);
    I32 first = verts[0], prev = 0, cur = first;
    while (true) {
      ord.push_back(cur);
      I32 nxt = 0;
      for (const I32 to : adj[cur]) {
        if (!cyc[to] || to == prev)
          continue;
        nxt = to;
        break;
      }
      if (!nxt)
        return false;
      prev = cur;
      cur  = nxt;
      if (cur == first)
        break;
      if (ord.size() > L)
        return false;
    }
    if (ord.size() != L)
      return false;

    Vec2D<I32> arm(n + 1);
    FOR(v, 1, n + 1) {
      if (!cyc[v])
        continue;
      for (const I32 to : adj[v]) {
        if (cyc[to])
          continue;
        if (!arm[v].empty())
          return false;
        if (!path_from(to, v, cyc, arm[v]))
          return false;
      }
    }

    const I32 k = L / 2;
    FOR(i, L) {
      Set<I32> corner;
      const I32 a = ord[i];
      const I32 b = ord[(i + 1) % L];
      const I32 c = ord[(i - k + 1 + L) % L];
      const I32 d = ord[(i + k) % L];
      corner.insert(a);
      corner.insert(b);
      corner.insert(c);
      corner.insert(d);

      bool ok = true;
      FOR(v, 1, n + 1) {
        if (cyc[v] && !arm[v].empty() && !corner.count(v))
          ok = false;
      }
      if (!ok)
        continue;

      X.assign(n + 1, -1);
      Y.assign(n + 1, 0);

      const I32 la   = arm[a].size();
      const I32 lb   = arm[b].size();
      const I32 low  = max(la, lb);
      const I32 base = -200000 + low;

      FOR(t, k) {
        const I32 left  = ord[(i - t + L) % L];
        const I32 right = ord[(i + 1 + t) % L];
        X[left]  = 0;
        Y[left]  = base + t;
        X[right] = 1;
        Y[right] = base + t;
      }

      auto put_down = [&](I32 v) {
        FOR(j, arm[v].size()) {
          const I32 u = arm[v][j];
          X[u] = X[v];
          Y[u] = Y[v] - j - 1;
        }
      };

      auto put_up = [&](I32 v) {
        FOR(j, arm[v].size()) {
          const I32 u = arm[v][j];
          X[u] = X[v];
          Y[u] = Y[v] + j + 1;
        }
      };

      put_down(a);
      put_down(b);
      put_up(c);
      put_up(d);

      if (valid())
        return true;
    }

    return false;
  }

  bool solve_one() {
    if (n == 1) {
      X.assign(n + 1, 0);
      Y.assign(n + 1, -200000);
      return true;
    }

    FOR(v, 1, n + 1) {
      if (adj[v].size() > 3)
        return false;
    }

    Vec<PairI32> starts;
    FOR(v, 1, n + 1) {
      if (adj[v].size() <= 1)
        starts.push_back({v, 0});
    }
    for (auto [u, v] : edges) {
      starts.push_back({u, v});
      starts.push_back({v, u});
    }

    constexpr I32 LIM = 2000;
    if (n > LIM && starts.size() > 200)
      starts.resize(200);

    for (auto [a, b] : starts) {
      if (run(a, b))
        return true;
    }
    if (try_tree_path())
      return true;
    if (try_unicyclic())
      return true;
    return false;
  }
};

void solve() {
  INT(n, m);

  Solver S;
  S.n = n;
  S.m = m;
  S.edges.reserve(m);
  S.adj.assign(n + 1, {});

  FOR(m) {
    INT(u, v);
    S.edges.push_back({u, v});
    S.adj[u].push_back(v);
    S.adj[v].push_back(u);
  }

  if (!S.solve_one()) {
    OUT("No");
    return;
  }

  OUT("Yes");
  FOR(i, 1, n + 1) {
    cout << S.X[i] << ' ' << S.Y[i] << '\n';
  }
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Stopwatch timer;
#endif

  I32 T;
  if (!(cin >> T))
    return 0;
  FOR(T)
  solve();

  return 0;
}

//===----------------------------------------------------------------------===//
