#ifndef CP_MODULES_DATA_STRUCTURES_ACTED_LAZY_SEG_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_ACTED_LAZY_SEG_TREE_HPP

#include "../_Common.hpp"

/// @brief Iterative lazy segment tree driven by a value monoid and an action monoid.
template <typename ActedMonoid>
struct ActedLazySegTree {
  using Value  = typename ActedMonoid::value_type;
  using Action = typename ActedMonoid::action_type;

  I32 n = 0;
  I32 log = 0;
  I32 size = 1;
  Vec<Value> tree;
  Vec<Action> lazy;
  Vec<char> has_lazy;
  VecI32 seg_len;

  ActedLazySegTree() = default;
  explicit ActedLazySegTree(I32 size_) { build(size_); }

  template <typename F>
  ActedLazySegTree(I32 size_, F init) {
    build(size_, init);
  }

  explicit ActedLazySegTree(const Vec<Value>& values) { build(values); }

  /// @brief Initializes a tree of size @p size_ with the value identity.
  void build(I32 size_) {
    build(size_, [](I32) -> Value { return ActedMonoid::value_identity(); });
  }

  /// @brief Initializes a tree from a vector of leaf values.
  void build(const Vec<Value>& values) {
    build(isz(values), [&](I32 i) -> Value { return values[i]; });
  }

  /// @brief Initializes a tree from a leaf generator.
  template <typename F>
  void build(I32 size_, F init) {
    n = std::max<I32>(size_, 0);
    log = 0;
    size = 1;
    while (size < std::max<I32>(n, 1)) {
      size <<= 1;
      ++log;
    }
    tree.assign(size << 1, ActedMonoid::value_identity());
    lazy.assign(size, ActedMonoid::action_identity());
    has_lazy.assign(size, false);
    seg_len.assign(size << 1, 1);
    for (I32 i = size - 1; i >= 1; --i) {
      seg_len[i] = seg_len[i << 1] + seg_len[i << 1 | 1];
    }
    FOR(i, n) tree[size + i] = init(i);
    for (I32 i = size - 1; i >= 1; --i) {
      pull(i);
    }
  }

  /// @brief Point assignment after pushing pending actions on the access path.
  void set(I32 idx, const Value& value) {
    my_assert(0 <= idx && idx < n);
    idx += size;
    push_path(idx);
    tree[idx] = value;
    rebuild_path(idx);
  }

  /// @brief Replaces one point with `combine(current, value)`.
  void multiply(I32 idx, const Value& value) {
    my_assert(0 <= idx && idx < n);
    idx += size;
    push_path(idx);
    tree[idx] = ActedMonoid::combine(tree[idx], value);
    rebuild_path(idx);
  }

  /// @brief Returns the current value at one point.
  [[nodiscard]] auto get(I32 idx) -> Value {
    my_assert(0 <= idx && idx < n);
    idx += size;
    push_path(idx);
    return tree[idx];
  }

  /// @brief Materializes the full current leaf array.
  [[nodiscard]] auto get_all() -> Vec<Value> {
    FOR(i, 1, size) push(i);
    return {tree.begin() + size, tree.begin() + size + n};
  }

  /// @brief Half-open range query on [l, r).
  [[nodiscard]] auto query(I32 l, I32 r) -> Value {
    my_assert(0 <= l && l <= r && r <= n);
    if (l == r) return ActedMonoid::value_identity();
    l += size;
    r += size;
    push_path(l);
    push_path(r - 1);
    Value left = ActedMonoid::value_identity();
    Value right = ActedMonoid::value_identity();
    while (l < r) {
      if (l & 1) left = ActedMonoid::combine(left, tree[l++]);
      if (r & 1) right = ActedMonoid::combine(tree[--r], right);
      l >>= 1;
      r >>= 1;
    }
    return ActedMonoid::combine(left, right);
  }

  [[nodiscard]] auto prod(I32 l, I32 r) -> Value { return query(l, r); }

  /// @brief Returns the aggregate of the whole array.
  [[nodiscard]] auto query_all() const -> Value { return tree[1]; }

  [[nodiscard]] auto prod_all() const -> Value { return tree[1]; }

  /// @brief Applies one lazy action to the half-open range [l, r).
  void apply(I32 l, I32 r, const Action& action) {
    my_assert(0 <= l && l <= r && r <= n);
    if (l == r) return;
    l += size;
    r += size;
    push_path(l);
    push_path(r - 1);
    const I32 l0 = l;
    const I32 r0 = r;
    while (l < r) {
      if (l & 1) apply_node(l++, action);
      if (r & 1) apply_node(--r, action);
      l >>= 1;
      r >>= 1;
    }
    for (I32 h = 1; h <= log; ++h) {
      if (((l0 >> h) << h) != l0) pull(l0 >> h);
      if (((r0 >> h) << h) != r0) pull((r0 - 1) >> h);
    }
  }

  /// @brief Compatibility alias matching the old `LazySegTree::update(l, r, tag)` surface.
  void update(I32 l, I32 r, const Action& action) { apply(l, r, action); }

  /// @brief Applies one action to every `i` such that `l <= (i xor xor_mask) < r`.
  void apply_xor_range(I32 l, I32 r, I32 xor_mask, const Action& action) {
    my_assert(size == std::max<I32>(n, 1));
    my_assert((n & (n - 1)) == 0);
    my_assert(0 <= l && l <= r && r <= n);
    my_assert(0 <= xor_mask && xor_mask < n);
    apply_xor_range_rec(1, 0, n, l, r, xor_mask, action);
  }

  /// @brief Largest `r` such that `check(query(l, r))` holds.
  template <typename F>
  [[nodiscard]] auto max_right(I32 l, F check) -> I32 {
    my_assert(0 <= l && l <= n);
    my_assert(check(ActedMonoid::value_identity()));
    if (l == n) return n;
    l += size;
    push_path(l);
    Value acc = ActedMonoid::value_identity();
    do {
      while ((l & 1) == 0) l >>= 1;
      const Value nxt = ActedMonoid::combine(acc, tree[l]);
      if (!check(nxt)) {
        while (l < size) {
          push(l);
          l <<= 1;
          const Value left = ActedMonoid::combine(acc, tree[l]);
          if (check(left)) {
            acc = left;
            ++l;
          }
        }
        return l - size;
      }
      acc = nxt;
      ++l;
    } while ((l & -l) != l);
    return n;
  }

  /// @brief Smallest `l` such that `check(query(l, r))` holds.
  template <typename F>
  [[nodiscard]] auto min_left(I32 r, F check) -> I32 {
    my_assert(0 <= r && r <= n);
    my_assert(check(ActedMonoid::value_identity()));
    if (r == 0) return 0;
    r += size;
    push_path(r - 1);
    Value acc = ActedMonoid::value_identity();
    do {
      --r;
      while (r > 1 && (r & 1)) r >>= 1;
      const Value nxt = ActedMonoid::combine(tree[r], acc);
      if (!check(nxt)) {
        while (r < size) {
          push(r);
          r = r << 1 | 1;
          const Value right = ActedMonoid::combine(tree[r], acc);
          if (check(right)) {
            acc = right;
            --r;
          }
        }
        return r + 1 - size;
      }
      acc = nxt;
    } while ((r & -r) != r);
    return 0;
  }

private:
  /// @brief Recomputes one internal node from its children.
  void pull(I32 idx) {
    tree[idx] = ActedMonoid::combine(tree[idx << 1], tree[idx << 1 | 1]);
  }

  /// @brief Applies one action directly to one segment-tree node.
  void apply_node(I32 idx, const Action& action) {
    tree[idx] = ActedMonoid::apply(tree[idx], action, seg_len[idx]);
    if (idx >= size) return;
    if (has_lazy[idx]) lazy[idx] = ActedMonoid::compose(lazy[idx], action);
    else {
      lazy[idx] = action;
      has_lazy[idx] = true;
    }
  }

  /// @brief Pushes one pending lazy tag to the two children of @p idx.
  void push(I32 idx) {
    if (!has_lazy[idx]) return;
    apply_node(idx << 1, lazy[idx]);
    apply_node(idx << 1 | 1, lazy[idx]);
    lazy[idx] = ActedMonoid::action_identity();
    has_lazy[idx] = false;
  }

  /// @brief Pushes every pending action on the root-to-node path.
  void push_path(I32 idx) {
    for (I32 h = log; h >= 1; --h) {
      push(idx >> h);
    }
  }

  /// @brief Rebuilds aggregates on the node-to-root path.
  void rebuild_path(I32 idx) {
    while (idx > 1) {
      idx >>= 1;
      pull(idx);
    }
  }

  void apply_xor_range_rec(
      I32 idx,
      I32 seg_l,
      I32 seg_r,
      I32 ql,
      I32 qr,
      I32 xor_mask,
      const Action& action) {
    if (qr <= seg_l || seg_r <= ql) return;
    if (ql <= seg_l && seg_r <= qr) {
      apply_node(idx, action);
      return;
    }
    push(idx);
    const I32 seg_m = (seg_l + seg_r) >> 1;
    const I32 bit = (seg_r - seg_l) >> 1;
    I32 left = idx << 1;
    I32 right = idx << 1 | 1;
    if (xor_mask & bit) std::swap(left, right);
    apply_xor_range_rec(left, seg_l, seg_m, ql, qr, xor_mask, action);
    apply_xor_range_rec(right, seg_m, seg_r, ql, qr, xor_mask, action);
    pull(idx);
  }
};

#endif
