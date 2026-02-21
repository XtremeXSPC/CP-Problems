#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

void solve() {
  INT(n);
  STR(s);

  I32 ones_parity = 0;
  for (char c : s) ones_parity ^= (c - '0');

  if ((n & 1) == 1 && ones_parity == 1) {
    OUT(-1);
    return;
  }

  const I32 k = (n & 1) ? 0 : ones_parity;
  Vec<I32> ops;
  ops.reserve(n);

  FOR(i, n) {
    const I32 bit = s[i] - '0';
    const I32 use = bit ^ k;
    if (use == 1) ops.eb(i + 1);
  }

  OUT(sz(ops));
  if (ops.empty()) {
    cout << '\n';
    return;
  }

  FOR(i, sz(ops)) {
    if (i) cout << ' ';
    cout << ops[i];
  }
  cout << '\n';
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
