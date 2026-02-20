#ifndef CP_MODULES_DATA_STRUCTURES_LAZY_SEG_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_LAZY_SEG_TREE_HPP

#include "_Common.hpp"

/**
 * @brief Recursive lazy-propagation segment tree for range add / range sum.
 * @tparam T Node aggregate type.
 * @tparam U Lazy delta type.
 */
template <typename T, typename U>
struct LazySegTree {
  I32 n;
  Vec<T> tree;
  Vec<U> lazy;

  /// @brief Constructs tree for given size (rounded up to power of two).
  LazySegTree(I32 size) : n(1) {
    while (n < size) n *= 2;
    tree.assign(2 * n, T{});
    lazy.assign(2 * n, U{});
  }

  /// @brief Pushes lazy updates from node @p v down to its children.
  void push(I32 v, I32 tl, I32 tr) {
    if (lazy[v] == U{}) return;
    tree[v] += lazy[v] * (tr - tl);
    if (tl + 1 != tr) {
      lazy[2*v] += lazy[v];
      lazy[2*v + 1] += lazy[v];
    }
    lazy[v] = U{};
  }

  /// @brief Adds @p val to all elements in interval [l, r).
  void update(I32 l, I32 r, U val, I32 v = 1, I32 tl = 0, I32 tr = -1) {
    if (tr == -1) tr = n;
    push(v, tl, tr);
    if (l >= tr || r <= tl) return;
    if (l <= tl && tr <= r) {
      lazy[v] += val;
      push(v, tl, tr);
      return;
    }
    I32 tm = (tl + tr) / 2;
    update(l, r, val, 2*v, tl, tm);
    update(l, r, val, 2*v + 1, tm, tr);
    push(2*v, tl, tm);
    push(2*v + 1, tm, tr);
    tree[v] = tree[2*v] + tree[2*v + 1];
  }

  /// @brief Returns sum of elements in interval [l, r).
  T query(I32 l, I32 r, I32 v = 1, I32 tl = 0, I32 tr = -1) {
    if (tr == -1) tr = n;
    if (l >= tr || r <= tl) return T{};
    push(v, tl, tr);
    if (l <= tl && tr <= r) return tree[v];
    I32 tm = (tl + tr) / 2;
    return query(l, r, 2*v, tl, tm) + query(l, r, 2*v + 1, tm, tr);
  }
};

#endif
