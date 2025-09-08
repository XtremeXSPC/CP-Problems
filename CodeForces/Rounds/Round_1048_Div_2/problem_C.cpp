//===----------------------------------------------------------------------===//
/**
 * @file: problem_C.cpp
 * @brief Codeforces Round 1048 (Div. 2) - Problem C
 * @author: Costantino Lombardi
 *
 * @status: PASSED
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

// Function to solve a single test case.
void solve() {
  ll k;
  ull target_chocola_count;
  cin >> k >> target_chocola_count;

  const ull balanced_state = 1ULL << k;
  
  if (target_chocola_count == balanced_state) {
    cout << 0 << "\n\n";
    return;
  }
  
  const ull total_cakes = 1ULL << (k + 1);
  
  // Use deque to insert operations at front, avoiding reversal.
  deque<int> op_sequence;

  ull current_chocola = target_chocola_count;

  // Work backwards from target to balanced state.
  while (current_chocola != balanced_state) {
    if (current_chocola > balanced_state) {
      current_chocola = 2 * current_chocola - total_cakes;
      op_sequence.push_front(2);
    } else {
      current_chocola *= 2;
      op_sequence.push_front(1);
    }
  }

  cout << op_sequence.size() << "\n";
  
  // Print space-separated sequence.
  if (!op_sequence.empty()) {
      for (size_t i = 0; i < op_sequence.size(); ++i) {
          if (i > 0) cout << " ";
          cout << op_sequence[i];
      }
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
