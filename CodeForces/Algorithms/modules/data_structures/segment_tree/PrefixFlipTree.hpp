#ifndef CP_MODULES_DATA_STRUCTURES_PREFIX_FLIP_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_PREFIX_FLIP_TREE_HPP

#include "SegmentTree.hpp"

/**
 * @brief Prefix sign-flip selector for constructive "flip prefix at positive index" tasks.
 *
 * @details Keeps a sparse active set of indices. Repeatedly:
 *  - finds the leftmost currently positive active value,
 *  - erases that index,
 *  - flips signs on the prefix ending there.
 *
 * Active indices often come from boundaries where a desired final sign pattern
 * changes, which is wrapped by prefix_flip_ops_for_signs_1based().
 */
template <typename T = I64>
struct PrefixFlipTree {
  struct Node {
    T mx = neg_infinity<T>;
    T mn = infinity<T>;
  };

  struct Merge {
    auto operator()(const Node& lhs, const Node& rhs) const -> Node {
      return {std::max(lhs.mx, rhs.mx), std::min(lhs.mn, rhs.mn)};
    }
  };

  SegmentTree<Node, Merge> seg;
  Vec<char> lazy;

  explicit PrefixFlipTree(const Vec<T>& values)
    : PrefixFlipTree(values, Vec<char>(values.size(), true)) {}

  PrefixFlipTree(const Vec<T>& values, const Vec<char>& active)
    : seg(make_leaves(values, active), Node{}, Merge{}), lazy(2 * seg.n, false) {}

  [[nodiscard]] auto first_positive() -> I32 {
    if (seg.tree[1].mx <= T{}) return -1;
    return first_positive(1, 0, seg.n);
  }

  void erase(I32 pos) { assign(pos, Node{}, 1, 0, seg.n); }

  void flip_prefix(I32 r) { flip_range(0, r, 1, 0, seg.n); }

  auto pop_first_positive_and_flip_1based() -> I32 {
    const I32 pos = first_positive();
    if (pos == -1) return -1;
    erase(pos);
    flip_prefix(pos + 1);
    return pos + 1;
  }

  auto build_operations_1based(I32 max_ops = -1) -> VecI32 {
    VecI32 ops;
    while (max_ops != 0) {
      const I32 pos = pop_first_positive_and_flip_1based();
      if (pos == -1) break;
      ops.push_back(pos);
      if (max_ops > 0) --max_ops;
    }
    return ops;
  }

private:
  static auto make_leaves(const Vec<T>& values, const Vec<char>& active) -> Vec<Node> {
    const I32 n = isz(values);
    Vec<Node> leaves(n);
    FOR(i, n) {
      if (active[i]) {
        leaves[i] = {values[i], values[i]};
      }
    }
    return leaves;
  }

  void apply(I32 node) {
    const Node old = seg.tree[node];
    seg.tree[node].mx = (old.mn == infinity<T> ? neg_infinity<T> : -old.mn);
    seg.tree[node].mn = (old.mx == neg_infinity<T> ? infinity<T> : -old.mx);
    lazy[node] ^= 1;
  }

  void push(I32 node) {
    if (!lazy[node] || node >= seg.n) return;
    apply(node << 1);
    apply(node << 1 | 1);
    lazy[node] = false;
  }

  void pull(I32 node) {
    seg.tree[node] = seg.op(
        seg.tree[node << 1],
        seg.tree[node << 1 | 1]);
  }

  auto first_positive(I32 node, I32 l, I32 r) -> I32 {
    if (seg.tree[node].mx <= T{}) return -1;
    if (l + 1 == r) return l;
    push(node);

    const I32 mid = (l + r) >> 1;
    if (seg.tree[node << 1].mx > T{}) {
      return first_positive(node << 1, l, mid);
    }
    return first_positive(node << 1 | 1, mid, r);
  }

  void assign(I32 pos, Node value, I32 node, I32 l, I32 r) {
    if (l + 1 == r) {
      seg.tree[node] = value;
      return;
    }
    push(node);

    const I32 mid = (l + r) >> 1;
    if (pos < mid) assign(pos, value, node << 1, l, mid);
    else assign(pos, value, node << 1 | 1, mid, r);
    pull(node);
  }

  void flip_range(I32 ql, I32 qr, I32 node, I32 l, I32 r) {
    if (qr <= l || r <= ql) return;
    if (ql <= l && r <= qr) {
      apply(node);
      return;
    }
    push(node);

    const I32 mid = (l + r) >> 1;
    flip_range(ql, qr, node << 1, l, mid);
    flip_range(ql, qr, node << 1 | 1, mid, r);
    pull(node);
  }
};

template <typename T>
auto prefix_flip_ops_for_signs_1based(const Vec<T>& values, const VecI32& signs) -> VecI32 {
  const I32 n = isz(values);
  Vec<char> active(n, false);
  I32 cnt = 0;

  FOR(i, n) {
    if (signs[i] == signs[i + 1]) continue;
    active[i] = true;
    ++cnt;
  }

  PrefixFlipTree<T> tree(values, active);
  return tree.build_operations_1based(cnt);
}

#endif
