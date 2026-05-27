#ifndef CP_MODULES_DATA_STRUCTURES_UTILITY_INTERVALS_HPP
#define CP_MODULES_DATA_STRUCTURES_UTILITY_INTERVALS_HPP

#include "../_Common.hpp"

/// @brief Disjoint half-open interval assignment on a fixed coordinate range.
template <typename Value, typename Coord = I64>
struct Intervals {
  Coord left_bound = 0;
  Coord right_bound = 0;
  Value none = Value{};
  I32 active_count = 0;
  Coord active_length = 0;
  Map<Coord, Value> seg;

  Intervals() = default;

  Intervals(Coord right_, const Value& none_) { build(0, right_, none_); }

  Intervals(Coord left_, Coord right_, const Value& none_) { build(left_, right_, none_); }

  /// @brief Initializes the represented range [left_, right_) with `none_`.
  void build(Coord left_, Coord right_, const Value& none_) {
    my_assert(left_ <= right_);
    left_bound = left_;
    right_bound = right_;
    none = none_;
    active_count = 0;
    active_length = 0;
    seg.clear();
    seg[left_bound] = none;
    seg[right_bound] = none;
  }

  /// @brief Returns `{l, r, value}` for the interval containing `x`.
  [[nodiscard]] auto get(Coord x) const -> Tuple<Coord, Coord, Value> {
    my_assert(left_bound <= x && x < right_bound);
    auto it = std::prev(seg.upper_bound(x));
    auto nxt = std::next(it);
    return {it->first, nxt->first, it->second};
  }

  /// @brief Enumerates intervals intersecting [l, r).
  template <typename F>
  void enumerate(Coord l, Coord r, F f) const {
    my_assert(left_bound <= l && l <= r && r <= right_bound);
    if (l == r) return;
    auto it = std::prev(seg.upper_bound(l));
    while (it->first < r) {
      auto nxt = std::next(it);
      f(std::max(it->first, l), std::min(nxt->first, r), it->second);
      it = nxt;
    }
  }

  /// @brief Assigns value `x` on [l, r), merging equal neighbors.
  void set(Coord l, Coord r, const Value& x) {
    my_assert(left_bound <= l && l <= r && r <= right_bound);
    if (l == r) return;
    split(l);
    split(r);
    auto it = seg.lower_bound(l);
    while (it->first < r) {
      auto nxt = std::next(it);
      remove_active(it->first, nxt->first, it->second);
      it = seg.erase(it);
    }
    seg[l] = x;
    add_active(l, r, x);
    merge_at(l);
    merge_at(r);
  }

  /// @brief Materializes all intervals as `{l, r, value}`.
  [[nodiscard]] auto segments() const -> Vec<Tuple<Coord, Coord, Value>> {
    Vec<Tuple<Coord, Coord, Value>> res;
    auto it = seg.begin();
    while (std::next(it) != seg.end()) {
      auto nxt = std::next(it);
      res.push_back({it->first, nxt->first, it->second});
      it = nxt;
    }
    return res;
  }

private:
  void split(Coord x) {
    if (x == left_bound || x == right_bound || seg.contains(x)) return;
    auto it = std::prev(seg.upper_bound(x));
    seg[x] = it->second;
    if (it->second != none) ++active_count;
  }

  void merge_at(Coord x) {
    if (x == left_bound || x == right_bound) return;
    auto it = seg.find(x);
    if (it == seg.end()) return;
    auto prv = std::prev(it);
    if (prv->second == it->second) {
      if (it->second != none) --active_count;
      seg.erase(it);
    }
  }

  void add_active(Coord l, Coord r, const Value& x) {
    if (x == none) return;
    ++active_count;
    active_length += r - l;
  }

  void remove_active(Coord l, Coord r, const Value& x) {
    if (x == none) return;
    --active_count;
    active_length -= r - l;
  }
};

#endif
