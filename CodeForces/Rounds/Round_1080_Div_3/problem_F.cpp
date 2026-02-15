#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct Parabola {
  I64 a, b, c;
  I32 id;
};

auto dominates(const Parabola& u, const Parabola& v) -> bool {
  // true iff u(x) < v(x) for every real x.
  if (u.a == v.a) {
    if (u.b != v.b) return false;
    return u.c < v.c;
  }
  if (u.a > v.a) return false;

  const I64 A = v.a - u.a;
  const I64 B = v.b - u.b;
  const I64 C = v.c - u.c;
  const __int128 disc = (__int128)B * B - (__int128)4 * A * C;
  return disc < 0;
}

void solve() {
  INT(n);

  Vec<Parabola> f(n);
  FOR(i, n) {
    IN(f[i].a, f[i].b, f[i].c);
    f[i].id = i;
  }

  Vec<Parabola> v = f;
  std::ranges::sort(v, [](const Parabola& x, const Parabola& y) {
    if (x.a != y.a) return x.a < y.a;
    if (x.b != y.b) return x.b < y.b;
    return x.c < y.c;
  });

  Vec<I32> dp_down(n, 1), dp_up(n, 1);

  FOR(i, n) {
    FOR(j, i) {
      if (dominates(v[j], v[i])) {
        dp_down[i] = std::max(dp_down[i], (I32)(dp_down[j] + 1));
      }
    }
  }

  FOR_R(i, n) {
    FOR(j, i + 1, n) {
      if (dominates(v[i], v[j])) {
        dp_up[i] = std::max(dp_up[i], (I32)(dp_up[j] + 1));
      }
    }
  }

  Vec<I32> ans(n);
  FOR(i, n) {
    ans[v[i].id] = dp_down[i] + dp_up[i] - 1;
  }

  OUT(ans);
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
