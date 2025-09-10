//===----------------------------------------------------------------------===//
/**
 * @file: problem_F.cpp
 * @brief Codeforces Round 1049 (Div. 2) - Problem F
 * @author: Costantino Lombardi
 *
 * @status: Compilation Error.
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
  #include <execution>
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

  // Type aliases using concepts.
  template <integral T>
  using Vector = vector<T>;

  // Concepts for template constraints.
  template <typename T>
  concept Arithmetic = is_arithmetic_v<T>;

  template <typename T>
  concept IntegralType = integral<T> && !same_as<T, bool>;

  // Constants.
  inline constexpr size_t MAX_CHECK_LIMIT    = 100;
  inline constexpr auto   INFINITE_REDUCTION = -1LL;

  // Result type using expected for error handling.
  enum class ReductionResult : int8_t { FiniteReduction, InfiniteReduction };

  template <IntegralType T>
  using SolutionResult = expected<T, ReductionResult>;

  // Core algorithm implementation.
  template <IntegralType ValueType = int64_t>
  class ArrayMinimizer {
   private:
    Vector<ValueType> elements;
    ValueType         sum{0};
    size_t            size{0};

    // Helper function to calculate the sum.
    [[nodiscard]] constexpr auto calculate_sum() const noexcept -> ValueType {
      return ranges::fold_left(elements, ValueType{0}, plus<>{});
    }

    // Check modular consistency using ranges algorithms.
    [[nodiscard]] constexpr auto check_modular_consistency(
        span<const ValueType> sorted_elements, size_t k) const noexcept -> bool {
      if (sorted_elements.size() < 2) return true;

      const auto reference_remainder = sorted_elements[1] % k;

      // Using ranges::all_of with projection.
      return ranges::all_of(sorted_elements.subspan(1), [reference_remainder, k](const auto& elem) {
        return elem % k == reference_remainder;
      });
    }

    // Calculate residue for given k.
    [[nodiscard]] constexpr auto calculate_residue(
        span<const ValueType> sorted_elements, size_t k) const noexcept -> ValueType {
      if (k == size) {
        return sum % k;
      }
      return (sorted_elements[0] + (k - 1) * sorted_elements[1]) % k;
    }

   public:
    // Constructor.
    template <ranges::input_range Range>
      requires convertible_to<ranges::range_value_t<Range>, ValueType>
    explicit ArrayMinimizer(Range&& input_range) :
        elements(ranges::begin(input_range), ranges::end(input_range)), size(elements.size()) {
      sum = calculate_sum();
    }

    // Deleted copy, enabled move.
    ArrayMinimizer(const ArrayMinimizer&)            = delete;
    ArrayMinimizer& operator=(const ArrayMinimizer&) = delete;
    ArrayMinimizer(ArrayMinimizer&&)                 = default;
    ArrayMinimizer& operator=(ArrayMinimizer&&)      = default;

    // Main solving function using modern return types.
    [[nodiscard]] auto solve() -> SolutionResult<ValueType> {
      // Edge case: single element.
      if (size == 1) {
        return sum;
      }

      // Sort using parallel execution policy for large arrays.
      if (size > 10000) {
        sort(std::execution::par_unseq, elements.begin(), elements.end());
      } else {
        ranges::sort(elements);
      }

      // Create span for efficient access.
      span<const ValueType> sorted_span{elements};

      // Phase 1: Check modular patterns using ranges.
      const auto check_limit = min(size - 1, MAX_CHECK_LIMIT);

      for (const auto k : views::iota(2UZ, check_limit + 1)) {
        if (!check_modular_consistency(sorted_span, k)) {
          return unexpected(ReductionResult::InfiniteReduction);
        }
      }

      // Phase 2: Calculate and verify final residue.
      const ValueType base_residue = (sorted_span[0] + sorted_span[1]) % 2;

      // Using ranges::for_each with early termination.
      optional<ReductionResult> error;

      ranges::for_each(views::iota(2UZ, min(size, MAX_CHECK_LIMIT) + 1), [&](size_t k) {
        if (error.has_value()) return;

        const auto current_residue = calculate_residue(sorted_span, k);
        if (current_residue != base_residue) {
          error = ReductionResult::InfiniteReduction;
        }
      });

      if (error.has_value()) {
        return unexpected(*error);
      }

      return sum - base_residue;
    }
  };

  // Factory function using deduction guides.
  template <ranges::input_range Range>
  auto make_minimizer(Range&& range) {
    using ValueType = ranges::range_value_t<Range>;
    return ArrayMinimizer<ValueType>(std::forward<Range>(range));
  }

  // Solution wrapper using structured bindings and monadic operations.
  class Solution {
   private:
    size_t test_cases;

    // Process single test case using coroutine-like style.
    void process_test_case() {
      size_t n;
      cin >> n;

      // Read array elements.
      Vector<int64_t> values(n);
      for (auto& val : values) {
        cin >> val;
      }

      // Create minimizer and solve.
      auto minimizer = make_minimizer(values);
      auto result    = minimizer.solve();

      // Pattern matching on result using expected.
      result
          .transform([](auto value) {
            cout << value << '\n';
            return value;
          })
          .or_else([](auto) {
            cout << INFINITE_REDUCTION << '\n';
            return SolutionResult<int64_t>{INFINITE_REDUCTION};
          });
    }

   public:
    void run() {
      ios_base::sync_with_stdio(false);
      cin.tie(nullptr);

      cin >> test_cases;

      // Process all test cases in parallel.
      ranges::for_each(views::iota(0UZ, test_cases), [this](auto) { process_test_case(); });
    }
  };

}  // namespace Math

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Solution orchestrator.
  Math::Solution solution;
  solution.run();
  return 0;
}

//===----------------------------------------------------------------------===//
