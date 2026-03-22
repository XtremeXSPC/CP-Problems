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

constexpr I32 MAX_LEN = 100000;
constexpr I32 MAX_SUM = 9 * MAX_LEN;

Vec2D<I32> candidates_by_sum;

auto digit_sum(I32 x) -> I32 {
  I32 sum = 0;
  while (x > 0) {
    sum += x % 10;
    x /= 10;
  }
  return sum;
}

auto consume_number_digits(I32 x, VecI32& freq) -> bool {
  while (x > 0) {
    const I32 digit = x % 10;
    --freq[digit];
    if (freq[digit] < 0) {
      return false;
    }
    x /= 10;
  }
  return true;
}

auto init_candidates() -> void {
  candidates_by_sum.assign(MAX_SUM + 1, {});

  FOR(v, 1, MAX_SUM + 1) {
    I64 total = 0;
    I32 cur = v;
    while (true) {
      total += cur;
      if (cur <= 9) {
        total += cur;
        break;
      }
      cur = digit_sum(cur);
    }

    if (total <= MAX_SUM) {
      candidates_by_sum[as<I32>(total)].eb(v);
    }
  }
}

void solve() {
  STR(s);
  const I32 n = as<I32>(s.size());

  if (n == 1) {
    OUT(s);
    return;
  }

  VecI32 freq(10, 0);
  I32 total_sum = 0;
  for (char ch : s) {
    const I32 d = ch - '0';
    ++freq[d];
    total_sum += d;
  }

  for (const I32 x1 : candidates_by_sum[total_sum]) {
    VecI32 rem = freq;

    bool valid = true;
    I32 cur    = x1;
    while (true) {
      if (!consume_number_digits(cur, rem)) {
        valid = false;
        break;
      }
      if (cur <= 9) {
        break;
      }
      cur = digit_sum(cur);
    }
    if (!valid) {
      continue;
    }

    I32 rem_sum = 0;
    FOR(d, 10) rem_sum += d * rem[d];
    if (rem_sum != x1) {
      continue;
    }

    I32 first = -1;
    FOR(d, 1, 10) {
      if (rem[d] > 0) {
        first = d;
        break;
      }
    }
    if (first == -1) {
      continue;
    }

    String answer;
    answer.reserve(n);

    answer.push_back(static_cast<char>('0' + first));
    --rem[first];

    FOR(d, 10) {
      if (rem[d] > 0) {
        answer.append(rem[d], static_cast<char>('0' + d));
      }
    }

    cur = x1;
    while (true) {
      answer += std::to_string(cur);
      if (cur <= 9) {
        break;
      }
      cur = digit_sum(cur);
    }

    OUT(answer);
    return;
  }

  OUT(s);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  init_candidates();

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
