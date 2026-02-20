#ifndef CP_MODULES_DATA_STRUCTURES_SPARSE_TABLE_HPP
#define CP_MODULES_DATA_STRUCTURES_SPARSE_TABLE_HPP

#include "_Common.hpp"

/// @brief Default min operation functor for SparseTable.
template <typename T>
struct SparseTableMinOp {
  constexpr T operator()(const T& a, const T& b) const { return a < b ? a : b; }
};

/// @brief Sparse table for static idempotent range queries.
template <typename T, typename Op = SparseTableMinOp<T>>
struct SparseTable {
  Vec<Vec<T>> table;
  Vec<I32> lg;
  Op op;

  SparseTable() = default;
  explicit SparseTable(const Vec<T>& v, Op merge_op = Op{}) { build(v, merge_op); }

  /// @brief Precomputes sparse table for input vector.
  void build(const Vec<T>& v, Op merge_op = Op{}) {
    op = merge_op;
    const I32 n = sz(v);
    lg.assign(n + 1, 0);
    FOR(i, 2, n + 1) lg[i] = lg[i / 2] + 1;

    if (n == 0) {
      table.clear();
      return;
    }

    const I32 max_log = lg[n] + 1;
    table.assign(max_log, Vec<T>(n));
    FOR(i, n) table[0][i] = v[i];

    FOR(k, 1, max_log) {
      FOR(i, n - (1 << k) + 1) {
        table[k][i] = op(table[k - 1][i], table[k - 1][i + (1 << (k - 1))]);
      }
    }
  }

  /// @brief Query over inclusive range [l, r].
  T query(I32 l, I32 r) const {
    const I32 k = lg[r - l + 1];
    return op(table[k][l], table[k][r - (1 << k) + 1]);
  }
};

#endif
