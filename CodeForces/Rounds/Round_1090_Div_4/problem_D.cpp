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

//===----------------------------------------------------------------------===//
/* Main Solver Function */

constexpr I32 SIEVE_LIMIT = 200000;
VecI64 Primes;
bool PFlags[SIEVE_LIMIT + 1];

void sieve() {
  FOR(i, 2, SIEVE_LIMIT + 1) {
    PFlags[i] = true;
  }
  FOR(N, 2, SIEVE_LIMIT + 1) {
    if (PFlags[N]) {
      Primes.push_back(N);
      if (1LL * N * N <= SIEVE_LIMIT) {
        FOR(M, N*N, SIEVE_LIMIT + 1, N)
          PFlags[M] = false;
      }
    }
  }
}


void solve() {
  I32 SeqLen;
  IN(SeqLen);

  VecI64 Res;
  Res.reserve(SeqLen);

  FOR(i, SeqLen) {
    Res.eb(Primes[i] * Primes[i + 1]);
  }

  OUT(Res);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  sieve();

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
