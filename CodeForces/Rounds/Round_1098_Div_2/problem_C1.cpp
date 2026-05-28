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

  I64 a = stoll(a_str);
  I32 L = a_str.size();

  char d1 = d[0], d2 = d[1];

  I64 min_diff = -1;

  auto check = [&](const string& s) {
    if (s.empty()) return;
    I64 val  = stoll(s);
    I64 diff = abs(a - val);
    if (min_diff == -1 || diff < min_diff) {
      min_diff = diff;
    }
  };

  if (L > 1) {
    check(string(L - 1, d2));
  }

  char first_nz = (d1 > '0' ? d1 : d2);
  check(first_nz + string(L, d1));

  FOR(i, 0, L) {
    char c_down = 0;
    for (char c : d) {
      if (c < a_str[i]) c_down = c;
    }
    if (c_down != 0) {
      check(a_str.substr(0, i) + c_down + string(L - 1 - i, d2));
    }

    char c_up = 0;
    for (char c : d) {
      if (c > a_str[i]) {
        c_up = c;
        break;
      }
    }
    if (c_up != 0) {
      check(a_str.substr(0, i) + c_up + string(L - 1 - i, d1));
    }

    if (a_str[i] != d1 && a_str[i] != d2) {
      break;
    }
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
