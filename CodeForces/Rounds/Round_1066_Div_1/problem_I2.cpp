#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

void solve() {
  INT(t);
  Vec<PairI32> queries(t);

  UnorderedMap<I32, I32> need_n;
  need_n.reserve(as<size_t>(t) * 2U + 16U);

  I32 max_n = 0;
  FOR(i, t) {
    INT(n, m);
    queries[i] = {n, m};
    max_n = max(max_n, n);
    auto [it, inserted] = need_n.emplace(m, n);
    if (!inserted) it->second = max(it->second, n);
  }

  VecI32 seen(max_n + 1, 0);
  Vec<U8> memo(max_n + 1, 0);
  I32 stamp = 0;

  UnorderedMap<I32, VecI32> evils_by_m;
  evils_by_m.reserve(need_n.size() * 2U + 16U);

  for (const auto& [m, n_limit] : need_n) {
    if ((m & 1) == 0) continue;

    VecI32 evil;
    evil.reserve(n_limit / max(1, m) + 8);
    if (m + 1 > n_limit) {
      evils_by_m.emplace(m, std::move(evil));
      continue;
    }

    const I32 half = (m + 1) / 2;
    VecI32 all;
    all.reserve(n_limit / max(1, m) + 8);
    all.push_back(0);
    all.push_back(m + 1);

    ++stamp;
    auto can = [&](auto& self, I32 n) -> bool {
      if (n <= 0) return false;
      if (seen[n] == stamp) return memo[n] != 0;

      const I32 j = as<I32>(upper_bound(all.begin(), all.end(), n) - all.begin()) - 1;
      const I32 s = all[j];
      if (j == 0) {
        const bool win = (n < m);
        seen[n] = stamp;
        memo[n] = as<U8>(win ? 1 : 0);
        return win;
      }

      if (n == s) {
        seen[n] = stamp;
        memo[n] = 0;
        return false;
      }

      const I32 d = n - s;
      bool win = false;

      if (j >= 1) {
        const I32 gap = s - all[j - 1];
        if (gap == m + 2) {
          const I32 x = d + 1;
          if (x <= m && x == half) win = true;
        }
      }

      if (!win && (d % 2 == 0)) {
        const I32 x = d / 2;
        win = !self(self, n - x);
      }

      if (!win && j >= 1) {
        const I32 prev = all[j - 1];
        const I32 e = n - prev;
        if ((e % 2) == 0) {
          const I32 x = e / 2;
          if (x <= m && x != d) win = !self(self, n - x);
        }
      }

      seen[n] = stamp;
      memo[n] = as<U8>(win ? 1 : 0);
      return win;
    };

    I32 s = m + 1;
    evil.push_back(s);
    while (true) {
      const I64 cand = as<I64>(s) + m + 1;
      if (cand > n_limit) break;
      const I32 mid = s + half;
      const bool cand_is_evil = can(can, mid);
      const I64 nxt = cand_is_evil ? cand : cand + 1;
      if (nxt > n_limit) break;
      s = as<I32>(nxt);
      evil.push_back(s);
      all.push_back(s);
    }

    evils_by_m.emplace(m, std::move(evil));
  }

  for (const auto& [n, m] : queries) {
    bool win = true;
    if ((m & 1) == 0) {
      win = (n % (m + 1) != 0);
    } else {
      const auto& ev = evils_by_m[m];
      win = !binary_search(ev.begin(), ev.end(), n);
    }
    YES(win);
  }
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  solve();

  return 0;
}

//===----------------------------------------------------------------------===//
