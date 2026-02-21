#ifndef CP_MODULES_DATA_STRUCTURES_SEGMENT_TREE_MIN_ADD_SEARCH_HPP
#define CP_MODULES_DATA_STRUCTURES_SEGMENT_TREE_MIN_ADD_SEARCH_HPP

#include "_Common.hpp"

/**
 * @brief Segment tree with range add and min/max-based positional search.
 *
 * @details Supports:
 *  - range add update on [l, r)
 *  - range min / range max query on [l, r)
 *  - rightmost index search in [l, r) for:
 *    - value == target
 *    - value <= target
 *
 * @Complexity: O(log n) per operation.
 */
template <typename T = I64>
struct SegmentTreeMinAddSearch {
  I32 n = 0;
  Vec<T> min_val;
  Vec<T> max_val;
  Vec<T> lazy;

  SegmentTreeMinAddSearch() = default;

  explicit SegmentTreeMinAddSearch(I32 size, T initial_value = T{}) {
    init(size, initial_value);
  }

  explicit SegmentTreeMinAddSearch(const Vec<T>& values) { init(values); }

  void init(I32 size, T initial_value = T{}) {
    size = std::max<I32>(size, 0);
    Vec<T> values(as<Size>(size), initial_value);
    init(values);
  }

  void init(const Vec<T>& values) {
    n = as<I32>(values.size());
    const I32 tree_size = std::max<I32>(1, 4 * std::max<I32>(n, 1) + 5);
    min_val.assign(as<Size>(tree_size), T{});
    max_val.assign(as<Size>(tree_size), T{});
    lazy.assign(as<Size>(tree_size), T{});
    if (n > 0) build(1, 0, n - 1, values);
  }

  /// @brief Adds @p delta to all elements in [l, r).
  void range_add(I32 l, I32 r, T delta) {
    normalize_range(l, r);
    if (l >= r || n == 0) return;
      range_add_impl(1, 0, n - 1, l, r - 1, delta);
  }

  /// @brief Minimum value in [l, r). Returns +inf for empty range.
  [[nodiscard]] T range_min(I32 l, I32 r) {
    normalize_range(l, r);
    if (l >= r || n == 0) return infinity<T>;
      return range_min_impl(1, 0, n - 1, l, r - 1);
  }

  /// @brief Maximum value in [l, r). Returns -inf for empty range.
  [[nodiscard]] T range_max(I32 l, I32 r) {
    normalize_range(l, r);
    if (l >= r || n == 0) return -infinity<T>;
      return range_max_impl(1, 0, n - 1, l, r - 1);
  }

  /// @brief Global minimum (all elements), +inf if empty.
  [[nodiscard]] T all_min() const {
    if (n == 0) return infinity<T>;
      return min_val[1];
  }

  /// @brief Global maximum (all elements), -inf if empty.
  [[nodiscard]] T all_max() const {
    if (n == 0) return -infinity<T>;
      return max_val[1];
  }

  /// @brief Finds rightmost index in [l, r) where value == target. Returns -1 if absent.
  [[nodiscard]] I32 find_rightmost_eq(I32 l, I32 r, T target) {
    normalize_range(l, r);
    if (l >= r || n == 0) return -1;
      return find_rightmost_eq_impl(1, 0, n - 1, l, r - 1, target);
  }

  /// @brief Finds rightmost index in [l, r) where value <= target. Returns -1 if absent.
  [[nodiscard]] I32 find_rightmost_leq(I32 l, I32 r, T target) {
    normalize_range(l, r);
    if (l >= r || n == 0) return -1;
      return find_rightmost_leq_impl(1, 0, n - 1, l, r - 1, target);
  }

private:
  static void normalize_range(I32& l, I32& r) {
    if (l < 0) l = 0;
    if (r < 0) r = 0;
  }

  void build(I32 node, I32 tl, I32 tr, const Vec<T>& values) {
    lazy[as<Size>(node)] = T{};
    if (tl == tr) {
      const T v = values[as<Size>(tl)];
      min_val[as<Size>(node)] = v;
      max_val[as<Size>(node)] = v;
      return;
    }

    const I32 tm = (tl + tr) >> 1;
    build(node << 1, tl, tm, values);
    build(node << 1 | 1, tm + 1, tr, values);
    pull(node);
  }

  inline void apply(I32 node, T delta) {
    min_val[as<Size>(node)] += delta;
    max_val[as<Size>(node)] += delta;
    lazy[as<Size>(node)] += delta;
  }

  inline void pull(I32 node) {
    min_val[as<Size>(node)] = std::min(
        min_val[as<Size>(node << 1)],
        min_val[as<Size>(node << 1 | 1)]);
    max_val[as<Size>(node)] = std::max(
        max_val[as<Size>(node << 1)],
        max_val[as<Size>(node << 1 | 1)]);
  }

  inline void push(I32 node) {
    const T lz = lazy[as<Size>(node)];
    if (lz == T{}) return;
    apply(node << 1, lz);
    apply(node << 1 | 1, lz);
    lazy[as<Size>(node)] = T{};
  }

  void range_add_impl(I32 node, I32 tl, I32 tr, I32 ql, I32 qr, T delta) {
    if (qr < tl || tr < ql) return;
    if (ql <= tl && tr <= qr) {
      apply(node, delta);
      return;
    }

    push(node);
    const I32 tm = (tl + tr) >> 1;
    if (ql <= tm) range_add_impl(node << 1, tl, tm, ql, qr, delta);
    if (qr > tm) range_add_impl(node << 1 | 1, tm + 1, tr, ql, qr, delta);
    pull(node);
  }

  T range_min_impl(I32 node, I32 tl, I32 tr, I32 ql, I32 qr) {
    if (qr < tl || tr < ql) return infinity<T>;
    if (ql <= tl && tr <= qr) return min_val[as<Size>(node)];

    push(node);
    const I32 tm = (tl + tr) >> 1;
    return std::min(
        range_min_impl(node << 1, tl, tm, ql, qr),
        range_min_impl(node << 1 | 1, tm + 1, tr, ql, qr));
  }

  T range_max_impl(I32 node, I32 tl, I32 tr, I32 ql, I32 qr) {
    if (qr < tl || tr < ql) return -infinity<T>;
    if (ql <= tl && tr <= qr) return max_val[as<Size>(node)];

    push(node);
    const I32 tm = (tl + tr) >> 1;
    return std::max(
        range_max_impl(node << 1, tl, tm, ql, qr),
        range_max_impl(node << 1 | 1, tm + 1, tr, ql, qr));
  }

  I32 find_rightmost_eq_impl(I32 node, I32 tl, I32 tr, I32 ql, I32 qr, T target) {
    if (qr < tl || tr < ql) return -1;
    if (target < min_val[as<Size>(node)] || target > max_val[as<Size>(node)]) {
      return -1;
    }
    if (tl == tr) return tl;

    push(node);
    const I32 tm = (tl + tr) >> 1;

    I32 res = -1;
    if (qr > tm) res = find_rightmost_eq_impl(node << 1 | 1, tm + 1, tr, ql, qr, target);
    if (res == -1 && ql <= tm) {
      res = find_rightmost_eq_impl(node << 1, tl, tm, ql, qr, target);
    }
    return res;
  }

  I32 find_rightmost_leq_impl(I32 node, I32 tl, I32 tr, I32 ql, I32 qr, T target) {
    if (qr < tl || tr < ql || min_val[as<Size>(node)] > target) return -1;
    if (tl == tr) return tl;

    push(node);
    const I32 tm = (tl + tr) >> 1;

    I32 res = -1;
    if (qr > tm) res = find_rightmost_leq_impl(node << 1 | 1, tm + 1, tr, ql, qr, target);
    if (res == -1 && ql <= tm) {
      res = find_rightmost_leq_impl(node << 1, tl, tm, ql, qr, target);
    }
    return res;
  }
};

#endif
