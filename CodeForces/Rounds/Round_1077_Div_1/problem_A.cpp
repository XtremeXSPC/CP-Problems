#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

void solve() {
  INT(n);
  VecI32 a(n);
  IN(a);

  if (ranges::is_sorted(a)) {
    OUT(-1);
    return;
  }

  VecI32 values = a;
  ranges::sort(values);
  values.erase(unique(values.begin(), values.end()), values.end());

  const I32 distinct_count = as<I32>(values.size());

  VecI32 rank_a(n);
  FOR(i, n) {
    rank_a[i] = as<I32>(lower_bound(values.begin(), values.end(), a[i]) - values.begin());
  }

  VecI32 sorted_a = a;
  ranges::sort(sorted_a);
  VecI32 rank_sorted(n);
  FOR(i, n) {
    rank_sorted[i] = as<I32>(lower_bound(values.begin(), values.end(), sorted_a[i]) - values.begin());
  }

  auto can_sort_with_k = [&](const I32 k) -> bool {
    if (k <= 0) return true;

    Set<I32> unvisited;
    FOR(i, distinct_count) unvisited.insert(i);

    VecI32 component(distinct_count, -1);
    I32 component_id = 0;
    VecI32 stack;

    while (!unvisited.empty()) {
      const I32 seed = *unvisited.begin();
      unvisited.erase(unvisited.begin());
      component[seed] = component_id;
      stack.push_back(seed);

      while (!stack.empty()) {
        const I32 u = stack.back();
        stack.pop_back();

        const I64 x = values[u];
        const I64 low_limit_value  = x - k + 1LL;
        const I64 high_limit_value = x + k;

        const I32 low_idx = as<I32>(lower_bound(values.begin(), values.end(), low_limit_value) - values.begin());
        auto it = unvisited.begin();
        while (it != unvisited.end() && *it < low_idx) {
          const I32 v = *it;
          it = unvisited.erase(it);
          component[v] = component_id;
          stack.push_back(v);
        }

        const I32 high_idx = as<I32>(lower_bound(values.begin(), values.end(), high_limit_value) - values.begin());
        it = unvisited.lower_bound(high_idx);
        while (it != unvisited.end()) {
          const I32 v = *it;
          it = unvisited.erase(it);
          component[v] = component_id;
          stack.push_back(v);
        }
      }

      ++component_id;
    }

    FOR(i, n) {
      if (component[rank_a[i]] != component[rank_sorted[i]]) return false;
    }
    return true;
  };

  const I32 max_diff = *ranges::max_element(a) - *ranges::min_element(a);

  I32 low = 0;
  I32 high = max_diff + 1;
  while (low + 1 < high) {
    const I32 mid = low + (high - low) / 2;
    if (can_sort_with_k(mid)) {
      low = mid;
    } else {
      high = mid;
    }
  }

  OUT(low);
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
