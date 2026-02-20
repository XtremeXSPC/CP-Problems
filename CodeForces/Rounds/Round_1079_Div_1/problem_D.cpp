#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);
  STR(s);

  Vec<bool> matched(n, false);

  // Greedily match all free "()" pairs.
  Vec<I32> stk;
  FOR(i, n) {
    if (s[i] == '(') {
      stk.push_back(i);
    } else if (s[i] == ')') {
      if (!stk.empty()) {
        matched[stk.back()] = true;
        matched[i] = true;
        stk.pop_back();
      }
    }
  }

  // Greedily match all free "[]" pairs.
  stk.clear();
  FOR(i, n) {
    if (s[i] == '[') {
      stk.push_back(i);
    } else if (s[i] == ']') {
      if (!stk.empty()) {
        matched[stk.back()] = true;
        matched[i] = true;
        stk.pop_back();
      }
    }
  }

  const I32 free_pairs = std::ranges::count(matched, true) / 2;
  I32 ans = n / 2 - free_pairs;

  // Check if we can fix the last pair with a single change.
  I32 closes = 0;
  bool seen_open = false;
  bool close_then_open = true;

  FOR(i, n) {
    if (matched[i]) continue;
    const bool is_close = (s[i] == ')' || s[i] == ']');
    if (is_close) {
      ++closes;
      if (seen_open) close_then_open = false;
    } else {
      seen_open = true;
    }
  }

  if (close_then_open && (closes & 1))
    ++ans;

  OUT(ans);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
