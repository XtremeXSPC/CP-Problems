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
#include "modules/data_structures/SparseTable.hpp"

//===----------------------------------------------------------------------===//
/* Helpers */

struct MinPairOp {
  auto operator()(const Pair<I32, I32>& lhs, const Pair<I32, I32>& rhs) const
    -> Pair<I32, I32> {
    return lhs.first < rhs.first ? lhs : rhs;
  }
};

struct MaxPairOp {
  auto operator()(const Pair<I32, I32>& lhs, const Pair<I32, I32>& rhs) const
    -> Pair<I32, I32> {
    return lhs.first > rhs.first ? lhs : rhs;
  }
};

auto same_sign(const I32 a, const I32 b, const I32 x) -> bool {
  return (a > x) == (b > x);
}

void prepend_layer(VecI32& profile, const VecI32& layer, const I32 x) {
  VecI32 merged;
  merged.reserve(sz(layer) + sz(profile));

  for (const I32 value : layer) {
    merged.push_back(value);
  }
  for (const I32 value : profile) {
    if (!merged.empty() && same_sign(merged.back(), value, x)) {
      merged.back() = value;
    } else {
      merged.push_back(value);
    }
  }

  profile.swap(merged);
}

auto tail_profile(const VecI32& values) -> VecI32 {
  const I32 keep = std::min<I32>(4, sz(values));
  return VecI32(values.end() - keep, values.end());
}

auto build_left_profile(
  const I32 idx,
  const VecI32& p,
  const SparseTable<Pair<I32, I32>, MinPairOp>& st_min,
  const SparseTable<Pair<I32, I32>, MaxPairOp>& st_max
) -> VecI32 {
  const I32 x = p[idx];

  I32 l = 0;
  I32 r = idx - 1;
  VecI32 profile;

  while (l <= r && sz(profile) < 5) {
    const auto mn = st_min.query(l, r);
    const auto mx = st_max.query(l, r);

    if (mx.first < x) {
      prepend_layer(profile, VecI32{mn.first}, x);
      break;
    }
    if (mn.first > x) {
      prepend_layer(profile, VecI32{mx.first}, x);
      break;
    }

    if (mx.second > mn.second) {
      prepend_layer(profile, VecI32{mx.first, mn.first}, x);
    } else {
      prepend_layer(profile, VecI32{mn.first, mx.first}, x);
    }
    l = std::max(mx.second, mn.second) + 1;
  }

  return tail_profile(profile);
}

auto build_right_profile(
  const I32 idx,
  const VecI32& p,
  const SparseTable<Pair<I32, I32>, MinPairOp>& st_min,
  const SparseTable<Pair<I32, I32>, MaxPairOp>& st_max
) -> VecI32 {
  const I32 x = p[idx];

  I32 l = idx + 1;
  I32 r = as<I32>(sz(p)) - 1;
  VecI32 profile;

  while (l <= r && sz(profile) < 5) {
    const auto mn = st_min.query(l, r);
    const auto mx = st_max.query(l, r);

    if (mx.first < x) {
      prepend_layer(profile, VecI32{mn.first}, x);
      break;
    }
    if (mn.first > x) {
      prepend_layer(profile, VecI32{mx.first}, x);
      break;
    }

    if (mx.second < mn.second) {
      prepend_layer(profile, VecI32{mx.first, mn.first}, x);
    } else {
      prepend_layer(profile, VecI32{mn.first, mx.first}, x);
    }
    r = std::min(mx.second, mn.second) - 1;
  }

  return tail_profile(profile);
}

auto solve_profiles(const VecI32& left, const VecI32& right, const I32 x) -> I32 {
  Array<Array<I32, 5>, 5> memo;
  for (auto& row : memo) {
    row.fill(-1);
  }

  const auto dfs = [&](const auto& self, const I32 li, const I32 ri) -> I32 {
    I32& memo_cell = memo[li][ri];
    if (memo_cell != -1) {
      return memo_cell;
    }

    I32 best  = INF32;
    bool move = false;

    if (li < sz(left) && ri < sz(right) && same_sign(left[li], right[ri], x)) {
      move = true;
      if (std::abs(left[li] - x) < std::abs(right[ri] - x)) {
        best = std::min(best, self(self, li + 1, ri));
      } else {
        best = std::min(best, self(self, li, ri + 1));
      }
    }
    if (sz(left) - li >= 3) {
      move = true;
      best = std::min(best, self(self, li + 1, ri));
    }
    if (sz(right) - ri >= 3) {
      move = true;
      best = std::min(best, self(self, li, ri + 1));
    }

    if (!move) {
      const I32 remaining_left  = as<I32>(sz(left)) - li;
      const I32 remaining_right = as<I32>(sz(right)) - ri;
      best = 1 + remaining_left + remaining_right;
    }

    memo_cell = best;
    return best;
  };

  return dfs(dfs, 0, 0);
}

//===----------------------------------------------------------------------===//
/* Main Solver Function */

void solve() {
  INT(n);
  VecI32 p(n);
  IN(p);

  Vec<Pair<I32, I32>> min_nodes(n), max_nodes(n);
  FOR(i, n) {
    min_nodes[i] = {p[i], i};
    max_nodes[i] = {p[i], i};
  }

  const SparseTable<Pair<I32, I32>, MinPairOp> st_min(min_nodes);
  const SparseTable<Pair<I32, I32>, MaxPairOp> st_max(max_nodes);

  VecI32 ans(n);
  FOR(i, n) {
    const I32 idx      = as<I32>(i);
    const VecI32 left  = build_left_profile(idx, p, st_min, st_max);
    const VecI32 right = build_right_profile(idx, p, st_min, st_max);
    ans[i]             = solve_profiles(left, right, p[idx]);
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
