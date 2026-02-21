#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"
#include "modules/Data_Structures.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

void solve() {
  constexpr I64 NEG_INF = SegmentTreeHull::NEG_INF;
  INT(n);
  Vec<I64> a(n + 1);
  FOR(i, 1, n + 1) IN(a[i]);

  Vec<Vec<I32>> adj(n + 1);
  FOR(_, n - 1) {
    INT(u, v);
    adj[u].eb(v);
    adj[v].eb(u);
  }

  Vec<I32> parent(n + 1, 0);
  Vec<I32> depth(n + 1, 0);
  Vec<I32> ptr(n + 1, 0);
  Vec<I32> order;
  order.reserve(n);

  Vec<I32> st;
  st.reserve(n);
  st.eb(1);

  while (!st.empty()) {
    const I32 u = st.back();
    if (ptr[u] == 0) order.eb(u);

    if (ptr[u] < as<I32>(adj[u].size())) {
      const I32 v = adj[u][ptr[u]++];
      if (v == parent[u]) continue;
      parent[v] = u;
      depth[v] = depth[u] + 1;
      st.eb(v);
    } else {
      st.pop_back();
    }
  }

  Vec<I64> s(n + 1, 0);
  Vec<I64> c(n + 1, 0);
  Vec<I32> d1(n + 1, 0);
  Vec<I32> d2(n + 1, 0);
  Vec<I32> heavy(n + 1, 0);

  FOR_R(i, 0, n) {
    const I32 u = order[i];
    s[u] = a[u];
    d1[u] = depth[u];

    I32 max_d = -1;
    I32 h_child = 0;
    for (I32 v : adj[u]) {
      if (parent[v] != u) continue;
      s[u] += s[v];
      if (d1[v] > max_d) {
        max_d = d1[v];
        h_child = v;
      }
    }

    if (h_child != 0) {
      d1[u] = max_d;
      heavy[u] = h_child;
    }

    I32 max_d2 = depth[u];
    for (I32 v : adj[u]) {
      if (parent[v] != u || v == h_child) continue;
      chmax(max_d2, d1[v]);
    }
    d2[u] = max_d2;
  }

  FOR_R(i, 0, n) {
    const I32 u = order[i];
    I64 cur = 0;
    for (I32 v : adj[u]) {
      if (parent[v] != u) continue;
      cur += c[v] + s[v];
    }
    c[u] = cur;
  }

  Vec<char> is_head(n + 1, true);
  is_head[0] = false;
  FOR(u, 1, n + 1) {
    if (heavy[u] != 0) is_head[heavy[u]] = false;
  }

  Vec<Vec<I32>> paths;
  paths.reserve(n);
  FOR(u, 1, n + 1) {
    if (!is_head[u]) continue;
    Vec<I32> path;
    I32 curr = u;
    while (curr != 0) {
      path.eb(curr);
      curr = heavy[curr];
    }
    paths.eb(std::move(path));
  }

  Vec<I64> final_ans(n + 1, 0);
  SegmentTreeHull seg;

  for (const auto& path : paths) {
    const I32 k = as<I32>(path.size()) - 1;
    const I32 d_max = depth[path.back()];

    Vec<I64> max_light(k + 1, NEG_INF);
    FOR(i, k + 1) {
      const I32 u = path[i];
      const I32 nxt = (i < k ? path[i + 1] : 0);
      for (I32 v : adj[u]) {
        if (parent[v] != u || v == nxt) continue;

        I64 best = NEG_INF;
        Vec<I32> light_st;
        light_st.eb(v);
        while (!light_st.empty()) {
          const I32 curr = light_st.back();
          light_st.pop_back();

          const I64 val = s[curr] * (as<I64>(d_max) + 1 - depth[curr]);
          chmax(best, val);

          for (I32 child : adj[curr]) {
            if (parent[child] == curr) light_st.eb(child);
          }
        }
        chmax(max_light[i], best);
      }
    }

    Vec<I64> suf_max(k + 1, NEG_INF);
    suf_max[k] = max_light[k];
    FOR_R(i, 0, k) suf_max[i] = max(max_light[i], suf_max[i + 1]);

    Vec<I64> ans_case2(k + 1, NEG_INF);
    if (k > 0) {
      Vec<Pair<I64, I64>> path_lines(k + 1, {0, 0});
      FOR(i, k + 1) {
        const I32 u = path[i];
        path_lines[i] = {s[u], s[u] * (1 - depth[u])};
      }

      seg.init(k, path_lines);

      Vec<tuple<I32, I32, I32>> mono_st;
      mono_st.reserve(k);

      FOR_R(i, 0, k) {
        const I32 v_val = d2[path[i]];
        I32 curr_l = i + 1;
        I32 curr_r = i + 1;

        while (!mono_st.empty() && get<0>(mono_st.back()) <= v_val) {
          curr_r = get<2>(mono_st.back());
          mono_st.pop_back();
        }
        mono_st.eb(v_val, curr_l, curr_r);

        seg.range_assign(curr_l, curr_r, v_val);
        ans_case2[i] = seg.range_max(i + 1, k);
      }
    }

    FOR(i, k + 1) {
      I64 max_inc = max<I64>(0, suf_max[i]);
      if (k > 0 && i < k) chmax(max_inc, ans_case2[i]);
      final_ans[path[i]] = c[path[i]] + max_inc;
    }
  }

  FOR(i, 1, n + 1) {
    if (i > 1) cout << ' ';
    cout << final_ans[i];
  }
  cout << '\n';
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
