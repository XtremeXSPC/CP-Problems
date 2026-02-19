#define NEED_IO

#include "templates/Base.hpp"
#include "templates/Constants.hpp"
#include "templates/Math.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, k);

  Vec<Pair<I32, I32>> edges;
  edges.reserve(n - 1);
  FOR(n - 1) {
    INT(x, y);
    edges.pb({x, y});
  }

  Vec<I32> a(n);
  IN(a);

  Vec<I32> b(k);
  IN(b);

  auto build_coord = [&]() -> Pair<UnorderedMap<I32, I32>, I32> {
    std::array<I32, 30> basis{};
    basis.fill(0);
    for (I32 x : b) {
      I32 y = x;
      for (I32 bit = 29; bit >= 0; --bit) {
        if (((y >> bit) & 1) == 0) continue;
        if (basis[bit] != 0) {
          y ^= basis[bit];
        } else {
          basis[bit] = y;
          break;
        }
      }
    }

    Vec<I32> basis_vec;
    basis_vec.reserve(30);
    FOR(bit, 30) {
      if (basis[bit] != 0) basis_vec.pb(basis[bit]);
    }

    const I32 d = as<I32>(basis_vec.size());
    const I32 m = 1 << d;
    Vec<I32> vals(m, 0);
    FOR(i, d) {
      const I32 bb = basis_vec[i];
      const I32 step = 1 << i;
      FOR(mask, step) vals[mask | step] = vals[mask] ^ bb;
    }

    UnorderedMap<I32, I32> coord;
    coord.reserve(as<std::size_t>(m) * 2);
    FOR(mask, m) coord[vals[mask]] = mask;
    return {std::move(coord), m};
  };

  auto [coord, m] = build_coord();

  Vec<I32> ib(m, 0);
  for (I32 x : b) ib[coord[x]] = 1;
  Vec<I32> j = ib;

  auto fwht = [&](Vec<I32>& v) -> void {
    for (I32 h = 1; h < m; h <<= 1) {
      const I32 step = h << 1;
      for (I32 i = 0; i < m; i += step) {
        FOR(off, h) {
          const I32 u = v[i + off];
          const I32 w = v[i + off + h];
          I32 x = u + w;
          if (x >= MOD) x -= MOD;
          I32 y = u - w;
          if (y < 0) y += MOD;
          v[i + off] = x;
          v[i + off + h] = y;
        }
      }
    }
  };
  fwht(j);

  Vec<I32> parity(m, 0);
  FOR(i, 1, m) parity[i] = parity[i >> 1] ^ (i & 1);

  const I64 inv_m = mod_pow<I64>(m, MOD - 2, MOD);

  Vec<Vec<I32>> adj(n);
  for (auto [x, y] : edges) {
    --x;
    --y;
    adj[x].pb(y);
    adj[y].pb(x);
  }

  Vec<I32> parent(n, -1), order;
  order.reserve(n);
  Vec<I32> st = {0};
  parent[0] = 0;
  while (!st.empty()) {
    const I32 v = st.back();
    st.pop_back();
    order.pb(v);
    for (I32 u : adj[v]) {
      if (u == parent[v]) continue;
      parent[u] = v;
      st.pb(u);
    }
  }

  Vec<Vec<I32>> children(n);
  FOR(v, 1, n) children[parent[v]].pb(v);

  Vec<I32> xor_sub = a;
  for (I32 i = n - 1; i >= 1; --i) {
    const I32 v = order[i];
    xor_sub[parent[v]] ^= xor_sub[v];
  }

  Vec<I32> c(n, 0);
  Vec<bool> has_d(n, false);
  Vec<Vec<I32>> d(n);

  for (I32 idx = n - 1; idx >= 0; --idx) {
    const I32 v = order[idx];
    bool has_cur = false;
    Vec<I32> dv;

    for (I32 u : children[v]) {
      const bool has_du = has_d[u];
      const I32 cu = c[u];

      if (cu == 0 && !has_du) continue;

      if (!has_cur) {
        has_cur = true;
        dv.assign(m, 1);
      }

      if (cu == 0) {
        if (has_du) {
          FOR(s, m) {
            dv[s] = as<I32>((as<I64>(dv[s]) * d[u][s]) % MOD);
          }
        }
      } else {
        const I32 tu = coord[xor_sub[u]];
        if (!has_du) {
          FOR(s, m) {
            I32 factor;
            if (parity[s & tu] == 0) {
              factor = 1 + cu;
              if (factor >= MOD) factor -= MOD;
            } else {
              factor = 1 - cu;
              if (factor < 0) factor += MOD;
            }
            dv[s] = as<I32>((as<I64>(dv[s]) * factor) % MOD);
          }
        } else {
          FOR(s, m) {
            I32 factor;
            if (parity[s & tu] == 0) {
              factor = d[u][s] + cu;
              if (factor >= MOD) factor -= MOD;
            } else {
              factor = d[u][s] - cu;
              if (factor < 0) factor += MOD;
            }
            dv[s] = as<I32>((as<I64>(dv[s]) * factor) % MOD);
          }
        }
      }

      has_d[u] = false;
      d[u].clear();
      c[u] = 0;
    }

    has_d[v] = has_cur;
    d[v] = std::move(dv);

    const auto it = coord.find(xor_sub[v]);
    if (it == coord.end()) {
      c[v] = 0;
      continue;
    }

    const I32 tv = it->second;
    I64 total = 0;
    if (!has_d[v]) {
      FOR(s, m) {
        const I32 term = j[s];
        total += (parity[s & tv] == 0 ? term : -term);
      }
    } else {
      FOR(s, m) {
        const I32 term = as<I32>((as<I64>(d[v][s]) * j[s]) % MOD);
        total += (parity[s & tv] == 0 ? term : -term);
      }
    }

    total %= MOD;
    if (total < 0) total += MOD;
    c[v] = as<I32>((total * inv_m) % MOD);
  }

  OUT(c[0]);
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
