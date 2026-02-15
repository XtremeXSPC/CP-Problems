#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct Tri {
  I32 x1, y1, x2, y2, x3, y3;
};

auto patch_9x9() -> Vec<Tri> {
  return {
      {8, 2, 9, 1, 9, 2}, {7, 1, 7, 2, 8, 1}, {8, 4, 9, 3, 9, 4},
      {7, 3, 7, 4, 8, 3}, {8, 8, 9, 8, 9, 9}, {7, 8, 7, 9, 8, 9},
      {8, 6, 8, 7, 9, 7}, {8, 5, 9, 5, 9, 6}, {1, 8, 1, 9, 2, 8},
      {2, 9, 3, 8, 3, 9}, {5, 1, 5, 2, 6, 1}, {5, 3, 6, 2, 6, 3},
      {3, 1, 4, 1, 4, 2}, {4, 8, 4, 9, 5, 9}, {5, 8, 6, 8, 6, 9},
      {1, 1, 1, 2, 2, 1}, {1, 6, 1, 7, 2, 6}, {2, 7, 3, 6, 3, 7},
      {1, 5, 2, 4, 2, 5}, {1, 3, 1, 4, 2, 3}, {2, 2, 3, 2, 3, 3},
      {3, 4, 4, 3, 4, 4}, {3, 5, 4, 5, 4, 6}, {4, 7, 5, 6, 5, 7},
      {5, 4, 5, 5, 6, 4}, {6, 5, 6, 6, 7, 5}, {6, 7, 7, 6, 7, 7},
  };
}

void add_block_3x2(Vec<Tri>& out, I32 x, I32 y) {
  out.pb({x, y, x, y + 1, x + 1, y});
  out.pb({x + 1, y + 1, x + 2, y + 1, x + 2, y});
}

void add_block_2x3(Vec<Tri>& out, I32 x, I32 y) {
  out.pb({x, y, x + 1, y, x, y + 1});
  out.pb({x + 1, y + 1, x, y + 2, x + 1, y + 2});
}

void solve() {
  INT(n);
  const I32 N = 3 * n;

  Vec<Tri> ans;

  if (n == 1) {
    ans.pb({1, 1, 1, 2, 2, 1});
    ans.pb({2, 3, 3, 2, 3, 3});
  } else if (n % 2 == 0) {
    FOR(x, 1, N + 1, 3) FOR(y, 1, N + 1, 2) add_block_3x2(ans, x, y);
  } else {
    static const Vec<Tri> kPatch9 = patch_9x9();
    ans.insert(ans.end(), all(kPatch9));

    FOR(x, 10, N + 1, 2) FOR(y, 1, 10, 3) add_block_2x3(ans, x, y);

    FOR(x, 1, N + 1, 3) FOR(y, 10, N + 1, 2) add_block_3x2(ans, x, y);
  }

  OUT(sz(ans));
  FOR(i, sz(ans)) {
    const auto& t = ans[i];
    OUT(t.x1, t.y1, t.x2, t.y2, t.x3, t.y3);
  }
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
