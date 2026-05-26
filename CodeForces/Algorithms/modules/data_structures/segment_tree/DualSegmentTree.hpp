#ifndef CP_MODULES_DATA_STRUCTURES_DUAL_SEGMENT_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_DUAL_SEGMENT_TREE_HPP

#include "../_Common.hpp"

/// @brief Dual segment tree for range action updates and point queries.
template <typename Monoid>
struct DualSegmentTree {
  using Value = typename Monoid::value_type;

  I32 n = 0;
  I32 log = 0;
  I32 size = 1;
  Vec<Value> lazy;
  Vec<char> has_lazy;

  DualSegmentTree() = default;
  explicit DualSegmentTree(I32 size_) { build(size_); }

  template <typename F>
  DualSegmentTree(I32 size_, F init) {
    build(size_, init);
  }

  explicit DualSegmentTree(const Vec<Value>& values) { build(values); }

  /// @brief Initializes all points with the monoid identity.
  void build(I32 size_) {
    build(size_, [](I32) -> Value { return Monoid::identity(); });
  }

  /// @brief Initializes leaves from a vector of point values.
  void build(const Vec<Value>& values) {
    build(isz(values), [&](I32 i) -> Value { return values[i]; });
  }

  /// @brief Initializes leaves from a point-value generator.
  template <typename F>
  void build(I32 size_, F init) {
    n = std::max<I32>(size_, 0);
    log  = 0;
    size = 1;
    while (size < std::max<I32>(n, 1)) {
      size <<= 1;
      ++log;
    }
    lazy.assign(size << 1, Monoid::identity());
    has_lazy.assign(size, false);
    FOR(i, n) lazy[size + i] = init(i);
  }

  /// @brief Returns the current value at one point after pushing pending actions.
  [[nodiscard]] auto get(I32 idx) -> Value {
    my_assert(0 <= idx && idx < n);
    idx += size;
    push_path(idx);
    return lazy[idx];
  }

  [[nodiscard]] auto query(I32 idx) -> Value { return get(idx); }

  /// @brief Materializes the full point array.
  [[nodiscard]] auto get_all() -> Vec<Value> {
    FOR(i, 1, size) push(i);
    return {lazy.begin() + size, lazy.begin() + size + n};
  }

  /// @brief Point assignment after resolving pending updates on the access path.
  void set(I32 idx, const Value& value) {
    my_assert(0 <= idx && idx < n);
    idx += size;
    push_path(idx);
    lazy[idx] = value;
  }

  /// @brief Applies one action to every point in [l, r).
  void apply(I32 l, I32 r, const Value& action) {
    my_assert(0 <= l && l <= r && r <= n);
    if (l == r) return;
    l += size;
    r += size;
    push_path(l);
    push_path(r - 1);
    while (l < r) {
      if (l & 1) all_apply(l++, action);
      if (r & 1) all_apply(--r, action);
      l >>= 1;
      r >>= 1;
    }
  }

private:
  /// @brief Applies one action to a tree node.
  void all_apply(I32 idx, const Value& action) {
    if (idx < size) {
      if (has_lazy[idx]) lazy[idx] = Monoid::combine(lazy[idx], action);
      else {
        lazy[idx] = action;
        has_lazy[idx] = true;
      }
      return;
    }
    lazy[idx] = Monoid::combine(lazy[idx], action);
  }

  /// @brief Pushes one internal lazy action to its two children.
  void push(I32 idx) {
    if (!has_lazy[idx]) return;
    all_apply(idx << 1, lazy[idx]);
    all_apply(idx << 1 | 1, lazy[idx]);
    lazy[idx] = Monoid::identity();
    has_lazy[idx] = false;
  }

  /// @brief Pushes every pending action on the root-to-node path.
  void push_path(I32 idx) {
    for (I32 h = log; h >= 1; --h) {
      push(idx >> h);
    }
  }
};

#endif
