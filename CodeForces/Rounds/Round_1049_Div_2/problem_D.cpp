//===----------------------------------------------------------------------===//
/**
 * @file: problem_D.cpp
 * @brief Codeforces Round 1049 (Div. 2) - Problem D
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

// Interval structure to hold start and finish points.
struct Interval {
  ll start, finish;

  [[nodiscard]] inline ll length() const noexcept { return finish - start; }

  [[nodiscard]] inline ll combined_value() const noexcept { return start + finish; }
};

// Class to process intervals and compute maximum length after optimal pairing.
class IntervalProcessor {
 private:
  int              count_;
  vector<Interval> intervals_;
  ll               initial_total_;

  [[nodiscard]] ll compute_optimal_pairing() const {
    // Transform intervals to combined representation.
    vector<ll> combined_vals;
    combined_vals.reserve(count_);

    ll finish_accumulator = 0;

    transform(
        intervals_.begin(), intervals_.end(), back_inserter(combined_vals),
        [&finish_accumulator](const Interval& iv) {
          finish_accumulator += iv.finish;
          return iv.combined_value();
        });

    // Generate ranking based on combined values.
    vector<size_t> ranking(count_);
    generate(ranking.begin(), ranking.end(), [n = size_t(0)]() mutable { return n++; });

    stable_sort(ranking.begin(), ranking.end(), [&combined_vals](size_t a, size_t b) {
      return combined_vals[a] < combined_vals[b];
    });

    // Build cumulative sums using ranked order.
    vector<ll> cumulative(count_);
    ll         running_sum = 0;

    for (int idx = 0; idx < count_; ++idx) {
      running_sum += combined_vals[ranking[idx]];
      cumulative[idx] = running_sum;
    }

    // Determine optimal configuration.
    if (count_ & 1) {
      return handle_odd_configuration(ranking, combined_vals, cumulative, finish_accumulator);
    } else {
      return handle_even_configuration(cumulative, finish_accumulator);
    }
  }

  [[nodiscard]] ll handle_even_configuration(const vector<ll>& cumulative, ll finish_total) const {
    const int half_point     = count_ >> 1;
    const ll  lower_half_sum = half_point ? cumulative[half_point - 1] : 0LL;
    return finish_total - lower_half_sum;
  }

  [[nodiscard]] ll handle_odd_configuration(
      const vector<size_t>& ranking,
      const vector<ll>&     combined_vals,
      const vector<ll>&     cumulative,
      ll                    finish_total) const {
    const int target_size  = count_ >> 1;
    ll        maximum_gain = numeric_limits<ll>::lowest();

    for (int excluded = 0; excluded < count_; ++excluded) {
      const ll adjusted_finish = finish_total - intervals_[ranking[excluded]].finish;

      ll lower_portion = 0;
      if (target_size > 0) {
        if (excluded < target_size) {
          // Excluded element is in lower half.
          lower_portion = cumulative[target_size] - combined_vals[ranking[excluded]];
        } else {
          // Excluded element is in upper half.
          lower_portion = cumulative[target_size - 1];
        }
      }

      const ll current_gain = adjusted_finish - lower_portion;
      maximum_gain          = max(maximum_gain, current_gain);
    }

    return maximum_gain;
  }

 public:
  explicit IntervalProcessor(int n) : count_(n), intervals_(), initial_total_(0) {
    intervals_.reserve(n);
  }

  void add_interval(ll start, ll finish) {
    intervals_.push_back({start, finish});
    initial_total_ += (finish - start);
  }

  [[nodiscard]] ll calculate_maximum_length() const {
    if (count_ == 0) return 0;

    const ll optimal_addition = compute_optimal_pairing();
    return initial_total_ + max(0LL, optimal_addition);
  }
};

// Function to solve a single test case.
void solve() {
  int n;
  cin >> n;

  IntervalProcessor processor(n);

  for (int i = 0; i < n; ++i) {
    ll left_bound, right_bound;
    cin >> left_bound >> right_bound;
    processor.add_interval(left_bound, right_bound);
  }

  cout << processor.calculate_maximum_length() << "\n";
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
