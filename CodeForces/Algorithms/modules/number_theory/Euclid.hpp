#ifndef CP_MODULES_NUMBER_THEORY_EUCLID_HPP
#define CP_MODULES_NUMBER_THEORY_EUCLID_HPP

#include "_Common.hpp"

/**
 * @brief Extended Euclidean algorithm.
 * @tparam T Integer type.
 * @return Tuple (g, x, y) such that ax + by = g = gcd(a, b).
 */
template <typename T>
std::tuple<T, T, T> extended_gcd(T a, T b) {
  if (b == 0) return {a, 1, 0};
  auto [g, x1, y1] = extended_gcd(b, a % b);
  T x = y1;
  T y = x1 - (a / b) * y1;
  return {g, x, y};
}

/**
 * @brief Modular inverse using extended gcd.
 * @return a^{-1} mod m, or -1 if inverse does not exist.
 */
template <typename T>
T mod_inverse(T a, T m) {
  auto [g, x, _] = extended_gcd(a, m);
  if (g != 1) return -1;  // No inverse exists.
  return ((x % m) + m) % m;
}

/**
 * @brief General Chinese Remainder Theorem for possibly non-coprime moduli.
 * @return Pair (x, M) meaning x mod M is the combined congruence, or {-1,-1}.
 */
template <typename T>
std::pair<T, T> chinese_remainder(const Vec<T>& a, const Vec<T>& m) {
  T x = 0, M = 1;

  FOR(i, sz(a)) {
    auto [g, p, q] = extended_gcd(M, m[i]);
    if ((a[i] - x) % g != 0) return {-1, -1};  // No solution.

    x += M * ((a[i] - x) / g * p % (m[i] / g));
    M *= m[i] / g;
    x = ((x % M) + M) % M;
  }

  return {x, M};
}

#endif
