//===----------------------------------------------------------------------===//
/**
 * @file: problem_F.cpp
 * @brief Codeforces Round 1049 (Div. 2) - Problem F
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

namespace Math {

  // Type aliases for clarity and consistency.
  using I64   = int64_t;
  using usize = size_t;

  template <integral T>
  using Vector = vector<T>;

  // Concepts for template constraints.
  template <typename T>
  concept IntegralType = integral<T> && !same_as<T, bool>;

  // Constants.
  inline constexpr usize MAX_CHECK_LIMIT    = 100;
  inline constexpr I64   INFINITE_REDUCTION = -1;

  // Result type using expected for modern error handling.
  enum class ReductionResult : int8_t { FiniteReduction, InfiniteReduction };

  template <IntegralType T>
  using SolutionResult = expected<T, ReductionResult>;

  // Core algorithm implementation
  template <IntegralType ValueType = I64>
  class ArrayMinimizer {
   private:
    Vector<ValueType> elements;
    ValueType         total_sum{0};
    usize             n{0};

    // Calculate sum using ranges.
    [[nodiscard]] auto compute_sum() const noexcept -> ValueType {
      return ranges::fold_left(elements, ValueType{0}, plus<>{});
    }

   public:
    // Constructor accepting any range of integral values.
    template <ranges::input_range Range>
      requires convertible_to<ranges::range_value_t<Range>, ValueType>
    explicit ArrayMinimizer(Range&& input) :
        elements(ranges::begin(input), ranges::end(input)), n(elements.size()) {
      total_sum = compute_sum();
    }

    // Solve the minimization problem.
    [[nodiscard]] auto solve() -> SolutionResult<ValueType> {
      // Special case: single element array.
      if (n == 1) {
        return total_sum;
      }

      // Sort the array for modular analysis.
      ranges::sort(elements);

      // Phase 1: Check modular consistency.
      const usize phase1_limit = min(n - 1, MAX_CHECK_LIMIT);

      for (usize k = 2; k < n && k <= MAX_CHECK_LIMIT; ++k) {
        // Use second smallest element as reference (index 1).
        const ValueType reference_mod = elements[1] % k;

        // Check all elements from second onwards.
        for (usize j = 1; j < n; ++j) {
          if (static_cast<ValueType>(elements[j] % k) != reference_mod) {
            // Different remainders indicate infinite reduction possible.
            return unexpected(ReductionResult::InfiniteReduction);
          }
        }
      }

      // Phase 2: Calculate and verify the final residue.
      const ValueType base_residue = (elements[0] + elements[1]) % 2;

      // Verify consistency across different k values (now including k=n).
      const usize phase2_limit = min(n, MAX_CHECK_LIMIT);

      for (usize k = 2; k <= phase2_limit; ++k) {
        ValueType current_residue;

        if (k == n) {
          // Special case: when k equals array size.
          current_residue = total_sum % k;
        } else {
          // General formula using two smallest elements.
          current_residue = (elements[0] + (k - 1) * elements[1]) % k;
        }

        if (current_residue != base_residue) {
          // Inconsistent residue indicates infinite reduction.
          return unexpected(ReductionResult::InfiniteReduction);
        }
      }

      // Minimum achievable sum is total minus the base residue.
      return total_sum - base_residue;
    }
  };

  // Solution orchestrator class.
  class Solution {
   private:
    // Process a single test case.
    void solve_test_case() {
      usize n;
      cin >> n;

      // Read array elements.
      Vector<I64> values(n);
      for (auto& val : values) {
        cin >> val;
      }

      // Create minimizer and compute result.
      ArrayMinimizer<I64> minimizer(values);
      auto                result = minimizer.solve();

      // Output result using monadic operations.
      if (result.has_value()) {
        cout << result.value() << '\n';
      } else {
        cout << INFINITE_REDUCTION << '\n';
      }
    }

   public:
    void run() {
      // Fast I/O setup.
      ios_base::sync_with_stdio(false);
      cin.tie(nullptr);

      usize test_count;
      cin >> test_count;

      // Process each test case.
      while (test_count--) {
        solve_test_case();
      }
    }
  };
}  // namespace Math

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  Math::Solution solution;
  solution.run();
  return 0;
}

//===----------------------------------------------------------------------===//
