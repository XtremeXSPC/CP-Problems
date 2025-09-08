//===----------------------------------------------------------------------===//
/**
 * @file: problem_F.cpp
 * @brief Codeforces Round 1048 (Div. 2) - Problem F
 * @author: Costantino Lombardi
 *
 * @status: In Progress
 */
//===----------------------------------------------------------------------===//
/* Included library */

// clang-format off
// Compiler optimizations:
#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC optimize("Ofast,unroll-loops,fast-math,O3")
  // Apple Silicon optimizations:
  #ifdef __aarch64__
    #pragma GCC target("+simd")
  #endif
#endif

#ifdef __clang__
  #pragma clang optimize on
#endif

// Sanitaze macro:
#ifdef USE_CLANG_SANITIZE
  #include "PCH.h"
#else
  #include <bits/stdc++.h>
#endif

// Debug macro:
#ifdef LOCAL
  #include "debug.h"
#else
  #define debug(...) 42
#endif
// clang-format on

//===----------------------------------------------------------------------===//
/* Type Aliases and Constants */

// Type aliases:
using ll       = long long;
using ull      = unsigned long long;
using ld       = long double;
using P_ii     = std::pair<int, int>;
using P_ll     = std::pair<long long, long long>;
using V_b      = std::vector<bool>;
using V_i      = std::vector<int>;
using V_ui     = std::vector<unsigned int>;
using V_ll     = std::vector<long long>;
using VV_i     = std::vector<std::vector<int>>;
using VV_ll    = std::vector<std::vector<long long>>;
using V_s      = std::vector<std::string>;
using VV_s     = std::vector<std::vector<std::string>>;
using VP_ii    = std::vector<std::pair<int, int>>;
using VVP_ii   = std::vector<std::vector<std::pair<int, int>>>;
using VP_ll    = std::vector<std::pair<long long, long long>>;
using VT_iii   = std::vector<std::tuple<int, int, int>>;
using Map_ll   = std::map<long long, long long>;
using VUMap_il = std::vector<std::unordered_map<int, ll>>;

// Mathematical constants:
constexpr long double PI   = 3.141592653589793238462643383279502884L;
constexpr long double E    = 2.718281828459045235360287471352662498L;
constexpr long double EPS  = 1e-9L;
constexpr int         INF  = 0x3f3f3f3f;
constexpr long long   LINF = 0x3f3f3f3f3f3f3f3fLL;
constexpr int         LIM  = 1000000 + 5;
constexpr int         MOD  = 1000000007;
constexpr int         MOD2 = 998244353;

using namespace std;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

// Reads input and prepares data structures.
void read_and_prepare_data(int n, int q, V_ll& init_d, 
               vector<pair<int, ll>>& ops, 
               map<ll, V_i>& ops_by_d, 
               V_ll& unique_d) {
  init_d.resize(n);
  set<ll> unique_set;

  for (int i = 0; i < n; ++i) {
    ll pos;
    cin >> pos;
    init_d[i] = pos - (i + 1);
    unique_set.insert(init_d[i]);
  }

  ops.resize(q);
  for (int i = 0; i < q; ++i) {
    cin >> ops[i].first >> ops[i].second;
    ll d_val = ops[i].second - ops[i].first;
    ops[i].second = d_val;
    unique_set.insert(d_val);
    ops_by_d[d_val].push_back(ops[i].first);
  }

  unique_d.assign(unique_set.begin(), unique_set.end());
}

// Precomputes modular inverses.
V_ll precompute_mod_inverses(int max_val) {
  V_ll inv(max_val + 1);
  if (max_val > 0) inv[1] = 1;
  for (int i = 2; i <= max_val; i++) {
    inv[i] = MOD - (MOD / i) * inv[MOD % i] % MOD;
  }
  return inv;
}

// Sweep-line algorithm to find expected d-values.
V_ll calculate_expected_d_values(int n, int q, const V_ll& init_d,
                 const vector<pair<int, ll>>& ops,
                 const map<ll, V_i>& ops_by_d,
                 const V_ll& unique_d,
                 const V_ll& inv) {
  V_ll expected(n + 1, 0);

  // Incremental state for sweep-line.
  V_i ops_ge(n + 1, 0);
  V_i ops_lt(n + 1, 0);
  int total_lt = 0;

  // Initialize state for first d-value.
  if (!unique_d.empty()) {
    ll v0 = unique_d[0];
    for (const auto& op : ops) {
      if (op.second >= v0) ops_ge[op.first]++;
      else {
        ops_lt[op.first]++;
        total_lt++;
      }
    }
  }

  // Sweep through unique d-values.
  for (size_t l = 0; l < unique_d.size(); ++l) {
    ll v = unique_d[l];

    if (l > 0) {
      ll v_prev = unique_d[l - 1];
      if (auto it = ops_by_d.find(v_prev); it != ops_by_d.end()) {
        for (int idx : it->second) {
          ops_ge[idx]--;
          ops_lt[idx]++;
          total_lt++;
        }
      }
    }

    int u_sum = 0, d_sum_lt = 0;
    for (int j = 1; j <= n; ++j) {
      u_sum += ops_ge[j];
      int u = u_sum; // Up-ops with index <= j.
      int d = total_lt - d_sum_lt; // Down-ops with index >= j.

      ll prob_ge = 0;
      if (u + d == 0) {
        if (init_d[j - 1] >= v) prob_ge = 1;
      } else {
        prob_ge = (static_cast<ll>(u) * inv[u + d]) % MOD;
      }

      ll delta = (l == 0) ? v : v - unique_d[l - 1];
      ll term = ((delta % MOD + MOD) % MOD * prob_ge) % MOD;
      expected[j] = (expected[j] + term) % MOD;

      d_sum_lt += ops_lt[j];
    }
  }
  return expected;
}

// Calculates final sums and prints result.
void calculate_and_print_final_sums(int n, int q, const V_ll& expected) {
  ll q_fact = 1;
  for (int i = 1; i <= q; ++i) {
    q_fact = (q_fact * i) % MOD;
  }

  for (int j = 1; j <= n; ++j) {
    ll total_d = (q_fact * expected[j]) % MOD;
    ll pos_offset = (q_fact * j) % MOD;
    ll final_ans = (total_d + pos_offset) % MOD;
    cout << (final_ans + MOD) % MOD << (j == n ? "" : " ");
  }
  cout << "\n";
}

// Function to solve a single test case.
void solve() {
  int n;
  ll m;
  int q;
  cin >> n >> m >> q;

  V_ll init_d, unique_d;
  vector<pair<int, ll>> ops;
  map<ll, V_i> ops_by_d;

  read_and_prepare_data(n, q, init_d, ops, ops_by_d, unique_d);

  if (q == 0) {
    for (int i = 0; i < n; ++i) {
      ll final_pos = (init_d[i] + (i + 1));
      cout << (final_pos % MOD + MOD) % MOD << (i == n - 1 ? "" : " ");
    }
    cout << "\n";
    return;
  }

  V_ll inv = precompute_mod_inverses(q);
  V_ll expected = calculate_expected_d_values(n, q, init_d, ops, ops_by_d, unique_d, inv);
  calculate_and_print_final_sums(n, q, expected);
}

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int T = 1;
  cin >> T;
  for ([[maybe_unused]] auto _ : views::iota(0, T)) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
