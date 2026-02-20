#pragma once
#include "Types.hpp"

//===----------------------------------------------------------------------===//
/* Advanced Bitwise Operations */

template <typename T>
[[gnu::always_inline]] constexpr I32 popcount(T x) {
  using Raw = std::remove_cv_t<T>;
  using U = std::conditional_t<std::is_same_v<Raw, bool>, U8, std::make_unsigned_t<Raw>>;
  if constexpr (sizeof(T) <= 4) return __builtin_popcount(static_cast<U32>(static_cast<U>(x)));
  else return __builtin_popcountll(static_cast<U64>(static_cast<U>(x)));
}

template <typename T>
[[gnu::always_inline]] constexpr I32 leading_zeros(T x) {
  using Raw = std::remove_cv_t<T>;
  using U = std::conditional_t<std::is_same_v<Raw, bool>, U8, std::make_unsigned_t<Raw>>;
  U ux = static_cast<U>(x);
  if (ux == 0) return sizeof(T) * 8;
  if constexpr (sizeof(T) <= 4) {
    return __builtin_clz(static_cast<U32>(ux)) - (32 - static_cast<I32>(sizeof(T) * 8));
  } else {
    return __builtin_clzll(static_cast<U64>(ux)) - (64 - static_cast<I32>(sizeof(T) * 8));
  }
}

template <typename T>
[[gnu::always_inline]] constexpr I32 trailing_zeros(T x) {
  using Raw = std::remove_cv_t<T>;
  using U = std::conditional_t<std::is_same_v<Raw, bool>, U8, std::make_unsigned_t<Raw>>;
  U ux = static_cast<U>(x);
  if (ux == 0) return sizeof(T) * 8;
  if constexpr (sizeof(T) <= 4) return __builtin_ctz(static_cast<U32>(ux));
  else return __builtin_ctzll(static_cast<U64>(ux));
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

template <typename T>
[[gnu::always_inline]] constexpr I32 parity_sign(T x) {
  return (popcount(x) & 1) ? -1 : 1;
}

template <typename T>
constexpr T kth_bit(I32 k) { return T(1) << k; }

template <typename T>
constexpr bool has_kth_bit(T x, I32 k) { return (x >> k) & 1; }

/// @brief Iterate over set bits in a mask, yielding their 0-based indices.
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

// Legacy aliases (Deprecated -- use popcount, bit_width, trailing_zeros instead):
template <typename T>
[[deprecated("use popcount() instead")]]
constexpr I32 popcnt(T x) { return popcount(x); }
template <typename T>
[[deprecated("use bit_width() - 1 instead")]]
constexpr I32 topbit(T x) { return bit_width(x) - 1; }
template <typename T>
[[deprecated("use trailing_zeros() instead")]]
constexpr I32 lowbit(T x) { return trailing_zeros(x); }
