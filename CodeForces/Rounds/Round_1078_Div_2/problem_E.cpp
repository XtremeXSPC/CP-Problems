#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n, m);
  const I32 total = n * m;

  Vec<I64> a(total, 0);
  FOR(idx, total) {
    LL(v);
    a[idx] = v;
  }

  Vec<I64> f(total, 0);
  FOR(i, n) {
    const I32 row_base = i * m;
    FOR(j, m) {
      const I32 idx = row_base + j;
      const I64 val = a[idx];
      if (i == 0 && j == 0) {
        f[idx] = val;
      } else if (i == 0) {
        f[idx] = val + f[idx - 1];
      } else if (j == 0) {
        f[idx] = val + f[idx - m];
      } else {
        f[idx] = val + std::max(f[idx - m], f[idx - 1]);
      }
    }
  }

  Vec<I64> g(total, 0);
  const I32 last_i = n - 1;
  const I32 last_j = m - 1;
  for (I32 i = last_i; i >= 0; --i) {
    const I32 row_base = i * m;
    for (I32 j = last_j; j >= 0; --j) {
      const I32 idx = row_base + j;
      const I64 val = a[idx];
      if (i == last_i && j == last_j) {
        g[idx] = val;
      } else if (i == last_i) {
        g[idx] = val + g[idx + 1];
      } else if (j == last_j) {
        g[idx] = val + g[idx + m];
      } else {
        g[idx] = val + std::max(g[idx + m], g[idx + 1]);
      }
    }
  }

  constexpr I64 NEG = -(1LL << 62);
  constexpr I64 POS_INF = (1LL << 62);

  const I32 layers = n + m - 1;
  Vec<I64> best1(layers, NEG), best2(layers, NEG);
  Vec<I32> pos1(layers, -1);

  FOR(i, n) {
    const I32 row_base = i * m;
    FOR(j, m) {
      const I32 idx = row_base + j;
      const I32 k = i + j;
      const I64 s_val = f[idx] + g[idx] - a[idx];
      if (s_val > best1[k]) {
        best2[k] = best1[k];
        best1[k] = s_val;
        pos1[k] = idx;
      } else if (s_val == best1[k]) {
        best2[k] = best1[k];
      } else if (s_val > best2[k]) {
        best2[k] = s_val;
      }
    }
  }

  I64 ans = POS_INF;
  FOR(i, n) {
    const I32 row_base = i * m;
    FOR(j, m) {
      const I32 idx = row_base + j;
      const I32 k = i + j;
      const I64 avoid = (pos1[k] != idx ? best1[k] : best2[k]);
      const I64 through = f[idx] + g[idx] - 3 * a[idx];
      const I64 cur = std::max(avoid, through);
      ans = std::min(ans, cur);
    }
  }

  std::cout << ans << '\n';
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
