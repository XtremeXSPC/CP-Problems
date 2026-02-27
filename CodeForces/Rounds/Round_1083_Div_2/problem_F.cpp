#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_FAST_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct DSU {
  VecI32 parent;
  VecI32 size;
  VecI64 comp_weight;
  VecBool has_outer;

  explicit DSU(const I32 n) :
      parent(as<Size>(n)), size(as<Size>(n), 1), comp_weight(as<Size>(n), 0),
      has_outer(as<Size>(n), false) {
    FOR(i, n) parent[i] = i;
  }

  [[nodiscard]] auto find(const I32 x) -> I32 {
    if (parent[x] == x) return x;
    return parent[x] = find(parent[x]);
  }

  void unite(const I32 a, const I32 b) {
    I32 ra = find(a);
    I32 rb = find(b);
    if (ra == rb) return;

    if (size[ra] < size[rb]) std::swap(ra, rb);
    parent[rb] = ra;
    size[ra] += size[rb];
    comp_weight[ra] += comp_weight[rb];
    has_outer[ra] = has_outer[ra] || has_outer[rb];
  }
};

void solve() {
  INT(n, m);

  Vec2D<I64> w(as<Size>(n - 1), VecI64(as<Size>(m)));
  Vec2D<I64> v(as<Size>(n), VecI64(as<Size>(m - 1)));
  FOR(i, n - 1) FOR(j, m) IN(w[i][j]);
  FOR(i, n) FOR(j, m - 1) IN(v[i][j]);

  VecStr p(as<Size>(n - 1));
  VecStr q(as<Size>(n));
  FOR(i, n - 1) IN(p[i]);
  FOR(i, n) IN(q[i]);

  const I32 rows = n - 1;
  const I32 cols = m - 1;
  const I32 cells = rows * cols;

  const auto cell_id = [&](const I32 r, const I32 c) -> I32 {
    return r * cols + c + 1;
  };

  DSU dsu(cells + 1);
  dsu.has_outer[0] = true;

  FOR(r, rows) {
    FOR(c, cols) {
      const I32 id = cell_id(r, c);
      dsu.comp_weight[id] = w[r][c] - w[r][c + 1] + v[r][c] - v[r + 1][c];
    }
  }

  // Constraints from vertical streets between (i, j) and (i + 1, j): p[i][j].
  FOR(r, rows) {
    FOR(c, m) {
      if (p[r][c] != '0') continue;
      const I32 left_face = (c == 0 ? 0 : cell_id(r, c - 1));
      const I32 right_face = (c == m - 1 ? 0 : cell_id(r, c));
      dsu.unite(left_face, right_face);
    }
  }

  // Constraints from horizontal streets between (i, j) and (i, j + 1): q[i][j].
  FOR(r, n) {
    FOR(c, cols) {
      if (q[r][c] != '0') continue;
      const I32 up_face = (r == 0 ? 0 : cell_id(r - 1, c));
      const I32 down_face = (r == n - 1 ? 0 : cell_id(r, c));
      dsu.unite(up_face, down_face);
    }
  }

  I64 answer = 0;
  FOR(node, cells + 1) {
    if (dsu.find(node) != node) continue;
    if (dsu.has_outer[node]) continue;
    if (dsu.comp_weight[node] > 0) answer += dsu.comp_weight[node];
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
