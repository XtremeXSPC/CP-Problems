#pragma once
#include "Types.hpp"
#include "Constants.hpp"

//===----------------------------------------------------------------------===//
/* Advanced Modular Arithmetic */

// Modular integer class with operator overloading and optimizations:
template <I64 MOD>
struct ModInt {
  static_assert(MOD > 0, "ModInt requires MOD > 0.");
  U64 value;

  static constexpr I64  mod() { return MOD; }

  constexpr ModInt() : value(0) {}
  constexpr ModInt(I64 x) {
    I64 r = x % MOD;
    value = static_cast<U64>(r < 0 ? r + MOD : r);
  }

  constexpr ModInt& operator+=(const ModInt& other) {
    if ((value += other.value) >= MOD)
      value -= MOD;
    return *this;
  }

  constexpr ModInt& operator-=(const ModInt& other) {
    if ((value += MOD - other.value) >= MOD)
      value -= MOD;
    return *this;
  }

  constexpr ModInt& operator*=(const ModInt& other) {
#if HAS_INT128
    value = static_cast<U64>(static_cast<U128>(value) * other.value % static_cast<U128>(MOD));
#else
    static_assert(MOD <= 2'000'000'000LL,
      "ModInt multiplication may overflow U64 for MOD > 2e9 without __int128 support.");
    value = value * other.value % static_cast<U64>(MOD);
#endif
    return *this;
  }

  constexpr ModInt& operator/=(const ModInt& other) { return *this *= other.inverse(); }

  constexpr ModInt operator+(const ModInt& other) const { return ModInt(*this) += other; }
  constexpr ModInt operator-(const ModInt& other) const { return ModInt(*this) -= other; }
  constexpr ModInt operator*(const ModInt& other) const { return ModInt(*this) *= other; }
  constexpr ModInt operator/(const ModInt& other) const { return ModInt(*this) /= other; }
  constexpr ModInt operator-() const { return ModInt(value ? MOD - value : 0); }

  constexpr bool operator==(const ModInt& other) const { return value == other.value; }
  constexpr bool operator!=(const ModInt& other) const { return value != other.value; }

  constexpr ModInt pow(I64 exp) const {
    ModInt result(1), base(*this);
    while (exp > 0) {
      if (exp & 1)
        result *= base;
      base *= base;
      exp >>= 1;
    }
    return result;
  }

  constexpr ModInt inverse() const {
    // Extended Euclidean algorithm; inverse exists iff gcd(value, MOD) == 1.
    I64 a = static_cast<I64>(value), b = MOD, u = 1, v = 0;
    while (b > 0) {
      I64 t = a / b;
      std::swap(a -= t * b, b);
      std::swap(u -= t * v, v);
    }
    if (a != 1) {
      my_assert(false && "ModInt inverse does not exist when gcd(value, MOD) != 1.");
      return ModInt(0);
    }
    return ModInt(u);
  }

  explicit             operator I64() const { return value; }
  friend std::ostream& operator<<(std::ostream& os, const ModInt& x) { return os << x.value; }
  friend std::istream& operator>>(std::istream& is, ModInt& x) {
    I64 val;
    is >> val;
    x = ModInt(val);
    return is;
  }
};

using mint  = ModInt<MOD>;
using mint2 = ModInt<MOD2>;
