#define NEED_CORE
#define NEED_FAST_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

namespace {

struct DynamicBitset {
  I32 bits = 0;
  Vec<U64> words;

  void init(I32 total_bits) {
    bits = std::max<I32>(1, total_bits);
    words.assign((bits + 63) >> 6, 0ULL);
    words[0] = 1ULL; // sum 0 reachable
  }

  void or_shift(I32 shift) {
    if (shift <= 0) return;
    const I32 word_shift = shift >> 6;
    const I32 bit_shift = shift & 63;
    const I32 n_words = as<I32>(words.size());

    for (I32 i = n_words - 1; i >= 0; --i) {
      const U64 cur = words[as<Size>(i)];
      if (cur == 0ULL) continue;

      const I32 dst = i + word_shift;
      if (dst >= n_words) continue;

      words[as<Size>(dst)] |= (cur << bit_shift);
      if (bit_shift != 0 && dst + 1 < n_words) {
        words[as<Size>(dst + 1)] |= (cur >> (64 - bit_shift));
      }
    }

    const I32 extra = (n_words << 6) - bits;
    if (extra > 0) {
      words.back() &= (~0ULL >> extra);
    }
  }

  [[nodiscard]] bool test(I32 pos) const {
    return ((words[as<Size>(pos >> 6)] >> (pos & 63)) & 1ULL) != 0ULL;
  }
};

[[nodiscard]] inline I64 mod_norm(I128 value, I64 mod) {
  I128 r = value % mod;
  if (r < 0) r += mod;
  return as<I64>(r);
}

} // namespace

void solve() {
  LL(n, x, y);
  STR(s);

  I64 w0 = 0;
  VLL v;
  v.reserve(as<Size>(s.size()));

  I64 a = 1;
  I64 cur = 0;
  bool first_q = true;

  for (char ch : s) {
    if (ch == '?') {
      if (first_q) {
        w0 = cur;
        first_q = false;
      } else {
        v.push_back(std::abs(cur));
      }
      cur = 0;
      a = 1;
    }

    if (ch == '1') a = -a;
    cur += a;
  }

  if (first_q) {
    w0 = cur;
  } else {
    v.push_back(std::abs(cur));
  }

  Map<I64, I32> counts;
  I64 sum_v_i64 = 0;
  for (I64 x_v : v) {
    ++counts[x_v];
    sum_v_i64 += x_v;
  }

  if (sum_v_i64 < 0 || sum_v_i64 > as<I64>(1e7)) {
    // Defensive limit; problem constraints keep this manageable.
    #ifndef NDEBUG
      assert(false && "unexpected subset-sum size");
    #endif
  }
  const I32 sum_v = as<I32>(sum_v_i64);

  VI grouped;
  grouped.reserve(as<Size>(counts.size() * 3));
  for (const auto& [value, cnt0] : counts) {
    if (value == 0) continue;
    I32 cnt = cnt0;
    I32 k = 1;
    while (cnt >= k) {
      grouped.push_back(as<I32>(value * k));
      cnt -= k;
      k <<= 1;
    }
    if (cnt > 0) {
      grouped.push_back(as<I32>(value * cnt));
    }
  }

  DynamicBitset reachable;
  reachable.init(sum_v + 1);
  for (I32 weight : grouped) reachable.or_shift(weight);

  const I64 base_s = w0 - sum_v_i64;
  Set<I128> unique_vals;

  for (I32 i = 0; i <= sum_v; ++i) {
    if (!reachable.test(i)) continue;

    const I64 s_n = base_s + 2LL * i;
    const I128 num = as<I128>(x) * s_n * s_n
                   + as<I128>(x - y) * s_n
                   + as<I128>(y) * n;
    unique_vals.insert(num / 2);
  }

  I64 ans = 0;
  for (I128 value : unique_vals) {
    ans += mod_norm(value, MOD2);
    if (ans >= MOD2) ans -= MOD2;
  }

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
