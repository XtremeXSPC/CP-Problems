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

constexpr I32 INF = 1 << 29;

enum Tag : I32 { BAD, PATH, PINT, FD, FS, NIL };

struct Prof {
  Tag t = BAD;
  I32 a = 0, b = 0;
};

Prof bad() { return {BAD, 0, 0}; }
Prof path(I32 len) { return {PATH, len, 0}; }
Prof pint(I32 a, I32 b) { return {PINT, a, b}; }
Prof fd(I32 d) { return {FD, min(d, INF), 0}; }
Prof fs(I32 s) { return s == 0 ? fd(0) : Prof{FS, s, 0}; }
Prof nil() { return {NIL, 0, 0}; }

Prof shift(const Prof& p) {
  switch (p.t) {
  case PATH: return path(p.a + 1);
  case PINT: return fd(1);
  case FD: return fd(p.a + 1);
  case FS: return fs(p.a - 1);
  default: return bad();
  }
}

I32 free_top(const Prof& p) {
  switch (p.t) {
  case PATH: return INF;
  case FD: return p.a + 1;
  case PINT: return 1;
  default: return -1;
  }
}

struct Dec {
  I32 up   = -1;
  I32 side = 0;
  I32 len  = 0;
  bool both_path = false;
};

Prof merge2(const Prof& A, const Prof& B, Dec& dec) {
  if (A.t == BAD || B.t == BAD) return bad();
  if (A.t == PATH && B.t == PATH) {
    dec = {0, 0, 0, true};
    return pint(A.a, B.a);
  }

  I32 best = INF;
  Dec take;
  const Prof* p[2] = {&A, &B};

  FOR(up, 2) {
    const Prof& U = *p[up];
    const Prof& S = *p[1 - up];
    const I32 lim = free_top(U);
    if (lim < 0) continue;

    if (S.t == PATH) {
      if (S.a <= lim && 0 < best) {
        best = 0;
        take = {up, 0, 0, false};
      }
      if (S.a - 1 < best) {
        best = S.a - 1;
        take = {up, 1, 0, false};
      }
    } else if (S.t == PINT) {
      if (S.a <= lim - 1 && S.b < best) {
        best = S.b;
        take = {up, 2, S.a, false};
      }
      if (S.b <= lim - 1 && S.a < best) {
        best = S.a;
        take = {up, 2, S.b, false};
      }
    }
  }

  if (take.up < 0) return bad();
  dec = take;
  return fs(best);
}

Prof merge_all(const Vec<Prof>& ps, Dec& dec) {
  Vec<Prof> q;
  for (const Prof& p : ps) {
    if (p.t != NIL) q.push_back(p);
  }

  if (q.empty()) return path(1);
  if (isz(q) == 1) return shift(q[0]);
  if (isz(q) == 2) return merge2(q[0], q[1], dec);
  return bad();
}

bool top_one(const Prof& p) {
  return p.t == PATH || p.t == FD || p.t == PINT;
}

bool top_fit(const Prof& A, const Prof& B) {
  const bool hasA = A.t != NIL;
  const bool hasB = B.t != NIL;
  if (!hasA && !hasB) return true;
  if (!hasA) return top_one(B);
  if (!hasB) return top_one(A);
  if (A.t == PATH && B.t == PATH) return true;
  if (A.t == FD && B.t == PATH) return B.a <= A.a;
  if (B.t == FD && A.t == PATH) return A.a <= B.a;
  return false;
}

struct Solver {
  I32 n = 0, m = 0;
  Vec2D<I32> adj, cyc, att;
  VecI32 deg, par, dep, ord;
  VecI32 cid, pos, ext, entry;
  Vec<Prof> down, up;
  VecI32 X, Y;
  bool fail = false;

  Prof beyond_att(I32 v) {
    const I32 h = ext[v];
    return par[h] == v ? down[h] : up[v];
  }

  Prof beyond(I32 u, I32 v) {
    return par[v] == u ? down[v] : up[u];
  }

  void build() {
    par.assign(n + 1, 0);
    dep.assign(n + 1, -1);
    cid.assign(n + 1, -1);
    pos.assign(n + 1, 0);
    ord.clear();
    ord.reserve(n);

    VecI32 it(n + 1, 0), st{1};
    dep[1] = 0;

    while (!st.empty()) {
      const I32 v = st.back();
      if (it[v] == 0) ord.push_back(v);

      if (it[v] == isz(adj[v])) {
        st.pop_back();
        continue;
      }

      const I32 to = adj[v][it[v]++];
      if (to == par[v]) continue;

      if (dep[to] == -1) {
        dep[to] = dep[v] + 1;
        par[to] = v;
        st.push_back(to);
      } else if (dep[to] < dep[v]) {
        VecI32 vs;
        for (I32 x = v; x != to; x = par[x]) vs.push_back(x);
        vs.push_back(to);
        reverse(all(vs));

        const I32 id = isz(cyc);
        if (isz(vs) % 2) fail = true;
        FOR(i, isz(vs)) {
          if (cid[vs[i]] != -1) fail = true;
          cid[vs[i]] = id;
          pos[vs[i]] = i;
        }
        cyc.push_back(std::move(vs));
        entry.push_back(to);
      }
    }

    ext.assign(n + 1, 0);
    att.assign(isz(cyc), {});
    FOR(v, 1, n + 1) {
      if (cid[v] < 0) continue;

      const VecI32& vs = cyc[cid[v]];
      const I32 L = isz(vs);
      const I32 p = pos[v];
      const I32 prev = vs[(p - 1 + L) % L];
      const I32 next = vs[(p + 1) % L];

      for (const I32 to : adj[v]) {
        if (to != prev && to != next) ext[v] = to;
      }
      if (ext[v]) att[cid[v]].push_back(v);
    }
  }

  bool phi_fit(I32 c, I32 root, I32 dir, I32& s) {
    const VecI32& vs = cyc[c];
    const I32 L = isz(vs);
    const I32 k = L / 2;
    const I32 p = pos[root];
    const I32 low  = ((p + dir) % L + L) % L;
    const I32 top1 = ((p + dir * k) % L + L) % L;
    const I32 top2 = ((p + dir * (k + 1)) % L + L) % L;

    Prof A = nil(), B = nil();
    s = 0;

    for (const I32 v : att[c]) {
      if (v == root) continue;
      const I32 q = pos[v];
      const Prof prof = beyond_att(v);
      if (prof.t == BAD) return false;

      if (q == low) {
        if (prof.t != PATH) return false;
        s = prof.a;
      } else if (q == top1) {
        A = prof;
      } else if (q == top2) {
        B = prof;
      } else {
        return false;
      }
    }

    return top_fit(A, B);
  }

  Prof best_phi(I32 c, I32 root, I32* out = nullptr) {
    if (isz(att[c]) > 4) return bad();

    I32 best = INF, take = 0;
    FOR(d, 2) {
      const I32 dir = (d == 0 ? 1 : -1);
      I32 s = 0;
      if (phi_fit(c, root, dir, s) && s < best) {
        best = s;
        take = dir;
      }
    }

    if (take == 0) return bad();
    if (out) *out = take;
    return fs(best);
  }

  void calc() {
    down.assign(n + 1, bad());
    up.assign(n + 1, nil());
    Dec dec;

    for (I32 i = n - 1; i >= 0; --i) {
      const I32 v = ord[i];
      if (cid[v] >= 0) {
        if (entry[cid[v]] == v) down[v] = best_phi(cid[v], v);
        continue;
      }

      Vec<Prof> ps;
      for (const I32 to : adj[v]) {
        if (to != par[v]) ps.push_back(down[to]);
      }
      down[v] = merge_all(ps, dec);
    }

    FOR(i, 1, n) {
      const I32 v = ord[i];
      const I32 p = par[v];

      if (cid[v] >= 0 && cid[v] == (p ? cid[p] : -2)) {
        up[v] = bad();
        continue;
      }
      if (cid[p] >= 0) {
        up[v] = best_phi(cid[p], p);
        continue;
      }

      Vec<Prof> ps;
      for (const I32 to : adj[p]) {
        if (to != v && to != par[p]) ps.push_back(down[to]);
      }
      if (p != ord[0]) ps.push_back(up[p]);
      up[v] = merge_all(ps, dec);
    }
  }

  struct Job {
    I32 u, from, col, row, dir;
  };

  Vec<Job> jobs;

  void push(I32 u, I32 from, I32 col, I32 row, I32 dir) {
    jobs.push_back({u, from, col, row, dir});
  }

  void place_cycle(I32 c, I32 p, I32 d, I32 col, I32 row, I32 dir) {
    const VecI32& vs = cyc[c];
    const I32 L = isz(vs);
    const I32 k = L / 2;

    FOR(t, L) {
      const I32 v = vs[((p + d * t) % L + L) % L];
      if (t == 0) {
        X[v] = col;
        Y[v] = row;
      } else if (t <= k) {
        X[v] = 1 - col;
        Y[v] = row + (t - 1) * dir;
      } else {
        X[v] = col;
        Y[v] = row + (2 * k - t) * dir;
      }
    }
  }

  void expand_cycle(I32 root, I32 col, I32 row, I32 dir) {
    const I32 c = cid[root];
    const I32 p = pos[root];
    const VecI32& vs = cyc[c];
    const I32 L = isz(vs);
    const I32 k = L / 2;

    I32 best = INF, take = 0;
    FOR(d, 2) {
      const I32 od = (d == 0 ? 1 : -1);
      I32 s = 0;
      if (phi_fit(c, root, od, s) && s < best) {
        best = s;
        take = od;
      }
    }

    place_cycle(c, p, take, col, row, dir);

    const I32 low  = ((p + take) % L + L) % L;
    const I32 top1 = ((p + take * k) % L + L) % L;
    const I32 top2 = ((p + take * (k + 1)) % L + L) % L;
    const I32 a = vs[low], b = vs[top1], e = vs[top2];

    if (ext[a] && a != root) push(ext[a], a, 1 - col, row - dir, -dir);
    if (ext[b]) push(ext[b], b, 1 - col, row + k * dir, dir);
    if (ext[e]) push(ext[e], e, col, row + k * dir, dir);
  }

  void expand_tree(I32 u, I32 from, I32 col, I32 row, I32 dir) {
    X[u] = col;
    Y[u] = row;

    VecI32 ch;
    Vec<Prof> ps;
    for (const I32 v : adj[u]) {
      if (v == from) continue;
      ch.push_back(v);
      ps.push_back(beyond(u, v));
    }

    if (ch.empty()) return;
    if (isz(ch) == 1) {
      push(ch[0], u, col, row + dir, dir);
      return;
    }

    Dec dec;
    merge2(ps[0], ps[1], dec);

    if (dec.both_path) {
      push(ch[0], u, col, row + dir, dir);
      push(ch[1], u, 1 - col, row, dir);
      return;
    }

    const I32 U = ch[dec.up];
    const I32 S = ch[1 - dec.up];
    push(U, u, col, row + dir, dir);

    if (dec.side == 0) {
      push(S, u, 1 - col, row, dir);
    } else if (dec.side == 1) {
      push(S, u, 1 - col, row, -dir);
    } else {
      X[S] = 1 - col;
      Y[S] = row;

      I32 a = -1, b = -1;
      for (const I32 v : adj[S]) {
        if (v == u) continue;
        (a < 0 ? a : b) = v;
      }

      const I32 len = beyond(S, a).a;
      const I32 up_child = (len == dec.len ? a : b);
      const I32 down_child = (up_child == a ? b : a);
      push(up_child, S, 1 - col, row + dir, dir);
      push(down_child, S, 1 - col, row - dir, -dir);
    }
  }

  void run() {
    while (!jobs.empty()) {
      const Job j = jobs.back();
      jobs.pop_back();

      if (cid[j.u] >= 0) expand_cycle(j.u, j.col, j.row, j.dir);
      else expand_tree(j.u, j.from, j.col, j.row, j.dir);
    }
  }

  bool valid() {
    Set<PairI32> seen;
    FOR(v, 1, n + 1) {
      if (X[v] < 0 || X[v] > 1) return false;
      if (Y[v] < -200000 || Y[v] > 200000) return false;
      if (!seen.insert({X[v], Y[v]}).second) return false;
    }

    FOR(v, 1, n + 1) {
      for (const I32 to : adj[v]) {
        if (v < to && abs(X[v] - X[to]) + abs(Y[v] - Y[to]) != 1) return false;
      }
    }
    return true;
  }

  void reset() {
    jobs.clear();
    X.assign(n + 1, -1);
    Y.assign(n + 1, 0);
  }

  bool accept() {
    run();
    return valid();
  }

  bool solve_case() {
    X.assign(n + 1, 0);
    Y.assign(n + 1, 0);

    if (n == 1) return true;

    FOR(v, 1, n + 1) {
      if (deg[v] >= 4) fail = true;
    }
    if (fail) return false;

    build();
    if (fail) return false;

    calc();

    Dec dec;
    FOR(v, 1, n + 1) {
      if (cid[v] >= 0 || deg[v] > 2) continue;

      Vec<Prof> ps;
      for (const I32 to : adj[v]) ps.push_back(beyond(v, to));
      if (merge_all(ps, dec).t == BAD) continue;

      reset();
      push(v, 0, 0, 0, 1);
      if (accept()) return true;
    }

    FOR(c, isz(cyc)) {
      const VecI32& vs = cyc[c];
      const I32 L = isz(vs);
      const I32 k = L / 2;
      if (isz(att[c]) > 4) continue;

      FOR(i, L) {
        bool fits = true;
        Prof A = nil(), B = nil();

        for (const I32 v : att[c]) {
          const I32 d = (pos[v] - i + L) % L;
          const Prof prof = beyond_att(v);
          if (prof.t == BAD) {
            fits = false;
            break;
          }

          if (d == 0 || d == 1) {
            if (prof.t != PATH) {
              fits = false;
              break;
            }
          } else if (d == k) {
            A = prof;
          } else if (d == k + 1) {
            B = prof;
          } else {
            fits = false;
            break;
          }
        }

        if (!fits || !top_fit(A, B)) continue;

        reset();
        place_cycle(c, i, 1, 0, 0, 1);

        const I32 a = vs[i];
        const I32 b = vs[(i + 1) % L];
        const I32 x = vs[(i + k) % L];
        const I32 y = vs[(i + k + 1) % L];

        if (ext[a]) push(ext[a], a, 0, -1, -1);
        if (ext[b]) push(ext[b], b, 1, -1, -1);
        if (ext[x]) push(ext[x], x, 1, k, 1);
        if (ext[y]) push(ext[y], y, 0, k, 1);

        if (accept()) return true;
      }
    }

    return false;
  }
};

void solve() {
  INT(n, m);

  Solver S;
  S.n = n;
  S.m = m;
  S.adj.assign(n + 1, {});
  S.deg.assign(n + 1, 0);

  FOR(m) {
    INT(u, v);
    S.adj[u].push_back(v);
    S.adj[v].push_back(u);
    ++S.deg[u];
    ++S.deg[v];
  }

  if (!S.solve_case()) {
    OUT("No");
    return;
  }

  OUT("Yes");
  FOR(i, 1, n + 1) cout << S.X[i] << ' ' << S.Y[i] << '\n';
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
