#if !defined(CP_TEMPLATE_PROFILE_RELAXED)
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_CORE
#define NEED_IO

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;
constexpr I32 CAP = 2000;

struct ActiveMap {
  Vec<Pair<U8, U8>> edges;

  auto get(U8 key) const -> U8 {
    const auto it = lower_bound(all(edges), Pair<U8, U8>{key, 0},
        [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; });
    if (it != edges.end() && it->first == key) return it->second;
    return key;
  }

  void erase_key(U8 key) {
    const auto it = lower_bound(all(edges), Pair<U8, U8>{key, 0},
        [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; });
    if (it != edges.end() && it->first == key) edges.erase(it);
  }

  auto find_key_by_value(U8 value) const -> U8 {
    for (const auto& [key, mapped] : edges) {
      if (mapped == value) return key;
    }
    return value;
  }

  void set_map(U8 key, U8 value) {
    erase_key(key);
    if (key == value) return;
    const auto it = lower_bound(all(edges), Pair<U8, U8>{key, 0},
        [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; });
    edges.insert(it, {key, value});
  }

  auto pack() const -> String {
    String out;
    out.resize(1 + 2 * edges.size());
    out[0] = as<char>(edges.size());
    FOR(i, sz(edges)) {
      out[1 + 2 * i] = as<char>(edges[i].first);
      out[1 + 2 * i + 1] = as<char>(edges[i].second);
    }
    return out;
  }
};

struct Solver {
  I32 n = 0, m = 0;
  UnorderedMap<String, I32> memo;
  I32 printed = 0;
  I32 limit = 0;

  static auto make_key(U8 t, U8 left, U8 cycles, const String& packed) -> String {
    String key;
    key.reserve(3 + packed.size());
    key.push_back(as<char>(t));
    key.push_back(as<char>(left));
    key.push_back(as<char>(cycles));
    key += packed;
    return key;
  }

  auto transition(U8 t, U8 left, U8 cycles, const ActiveMap& state, bool take_left,
                  U8& next_left, U8& next_cycles, ActiveMap& next_state) const -> bool {
    const U8 value = as<U8>(t + 1);
    const U8 right = as<U8>(t - left);
    const U8 idx = take_left ? as<U8>(left + 1) : as<U8>(n - right);

    const U8 endpoint = state.get(value);
    next_state = state;
    next_state.erase_key(value);

    const bool closes_cycle = (idx == endpoint);
    next_cycles = as<U8>(cycles + (closes_cycle ? 1 : 0));
    if (next_cycles > as<U8>(m)) return false;

    if (!closes_cycle) {
      const U8 key = next_state.find_key_by_value(idx);
      next_state.set_map(key, endpoint);
    }

    next_left = take_left ? as<U8>(left + 1) : left;
    return true;
  }

  auto count_from(U8 t, U8 left, U8 cycles, const ActiveMap& state) -> I32 {
    if (as<I32>(cycles) + 1 > m) return 0;
    if (as<I32>(cycles) + (n - as<I32>(t)) < m) return 0;

    if (t == as<U8>(n - 1)) {
      return (as<I32>(cycles) + 1 == m) ? 1 : 0;
    }

    const String packed = state.pack();
    const String key = make_key(t, left, cycles, packed);
    if (const auto it = memo.find(key); it != memo.end()) return it->second;

    I32 ways = 0;
    FOR(branch, 2) {
      if (ways == CAP) break;
      const bool take_left = (branch == 0);
      U8 next_left = 0, next_cycles = 0;
      ActiveMap next_state;
      if (!transition(t, left, cycles, state, take_left, next_left, next_cycles, next_state)) {
        continue;
      }
      ways += count_from(as<U8>(t + 1), next_left, next_cycles, next_state);
      if (ways > CAP) ways = CAP;
    }

    memo.emplace(key, ways);
    return ways;
  }

  void emit(U8 t, U8 left, U8 cycles, const ActiveMap& state, Vec<I32>& p) {
    if (printed >= limit) return;
    if (t == as<U8>(n - 1)) {
      p[left + 1] = n;
      FOR(i, 1, n + 1) {
        if (i > 1) cout << ' ';
        cout << p[i];
      }
      cout << '\n';
      ++printed;
      p[left + 1] = 0;
      return;
    }

    const U8 value = as<U8>(t + 1);
    const U8 right = as<U8>(t - left);
    const U8 idx_left = as<U8>(left + 1);
    const U8 idx_right = as<U8>(n - right);

    FOR(branch, 2) {
      const bool take_left = (branch == 0);
      U8 next_left = 0, next_cycles = 0;
      ActiveMap next_state;
      if (!transition(t, left, cycles, state, take_left, next_left, next_cycles, next_state)) {
        continue;
      }

      if (count_from(as<U8>(t + 1), next_left, next_cycles, next_state) == 0) {
        continue;
      }

      p[take_left ? idx_left : idx_right] = value;
      emit(as<U8>(t + 1), next_left, next_cycles, next_state, p);
      p[take_left ? idx_left : idx_right] = 0;
      if (printed >= limit) return;
    }
  }

  void run() {
    if (m < 1 || m > n) {
      OUT(0);
      return;
    }

    const ActiveMap empty;
    const I32 total = count_from(0, 0, 0, empty);
    limit = min(total, CAP);
    OUT(limit);
    if (limit == 0) return;

    Vec<I32> permutation(n + 1, 0);
    emit(0, 0, 0, empty, permutation);
  }
};

void solve() {
  INT(n, m);
  Solver solver;
  solver.n = n;
  solver.m = m;
  solver.memo.reserve(1 << 20);
  solver.run();
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
