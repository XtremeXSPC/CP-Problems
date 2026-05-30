#ifndef CP_TEMPLATE_PROFILE_STRICT
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

void solve() {
  I32 n, k;
  IN(n, k);

  VecI32 Fix(n + 1, -1);
  Vec2D<char> rq_b(n + 1, Vec<char>(n + 1, 0));
  Vec2D<char> rq_a(n + 1, Vec<char>(n + 1, 0));

  bool VL = true;

  FOR(i, k) {
    I32 s;
    IN(s);
    I32 r, c;
    IN(r, c);
    String p = "";
    if (s > 1) {
      IN(p);
    }

    I32 k_idx = (s + 1) / 2;

    Vec<char> Ta(k_idx, 0), Tb(k_idx, 0), CHK(k_idx, 0);
    I32 cr = r, cc = c;
    I32 r_main = -1;

    FOR(step, len(p) + 1) {
      I32 d = cr + cc - 1;
      if (d == n) r_main = cr;

      if (step < len(p)) {
        char ch = p[step];
        if (d < n) {
          I32 x = n - d;
          if (ch == 'D') Tb[x] = 1;
          else Ta[x] = 1;
          CHK[x] = 1;
        } else {
          I32 y = d - n + 1;
          if (ch == 'R') {
            if (CHK[y] && Ta[y]) VL = false;
            Tb[y] = 1;
          } else {
            if (CHK[y] && Tb[y]) VL = false;
            Ta[y] = 1;
          }
        }
        if (ch == 'D') cr++;
        else cc++;
      }
    }

    if (!VL) continue;

    I32 bc = 0;
    FOR(x, 1, k_idx) {
      if (Tb[x]) {
        rq_b[k_idx][x] = 1;
        bc++;
      }
      if (Ta[x]) {
        rq_a[k_idx][x] = 1;
      }
    }

    I32 pos = r_main - 1;
    Fix[k_idx] = pos;

    I32 G = pos - bc;
    if (G < 0 || G > n - k_idx) VL = false;
  }

  if (!VL) {
    OUT(0);
    return;
  }

  VecI32 occ(n, 0);
  FOR(x, 1, n + 1) {
    if (Fix[x] != -1) {
      if (occ[Fix[x]]) {
        OUT(0);
        return;
      }
      occ[Fix[x]] = 1;
    }
  }

  VecI32 L(n + 1, 0);
  VecI32 R(n + 1, n - 1);

  FOR(x, 1, n + 1) {
    FOR(m, x + 1, n + 1) {
      if (Fix[m] != -1) {
        if (rq_b[m][x]) R[x] = min(R[x], Fix[m] - 1);
        if (rq_a[m][x]) L[x] = max(L[x], Fix[m] + 1);
      }
    }
    if (Fix[x] != -1) {
      if (Fix[x] < L[x] || Fix[x] > R[x]) {
        OUT(0);
        return;
      }
      L[x] = Fix[x];
      R[x] = Fix[x];
    }
    if (L[x] > R[x]) {
      OUT(0);
      return;
    }
  }

  struct Interval {
    I32 l, r, e;
    auto operator<=>(const Interval&) const = default;
  };

  Vec<Interval> U;
  FOR(x, 1, n + 1) {
    if (Fix[x] == -1) {
      I32 e = 0;
      FOR(p, L[x], R[x] + 1) {
        if (!occ[p]) e++;
      }
      if (e == 0) {
        OUT(0);
        return;
      }
      U.push_back({L[x], R[x], e});
    }
  }

  ranges::sort(U, [](const Interval& a, const Interval& b) {
    if (a.e != b.e) return a.e < b.e;
    return (a.r - a.l) < (b.r - b.l);
  });

  I64 ans = 1;

  FOR(i, sz(U)) {
    I32 cnt = 0;
    FOR(j, i) {
      if (U[j].l >= U[i].l && U[j].r <= U[i].r) {
        cnt++;
      }
    }
    I32 choices = U[i].e - cnt;
    if (choices <= 0) {
      OUT(0);
      return;
    }
    ans = (ans * choices) % MOD;
  }

  OUT(ans);
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
