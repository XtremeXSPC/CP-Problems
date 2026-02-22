#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

void solve() {
  LL(n, m);
  using Edge = Pair<I32, I32>;

  auto encode = [](I32 u, I32 v) -> U64 {
    if (u > v) swap(u, v);
    return (as<U64>(u) << 32) | as<U64>(as<U32>(v));
  };

  auto minimal_odd_k_for_m = [&](I64 need) -> I32 {
    I64 d = as<I64>(sqrtl(1.0L + 8.0L * as<F80>(need)));
    while ((d + 1) * (d + 1) <= 1 + 8 * need) ++d;
    while (d * d > 1 + 8 * need) --d;
    I64 k = (1 + d) / 2;
    while (k * (k - 1) / 2 < need) ++k;
    if ((k & 1) == 0) ++k;
    return as<I32>(k);
  };

  auto build_circulant = [&](I32 k, I32 deg_even) -> Vec<Edge> {
    const I32 half = deg_even / 2;
    Vec<Edge> edges;
    edges.reserve(as<Size>(as<I64>(k) * deg_even / 2));
    FOR(d, 1, half + 1) {
      FOR(i, 1, k + 1) {
        I32 j = i + d;
        if (j > k) j -= k;
        if (i < j) {
          edges.push_back({i, j});
        } else {
          edges.push_back({j, i});
        }
      }
    }
    return edges;
  };

  auto construct_on_k = [&](I32 k, I64 need) -> Vec<Edge> {
    const I64 q = (2 * need) / k;
    const I64 r = (2 * need) % k;

    if ((q & 1) == 0) {
      Vec<Edge> edges = build_circulant(k, as<I32>(q));
      const I32 shift = k / 2;
      edges.reserve(edges.size() + as<Size>(r / 2));
      FOR(i, 1, as<I32>(r / 2) + 1) edges.push_back({i, i + shift});
      return edges;
    }

    const Vec<Edge> base = build_circulant(k, as<I32>(q + 1));
    const I32 limit = k - as<I32>(r);
    UnorderedSet<U64> removed;
    removed.reserve(as<Size>(limit));
    for (I32 i = 1; i < limit; i += 2) removed.insert(encode(i, i + 1));

    Vec<Edge> edges;
    edges.reserve(base.size());
    for (auto [u, v] : base) {
      if (!removed.contains(encode(u, v))) edges.push_back({u, v});
    }
    return edges;
  };

  auto construct_odd_total = [&](I32 n_odd, I64 need) -> Vec<Edge> {
    const I32 k = minimal_odd_k_for_m(need);
    (void)n_odd;
    return construct_on_k(k, need);
  };

  auto edges_for_m12 = [&]() -> Vec<Edge> {
    Vec<Edge> edges;
    edges.reserve(12);
    FOR(i, 1, 7) {
      FOR(j, i + 1, 7) {
        if (i <= 3 && j <= 3) continue;
        edges.push_back({i, j});
      }
    }
    return edges;
  };

  auto complement_edges = [&](I32 n_all, const Vec<Edge>& small) -> Vec<Edge> {
    UnorderedSet<U64> s;
    s.reserve(small.size() * 2 + 1);
    for (auto [u, v] : small) s.insert(encode(u, v));

    Vec<Edge> edges;
    const I64 total = as<I64>(n_all) * (n_all - 1) / 2;
    edges.reserve(as<Size>(total - as<I64>(small.size())));

    FOR(u, 1, n_all + 1) {
      FOR(v, u + 1, n_all + 1) {
        if (u == n_all || v == n_all) {
          edges.push_back({u, v});
        } else if (!s.contains(encode(u, v))) {
          edges.push_back({u, v});
        }
      }
    }
    return edges;
  };

  auto construct_dense_comp12 = [&](I32 n_all) -> Vec<Edge> {
    UnorderedSet<U64> missing;
    missing.reserve(32);
    FOR(i, 1, 7) {
      FOR(j, i + 1, 7) {
        if (i <= 3 && j <= 3) continue;
        missing.insert(encode(as<I32>(i), as<I32>(j)));
      }
    }

    Vec<Edge> edges;
    const I64 total = as<I64>(n_all) * (n_all - 1) / 2;
    edges.reserve(as<Size>(total - 12));
    FOR(u, 1, n_all + 1) {
      FOR(v, u + 1, n_all + 1) {
        if (!missing.contains(encode(as<I32>(u), as<I32>(v)))) {
          edges.push_back({as<I32>(u), as<I32>(v)});
        }
      }
    }
    return edges;
  };

  auto bad_small = [](I64 x) -> bool { return x == 1 || x == 2 || x == 4 || x == 6; };
  auto bad_comp = [](I64 x) -> bool { return x == 0 || x == 1 || x == 2 || x == 4 || x == 6; };

  if (bad_small(m)) {
    OUT("No");
    return;
  }

  Vec<Edge> edges;
  if (m == 12) {
    edges = edges_for_m12();
  } else if ((n & 1) == 1) {
    edges = construct_odd_total(as<I32>(n), m);
  } else {
    const I64 threshold = (n - 1) * (n - 2) / 2;
    if (m <= threshold) {
      edges = construct_odd_total(as<I32>(n - 1), m);
    } else {
      const I64 m_comp = n * (n - 1) / 2 - m;
      if (m_comp == 12) {
        edges = construct_dense_comp12(as<I32>(n));
      } else
      if (bad_comp(m_comp)) {
        OUT("No");
        return;
      } else {
        const Vec<Edge> edges_comp = construct_odd_total(as<I32>(n - 1), m_comp);
        edges = complement_edges(as<I32>(n), edges_comp);
      }
    }
  }

  OUT("Yes");
  for (auto [u, v] : edges) OUT(u, v);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
  I32 T = 0;
  if (!(cin >> T)) return 0;
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
