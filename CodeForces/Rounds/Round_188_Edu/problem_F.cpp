#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_MATH
#define NEED_TIMER
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

struct Entry {
  I32 value = 0;
  I64 min_weight  = 0;
  I64 base_weight = 0;
};

auto norm(I64 value) -> I64 {
  value %= MOD2;
  if (value < 0) {
    value += MOD2;
  }
  return value;
}

void solve() {
  INT(n, m);
  VecI32 a(n), ks(m);
  IN(a);
  IN(ks);

  VecI32 left(n), right(n), st;
  st.reserve(n);

  for (I32 i = 0; i < n; ++i) {
    while (!st.empty() && a[st.back()] >= a[i]) {
      st.pop_back();
    }
    left[i] = st.empty() ? -1 : st.back();
    st.eb(i);
  }

  st.clear();
  for (I32 i = n - 1; i >= 0; --i) {
    while (!st.empty() && a[st.back()] > a[i]) {
      st.pop_back();
    }
    right[i] = st.empty() ? n : st.back();
    st.eb(i);
  }

  Vec<Entry> entries;
  entries.reserve(n);
  for (I32 i = 0; i < n; ++i) {
    const I64 min_weight  = as<I64>(i - left[i]) * (right[i] - i) % MOD2;
    const I64 base_weight = as<I64>(i + 1) * (n - i) % MOD2;
    entries.eb(Entry{a[i], min_weight, base_weight});
  }

  std::sort(all(entries), [](const Entry& lhs, const Entry& rhs) { return lhs.value < rhs.value; });

  VecI32 values;
  values.reserve(n);
  VecI64 pref_inv(1, 0), pref_weight(1, 0), pref_const(1, 0);

  I64 baseline = 0;
  for (I32 i = 0; i < n;) {
    const I32 value = entries[i].value;
    I64 min_weight  = 0;
    I64 base_weight = 0;

    while (i < n && entries[i].value == value) {
      min_weight  = (min_weight  + entries[i].min_weight) % MOD2;
      base_weight = (base_weight + entries[i].base_weight) % MOD2;
      ++i;
    }

    min_weight %= MOD2;
    const I64 inv  = mod_pow(as<I64>(value), MOD2 - 2, MOD2);
    const I64 term = norm(2 - value - inv);

    values.eb(value);
    baseline = (baseline + base_weight * inv) % MOD2;
    pref_inv.eb((pref_inv.back() + min_weight * inv) % MOD2);
    pref_weight.eb((pref_weight.back() + min_weight) % MOD2);
    pref_const.eb((pref_const.back() + min_weight * term) % MOD2);
  }

  const I64 total_inv = pref_inv.back();
  I32 pos = 0;
  for (I32 k : ks) {
    while (pos < as<I32>(values.size()) && values[pos] <= k + 1) {
      ++pos;
    }

    const I64 gain_full    = (as<I64>(k) % MOD2) * pref_weight[pos] % MOD2 + pref_const[pos];
    const I64 gain_partial = (as<I64>(k) % MOD2) * norm(total_inv - pref_inv[pos]) % MOD2;

    OUT(norm(baseline + gain_full + gain_partial));
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
