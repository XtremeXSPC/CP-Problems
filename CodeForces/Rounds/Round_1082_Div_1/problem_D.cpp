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
#include "modules/data_structures/SegmentTree.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct Node {
  I64 mx   = -1;
  I64 cnt  = 0;
  I64 pref = 0;
  I64 suff = 0;
  I32 len  = 0;
  bool all = true;
};

auto make_leaf(I64 weight) -> Node {
  Node node;
  node.len = 1;
  node.mx  = weight;
  if (weight >= 0) {
    node.cnt  = 1;
    node.pref = 1;
    node.suff = 1;
  }
  return node;
}

auto merge_node(const Node& left, const Node& right) -> Node {
  if (left.len == 0) {
    return right;
  }
  if (right.len == 0) {
    return left;
  }

  Node res;
  res.len = left.len + right.len;

  if (left.mx == right.mx) {
    res.mx   = left.mx;
    res.cnt  = left.cnt + right.cnt + left.suff * right.pref;
    res.pref = left.pref + (left.all ? right.pref : 0);
    res.suff = right.suff + (right.all ? left.suff : 0);
    res.all  = left.all && right.all;
  } else if (left.mx > right.mx) {
    res.mx   = left.mx;
    res.cnt  = left.cnt;
    res.pref = left.pref;
    res.suff = 0;
    res.all  = false;
  } else {
    res.mx   = right.mx;
    res.cnt  = right.cnt;
    res.pref = 0;
    res.suff = right.suff;
    res.all  = false;
  }

  return res;
}

struct NodeOp {
  auto operator()(const Node& left, const Node& right) const -> Node {
    return merge_node(left, right);
  }
};

void solve() {
  INT(n, q);

  VecI32 a(n + 1);
  FOR(i, 1, n + 1) IN(a[i]);

  Vec<Set<I32>> pos(n + 1);
  FOR(i, 1, n + 1) {
    pos[a[i]].insert(i);
  }

  Vec<Node> init_tree(n, make_leaf(-1));
  FOR(v, 1, n + 1) {
    if (pos[v].empty()) {
      continue;
    }
    const I32 first = *pos[v].begin();
    const I32 last  = *pos[v].rbegin();
    init_tree[first - 1] = make_leaf(as<I64>(last - first));
  }

  SegmentTree<Node, NodeOp> seg(init_tree, Node{}, NodeOp{});

  auto refresh = [&](I32 idx) -> void {
    if (idx < 1 || idx > n) {
      return;
    }
    const I32 value = a[idx];
    I64 weight = -1;
    if (!pos[value].empty() && *pos[value].begin() == idx) {
      weight = as<I64>(*pos[value].rbegin() - *pos[value].begin());
    }
    seg.update(idx - 1, make_leaf(weight));
  };

  FOR(_, q) {
    INT(idx, x);

    if (a[idx] != x) {
      const I32 old_value = a[idx];

      VecI32 affected = {idx};
      if (!pos[old_value].empty()) {
        affected.eb(*pos[old_value].begin());
      }
      if (!pos[x].empty()) {
        affected.eb(*pos[x].begin());
      }

      pos[old_value].erase(idx);
      pos[x].insert(idx);
      a[idx] = x;

      if (!pos[old_value].empty()) {
        affected.eb(*pos[old_value].begin());
      }
      if (!pos[x].empty()) {
        affected.eb(*pos[x].begin());
      }

      std::sort(all(affected));
      affected.erase(std::unique(all(affected)), affected.end());

      for (I32 p : affected) {
        refresh(p);
      }
    }

    const Node& root = seg.tree[1];
    if (root.mx <= 0) {
      OUT(0, 0);
    } else {
      OUT(root.mx, root.cnt);
    }
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
