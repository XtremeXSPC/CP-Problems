#pragma once
#include "Types.hpp"

//===----------------------------------------------------------------------===//
/* Lightweight Stopwatch Utility */

struct Stopwatch {
  using Clock = std::chrono::high_resolution_clock;
  Clock::time_point start;

  Stopwatch() : start(Clock::now()) {}

  [[gnu::always_inline]] F64 elapsed() const { return std::chrono::duration<F64>(Clock::now() - start).count(); }

  void reset() { start = Clock::now(); }

  [[gnu::always_inline]] bool within(F64 limit) const { return elapsed() < limit; }
};
