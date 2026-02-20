#ifndef CP_MODULES_NUMBER_THEORY_DISCRETE_LOG_HPP
#define CP_MODULES_NUMBER_THEORY_DISCRETE_LOG_HPP

#include "_Common.hpp"

/**
 * @brief Discrete logarithm via baby-step giant-step.
 * @return Smallest non-negative x such that a^x = b (mod m), or -1 if none.
 */
inline I64 discrete_log(I64 a, I64 b, I64 m) {
  I64 n = sqrt(m) + 1;

  std::unordered_map<I64, I64> values;
  I64 cur = b;
  FOR(p, n) {
    values[cur] = p;
    cur = (__int128)cur * a % m;
  }

  I64 factor = mod_pow(a, n, m);
  cur = factor;

  FOR(q, 1, n + 1) {
    if (values.count(cur)) {
      I64 ans = q * n - values[cur];
      if (mod_pow(a, ans, m) == b) return ans;
    }
    cur = (__int128)cur * factor % m;
  }

  return -1;  // No solution.
}

#endif
