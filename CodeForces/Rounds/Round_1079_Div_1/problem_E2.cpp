#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"
#include "modules/data_structures/SparseTable.hpp"
#include "modules/strings/SuffixArray.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

void solve() {
  INT(n, m);
  STR(s, t);

  String all = s + "%" + t + "$";
  SuffixArray sa(all);
  SparseTable<I32> lcp_st(sa.lcp);

  auto get_lcp = [&](I32 i, I32 j) -> I32 {
    if (i == j) return as<I32>(all.size()) - i;
    I32 ri = sa.rank[i];
    I32 rj = sa.rank[j];
    if (ri > rj) swap(ri, rj);
    return lcp_st.query(ri, rj - 1);
  };

  Vec<I32> from_s;
  FOR(i, as<I32>(sa.sa.size())) {
    if (sa.sa[i] < n) from_s.eb(i);
  }

  auto compare_suffix = [&](I32 start, I32 pos, char ch, I32 idx) -> Pair<I32, I32> {
    if (idx == 0 || idx > n) return {1, 0};

    pos -= start;
    idx--;
    I32 p = sa.sa[from_s[idx]];

    const I32 lcp1 = get_lcp(p, n + 1 + start);
    if (lcp1 < pos) {
      const char a = all[p + lcp1];
      const char b = all[n + 1 + start + lcp1];
      return (a < b) ? Pair<I32, I32>{1, lcp1} : Pair<I32, I32>{-1, lcp1};
    }

    if (lcp1 > pos) {
      char a = all[p + pos];
      char b;
      I32 len = pos;
      if (a != ch) {
        b = ch;
      } else {
        len = lcp1;
        a = all[p + lcp1];
        b = all[n + 1 + start + lcp1];
      }
      return (a < b) ? Pair<I32, I32>{1, len} : Pair<I32, I32>{-1, len};
    }

    char a = all[p + lcp1];
    char b = ch;
    if (a < b) return {1, pos};
    if (a > b) return {-1, pos};

    const I32 lcp2 = get_lcp(p + lcp1 + 1, n + 1 + start + lcp1 + 1);
    a = all[p + lcp1 + 1 + lcp2];
    b = all[n + 1 + start + lcp1 + 1 + lcp2];
    return (a < b) ? Pair<I32, I32>{1, pos + 1 + lcp2} : Pair<I32, I32>{-1, pos + 1 + lcp2};
  };

  auto get_max_len = [&](I32 start, I32 pos, char ch) -> I32 {
    I32 l = 0;
    I32 r = as<I32>(from_s.size());
    while (l <= r) {
      const I32 mid = l + (r - l) / 2;
      if (compare_suffix(start, pos, ch, mid).first == -1) {
        r = mid - 1;
      } else {
        l = mid + 1;
      }
    }
    I32 res = 0;
    res = max(res, compare_suffix(start, pos, ch, l - 1).second);
    res = max(res, compare_suffix(start, pos, ch, l).second);
    return res;
  };

  I32 ans = 0;
  I32 start = 0;
  I32 best = -1;

  FOR(i, m) {
    I32 mx_len = 0;
    FOR(ch, 'a', 'z' + 1) mx_len = max(mx_len, get_max_len(start, i, as<char>(ch)));
    best = max(best, mx_len);

    if (mx_len < i - start + 1) {
      ans++;
      start += best;
      best = -1;
      i = start - 1;
    }
  }

  if (start != m) ans++;
  OUT(ans);
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
