#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
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

struct IncEdge {
  I32 w;
  I64 S_wu;
  I32 in_idx;
};

struct PrefixNode {
  I32 max1;
  I32 idx1;
  I32 max2;
  I32 idx2;
};

void solve() {
  I32 n, k;
  cin >> n >> k;

  VecI64 a(n + 1);
  I64 total_weight = 0;
  for (I32 i = 1; i <= n; ++i) {
    cin >> a[i];
    total_weight += a[i];
  }

  VecI32 head(n + 1, -1);
  VecI32 nxt(2 * n - 2);
  VecI32 to(2 * n - 2);

  for (I32 i = 0; i < n - 1; ++i) {
    I32 u, v;
    cin >> u >> v;
    to[2 * i]  = v;
    nxt[2 * i] = head[u];
    head[u] = 2 * i;

    to[2 * i + 1]  = u;
    nxt[2 * i + 1] = head[v];
    head[v] = 2 * i + 1;
  }

  VecI32 q;
  q.push_back(1);
  VecI32 parent(n + 1, 0);
  VecI32 parent_edge(n + 1, -1);
  for (I32 i = 0; i < q.size(); ++i) {
    I32 u = q[i];
    for (I32 e = head[u]; e != -1; e = nxt[e]) {
      I32 v = to[e];
      if (v != parent[u]) {
        parent[v] = u;
        parent_edge[v] = e ^ 1; // edge v -> u
        q.push_back(v);
      }
    }
  }

  VecI32 order_top_down = q;
  VecI32 order_bottom_up = q;
  std::reverse(order_bottom_up.begin(), order_bottom_up.end());

  VecI64 sub_wt(n + 1, 0);
  for (I32 u : order_bottom_up) {
    sub_wt[u] += a[u];
    if (u != 1) {
      sub_wt[parent[u]] += sub_wt[u];
    }
  }

  VecI64 S(2 * n - 2);
  for (I32 u : order_top_down) {
    for (I32 e = head[u]; e != -1; e = nxt[e]) {
      I32 v = to[e];
      if (v == parent[u]) {
        S[e] = total_weight - sub_wt[u];
      } else {
        S[e] = sub_wt[v];
      }
    }
  }

  VecI32 deg_in(n + 1, 0);
  for (I32 e = 0; e < 2 * n - 2; ++e) {
    deg_in[to[e]]++;
  }
  VecI32 inc_start(n + 2, 0);
  for (I32 u = 1; u <= n; ++u) {
    inc_start[u + 1] = inc_start[u] + deg_in[u];
  }
  Vec<IncEdge> flat_inc(2 * n - 2);
  VecI32 inc_ptr = inc_start;
  for (I32 u = 1; u <= n; ++u) {
    for (I32 e = head[u]; e != -1; e = nxt[e]) {
      I32 v = to[e];
      flat_inc[inc_ptr[v]++] = {u, S[e], e};
    }
  }

  for (I32 u = 1; u <= n; ++u) {
    std::sort(flat_inc.begin() + inc_start[u], flat_inc.begin() + inc_start[u + 1], [](const IncEdge& A, const IncEdge& B) {
      return A.S_wu < B.S_wu;
    });
  }

  VecI32 deg_out(n + 1, 0);
  for (I32 u = 1; u <= n; ++u) {
    for (I32 e = head[u]; e != -1; e = nxt[e]) {
      if (to[e] != parent[u]) deg_out[u]++;
    }
  }
  VecI32 out_start(n + 2, 0);
  for (I32 u = 1; u <= n; ++u) {
    out_start[u + 1] = out_start[u] + deg_out[u];
  }
  VecI32 flat_out(std::max(1, n - 1));
  VecI32 out_ptr = out_start;
  for (I32 u = 1; u <= n; ++u) {
    for (I32 e = head[u]; e != -1; e = nxt[e]) {
      if (to[e] != parent[u]) {
        flat_out[out_ptr[u]++] = e;
      }
    }
    std::sort(flat_out.begin() + out_start[u], flat_out.begin() + out_start[u + 1], [&](I32 e1, I32 e2) {
      return S[e1] < S[e2];
    });
  }

  VecI32 dp(2 * n - 2);
  Vec<PrefixNode> suff(n + 1);

  auto check = [&](I64 x) -> bool {
    for (I32 e = 0; e < 2 * n - 2; ++e) {
      if (S[e] <= total_weight - x) dp[e] = 1;
      else dp[e] = -1e9;
    }

    // Pass A (Bottom-up)
    for (I32 u : order_bottom_up) {
      if (u == 1) continue;
      I32 d = deg_in[u];
      if (d == 0) continue;
      I32 st = inc_start[u];

      for (I32 i = d - 1; i >= 0; --i) {
        I32 val = dp[flat_inc[st + i].in_idx];
        if (i == d - 1) {
          suff[i] = {val, i, (I32)-1e9, -1};
        } else {
          suff[i] = suff[i+1];
          if (val > suff[i].max1) {
            suff[i].max2 = suff[i].max1;
            suff[i].idx2 = suff[i].idx1;
            suff[i].max1 = val;
            suff[i].idx1 = i;
          } else if (val > suff[i].max2) {
            suff[i].max2 = val;
            suff[i].idx2 = i;
          }
        }
      }

      I32 p = parent[u];
      I32 e_up  = parent_edge[u];
      I64 limit = S[e_up] + x;

      I32 l = 0, r = d - 1, best_idx = d;
      while (l <= r) {
        I32 mid = l + (r - l) / 2;
        if (flat_inc[st + mid].S_wu >= limit) {
          best_idx = mid;
          r = mid - 1;
        } else {
          l = mid + 1;
        }
      }

      if (best_idx < d) {
        I32 max_val = suff[best_idx].max1;
        if (suff[best_idx].idx1 != -1 && flat_inc[st + suff[best_idx].idx1].w == p) {
          max_val = suff[best_idx].max2;
        }
        dp[e_up] = std::max(dp[e_up], 1 + max_val);
      }
    }

    // Pass B (Top-down)
    for (I32 u : order_top_down) {
      I32 d = deg_in[u];
      if (d == 0) continue;
      I32 st = inc_start[u];

      for (I32 i = d - 1; i >= 0; --i) {
        I32 val = dp[flat_inc[st + i].in_idx];
        if (i == d - 1) {
          suff[i] = {val, i, (I32)-1e9, -1};
        } else {
          suff[i] = suff[i+1];
          if (val > suff[i].max1) {
            suff[i].max2 = suff[i].max1;
            suff[i].idx2 = suff[i].idx1;
            suff[i].max1 = val;
            suff[i].idx1 = i;
          } else if (val > suff[i].max2) {
            suff[i].max2 = val;
            suff[i].idx2 = i;
          }
        }
      }

      I32 ptr = 0;
      for (I32 idx = out_start[u]; idx < out_start[u + 1]; ++idx) {
        I32 e = flat_out[idx];
        I32 c = to[e];
        I64 limit = S[e] + x;

        while (ptr < d && flat_inc[st + ptr].S_wu < limit) {
          ptr++;
        }

        I32 best_idx = ptr;
        if (best_idx < d) {
          I32 max_val = suff[best_idx].max1;
          if (suff[best_idx].idx1 != -1 && flat_inc[st + suff[best_idx].idx1].w == c) {
            max_val = suff[best_idx].max2;
          }
          dp[e] = std::max(dp[e], 1 + max_val);
        }
      }
    }

    for (I32 e = 0; e < 2 * n - 2; ++e) {
      if (dp[e] >= k && S[e] >= x) return true;
    }
    return false;
  };

  I64 low = 1, high = total_weight / (k + 1);
  I64 ans = -1;
  while (low <= high) {
    I64 mid = low + (high - low) / 2;
    if (check(mid)) {
      ans = mid;
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  cout << ans << "\n";
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
