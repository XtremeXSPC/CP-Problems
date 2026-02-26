#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_FAST_IO

#include "templates/Base.hpp"
#include "modules/data_structures/DynamicBitset.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

[[nodiscard]] auto has_symmetric_rl(const String& s, I32 l, I32 r) -> bool {
  while (l < r) {
    if (s[l] == 'R' && s[r] == 'L') return true;
    ++l;
    --r;
  }
  return false;
}

enum class CoreStatus : I32 { Impossible = 0, Unique = 1, CoreFound = 2 };

struct CoreInfo {
  CoreStatus status;
  I32 left;
  I32 right;
};

[[nodiscard]] auto find_core_interval(const String& s) -> CoreInfo {
  I32 l = 0;
  I32 r = as<I32>(s.size()) - 1;

  while (l < r) {
    const char cl = s[l];
    const char cr = s[r];

    if (cl == 'L' && cr == 'R') {
      ++l;
      --r;
      continue;
    }

    if (cl == 'R' && cr == 'L') {
      if (l + 1 >= r) return {CoreStatus::Unique, -1, -1};
      return {CoreStatus::CoreFound, l, r};
    }

    if (has_symmetric_rl(s, l + 1, r - 1)) {
      return {CoreStatus::Impossible, -1, -1};
    }
    return {CoreStatus::Unique, -1, -1};
  }

  return {CoreStatus::Unique, -1, -1};
}

[[nodiscard]] auto count_case_x_greater_than_y(const String& core) -> I64 {
  Vec<I32> pos_l;
  Vec<I32> pos_r_from_right;
  pos_l.reserve(core.size());
  pos_r_from_right.reserve(core.size());

  for (I32 i = 0; i < as<I32>(core.size()); ++i) {
    if (core[i] == 'L') pos_l.push_back(i);
  }
  for (I32 i = as<I32>(core.size()) - 1; i >= 0; --i) {
    if (core[i] == 'R') pos_r_from_right.push_back(i);
  }

  const I32 lim = std::min(as<I32>(pos_l.size()), as<I32>(pos_r_from_right.size()));
  I64 ways = 0;
  for (I32 i = 0; i < lim; ++i) {
    if (pos_l[i] < pos_r_from_right[i]) ++ways;
  }
  return ways;
}

[[nodiscard]] auto count_case_a_ge_b(const String& core, const bool allow_equal) -> I64 {
  const I32 n = as<I32>(core.size());

  Vec<I32> l_before(as<Size>(n), 0);
  Vec<I32> r_after(as<Size>(n), 0);
  {
    I32 acc = 0;
    for (I32 i = 0; i < n; ++i) {
      l_before[i] = acc;
      if (core[i] == 'L') ++acc;
    }
  }
  {
    I32 acc = 0;
    for (I32 i = n - 1; i >= 0; --i) {
      r_after[i] = acc;
      if (core[i] == 'R') ++acc;
    }
  }

  Vec<Vec<I32>> r_pos_by_l_count(as<Size>(n + 1));
  for (I32 i = 0; i < n; ++i) {
    if (core[i] == 'R') r_pos_by_l_count[l_before[i]].push_back(i);
  }

  Vec<Vec<I32>> queries_by_right(as<Size>(n));
  I64 ways = 0;

  const auto add_query = [&](const I32 left, const I32 right, auto& self) -> void {
    if (left > right + 1) return;
    if (left >= right) {
      ++ways;
      return;
    }
    self[right].push_back(left);
  };

  for (I32 y = n - 1; y >= 1; --y) {
    if (core[y] != 'L') continue;

    if (core[y - 1] == 'L') {
      if (!allow_equal) continue;
      const I32 c = r_after[y];
      if (!r_pos_by_l_count[c].empty()) {
        const I32 x = r_pos_by_l_count[c].back();
        add_query(x + 1, y - 1, queries_by_right);
      }
      continue;
    }

    I32 run_r = 0;
    while (y - 1 - run_r >= 0 && core[y - 1 - run_r] == 'R') ++run_r;

    const I32 base = r_after[y];
    const I32 upto = std::min<I32>(n, base + run_r);
    for (I32 c = base; c <= upto; ++c) {
      if (!allow_equal && c == base) continue;
      for (const I32 x : r_pos_by_l_count[c]) {
        const I32 trimmed_right = y - 1 - (c - base);
        add_query(x + 1, trimmed_right, queries_by_right);
      }
    }
  }

  DynamicBitset has_r(n);
  DynamicBitset bad_left(n);
  for (I32 right = 0; right < n; ++right) {
    bad_left.shift_right_one();

    if (core[right] == 'L') {
      bad_left.or_with(has_r);
    } else {
      has_r.set_bit(right);
    }

    for (const I32 left : queries_by_right[right]) {
      if (!bad_left.test_bit(left)) ++ways;
    }
  }

  return ways;
}

[[nodiscard]] auto count_core_ways(const String& core) -> I64 {
  I64 ways = 0;
  ways += count_case_x_greater_than_y(core);
  ways += count_case_a_ge_b(core, true);

  String mirrored = core;
  std::reverse(mirrored.begin(), mirrored.end());
  for (char& c : mirrored) c = (c == 'L' ? 'R' : 'L');
  ways += count_case_a_ge_b(mirrored, false);

  return ways % MOD2;
}

void solve() {
  INT(n);
  STR(s);

  const CoreInfo info = find_core_interval(s);
  if (info.status == CoreStatus::Impossible) {
    OUT(0);
    return;
  }
  if (info.status == CoreStatus::Unique) {
    OUT(1);
    return;
  }

  const String core = s.substr(as<Size>(info.left), as<Size>(info.right - info.left + 1));
  OUT(count_core_ways(core));
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
