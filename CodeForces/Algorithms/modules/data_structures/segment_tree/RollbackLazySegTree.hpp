#ifndef CP_MODULES_DATA_STRUCTURES_ROLLBACK_LAZY_SEG_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_ROLLBACK_LAZY_SEG_TREE_HPP

#include "RollbackArray.hpp"
#include "../algebra/Algebra.hpp"

/// @brief Iterative lazy segment tree with snapshot and rollback support.
template <typename ActedMonoid>
struct RollbackLazySegTree {
  using Value = typename ActedMonoid::value_type;
  using Action = typename ActedMonoid::action_type;

  struct Snapshot {
    Size tree = 0;
    Size lazy = 0;
    Size has_lazy = 0;
  };

  I32 n = 0;
  I32 log = 0;
  I32 size = 1;
  RollbackArray<Value> tree;
  RollbackArray<Action> lazy;
  RollbackArray<char> has_lazy;
  VecI32 seg_len;

  RollbackLazySegTree() = default;
  explicit RollbackLazySegTree(I32 size_) { build(size_); }

  template <typename F>
  RollbackLazySegTree(I32 size_, F init) {
    build(size_, init);
  }

  explicit RollbackLazySegTree(const Vec<Value>& values) { build(values); }

  /// @brief Initializes a tree of size @p size_ with the value identity.
  void build(I32 size_) {
    build(size_, [](I32) -> Value { return ActedMonoid::value_identity(); });
  }

  /// @brief Initializes a tree from a vector of leaves.
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
    tree = RollbackArray<Value>(Vec<Value>(size << 1, ActedMonoid::value_identity()));
    lazy = RollbackArray<Action>(Vec<Action>(size, ActedMonoid::action_identity()));
    has_lazy = RollbackArray<char>(Vec<char>(size, false));
    seg_len.assign(size << 1, 1);
    for (I32 i = size - 1; i >= 1; --i) {
      seg_len[i] = seg_len[i << 1] + seg_len[i << 1 | 1];
    }
    FOR(i, n) tree.set(size + i, init(i));
    for (I32 i = size - 1; i >= 1; --i) {
      pull(i);
    }
  }

  /// @brief Returns the current rollback token.
  [[nodiscard]] auto snapshot() const -> Snapshot {
    return Snapshot{
        .tree = tree.snapshot(),
        .lazy = lazy.snapshot(),
        .has_lazy = has_lazy.snapshot(),
    };
  }

  /// @brief Restores the tree to a previous snapshot.
  void rollback(Snapshot snap) {
    tree.rollback(snap.tree);
    lazy.rollback(snap.lazy);
    has_lazy.rollback(snap.has_lazy);
  }

  /// @brief Point assignment after pushing pending actions on the access path.
  void set(I32 idx, const Value& value) {
    my_assert(0 <= idx && idx < n);
    idx += size;
    push_path(idx);
    tree.set(idx, value);
    rebuild_path(idx);
  }

  /// @brief Replaces one point with `combine(current, value)`.
  void multiply(I32 idx, const Value& value) {
    my_assert(0 <= idx && idx < n);
    idx += size;
    push_path(idx);
    tree.set(idx, ActedMonoid::combine(tree.get(idx), value));
    rebuild_path(idx);
  }

  /// @brief Returns the current value at one point.
  [[nodiscard]] auto get(I32 idx) -> Value {
    my_assert(0 <= idx && idx < n);
    idx += size;
    push_path(idx);
    return tree.get(idx);
  }

  /// @brief Materializes the full current leaf array.
  [[nodiscard]] auto get_all() -> Vec<Value> {
    FOR(i, 1, size) push(i);
    const Vec<Value>& all = tree.get_all();
    return {all.begin() + size, all.begin() + size + n};
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
      if (l & 1) left = ActedMonoid::combine(left, tree.get(l++));
      if (r & 1) right = ActedMonoid::combine(tree.get(--r), right);
      l >>= 1;
      r >>= 1;
    }
    return ActedMonoid::combine(left, right);
  }

  [[nodiscard]] auto prod(I32 l, I32 r) -> Value { return query(l, r); }

  /// @brief Aggregate over the whole array.
  [[nodiscard]] auto prod_all() const -> Value { return tree.get(1); }

  /// @brief Applies one lazy action to [l, r).
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
      const Value nxt = ActedMonoid::combine(acc, tree.get(l));
      if (!check(nxt)) {
        while (l < size) {
          push(l);
          l <<= 1;
          const Value left = ActedMonoid::combine(acc, tree.get(l));
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
      const Value nxt = ActedMonoid::combine(tree.get(r), acc);
      if (!check(nxt)) {
        while (r < size) {
          push(r);
          r = r << 1 | 1;
          const Value right = ActedMonoid::combine(tree.get(r), acc);
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
  void pull(I32 idx) {
    tree.set(idx, ActedMonoid::combine(tree.get(idx << 1), tree.get(idx << 1 | 1)));
  }

  void apply_node(I32 idx, const Action& action) {
    tree.set(idx, ActedMonoid::apply(tree.get(idx), action, seg_len[idx]));
    if (idx >= size) return;
    if (has_lazy.get(idx)) lazy.set(idx, ActedMonoid::compose(lazy.get(idx), action));
    else {
      lazy.set(idx, action);
      has_lazy.set(idx, true);
    }
  }

  void push(I32 idx) {
    if (!has_lazy.get(idx)) return;
    const Action action = lazy.get(idx);
    apply_node(idx << 1, action);
    apply_node(idx << 1 | 1, action);
    lazy.set(idx, ActedMonoid::action_identity());
    has_lazy.set(idx, false);
  }

  void push_path(I32 idx) {
    for (I32 h = log; h >= 1; --h) {
      push(idx >> h);
    }
  }

  void rebuild_path(I32 idx) {
    while (idx > 1) {
      idx >>= 1;
      pull(idx);
    }
  }
};

#endif
