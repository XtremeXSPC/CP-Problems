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

void solve() {
  STR(a_str);
  INT(n);
  Vec<char> d(n);
  FOR(i, 0, n) {
    INT(x);
    d[i] = char('0' + x);
  }

  U64 a = stoull(a_str);
  I32 L = a_str.size();

  char d_min = d.front();
  char d_max = d.back();

  U64 min_diff = ~0ULL;

  auto check = [&](const string& s) {
    if (s.empty()) return;
    U64 val  = stoull(s);
    U64 diff = (a > val ? a - val : val - a);
    if (diff < min_diff) {
      min_diff = diff;
    }
  };

  if (L > 1) {
    check(string(L - 1, d_max));
  }

  char first_nz = 0;
  for (char c : d) {
    if (c > '0') {
      first_nz = c;
      break;
    }
  }
  if (first_nz != 0) {
    check(first_nz + string(L, d_min));
  }

  FOR(i, 0, L) {
    char c_down = 0;
    for (char c : d) {
      if (c < a_str[i]) c_down = c;
    }
    if (c_down != 0) {
      check(a_str.substr(0, i) + c_down + string(L - 1 - i, d_max));
    }

    char c_up = 0;
    for (char c : d) {
      if (c > a_str[i]) {
        c_up = c;
        break;
      }
    }
    if (c_up != 0) {
      check(a_str.substr(0, i) + c_up + string(L - 1 - i, d_min));
    }

    bool match = false;
    for (char c : d) {
      if (c == a_str[i]) {
        match = true;
        break;
      }
    }
    if (!match) break;

    if (i == L - 1) {
      check(a_str);
    }
  }

  OUT(min_diff);
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
