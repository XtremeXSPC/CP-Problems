#ifndef CP_MODULES_NUMBER_THEORY_EUCLID_HPP
#define CP_MODULES_NUMBER_THEORY_EUCLID_HPP

#include "_Common.hpp"

/// @brief Extended Euclidean Algorithm returning gcd and coefficients.
template <typename T>
std::tuple<T, T, T> extended_gcd(T a, T b) {
  if (b == 0) return {a, 1, 0};
  auto [g, x1, y1] = extended_gcd(b, a % b);
  T x = y1;
  T y = x1 - (a / b) * y1;
  return {g, x, y};
}

/// @brief Modular inverse of a modulo m using Extended Euclidean Algorithm.
template <typename T>
T mod_inverse(T a, T m) {
  auto [g, x, _] = extended_gcd(a, m);
  if (g != 1) return -1;  // No inverse exists.
  return ((x % m) + m) % m;
}

/// @brief General Chinese Remainder Theorem for possibly non-coprime moduli.
template <typename T>
std::pair<T, T> chinese_remainder(const Vec<T>& a, const Vec<T>& m) {
  if (a.size() != m.size()) return {-1, -1};

  T x = 0, M = 1;

  FOR(i, sz(a)) {
    if (m[i] <= 0) return {-1, -1};
    auto [g, p, q] = extended_gcd(M, m[i]);
    (void)q;

    T diff = a[i] - x;
    if (diff % g != 0) return {-1, -1};  // No solution.

    T mod = m[i] / g;
    T p_norm = p % mod;
    if (p_norm < 0) p_norm += mod;
    T t = as<T>((as<__int128>(diff / g) * p_norm) % mod);
    if (t < 0) t += mod;

    __int128 next_x = as<__int128>(x) + as<__int128>(M) * t;
    __int128 next_M = as<__int128>(M) * mod;
    if (next_M > std::numeric_limits<T>::max()) return {-1, -1};

    x = as<T>(next_x % next_M);
    M = as<T>(next_M);
    x = ((x % M) + M) % M;
  }

  return {x, M};
}

#endif
