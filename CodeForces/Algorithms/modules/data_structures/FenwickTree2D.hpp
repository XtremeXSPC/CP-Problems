#ifndef CP_MODULES_DATA_STRUCTURES_FENWICK_TREE_2D_HPP
#define CP_MODULES_DATA_STRUCTURES_FENWICK_TREE_2D_HPP

#include "_Common.hpp"

/// @brief 2D Fenwick tree for point updates and rectangle sums.
template <typename T>
struct FenwickTree2D {
  I32 n, m;
  Vec2<T> tree;

  /// @brief Constructs rows x cols empty BIT.
  FenwickTree2D(I32 rows, I32 cols) : n(rows), m(cols), tree(n + 1, Vec<T>(m + 1)) {}

  /// @brief Adds @p val at cell (x, y).
  void add(I32 x, I32 y, T val) {
    x++; y++;
    for (I32 i = x; i <= n; i += i & -i) {
      for (I32 j = y; j <= m; j += j & -j) {
        tree[i][j] += val;
      }
    }
  }

  /// @brief Prefix sum over rectangle [0..x] x [0..y].
  T sum(I32 x, I32 y) {
    x++; y++;
    T res{};
    for (I32 i = x; i > 0; i -= i & -i) {
      for (I32 j = y; j > 0; j -= j & -j) {
        res += tree[i][j];
      }
    }
    return res;
  }

  /// @brief Rectangle sum over inclusive box [x1..x2] x [y1..y2].
  T range_sum(I32 x1, I32 y1, I32 x2, I32 y2) {
    return sum(x2, y2) - sum(x1 - 1, y2) - sum(x2, y1 - 1) + sum(x1 - 1, y1 - 1);
  }
};

#endif
