#pragma once
#include "Types.hpp"

//===----------------------------------------------------------------------===//
/* Advanced Bitwise Operations */

// Enhanced bit manipulation with SIMD hints:
template <typename T>
[[gnu::always_inline]] constexpr I32 popcount(T x) {
  if constexpr (sizeof(T) <= 4) return __builtin_popcount(x);
  else return __builtin_popcountll(x);
}

template <typename T>
[[gnu::always_inline]] constexpr I32 leading_zeros(T x) {
  if (x == 0) return sizeof(T) * 8;
  if constexpr (sizeof(T) <= 4) return __builtin_clz(x);
  else return __builtin_clzll(x);
}

template <typename T>
[[gnu::always_inline]] constexpr I32 trailing_zeros(T x) {
  if (x == 0) return sizeof(T) * 8;
  if constexpr (sizeof(T) <= 4) return __builtin_ctz(x);
  else return __builtin_ctzll(x);
}

template <typename T>
[[gnu::always_inline]] constexpr I32 bit_width(T x) {
  return sizeof(T) * 8 - leading_zeros(x);
}

template <typename T>
[[gnu::always_inline]] constexpr T bit_floor(T x) {
  if (x == 0) return 0;
  return T(1) << (bit_width(x) - 1);
}

template <typename T>
[[gnu::always_inline]] constexpr T bit_ceil(T x) {
  if (x <= 1) return 1;
  return T(1) << bit_width(x - 1);
}

// Legacy aliases:
template <typename T> constexpr I32 popcnt(T x) { return popcount(x); }
template <typename T> constexpr I32 topbit(T x) { return bit_width(x) - 1; }
template <typename T> constexpr I32 lowbit(T x) { return trailing_zeros(x); }

// Bit iteration utilities:
template <typename T>
constexpr T kth_bit(I32 k) { return T(1) << k; }

template <typename T>
constexpr bool has_kth_bit(T x, I32 k) { return (x >> k) & 1; }

// Bit iteration ranges:
template <typename T>
struct bit_range {
  T mask;
  struct iterator {
    T current;
    iterator(T mask) : current(mask) {}
    I32 operator*() const { return trailing_zeros(current); }
    iterator& operator++() { current &= current - 1; return *this; }
    bool operator!=(const iterator&) const { return current != 0; }
  };
  bit_range(T mask) : mask(mask) {}
  iterator begin() const { return iterator(mask); }
  iterator end() const { return iterator(0); }
};

template <typename T>
struct subset_range {
  T mask;
  struct iterator {
    T subset, original;
    bool finished;
    iterator(T mask) : subset(mask), original(mask), finished(false) {}
    T operator*() const { return original ^ subset; }
    iterator& operator++() {
      if (subset == 0) finished = true;
      else subset = (subset - 1) & original;
      return *this;
    }
    bool operator!=(const iterator&) const { return !finished; }
  };
  subset_range(T mask) : mask(mask) {}
  iterator begin() const { return iterator(mask); }
  iterator end() const { return iterator(0); }
};
