#pragma once
#include "templates/core/TypeTraits.hpp"
#ifdef CP_USE_ADVANCED
  #include "templates/advanced/Concepts.hpp"
  #define CP_BITOPS_TPL template <cp::Integral T>
#else
  #define CP_BITOPS_TPL template <class T>
#endif

//===----------------------------------------------------------------------===//
/* Advanced Bitwise Operations */

CP_BITOPS_TPL
[[gnu::always_inline]] constexpr I32 popcount(T x) {
  using Raw = std::remove_cv_t<T>;
  using U   = cp::make_unsigned_t<Raw>;
  if constexpr (sizeof(Raw) <= 4)
    return __builtin_popcount(static_cast<U32>(static_cast<U>(x)));
  else if constexpr (sizeof(Raw) <= 8)
    return __builtin_popcountll(static_cast<U64>(static_cast<U>(x)));
  else {
    const U ux = static_cast<U>(x);
    return __builtin_popcountll(static_cast<U64>(ux)) + __builtin_popcountll(static_cast<U64>(ux >> 64));
  }
}

CP_BITOPS_TPL
[[gnu::always_inline]] constexpr I32 leading_zeros(T x) {
  using Raw = std::remove_cv_t<T>;
  using U   = cp::make_unsigned_t<Raw>;
  U ux      = static_cast<U>(x);
  if (ux == 0)
    return sizeof(Raw) * 8;
  if constexpr (sizeof(Raw) <= 4) {
    return __builtin_clz(static_cast<U32>(ux)) - (32 - static_cast<I32>(sizeof(Raw) * 8));
  } else if constexpr (sizeof(Raw) <= 8) {
    return __builtin_clzll(static_cast<U64>(ux)) - (64 - static_cast<I32>(sizeof(Raw) * 8));
  } else {
    const U64 hi = static_cast<U64>(ux >> 64);
    if (hi != 0)
      return __builtin_clzll(hi);
    return 64 + __builtin_clzll(static_cast<U64>(ux));
  }
}

CP_BITOPS_TPL
[[gnu::always_inline]] constexpr I32 trailing_zeros(T x) {
  using Raw = std::remove_cv_t<T>;
  using U   = cp::make_unsigned_t<Raw>;
  U ux      = static_cast<U>(x);
  if (ux == 0)
    return sizeof(Raw) * 8;
  if constexpr (sizeof(Raw) <= 4)
    return __builtin_ctz(static_cast<U32>(ux));
  else if constexpr (sizeof(Raw) <= 8)
    return __builtin_ctzll(static_cast<U64>(ux));
  else {
    const U64 lo = static_cast<U64>(ux);
    if (lo != 0)
      return __builtin_ctzll(lo);
    return 64 + __builtin_ctzll(static_cast<U64>(ux >> 64));
  }
}

CP_BITOPS_TPL
[[gnu::always_inline]] constexpr I32 bit_width(T x) {
  return sizeof(T) * 8 - leading_zeros(x);
}

CP_BITOPS_TPL
[[gnu::always_inline]] constexpr T bit_floor(T x) {
  if (x == 0)
    return 0;
  return T(1) << (bit_width(x) - 1);
}

CP_BITOPS_TPL
[[gnu::always_inline]] constexpr T bit_ceil(T x) {
  if (x <= 1)
    return 1;
  return T(1) << bit_width(x - 1);
}

CP_BITOPS_TPL
[[gnu::always_inline]] constexpr I32 parity_sign(T x) { return (popcount(x) & 1) ? -1 : 1; }

CP_BITOPS_TPL
constexpr T kth_bit(I32 k) { return T(1) << k; }

CP_BITOPS_TPL
constexpr bool has_kth_bit(T x, I32 k) { return (x >> k) & 1; }

/// @brief Iterate over set bits in a mask, yielding their 0-based indices.
CP_BITOPS_TPL

struct bit_range {
  T mask;

  struct iterator {
    T current;

    iterator(T mask) : current(mask) {}
    I32 operator*() const { return trailing_zeros(current); }

    iterator& operator++() {
      current &= current - 1;
      return *this;
    }

    bool operator!=(const iterator&) const { return current != 0; }
  };

  bit_range(T mask) : mask(mask) {}
  iterator begin() const { return iterator(mask); }
  iterator end() const { return iterator(0); }
};

CP_BITOPS_TPL

struct subset_range {
  T mask;

  struct iterator {
    T    subset, original;
    bool finished;

    iterator(T mask) : subset(mask), original(mask), finished(false) {}
    T operator*() const { return original ^ subset; }

    iterator& operator++() {
      if (subset == 0)
        finished = true;
      else
        subset = (subset - 1) & original;
      return *this;
    }

    bool operator!=(const iterator&) const { return !finished; }
  };

  subset_range(T mask) : mask(mask) {}
  iterator begin() const { return iterator(mask); }
  iterator end() const { return iterator(0); }
};

#undef CP_BITOPS_TPL
