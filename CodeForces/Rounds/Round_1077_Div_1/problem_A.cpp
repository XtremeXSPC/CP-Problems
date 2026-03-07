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

    VecI32 left_close(distinct_count, 0);
    VecI32 right_close(distinct_count, distinct_count); // exclusive

    I32 left_ptr = 0;
    FOR(i, distinct_count) {
      while (left_ptr < i && values[i] - values[left_ptr] >= k) ++left_ptr;
      left_close[i] = left_ptr;
    }

    I32 right_ptr = 0;
    FOR(i, distinct_count) {
      if (right_ptr < i) right_ptr = i;
      while (right_ptr < distinct_count && values[right_ptr] - values[i] < k) {
        ++right_ptr;
      }
      right_close[i] = right_ptr;
    }

    VecI32 next_unvisited(distinct_count + 1);
    FOR(i, distinct_count + 1) next_unvisited[i] = i;

    auto find_next = [&](auto& self, I32 x) -> I32 {
      if (next_unvisited[x] == x) return x;
      next_unvisited[x] = self(self, next_unvisited[x]);
      return next_unvisited[x];
    };

    auto erase_idx = [&](I32 idx) {
      next_unvisited[idx] = find_next(find_next, idx + 1);
    };

    VecI32 component(distinct_count, -1);
    I32 component_id = 0;

    for (I32 seed = find_next(find_next, 0); seed < distinct_count; seed = find_next(find_next, 0)) {
      I32 inter_left  = left_close[seed];
      I32 inter_right = right_close[seed] - 1;

      component[seed] = component_id;
      erase_idx(seed);

      while (true) {
        bool added = false;

        for (I32 x = find_next(find_next, 0); x < inter_left; x = find_next(find_next, x)) {
          component[x] = component_id;
          inter_left  = max(inter_left, left_close[x]);
          inter_right = min(inter_right, right_close[x] - 1);
          erase_idx(x);
          added = true;
        }

        for (I32 x = find_next(find_next, inter_right + 1); x < distinct_count; x = find_next(find_next, x)) {
          component[x] = component_id;
          inter_left  = max(inter_left, left_close[x]);
          inter_right = min(inter_right, right_close[x] - 1);
          erase_idx(x);
          added = true;
        }

        if (!added) break;
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
