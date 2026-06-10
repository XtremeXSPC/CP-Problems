#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_MACROS
#define NEED_TIMER
#define NEED_CORE
#define NEED_IO
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

constexpr I64 NEG = Limits<I64>::min() / 4;

struct Tab {
  I32 lo = 0;
  VecI64 a;

  [[nodiscard]] I32 hi() const { return lo + a.size() - 1; }

  [[nodiscard]] I64 get(I32 c) const {
    if (c < lo || c > hi()) return NEG;
    return a[c - lo];
  }
};

void solve() {
  INT(n, k);
  const I32 cap = k - 1;

  VecI64 w(n + 1);
  FOR(i, 1, n + 1) IN(w[i]);

  Vec2D<I32> adj(n + 1);
  FOR(n - 1) {
    INT(u, v);
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  auto conv = [&](const Tab& A, const Tab& B, I32 lo, I32 hi) -> Tab {
    Tab C{lo, VecI64(hi - lo + 1, NEG)};

    FOR(j, B.lo, B.hi() + 1) {
      const I64 bj = B.a[j - B.lo];
      if (bj == NEG) continue;

      const I32 from = max(lo, j + A.lo);
      const I32 to = min(hi, j + A.hi());
      FOR(c, from, to + 1) {
        const I64 av = A.a[c - j - A.lo];
        if (av == NEG) continue;
        C.a[c - lo] = max(C.a[c - lo], av + bj);
      }
    }
    return C;
  };

  auto close = [&](const Tab& A, I64 val, I32 lo, I32 hi) -> Tab {
    Tab B{lo, VecI64(hi - lo + 1, NEG)};

    FOR(c, lo, hi + 1) {
      I64 best = (c == 0 ? 0 : NEG);
      best = max(best, A.get(c));

      if (c >= 1) {
        const I64 prev = A.get(c - 1);
        if (prev != NEG) best = max(best, prev + val * c);
      }
      B.a[c - lo] = best;
    }
    return B;
  };

  VecI32 par(n + 1, 0), ord, sub(n + 1, 1);
  Vec2D<I32> ch(n + 1);
  ord.reserve(n);

  VecI32 st{1};
  par[1] = -1;
  while (!st.empty()) {
    const I32 u = st.back();
    st.pop_back();
    ord.push_back(u);

    for (const I32 v : adj[u]) {
      if (v == par[u]) continue;
      par[v] = u;
      ch[u].push_back(v);
      st.push_back(v);
    }
  }
  for (I32 id = n - 1; id > 0; --id) {
    sub[par[ord[id]]] += sub[ord[id]];
  }

  auto h_lo = [&](I32 u) -> I32 { return max(0, cap - sub[u] + 1); };
  auto h_hi = [&](I32 u) -> I32 { return min(n - sub[u], cap); };

  Vec<Tab> down(n + 1), up(n + 1);
  for (I32 id = n - 1; id >= 0; --id) {
    const I32 u = ord[id];
    Tab cur{0, {0}};
    I32 got = 0;
    for (const I32 v : ch[u]) {
      got += sub[v];
      cur = conv(cur, down[v], 0, min(got, cap));
    }
    down[u] = close(cur, w[u], 0, min(sub[u], cap));
  }

  VecI64 ans(n + 1);
  up[1] = Tab{0, {0}};

  for (const I32 u : ord) {
    const VecI32& kids = ch[u];
    const I32 m = kids.size();
    Tab all{0, {0}};

    if (m > 0) {
      Vec<Tab> seg(4 * m);
      VecI32 mass(4 * m, 0);

      auto build = [&](auto&& self, I32 p, I32 l, I32 r) -> void {
        if (l == r) {
          seg[p]  = down[kids[l]];
          mass[p] = sub[kids[l]];
          return;
        }
        const I32 mid = (l + r) / 2;
        self(self, 2 * p, l, mid);
        self(self, 2 * p + 1, mid + 1, r);
        mass[p] = mass[2 * p] + mass[2 * p + 1];
        seg[p]  = conv(seg[2 * p], seg[2 * p + 1], 0, min(mass[p], cap));
      };

      auto dfs = [&](auto&& self, I32 p, I32 l, I32 r, const Tab& out) -> void {
        if (l == r) {
          const I32 v = kids[l];
          up[v] = close(out, w[u], h_lo(v), h_hi(v));
          return;
        }
        const I32 mid = (l + r) / 2;
        self(self, 2 * p, l, mid,
             conv(out, seg[2 * p + 1], max(0, cap - mass[2 * p] - 1), cap));
        self(self, 2 * p + 1, mid + 1, r,
             conv(out, seg[2 * p], max(0, cap - mass[2 * p + 1] - 1), cap));
      };

      build(build, 1, 0, m - 1);

      Tab base{max(0, cap - mass[1] - 1), VecI64()};
      base.a.assign(cap - base.lo + 1, NEG);
      FOR(c, max(base.lo, up[u].lo), min(cap, up[u].hi()) + 1) {
        base.a[c - base.lo] = up[u].a[c - up[u].lo];
      }

      dfs(dfs, 1, 0, m - 1, base);
      all = move(seg[1]);
    }

    I64 best = NEG;
    FOR(j, max(all.lo, cap - up[u].hi()), min(all.hi(), cap - up[u].lo) + 1) {
      const I64 x = all.a[j - all.lo];
      const I64 y = up[u].a[cap - j - up[u].lo];
      if (x != NEG && y != NEG) best = max(best, x + y);
    }
    ans[u] = w[u] * k + best;
    up[u] = Tab{};
  }

  FOR(i, 1, n + 1) {
    cout << ans[i] << " \n"[i == n];
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
