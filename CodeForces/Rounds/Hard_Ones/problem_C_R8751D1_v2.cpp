#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_MACROS
#define NEED_TIMER
#define NEED_CONTAINERS
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

// CF 1578C - Cactus Lady and her Cing
// Profile DP on the cactus block structure, per the amended blueprint:
//   profiles: BOT | P(len) | Pint(a,b) | F(D,0) | F(0,s)
//   bottom-up pass (dn), rerooting pass (up), cycle evaluation Phi,
//   root search over deg<=2 tree vertices and cycles-as-bottom,
//   constructive replay with an explicit job stack.

using namespace std;

static constexpr int PINF = 1 << 29;

// Profiles
enum Tag : int { BOT, TPATH, TPINT, TFD, TFS, TNONE };

struct Prof {
  Tag t;
  int a, b;
};

static Prof mkBot() {
  return {BOT, 0, 0};
}

static Prof mkP(int l) {
  return {TPATH, l, 0};
}

static Prof mkPint(int a, int b) {
  return {TPINT, a, b};
}

static Prof mkFd(int D) {
  return {TFD, min(D, PINF), 0};
}

static Prof mkFs(int s) {
  return s == 0 ? mkFd(0) : Prof{TFS, s, 0};
}

static Prof mkNone() {
  return {TNONE, 0, 0};
}

// child placed straight above the merge vertex (single-piece rule)
static Prof shiftP(const Prof& p) {
  switch (p.t) {
  case TPATH:
    return mkP(p.a + 1);
  case TPINT:
    return mkFd(1); // both arms upward
  case TFD:
    return mkFd(p.a + 1);
  case TFS:
    return mkFs(p.a - 1); // protrusion absorbed one row
  default:
    return mkBot();
  }
}

// first occupied row of the opposite column (merge-vertex frame) when piece sits in UP slot;
// -1 = piece cannot occupy UP next to a sibling
static int topFree(const Prof& p) {
  switch (p.t) {
  case TPATH:
    return PINF;
  case TFD:
    return p.a + 1;
  case TPINT:
    return 1;
  default:
    return -1; // FS protrusion would hit the SIDE root
  }
}

struct Dec {
  int  up       = -1;
  int  sideMode = 0;
  int  upLen    = 0;
  bool bothP    = false;
};

// sideMode: 0 = ascending path, 1 = descending path, 2 = Pint split (upLen = upward arm)

static Prof merge2(const Prof& A, const Prof& B, Dec& dec) {
  if (A.t == BOT || B.t == BOT)
    return mkBot();
  if (A.t == TPATH && B.t == TPATH) { // path with internal root: keep arms
    dec = Dec{0, 0, 0, true};
    return mkPint(A.a, B.a);
  }
  int         bestS = INT_MAX;
  Dec         bd;
  const Prof* pr[2] = {&A, &B};
  for (int u = 0; u < 2; u++) { // try both UP/SIDE assignments
    const Prof& U  = *pr[u];
    const Prof& S  = *pr[1 - u];
    int         tf = topFree(U);
    if (tf < 0)
      continue;
    if (S.t == TPATH) {
      if (S.a <= tf && 0 < bestS) {
        bestS = 0;
        bd    = {u, 0, 0, false};
      }
      if (S.a - 1 < bestS) {
        bestS = S.a - 1;
        bd    = {u, 1, 0, false};
      }
    } else if (S.t == TPINT) { // segment glued by a rung: one arm up, one down
      if (S.a <= tf - 1 && S.b < bestS) {
        bestS = S.b;
        bd    = {u, 2, S.a, false};
      }
      if (S.b <= tf - 1 && S.a < bestS) {
        bestS = S.a;
        bd    = {u, 2, S.b, false};
      }
    }
    // genuinely fat SIDE (TFD/TFS): excluded by the Exclusion Lemma
  }
  if (bd.up < 0)
    return mkBot();
  dec = bd;
  return mkFs(bestS);
}

static Prof mergeAt(Vec<Prof>& ps, Dec& dec) {
  Vec<Prof> q;
  for (auto& p : ps)
    if (p.t != TNONE)
      q.push_back(p);
  if (q.empty())
    return mkP(1);
  if (q.size() == 1)
    return shiftP(q[0]);
  if (q.size() == 2)
    return merge2(q[0], q[1], dec);
  return mkBot();
}

// top-corner compatibility above a cycle rectangle (pieces start at the same row)
static bool topOk1(const Prof& p) {
  return p.t == TPATH || p.t == TFD || p.t == TPINT;
}

static bool compatTop(const Prof& A, const Prof& B) {
  bool a = A.t != TNONE, b = B.t != TNONE;
  if (!a && !b)
    return true;
  if (!a)
    return topOk1(B);
  if (!b)
    return topOk1(A);
  if (A.t == TPATH && B.t == TPATH)
    return true;
  if (A.t == TFD && B.t == TPATH)
    return B.a <= A.a; // path height <= fat clearance
  if (B.t == TFD && A.t == TPATH)
    return A.a <= B.a;
  return false; // fat+fat, Pint+sibling, FS: impossible
}

// Fast IO
static char   ibuf[1 << 25];
static size_t ipos = 0, ilen = 0;

static int readInt() {
  while (ipos < ilen && (ibuf[ipos] < '0' || ibuf[ipos] > '9') && ibuf[ipos] != '-')
    ipos++;
  bool neg = false;
  if (ipos < ilen && ibuf[ipos] == '-') {
    neg = true;
    ipos++;
  }
  int x = 0;
  while (ipos < ilen && ibuf[ipos] >= '0' && ibuf[ipos] <= '9')
    x = x * 10 + (ibuf[ipos++] - '0');
  return neg ? -x : x;
}

static string obuf;

// Per-test Solver
struct Solver {
  int          n, m;
  Vec2D<I32>  adj;
  VecI32          par, dep, order, deg;
  VecI32          cid, cpos, extNb;
  Vec2D<I32>  cycVs, attL;
  VecI32          cycEntry;
  Vec<Prof> dn, up;
  VecI32          X, Y;
  bool         bad = false;

  Prof pba(int x) { // piece beyond attachment x
    int h = extNb[x];
    return par[h] == x ? dn[h] : up[x];
  }

  Prof pieceBeyond(int u, int w) { // piece beyond neighbor w of u
    return par[w] == u ? dn[w] : up[u];
  }

  void dfsBuild() {
    par.assign(n + 1, 0);
    dep.assign(n + 1, -1);
    order.clear();
    order.reserve(n);
    cid.assign(n + 1, -1);
    cpos.assign(n + 1, 0);
    VecI32 it(n + 1, 0), st;
    st.reserve(n);
    st.push_back(1);
    dep[1] = 0;
    while (!st.empty()) {
      int v = st.back();
      if (it[v] == 0)
        order.push_back(v);
      if (it[v] < (int)adj[v].size()) {
        int w = adj[v][it[v]++];
        if (w == par[v])
          continue;
        if (dep[w] == -1) {
          dep[w] = dep[v] + 1;
          par[w] = v;
          st.push_back(w);
        } else if (dep[w] < dep[v]) { // back edge: extract the cycle
          VecI32 tmp;
          int x = v;
          while (x != w) {
            tmp.push_back(x);
            x = par[x];
          }
          tmp.push_back(w);
          reverse(tmp.begin(), tmp.end()); // w = position 0
          int c = (int)cycVs.size();
          if ((int)tmp.size() % 2 != 0)
            bad = true;
          for (int i = 0; i < (int)tmp.size(); i++) {
            if (cid[tmp[i]] != -1)
              bad = true;
            cid[tmp[i]]  = c;
            cpos[tmp[i]] = i;
          }
          cycVs.push_back(std::move(tmp));
          cycEntry.push_back(w);
        }
      } else
        st.pop_back();
    }
    // external neighbors of cycle vertices
    extNb.assign(n + 1, 0);
    attL.assign(cycVs.size(), {});
    for (int v = 1; v <= n; v++) {
      if (cid[v] < 0)
        continue;
      auto& vs = cycVs[cid[v]];
      int   L = (int)vs.size(), p = cpos[v];
      int   pr = vs[(p - 1 + L) % L], nx = vs[(p + 1) % L];
      for (int w : adj[v])
        if (w != pr && w != nx)
          extNb[v] = w;
      if (extNb[v])
        attL[cid[v]].push_back(v);
    }
  }

  // cycle hanging from `entryV` (its external edge is the parent side), orientation o
  bool phiCheck(int c, int entryV, int o, int& sOut) {
    auto& vs = cycVs[c];
    int   L = (int)vs.size(), k = L / 2;
    int   e  = cpos[entryV];
    int   c1 = ((e + o) % L + L) % L;           // bottom twin
    int   c2 = ((e + o * k) % L + L) % L;       // top, opposite column
    int   c3 = ((e + o * (k + 1)) % L + L) % L; // top, entry column
    int   s  = 0;
    Prof  t2 = mkNone(), t3 = mkNone();
    for (int x : attL[c]) {
      if (x == entryV)
        continue;
      int  p  = cpos[x];
      Prof pe = pba(x);
      if (pe.t == BOT)
        return false;
      if (p == c1) {
        if (pe.t != TPATH)
          return false;
        s = pe.a;
      } // descending pendant
      else if (p == c2)
        t2 = pe;
      else if (p == c3)
        t3 = pe;
      else
        return false; // attachment not at a corner
    }
    if (!compatTop(t2, t3))
      return false;
    sOut = s;
    return true;
  }

  Prof bestPhi(int c, int entryV, int* oOut = nullptr) {
    int bs = INT_MAX, bo = 0, s;
    for (int o : {1, -1})
      if (phiCheck(c, entryV, o, s) && s < bs) {
        bs = s;
        bo = o;
      }
    if (bo == 0)
      return mkBot();
    if (oOut)
      *oOut = bo;
    return mkFs(bs);
  }

  void computeDP() {
    dn.assign(n + 1, mkBot());
    up.assign(n + 1, mkNone());
    Dec d;
    // bottom-up (reverse preorder = valid postorder)
    for (int i = n - 1; i >= 0; i--) {
      int v = order[i];
      if (cid[v] >= 0) {
        if (cycEntry[cid[v]] == v)
          dn[v] = bestPhi(cid[v], v);
        continue; // non-entry cycle vertices: dn unused
      }
      Vec<Prof> ps;
      for (int w : adj[v])
        if (w != par[v])
          ps.push_back(dn[w]);
      dn[v] = mergeAt(ps, d);
    }
    // rerooting (preorder)
    for (int i = 1; i < n; i++) {
      int v = order[i], p = par[v];
      if (cid[v] >= 0 && cid[v] == (p ? cid[p] : -2)) {
        up[v] = mkBot();
        continue;
      } // unused
      if (cid[p] >= 0) {
        up[v] = bestPhi(cid[p], p);
        continue;
      } // complement through a cycle
      Vec<Prof> ps;
      for (int w : adj[p])
        if (w != v && w != par[p])
          ps.push_back(dn[w]);
      if (p != order[0])
        ps.push_back(up[p]);
      up[v] = mergeAt(ps, d);
    }
  }

  // ---------- placement ----------
  struct Job {
    int u, from, col, row, dir;
  };

  Vec<Job> jobs;

  void push(int u, int from, int col, int row, int dir) { jobs.push_back({u, from, col, row, dir}); }

  void placeCycleCoords(int c, int e, int o, int col, int row, int dir) {
    auto& vs = cycVs[c];
    int   L = (int)vs.size(), k = L / 2;
    for (int t = 0; t < L; t++) {
      int v2 = vs[((e + o * t) % L + L) % L];
      if (t == 0) {
        X[v2] = col;
        Y[v2] = row;
      } else if (t <= k) {
        X[v2] = 1 - col;
        Y[v2] = row + (t - 1) * dir;
      } else {
        X[v2] = col;
        Y[v2] = row + (2 * k - t) * dir;
      }
    }
  }

  void cyclePlace(int u, int /*from*/, int col, int row, int dir) {
    int   c = cid[u], e = cpos[u];
    auto& vs = cycVs[c];
    int   L = (int)vs.size(), k = L / 2;
    int   bs = INT_MAX, bo = 0, s;
    for (int o : {1, -1})
      if (phiCheck(c, u, o, s) && s < bs) {
        bs = s;
        bo = o;
      }
    // bo != 0 guaranteed: the DP accepted this fragment
    placeCycleCoords(c, e, bo, col, row, dir);
    int c1 = ((e + bo) % L + L) % L, c2 = ((e + bo * k) % L + L) % L, c3 = ((e + bo * (k + 1)) % L + L) % L;
    int v1 = vs[c1], v2 = vs[c2], v3 = vs[c3];
    if (extNb[v1] && extNb[v1] != /*parent never*/ -1 && v1 != u)
      push(extNb[v1], v1, 1 - col, row - dir, -dir);
    if (extNb[v2])
      push(extNb[v2], v2, 1 - col, row + k * dir, dir);
    if (extNb[v3])
      push(extNb[v3], v3, col, row + k * dir, dir);
  }

  void expandTree(int u, int from, int col, int row, int dir) {
    X[u] = col;
    Y[u] = row;
    VecI32          ws;
    Vec<Prof> ps;
    for (int w : adj[u])
      if (w != from) {
        ws.push_back(w);
        ps.push_back(pieceBeyond(u, w));
      }
    if (ws.empty())
      return;
    if (ws.size() == 1) {
      push(ws[0], u, col, row + dir, dir);
      return;
    }
    Dec  dec;
    Prof r = merge2(ps[0], ps[1], dec);
    (void)r;
    if (dec.bothP) { // internal-path vertex used upward
      push(ws[0], u, col, row + dir, dir);
      push(ws[1], u, 1 - col, row, dir);
      return;
    }
    int U = ws[dec.up], S = ws[1 - dec.up];
    push(U, u, col, row + dir, dir);
    if (dec.sideMode == 0)
      push(S, u, 1 - col, row, dir);
    else if (dec.sideMode == 1)
      push(S, u, 1 - col, row, -dir);
    else { // Pint side: split arms in place
      X[S]   = 1 - col;
      Y[S]   = row;
      int a1 = -1, a2 = -1;
      for (int w : adj[S])
        if (w != u)
          (a1 < 0 ? a1 : a2) = w;
      int l1 = pieceBeyond(S, a1).a;
      int nu = (l1 == dec.upLen ? a1 : a2), nd = (nu == a1 ? a2 : a1);
      push(nu, S, 1 - col, row + dir, dir);
      push(nd, S, 1 - col, row - dir, -dir);
    }
  }

  void runJobs() {
    while (!jobs.empty()) {
      Job j = jobs.back();
      jobs.pop_back();
      if (cid[j.u] >= 0)
        cyclePlace(j.u, j.from, j.col, j.row, j.dir);
      else
        expandTree(j.u, j.from, j.col, j.row, j.dir);
    }
  }

  bool valid() {
    set<pair<int, int>> seen;
    for (int v = 1; v <= n; v++) {
      if (X[v] < 0 || X[v] > 1 || Y[v] < -200'000 || Y[v] > 200'000)
        return false;
      if (!seen.insert({X[v], Y[v]}).second)
        return false;
    }
    for (int v = 1; v <= n; v++)
      for (int w : adj[v])
        if (v < w && abs(X[v] - X[w]) + abs(Y[v] - Y[w]) != 1)
          return false;
    return true;
  }

  void resetPlacement() {
    jobs.clear();
    X.assign(n + 1, -1);
    Y.assign(n + 1, 0);
  }

  bool acceptCandidate() {
    runJobs();
    if (!valid())
      return false;
    emitYes();
    return true;
  }

  void solve() {
    if (n == 1) {
      obuf += "Yes\n0 0\n";
      return;
    }
    for (int v = 1; v <= n; v++)
      if (deg[v] >= 4)
        bad = true;
    if (!bad) {
      dfsBuild();
    }
    if (bad) {
      obuf += "No\n";
      return;
    }
    computeDP();

    Dec d;
    // candidate roots: tree vertices of degree <= 2
    for (int v = 1; v <= n; v++) {
      if (cid[v] >= 0 || deg[v] > 2)
        continue;
      Vec<Prof> ps;
      for (int w : adj[v])
        ps.push_back(pieceBeyond(v, w));
      if (mergeAt(ps, d).t == BOT)
        continue;
      resetPlacement();
      push(v, 0, 0, 0, 1); // expandTree re-derives the decision
      if (acceptCandidate())
        return;
    }
    // candidate roots: cycle as the bottom of the layout
    for (int c = 0; c < (int)cycVs.size(); c++) {
      auto& vs = cycVs[c];
      int   L = (int)vs.size(), k = L / 2;
      for (int i = 0; i < L; i++) {
        bool ok = true;
        Prof tA = mkNone(), tB = mkNone();
        for (int x : attL[c]) {
          int  dpos = (cpos[x] - i + L) % L;
          Prof pe   = pba(x);
          if (pe.t == BOT) {
            ok = false;
            break;
          }
          if (dpos == 0 || dpos == 1) {
            if (pe.t != TPATH) {
              ok = false;
              break;
            }
          } // pendants below
          else if (dpos == k)
            tA = pe;
          else if (dpos == k + 1)
            tB = pe;
          else {
            ok = false;
            break;
          }
        }
        if (!ok || !compatTop(tA, tB))
          continue;
        resetPlacement();
        placeCycleCoords(c, i, 1, 0, 0, 1);
        int v0 = vs[i], v1 = vs[(i + 1) % L];
        int vA = vs[(i + k) % L], vB = vs[(i + k + 1) % L];
        if (extNb[v0])
          push(extNb[v0], v0, 0, -1, -1);
        if (extNb[v1])
          push(extNb[v1], v1, 1, -1, -1);
        if (extNb[vA])
          push(extNb[vA], vA, 1, k, 1);
        if (extNb[vB])
          push(extNb[vB], vB, 0, k, 1);
        if (acceptCandidate())
          return;
      }
    }
    obuf += "No\n";
  }

  void emitYes() {
    obuf += "Yes\n";
    char tmp[24];
    for (int v = 1; v <= n; v++) {
      int len = snprintf(tmp, sizeof tmp, "%d %d\n", X[v], Y[v]);
      obuf.append(tmp, len);
    }
  }
};

//===----------------------------------------------------------------------===//
/* Main Function */

int main() {
  ilen  = fread(ibuf, 1, sizeof ibuf, stdin);
  int t = readInt();
  obuf.reserve(1 << 22);
  while (t--) {
    Solver S;
    S.n = readInt();
    S.m = readInt();
    S.adj.assign(S.n + 1, {});
    S.deg.assign(S.n + 1, 0);
    for (int i = 0; i < S.m; i++) {
      int a = readInt(), b = readInt();
      S.adj[a].push_back(b);
      S.adj[b].push_back(a);
      S.deg[a]++;
      S.deg[b]++;
    }
    S.solve();
  }
  fwrite(obuf.data(), 1, obuf.size(), stdout);
  return 0;
}

//===----------------------------------------------------------------------===//
