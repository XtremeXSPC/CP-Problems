#ifndef CP_MODULES_DATA_STRUCTURES_SEGMENT_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_SEGMENT_TREE_HPP

#include "_Common.hpp"

/**
 * @brief Iterative segment tree for point updates and range queries.
 * @tparam T Value type.
 * @tparam F Associative merge function type.
 */
template <typename T, typename F = std::plus<T>>
struct SegmentTree {
  I32 n;
  Vec<T> tree;
  F op;
  T identity;

  /// @brief Constructs empty segment tree with identity element.
  SegmentTree(I32 size, T id = T{}, F func = F{})
    : n(1), op(func), identity(id) {
    while (n < size) n *= 2;
    tree.assign(2 * n, identity);
  }

  /// @brief Constructs segment tree from initial array.
  SegmentTree(const Vec<T>& v, T id = T{}, F func = F{})
    : SegmentTree(sz(v), id, func) {
    FOR(i, sz(v)) tree[n + i] = v[i];
    FOR_R(i, 1, n) tree[i] = op(tree[2*i], tree[2*i + 1]);
  }

  /// @brief Point assignment at position @p pos.
  void update(I32 pos, T val) {
    pos += n;
    tree[pos] = val;
    while (pos > 1) {
      tree[pos >> 1] = op(tree[pos], tree[pos ^ 1]);
      pos >>= 1;
    }
  }

  /// @brief Range query over half-open interval [l, r).
  T query(I32 l, I32 r) {  // [l, r)
    T res_left = identity, res_right = identity;
    l += n; r += n;
    while (l < r) {
      if (l & 1) res_left = op(res_left, tree[l++]);
      if (r & 1) res_right = op(tree[--r], res_right);
      l >>= 1; r >>= 1;
    }
    return op(res_left, res_right);
  }

  /// @brief Returns current value at index @p pos.
  T get(I32 pos) { return tree[n + pos]; }
};

#endif
