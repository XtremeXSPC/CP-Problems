#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_TIMER
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

#include "modules/Number_Theory.hpp"

//===----------------------------------------------------------------------===//
/* Global State */

using PrimeGain    = Pair<I64, I32>;
using VecPrimeGain = Vec<PrimeGain>;

UnorderedMap<I64, VecI64> factor_cache;

auto get_shared_primes(const I64 value) -> const VecI64& {
  if (auto it = factor_cache.find(value); it != factor_cache.end()) {
    return it->second;
  }

  VecI64 factors = factorize(value);
  factors.erase(std::unique(all(factors)), factors.end());
  return factor_cache.emplace(value, std::move(factors)).first->second;
}

auto get_extension_gain(const VecPrimeGain& gains, const I64 prime) -> I32 {
  for (const auto& [current_prime, extra] : gains) {
    if (current_prime == prime) {
      return extra;
    }
  }
  return 0;
}

auto update_best_gain(VecPrimeGain& gains, const I64 prime, const I32 delta) -> void {
  for (auto& [current_prime, best_delta] : gains) {
    if (current_prime == prime) {
      chmax(best_delta, delta);
      return;
    }
  }
  gains.emplace_back(prime, delta);
}

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);

  VecI64 values(n + 1, 0);
  VecI32 subtree_size(n + 1, 0);
  VecI32 best_edges(n + 1, 0);
  VecI32 max_extra(n + 1, 0);
  Vec<VecPrimeGain> extension_gain(n + 1);

  FOR(step, n) {
    const I32 u = n - step;

    I64 value;
    I32 child_count;
    IN(value, child_count);
    values[u] = value;

    I32 base_edges = 0;
    I32 total_size = 1;
    VecPrimeGain best_delta_by_prime;

    FOR(_, child_count) {
      INT(v);
      base_edges += best_edges[v];
      total_size += subtree_size[v];

      const I64 shared_gcd = std::gcd(value, values[v]);
      if (shared_gcd == 1) {
        continue;
      }

      const VecI64& shared_primes = get_shared_primes(shared_gcd);
      for (const I64 prime : shared_primes) {
        const I32 child_extra = get_extension_gain(extension_gain[v], prime);
        const I32 delta = child_extra + 1 - max_extra[v];
        update_best_gain(best_delta_by_prime, prime, delta);
      }
    }

    subtree_size[u] = total_size;

    I32 best_extension = 0;
    VecPrimeGain stored_gain;
    stored_gain.reserve(sz(best_delta_by_prime));
    for (const auto& [prime, delta] : best_delta_by_prime) {
      if (delta > 0) {
        stored_gain.emplace_back(prime, delta);
        chmax(best_extension, delta);
      }
    }

    sort(all(stored_gain));
    extension_gain[u] = std::move(stored_gain);
    max_extra[u] = best_extension;
    best_edges[u] = base_edges + best_extension;

    OUT(subtree_size[u] - best_edges[u]);
    FLUSH();
  }
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
