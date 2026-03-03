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
#include "templates/Math.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

constexpr I32 kMaxPairs = 300'000;
constexpr I32 kMaxFact  = 2 * kMaxPairs + 5;

VecI64 fact(kMaxFact, 1);
VecI64 inv_fact(kMaxFact, 1);
VecI64 inv_num(kMaxFact, 1);
bool precomputed = false;

void init_combinatorics() {
  if (precomputed) return;
  precomputed = true;

  for (I32 i = 2; i < kMaxFact; ++i) {
    inv_num[i] = MOD2 - (MOD2 / i) * inv_num[MOD2 % i] % MOD2;
  }

  for (I32 i = 1; i < kMaxFact; ++i) {
    fact[i] = fact[i - 1] * i % MOD2;
  }

  inv_fact.back() = ::mod_pow(fact.back(), MOD2 - 2, MOD2);
  for (I32 i = kMaxFact - 1; i >= 1; --i) {
    inv_fact[i - 1] = inv_fact[i] * i % MOD2;
  }
}

auto catalan(I32 x) -> I64 {
  const I64 choose = fact[2 * x] * inv_fact[x] % MOD2 * inv_fact[x] % MOD2;
  return choose * inv_num[x + 1] % MOD2;
}

auto inv_catalan(I32 x) -> I64 {
  return (x + 1LL) % MOD2 * fact[x] % MOD2 * fact[x] % MOD2 * inv_fact[2 * x] % MOD2;
}

void solve() {
  INT(n);

  struct Interval {
    I32 l;
    I32 r;
    I32 id;
  };

  VecI32 left(n + 1);
  VecI32 right(n + 1);
  left[0] = 1;
  right[0] = 2 * n;

  Vec<Interval> nodes;
  nodes.reserve(n + 1);
  nodes.push_back({left[0], right[0], 0});

  FOR(i, 1, n + 1) {
    INT(l, r);
    left[i] = l;
    right[i] = r;
    nodes.push_back({l, r, as<I32>(i)});
  }

  ranges::sort(nodes, [](const Interval& a, const Interval& b) {
    if (a.l != b.l) return a.l < b.l;
    if (a.r != b.r) return a.r > b.r;
    return a.id < b.id;
  });

  VecI32 parent(n + 1, -1);
  Vec<Interval> stack;
  stack.reserve(n + 1);

  for (const auto& node : nodes) {
    while (!stack.empty() && stack.back().r < node.r) {
      stack.pop_back();
    }
    if (!stack.empty()) parent[node.id] = stack.back().id;
    stack.push_back(node);
  }

  VecI32 free_pairs(n + 1, 0);
  VecI64 child_len_sum(n + 1, 0);

  FOR(i, 1, n + 1) {
    child_len_sum[parent[i]] += right[i] - left[i] + 1LL;
  }

  FOR(i, n + 1) {
    const I64 total_len = (i == 0 ? 2LL * n : right[i] - left[i] + 1LL);
    const I64 delta = (i == 0 ? 0 : 2);
    free_pairs[i] = as<I32>((total_len - delta - child_len_sum[i]) / 2);
  }

  VecI64 cat(n + 1, 1);
  VecI64 inv_cat(n + 1, 1);
  FOR(i, n + 1) {
    cat[i] = catalan(i);
    inv_cat[i] = inv_catalan(i);
  }

  I64 current_answer = 1;
  FOR(i, n + 1) {
    current_answer = current_answer * cat[free_pairs[i]] % MOD2;
  }

  VecI32 dsu(n + 1);
  iota(dsu.begin(), dsu.end(), 0);

  auto find_root = [&](auto& self, I32 x) -> I32 {
    if (dsu[x] == x) return x;
    dsu[x] = self(self, dsu[x]);
    return dsu[x];
  };

  VecI64 answer(n + 1, 0);
  answer[n] = current_answer;

  for (I32 i = n; i >= 1; --i) {
    const I32 u = i;
    const I32 p = find_root(find_root, parent[u]);

    current_answer = current_answer * inv_cat[free_pairs[u]] % MOD2;
    current_answer = current_answer * inv_cat[free_pairs[p]] % MOD2;

    free_pairs[p] += free_pairs[u] + 1;
    current_answer = current_answer * cat[free_pairs[p]] % MOD2;

    dsu[u] = p;
    answer[i - 1] = current_answer;
  }

  OUT(answer);
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
  init_combinatorics();

#ifdef LOCAL
  Timer timer;
  // init_debug_log();
#endif

  INT(T);
  FOR(T) solve();

  return 0;
}

//===----------------------------------------------------------------------===//
