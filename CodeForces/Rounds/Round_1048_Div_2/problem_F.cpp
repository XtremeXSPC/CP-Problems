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

// Fast exponentiation modulo MOD.
ll power(ll base, ll exp) {
    ll res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

// Modular inverse using Fermat's Little Theorem.
ll modInverse(ll n) {
    return power(n, MOD - 2);
}

// Event structure for sorting initial values and operations.
struct Event {
  ll val;
  int type; // 0: initial, 1: operation.
  int index;

  bool operator<(const Event& other) const {
    if (val != other.val) return val < other.val;
    return type < other.type;
  }
};

// Function to solve a single test case.
void solve() {
    int n;
    ll m;
    int q;
    cin >> n >> m >> q;

    V_ll initial_d(n);
    V_ll W;
    for (int i = 0; i < n; ++i) {
        ll val;
        cin >> val;
        initial_d[i] = val - (i + 1);
        W.push_back(initial_d[i]);
    }

    vector<pair<int, ll>> ops(q);
    for (int i = 0; i < q; ++i) {
        cin >> ops[i].first >> ops[i].second;
        ops[i].second -= ops[i].first;
        W.push_back(ops[i].second);
    }

    sort(W.begin(), W.end());
    W.erase(unique(W.begin(), W.end()), W.end());

    V_ll fact(q + 1);
    fact[0] = 1;
    for (int i = 1; i <= q; ++i) {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
    ll fact_q = fact[q];

    V_ll total_d_sum(n + 1, 0);
    ll last_v = 0;

    // Process each unique value in W.
    for (ll V : W) {
        ll diff = V - last_v;
        // Avoid processing same value twice, allow first value.
        if (diff == 0 && V != W[0]) continue;

        int j0 = n + 1;
        for (int i = 0; i < n; ++i) {
            if (initial_d[i] >= V) {
                j0 = i + 1;
                break;
            }
        }

        V_ll up_counts(n + 1, 0);
        V_ll down_counts(n + 1, 0);
        for (const auto& op : ops) {
            if (op.second >= V) up_counts[op.first]++;
            else down_counts[op.first]++;
        }

        V_ll up_prefix(n + 1, 0);
        V_ll down_suffix(n + 2, 0);
        for (int k = 1; k <= n; ++k) up_prefix[k] = up_prefix[k - 1] + up_counts[k];
        for (int k = n; k >= 1; --k) down_suffix[k] = down_suffix[k + 1] + down_counts[k];

        ll diff_mod = (diff % MOD + MOD) % MOD;

        // Calculate contributions for each k.
        for (int k = 1; k <= n; ++k) {
            ll rel_u = up_prefix[k];
            ll rel_d = down_suffix[k]; // Relevant down-ops are at indices >= k.
            ll M = rel_u + rel_d;

            ll num_perms = 0;
            if (M == 0) {
                if (j0 <= k) num_perms = fact_q;
            } else {
                num_perms = (((fact_q * rel_u) % MOD) * modInverse(M)) % MOD;
            }

            ll term = (diff_mod * num_perms) % MOD;
            total_d_sum[k] = (total_d_sum[k] + term) % MOD;
        }
        last_v = V;
    }

    // Output final results.
    for (int k = 1; k <= n; ++k) {
        ll fact_q_k = (fact_q * k) % MOD;
        ll final_pos_sum = (total_d_sum[k] + fact_q_k) % MOD;
        cout << (final_pos_sum + MOD) % MOD << (k == n ? "" : " ");
    }
    cout << "\n";
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
