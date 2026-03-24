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
#include "modules/data_structures/FenwickTree.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct State {
  I32 depth = 0;
  I32 leaf  = 0;
  I32 heavy = -1;
  I64 sum   = 0;
};

struct Candidate {
  bool valid = false;
  I32 nei    = -1;
  I32 depth  = -1;
  I32 leaf   = Limits<I32>::max();
  I64 sum    = 0;
};

auto better(const Candidate& a, const Candidate& b) -> bool {
  if (!a.valid) return false;
  if (!b.valid) return true;
  if (a.depth != b.depth) return a.depth > b.depth;
  return a.leaf < b.leaf;
}

auto leaf_state(I32 u, const VecI64& values) -> State {
  State st;
  st.depth = 0;
  st.leaf  = u;
  st.heavy = -1;
  st.sum   = values[u];
  return st;
}

void solve() {
  INT(n, k);

  VecI64 values(n + 1);
  FOR(i, 1, n + 1) IN(values[i]);

  Vec2D<I32> graph(n + 1);
  FOR(_, n - 1) {
    INT(u, v);
    graph[u].eb(v);
    graph[v].eb(u);
  }

  VecI32 parent(n + 1, 0);
  Vec2D<I32> children(n + 1);
  VecI32 order;
  order.reserve(n);

  {
    VecI32 st = {1};
    parent[1] = 0;
    while (!st.empty()) {
      const I32 u = st.back();
      st.pop_back();
      order.eb(u);
      for (I32 v : graph[u]) {
        if (v == parent[u]) continue;
        parent[v] = u;
        children[u].eb(v);
        st.eb(v);
      }
    }
  }

  Vec<State> down(n + 1), up(n + 1), root_state(n + 1);

  for (I32 it = n - 1; it >= 0; --it) {
    const I32 u = order[it];
    if (children[u].empty()) {
      down[u] = leaf_state(u, values);
      continue;
    }

    Candidate best;
    for (I32 v : children[u]) {
      Candidate cand;
      cand.valid = true;
      cand.nei   = v;
      cand.depth = down[v].depth + 1;
      cand.leaf  = down[v].leaf;
      cand.sum   = values[u] + down[v].sum;
      if (better(cand, best)) best = cand;
    }

    State st;
    st.depth = best.depth;
    st.leaf  = best.leaf;
    st.heavy = best.nei;
    st.sum   = best.sum;
    down[u]  = st;
  }

  auto get_state = [&](I32 par, I32 node) -> const State& {
    if (par == 0) {
      return root_state[node];
    }
    if (parent[node] == par) {
      return down[node];
    }
    my_assert(parent[par] == node);
    return up[par];
  };

  for (I32 u : order) {
    Candidate best1, best2;

    for (I32 v : graph[u]) {
      const State& st = get_state(u, v);
      Candidate cand;
      cand.valid = true;
      cand.nei   = v;
      cand.depth = st.depth + 1;
      cand.leaf  = st.leaf;
      cand.sum   = values[u] + st.sum;

      if (better(cand, best1)) {
        best2 = best1;
        best1 = cand;
      } else if (better(cand, best2)) {
        best2 = cand;
      }
    }

    if (!best1.valid) {
      root_state[u] = leaf_state(u, values);
    } else {
      State st;
      st.depth = best1.depth;
      st.leaf  = best1.leaf;
      st.heavy = best1.nei;
      st.sum   = best1.sum;
      root_state[u] = st;
    }

    for (I32 v : children[u]) {
      const Candidate chosen = (best1.valid && best1.nei == v ? best2 : best1);
      if (!chosen.valid) {
        up[v] = leaf_state(u, values);
      } else {
        State st;
        st.depth = chosen.depth;
        st.leaf  = chosen.leaf;
        st.heavy = chosen.nei;
        st.sum   = chosen.sum;
        up[v] = st;
      }
    }
  }

  VecI64 coords;
  coords.reserve(3 * n + 5);
  FOR(u, 1, n + 1) {
    coords.eb(root_state[u].sum - values[u]);
    for (I32 v : graph[u]) {
      coords.eb(get_state(u, v).sum);
    }
  }
  std::sort(all(coords));
  coords.erase(std::unique(all(coords)), coords.end());

  FenwickTree<I64> bit_cnt(as<I32>(coords.size()));
  FenwickTree<I64> bit_sum(as<I32>(coords.size()));
  I64 active_count = 0;
  I64 active_sum   = 0;

  auto add_value = [&](I64 value, I32 delta) -> void {
    const I32 idx = as<I32>(std::lower_bound(all(coords), value) - coords.begin());
    bit_cnt.add(idx, delta);
    bit_sum.add(idx, value * delta);
    active_count += delta;
    active_sum   += value * delta;
  };

  auto query_top = [&](I64 take) -> I64 {
    if (take <= 0) return 0;
    if (take >= active_count) return active_sum;

    const I64 need_small = active_count - take;
    const I32 idx = bit_cnt.lower_bound(need_small);
    const I64 cnt_before = (idx > 0 ? bit_cnt.sum(idx - 1) : 0);
    const I64 sum_before = (idx > 0 ? bit_sum.sum(idx - 1) : 0);
    const I64 at_idx     = need_small - cnt_before;
    const I64 small_sum  = sum_before + at_idx * coords[idx];
    return active_sum - small_sum;
  };

  auto is_excluded = [&](I32 node, I32 par, I32 nei) -> bool {
    if (par != 0 && nei == par) {
      return true;
    }
    const I32 heavy = get_state(par, node).heavy;
    return nei == heavy;
  };

  auto update_node_par = [&](I32 node, I32 old_par, I32 new_par) -> void {
    VecI32 cand;
    cand.reserve(4);

    if (old_par != 0) {
      cand.eb(old_par);
    }
    if (new_par != 0) {
      cand.eb(new_par);
    }

    const I32 old_heavy = get_state(old_par, node).heavy;
    const I32 new_heavy = get_state(new_par, node).heavy;
    if (old_heavy != -1) {
      cand.eb(old_heavy);
    }
    if (new_heavy != -1) {
      cand.eb(new_heavy);
    }

    std::sort(all(cand));
    cand.erase(std::unique(all(cand)), cand.end());

    for (I32 nei : cand) {
      const bool old_ex = is_excluded(node, old_par, nei);
      const bool new_ex = is_excluded(node, new_par, nei);
      if (old_ex == new_ex) {
        continue;
      }

      const I64 path_sum = get_state(node, nei).sum;
      if (old_ex && !new_ex) {
        add_value(path_sum, +1);
      } else {
        add_value(path_sum, -1);
      }
    }
  };

  auto apply_contrib = [&](I32 node, I32 par, I32 delta) -> void {
    const State& st_parent = get_state(par, node);
    const I32 heavy = st_parent.heavy;

    for (I32 nei : graph[node]) {
      if (nei == par) continue;
      if (nei == heavy) continue;
      const I64 path_sum = get_state(node, nei).sum;
      add_value(path_sum, delta);
    }
  };

  auto move_root = [&](I32 old_root, I32 new_root) -> void {
    update_node_par(old_root, 0, new_root);
    update_node_par(new_root, old_root, 0);
  };

  FOR(u, 1, n + 1) {
    const I32 par = (u == 1 ? 0 : parent[u]);
    apply_contrib(u, par, +1);
  }

  I64 answer = 0;
  auto evaluate_root = [&](I32 root) -> void {
    I64 best = values[root];
    if (k > 1) {
      const I64 root_path = root_state[root].sum - values[root];
      add_value(root_path, +1);
      const I64 take = std::min<I64>(k - 1, active_count);
      best += query_top(take);
      add_value(root_path, -1);
    }
    answer = std::max(answer, best);
  };

  struct Frame {
    I32 u;
    I32 idx;
  };

  Vec<Frame> st;
  st.reserve(n);
  st.push_back({1, 0});

  while (!st.empty()) {
    Frame& cur = st.back();

    if (cur.idx == 0) {
      evaluate_root(cur.u);
    }

    if (cur.idx < as<I32>(children[cur.u].size())) {
      const I32 v = children[cur.u][cur.idx++];
      move_root(cur.u, v);
      st.push_back({v, 0});
    } else {
      const I32 u = cur.u;
      st.pop_back();
      if (!st.empty()) {
        const I32 p = st.back().u;
        move_root(u, p);
      }
    }
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
