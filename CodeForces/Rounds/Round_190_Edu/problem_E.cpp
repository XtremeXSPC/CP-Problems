#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_MACROS
#define NEED_TIMER
#define NEED_CONTAINERS
#define NEED_CONSTANTS
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"
#include "modules/data_structures/SegmentTree.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct SNode {
  I64 mn_s = INF64;
  I64 mn_p = INF64;
  I64 mn_c = INF64;
};

struct Merge {
  SNode operator()(const SNode& a, const SNode& b) const {
    return {
      min(a.mn_s, b.mn_s),
      min(a.mn_p, b.mn_p),
      min(a.mn_c, b.mn_c)
    };
  }
};

struct Pt {
  I32 p, c;
};

struct Qry {
  I32 id, tp, tc, d;
};

void solve() {
  INT(n);

  VecI32 p_arr(n), c_arr(n);
  FOR(i, n) IN(p_arr[i]);
  FOR(i, n) IN(c_arr[i]);

  INT(m);
  VecI32 tp_arr(m), tc_arr(m), d_arr(m);
  FOR(i, m) IN(tp_arr[i]);
  FOR(i, m) IN(tc_arr[i]);
  FOR(i, m) IN(d_arr[i]);

  Vec<Pt> pts(n);
  FOR(i, n) pts[i] = {p_arr[i], c_arr[i]};

  Vec<Pt> pt_pa = pts;
  ranges::sort(pt_pa, [](const Pt& a, const Pt& b) { return a.p < b.p; });
  VecI32 prf_c(n);
  I32 cur_c = 2e9;
  FOR(i, n) {
    cur_c = min(cur_c, pt_pa[i].c);
    prf_c[i] = cur_c;
  }

  Vec<Pt> pt_ca = pts;
  ranges::sort(pt_ca, [](const Pt& a, const Pt& b) { return a.c < b.c; });
  VecI32 prf_p(n);
  I32 cur_p = 2e9;
  FOR(i, n) {
    cur_p = min(cur_p, pt_ca[i].p);
    prf_p[i] = cur_p;
  }

  VecI32 cc;
  FOR(i, n) cc.push_back(pts[i].c);
  UNIQUE(cc);
  I32 M = sz(cc);

  SegmentTree<SNode, Merge> seg(M, SNode(), Merge());

  Vec<Qry> qs(m);
  FOR(i, m) {
    qs[i] = {(I32)i, tp_arr[i], tc_arr[i], d_arr[i]};
  }

  ranges::sort(qs, [](const Qry& a, const Qry& b) { return a.tp > b.tp; });

  Vec<Pt> pt_pd = pts;
  ranges::sort(pt_pd, [](const Pt& a, const Pt& b) { return a.p > b.p; });

  VecI64 ans(m, INF64);

  I32 ptr = 0;
  for (auto& q : qs) {
    while (ptr < n && pt_pd[ptr].p >= q.tp) {
      I32   idx = LB(cc, pt_pd[ptr].c);
      SNode cur = seg.get(idx);
      SNode nw  = { pt_pd[ptr].p + pt_pd[ptr].c, (I64)pt_pd[ptr].p, (I64)pt_pd[ptr].c };
      seg.update(idx, seg.op(cur, nw));
      ptr++;
    }

    Pt dp = {q.tp, -1};
    auto it_p = ranges::lower_bound(pt_pa, dp, [](const Pt& a, const Pt& b) { return a.p < b.p; });
    I32 cnt_p = (I32)distance(pt_pa.begin(), it_p);

    if (cnt_p > 0 && prf_c[cnt_p - 1] < q.tc) {
      ans[q.id] = 0;
      continue;
    }

    I64 ca = INF64;

    Pt dc = {-1, q.tc};
    auto it_c = ranges::lower_bound(pt_ca, dc, [](const Pt& a, const Pt& b) { return a.c < b.c; });
    I32 cnt_c = (I32)distance(pt_ca.begin(), it_c);

    if (cnt_c > 0) {
      I64 mnP = prf_p[cnt_c - 1];
      ca = min(ca, min(mnP, (I64)q.tp + q.d));
    }

    if (cnt_p > 0) {
      I64 mnC = prf_c[cnt_p - 1];
      ca = min(ca, min(mnC, (I64)q.tc + q.d));
    }

    I32 q_c = LB(cc, q.tc);
    if (q_c < M) {
      SNode res = seg.query(q_c, M);
      if (res.mn_s != INF64) {
        I64 r4 = min({
          res.mn_s,
          res.mn_p + q.tc + q.d,
          res.mn_c + q.tp + q.d,
          (I64)q.tp + q.tc + 2LL * q.d
        });
        ca = min(ca, r4);
      }
    }

    ans[q.id] = ca;
  }

  FOR(i, m) {
    OUT(ans[i]);
  }
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Stopwatch timer;
#endif

  solve();

  return 0;
}

//===----------------------------------------------------------------------===//
