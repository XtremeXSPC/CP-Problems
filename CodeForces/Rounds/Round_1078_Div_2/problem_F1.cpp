#define NEED_IO

#include "templates/Base.hpp"
#include "templates/Constants.hpp"
#define Timer CP_MATH_TIMER
#include "templates/Math.hpp"
#undef Timer

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, k);

  Vec<Vec<I32>> adj(n + 1);
  FOR(n - 1) {
    INT(v, u);
    adj[v].push_back(u);
    adj[u].push_back(v);
  }

  Vec<I32> a(n + 1);
  FOR(v, 1, n + 1) IN(a[v]);

  Vec<I32> b(k);
  IN(b);

  auto build_space = [&]() -> std::tuple<I32, Vec<I32>, std::unordered_map<I32, I32>> {
    Vec<I32> basis;
    for (I32 x : b) {
      for (I32 y : basis) {
        if ((x ^ y) < x) x ^= y;
      }
      if (x > 0) {
        basis.push_back(x);
        std::sort(basis.begin(), basis.end(), std::greater<>());
      }
    }

    const I32 sz = 1 << static_cast<I32>(basis.size());
    Vec<I32> val(sz, 0);
    std::unordered_map<I32, I32> id;
    id.reserve(static_cast<std::size_t>(sz) * 2);

    FOR(msk, sz) {
      I32 x = 0;
      FOR(bit, static_cast<I32>(basis.size())) {
        if (msk & (1 << bit)) x ^= basis[bit];
      }
      val[msk] = x;
      id[x] = msk;
    }
    return {sz, std::move(val), std::move(id)};
  };

  auto [sz, val, id] = build_space();
  std::unordered_set<I32> in_b;
  in_b.reserve(static_cast<std::size_t>(k) * 2);
  for (I32 x : b) in_b.insert(x);

  auto fwht = [&](Vec<I32>& f, bool inv) -> void {
    for (I32 len = 1; len < sz; len <<= 1) {
      for (I32 i = 0; i < sz; i += (len << 1)) {
        FOR(j, len) {
          const I32 x = f[i + j];
          const I32 y = f[i + j + len];
          I32 s = x + y;
          if (s >= MOD) s -= MOD;
          I32 d = x - y;
          if (d < 0) d += MOD;
          f[i + j] = s;
          f[i + j + len] = d;
        }
      }
    }
    if (inv) {
      const I64 inv_sz = mod_pow<I64>(sz, MOD - 2, MOD);
      FOR(i, sz) f[i] = static_cast<I32>((static_cast<I64>(f[i]) * inv_sz) % MOD);
    }
  };

  Vec<I32> par(n + 1, -1), ord;
  ord.reserve(n);
  Vec<I32> st = {1};
  par[1] = 0;
  while (!st.empty()) {
    const I32 u = st.back();
    st.pop_back();
    ord.push_back(u);
    for (I32 v : adj[u]) {
      if (v == par[u]) continue;
      par[v] = u;
      st.push_back(v);
    }
  }

  Vec<I32> post = ord;
  std::reverse(post.begin(), post.end());

  Vec<I32> sx(n + 1, 0);
  for (I32 u : post) {
    I32 x = a[u];
    for (I32 v : adj[u]) {
      if (v == par[u]) continue;
      x ^= sx[v];
    }
    sx[u] = x;
  }

  Vec<I32> dp(static_cast<std::size_t>(n + 1) * static_cast<std::size_t>(sz), 0);
  auto row = [&](I32 u) -> I32* {
    return dp.data() + static_cast<std::size_t>(u) * static_cast<std::size_t>(sz);
  };

  for (I32 u : post) {
    Vec<I32> cur(sz, 1);

    for (I32 v : adj[u]) {
      if (v == par[u]) continue;

      I32* d = row(v);
      Vec<I32> nxt(sz);
      std::memcpy(nxt.data(), d, static_cast<std::size_t>(sz) * sizeof(I32));

      I32 add = 0;
      FOR(msk, sz) {
        if (in_b.contains(sx[v] ^ val[msk])) {
          add += d[msk];
          if (add >= MOD) add -= MOD;
        }
      }

      auto it = id.find(sx[v]);
      if (it != id.end()) {
        nxt[it->second] += add;
        if (nxt[it->second] >= MOD) nxt[it->second] -= MOD;
      }

      fwht(nxt, false);
      FOR(msk, sz) {
        cur[msk] = static_cast<I32>((static_cast<I64>(cur[msk]) * nxt[msk]) % MOD);
      }
    }

    fwht(cur, true);
    std::memcpy(row(u), cur.data(), static_cast<std::size_t>(sz) * sizeof(I32));
  }

  I32 ans = 0;
  I32* root = row(1);
  FOR(msk, sz) {
    if (in_b.contains(sx[1] ^ val[msk])) {
      ans += root[msk];
      if (ans >= MOD) ans -= MOD;
    }
  }
  OUT(ans);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
  I32 T = 0;
  if (!(std::cin >> T)) return 0;
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
