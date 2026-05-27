#ifndef CP_MODULES_DATA_STRUCTURES_UTILITY_DOUBLING_HPP
#define CP_MODULES_DATA_STRUCTURES_UTILITY_DOUBLING_HPP

#include "../_Common.hpp"

/// @brief Binary lifting over transitions with an accumulated monoid value.
template <typename Monoid>
struct Doubling {
  using Value = typename Monoid::value_type;

  I32 n = 0;
  I32 levels = 0;
  bool built = false;
  Vec<VecI32> to;
  Vec<Vec<Value>> prod;

  Doubling() = default;

  Doubling(I32 size_, I32 levels_ = 60) { build_empty(size_, levels_); }

  /// @brief Initializes `size_` states and enough levels for steps below 2^levels_.
  void build_empty(I32 size_, I32 levels_ = 60) {
    my_assert(size_ >= 0 && levels_ > 0);
    n = size_;
    levels = levels_;
    built = false;
    to.assign(levels, VecI32(n, -1));
    prod.assign(levels, Vec<Value>(n, Monoid::identity()));
  }

  /// @brief Sets one base transition from `v` to `nxt`, with edge value `value`.
  void set_next(I32 v, I32 nxt, const Value& value = Monoid::identity()) {
    my_assert(!built);
    my_assert(0 <= v && v < n);
    my_assert(-1 <= nxt && nxt < n);
    to[0][v] = nxt;
    prod[0][v] = value;
  }

  /// @brief Alias for set_next().
  void add(I32 v, I32 nxt, const Value& value = Monoid::identity()) { set_next(v, nxt, value); }

  /// @brief Builds all binary-lifting layers.
  void build() {
    my_assert(!built);
    built = true;
    FOR(k, levels - 1) {
      FOR(v, n) {
        const I32 mid = to[k][v];
        if (mid == -1) {
          to[k + 1][v] = -1;
          prod[k + 1][v] = prod[k][v];
        } else {
          to[k + 1][v] = to[k][mid];
          prod[k + 1][v] = Monoid::combine(prod[k][v], prod[k][mid]);
        }
      }
    }
  }

  /// @brief Returns `{state after steps, accumulated value}`.
  [[nodiscard]] auto jump(I32 v, I64 steps) const -> Pair<I32, Value> {
    my_assert(built);
    my_assert(-1 <= v && v < n);
    my_assert(steps >= 0);
    Value acc = Monoid::identity();
    FOR(k, levels) {
      if (v == -1) break;
      if ((steps >> k) & 1) {
        acc = Monoid::combine(acc, prod[k][v]);
        v = to[k][v];
      }
    }
    my_assert((steps >> levels) == 0);
    return {v, acc};
  }

  /// @brief Largest step count preserving `check(next_state, accumulated_value)`.
  template <typename F>
  [[nodiscard]] auto max_step(I32 v, F check) const -> I64 {
    my_assert(built);
    my_assert(0 <= v && v < n);
    Value acc = Monoid::identity();
    I64 steps = 0;
    my_assert(check(v, acc));
    FOR_R(k, levels) {
      const I32 nxt = to[k][v];
      if (nxt == -1) continue;
      const Value cand = Monoid::combine(acc, prod[k][v]);
      if (check(nxt, cand)) {
        steps += I64(1) << k;
        v = nxt;
        acc = cand;
      }
    }
    return steps;
  }
};

#endif
