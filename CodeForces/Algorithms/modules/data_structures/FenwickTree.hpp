#ifndef CP_MODULES_DATA_STRUCTURES_FENWICK_TREE_HPP
#define CP_MODULES_DATA_STRUCTURES_FENWICK_TREE_HPP

#include "_Common.hpp"

/// @brief Fenwick tree (Binary Indexed Tree) for prefix sums.
template <typename T>
struct FenwickTree {
  I32 n;
  Vec<T> tree;

  /// @brief Constructs empty BIT of given size.
  FenwickTree(I32 size) : n(size), tree(size + 1, T{}) {}

  /// @brief Adds @p val at position @p idx.
  void add(I32 idx, T val) {
    idx++;  // 1-indexed internally.
    while (idx <= n) {
      tree[idx] += val;
      idx += idx & -idx;
    }
  }

  /// @brief Prefix sum on [0, idx].
  T sum(I32 idx) {
    idx++;
    T res{};
    while (idx > 0) {
      res += tree[idx];
      idx -= idx & -idx;
    }
    return res;
  }

  /// @brief Range sum on [l, r].
  T range_sum(I32 l, I32 r) {  // Sum [l, r].
    return sum(r) - (l > 0 ? sum(l - 1) : T{});
  }

  /// @brief Finds smallest index with prefix sum >= val.
  I32 lower_bound(T val) {
    if (val <= T{}) return 0;
    I32 pos = 0;
    I32 pw = 1;
    while (pw <= n) pw *= 2;
    pw /= 2;

    while (pw > 0) {
      I32 next = pos + pw;
      if (next <= n && tree[next] < val) {
        val -= tree[next];
        pos = next;
      }
      pw /= 2;
    }
    return pos;
  }
};

#endif
