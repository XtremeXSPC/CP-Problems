#ifndef CP_MODULES_DATA_STRUCTURES_MONOID_SEGMENT_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_MONOID_SEGMENT_TREE_HPP

#include "../algebra/Algebra.hpp"

/// @brief Iterative segment tree driven by a static monoid interface.
template <typename Monoid>
struct MonoidSegmentTree {
  using Value = typename Monoid::value_type;

  I32 n = 0;
  I32 log = 0;
  I32 size = 1;
  Vec<Value> tree;

  MonoidSegmentTree() = default;
  explicit MonoidSegmentTree(I32 size_) { build(size_); }

  template <typename F>
  MonoidSegmentTree(I32 size_, F init) {
    build(size_, init);
  }

  explicit MonoidSegmentTree(const Vec<Value>& values) { build(values); }

  /// @brief Initializes a tree of size @p size_ filled with the monoid identity.
  void build(I32 size_) {
    build(size_, [](I32) -> Value { return Monoid::identity(); });
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
    tree.assign(size << 1, Monoid::identity());
    FOR(i, n) tree[size + i] = init(i);
    FOR_R(i, 1, size) pull(i);
  }

  [[nodiscard]] auto get(I32 idx) const -> Value {
    return tree[size + idx];
  }

  /// @brief Returns the current leaf array.
  [[nodiscard]] auto get_all() const -> Vec<Value> {
    return {tree.begin() + size, tree.begin() + size + n};
  }

  /// @brief Point assignment.
  void set(I32 idx, const Value& value) {
    my_assert(0 <= idx && idx < n);
    idx += size;
    tree[idx] = value;
    while (idx > 1) {
      idx >>= 1;
      pull(idx);
    }
  }

  void update(I32 idx, const Value& value) { set(idx, value); }

  /// @brief Replaces a point with `combine(current, value)`.
  void multiply(I32 idx, const Value& value) {
    my_assert(0 <= idx && idx < n);
    idx += size;
    tree[idx] = Monoid::combine(tree[idx], value);
    while (idx > 1) {
      idx >>= 1;
      pull(idx);
    }
  }

  /// @brief Half-open range query on [l, r).
  [[nodiscard]] auto query(I32 l, I32 r) const -> Value {
    my_assert(0 <= l && l <= r && r <= n);
    Value left = Monoid::identity();
    Value right = Monoid::identity();
    l += size;
    r += size;
    while (l < r) {
      if (l & 1) left = Monoid::combine(left, tree[l++]);
      if (r & 1) right = Monoid::combine(tree[--r], right);
      l >>= 1;
      r >>= 1;
    }
    return Monoid::combine(left, right);
  }

  [[nodiscard]] auto prod(I32 l, I32 r) const -> Value { return query(l, r); }

  /// @brief Returns the canonical segment-tree node ids covering [l, r).
  [[nodiscard]] auto prod_ids(I32 l, I32 r) const -> VecI32 {
    my_assert(0 <= l && l <= r && r <= n);
    VecI32 left_ids;
    VecI32 right_ids;
    l += size;
    r += size;
    while (l < r) {
      if (l & 1) left_ids.push_back(l++);
      if (r & 1) right_ids.push_back(--r);
      l >>= 1;
      r >>= 1;
    }
    std::reverse(all(right_ids));
    left_ids.insert(left_ids.end(), all(right_ids));
    return left_ids;
  }

  [[nodiscard]] auto query_all() const -> Value { return tree[1]; }

  [[nodiscard]] auto prod_all() const -> Value { return tree[1]; }

  /// @brief Range query over `a[i xor xor_mask]` for a commutative monoid.
  [[nodiscard]] auto xor_query(I32 l, I32 r, I32 xor_mask) const -> Value {
    my_assert(0 <= l && l <= r && r <= n);
    my_assert(0 <= xor_mask && xor_mask < size);
    Value acc = Monoid::identity();
    for (I32 depth = 0; depth <= log && l < r; ++depth) {
      const I32 base = size >> depth;
      if (l & 1) acc = Monoid::combine(acc, tree[base + ((l++) ^ xor_mask)]);
      if (r & 1) acc = Monoid::combine(acc, tree[base + ((--r) ^ xor_mask)]);
      l >>= 1;
      r >>= 1;
      xor_mask >>= 1;
    }
    return acc;
  }

  [[nodiscard]] auto xor_prod(I32 l, I32 r, I32 xor_mask) const -> Value {
    return xor_query(l, r, xor_mask);
  }

  /// @brief Largest `r` such that `check(query(l, r))` holds.
  template <typename F>
  [[nodiscard]] auto max_right(I32 l, F check) const -> I32 {
    my_assert(0 <= l && l <= n);
    my_assert(check(Monoid::identity()));
    if (l == n) return n;
    l += size;
    Value acc = Monoid::identity();
    do {
      while ((l & 1) == 0) l >>= 1;
      const Value nxt = Monoid::combine(acc, tree[l]);
      if (!check(nxt)) {
        while (l < size) {
          l <<= 1;
          const Value left = Monoid::combine(acc, tree[l]);
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
  [[nodiscard]] auto min_left(I32 r, F check) const -> I32 {
    my_assert(0 <= r && r <= n);
    my_assert(check(Monoid::identity()));
    if (r == 0) return 0;
    r += size;
    Value acc = Monoid::identity();
    do {
      --r;
      while (r > 1 && (r & 1)) r >>= 1;
      const Value nxt = Monoid::combine(tree[r], acc);
      if (!check(nxt)) {
        while (r < size) {
          r = r << 1 | 1;
          const Value right = Monoid::combine(tree[r], acc);
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
    tree[idx] = Monoid::combine(tree[idx << 1], tree[idx << 1 | 1]);
  }
};

#endif
