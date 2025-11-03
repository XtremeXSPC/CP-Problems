//===---------------------------------------------------------------------===//
/**
 * @file: Tester.cpp
 * @brief: Tester for coding data structures and algorithms.
 * @version: 0.1
 * @date: 2025-05-20
 *
 * @details: Examined problem: Codeforces R1042 Div. 3 - Problem H - CopriMe
 *
 * @copyright: Copyright MIT 2025
 *
 */
//===---------------------------------------------------------------------===//
/* Included library */

// clang-format off
// Compiler optimizations:
#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC optimize("Ofast,unroll-loops,fast-math,O3")
  // x86_64 specific optimizations:
  #ifdef __x86_64__
    #pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt")
  #endif
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
  // Policy-Based Data Structures:
  #include <ext/pb_ds/assoc_container.hpp>
  #include <ext/pb_ds/tree_policy.hpp>
#endif

// Debug macro:
#ifdef LOCAL
  #include "../Algorithms/libs/debug.h"
#else
  #define debug(...) 42
  #define debug_if(...) 42
  #define debug_tree(...) 42
  #define debug_tree_verbose(...) 42
  #define debug_line() 42
  #define my_assert(...) 42
  #define COUNT_CALLS(...) 42
#endif

//===----------------------------------------------------------------------===//
/* Namespaces and Type Aliases */

// Type aliases.
using ll   = long long;
using ull  = unsigned long long;
using ld   = long double;
using U8   = u_int8_t;
using U16  = u_int16_t;
using U32  = u_int32_t;
using U64  = u_int64_t;
#ifdef __SIZEOF_INT128__
  using I128 = __int128;
  using U128 = unsigned __int128;
#else
  using I128 = long long;
  using U128 = unsigned long long;
#endif
using I128 = __int128;
using U128 = unsigned __int128;
#ifdef __FLOAT128__
  using F128 = __float128;
#else
  using F128 = long double;
#endif

// Type aliases for advanced types.
using P_ii     = std::pair<int, int>;
using P_ll     = std::pair<long long, long long>;
using V_b      = std::vector<bool>;
using V_i      = std::vector<int>;
using V_ui     = std::vector<unsigned int>;
using V_ll     = std::vector<long long>;
using VV_i     = std::vector<V_i>;
using VV_ll    = std::vector<V_ll>;
using V_s      = std::vector<std::string>;
using VV_s     = std::vector<V_s>;
using VP_ii    = std::vector<P_ii>;
using VVP_ii   = std::vector<VP_ii>;
using VP_ll    = std::vector<P_ll>;
using VT_iii   = std::vector<std::tuple<int, int, int>>;
using Map_ll   = std::map<long long, long long>;
using VUMap_il = std::vector<std::unordered_map<int, ll>>;

// Namespace shortcuts.
using namespace std;
using namespace __gnu_pbds;

// Ordered set (tree-based) - Supports order_of_key and find_by_order.
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

// Mathematical constants.
constexpr long double PI   = 3.141592653589793238462643383279502884L;
constexpr long double E    = 2.718281828459045235360287471352662498L;
constexpr long double EPS  = 1e-9L;
constexpr int         INF  = 0x3f3f3f3f;
constexpr long long   LINF = 0x3f3f3f3f3f3f3f3fLL;
constexpr int         LIM  = 1000000 + 5;
constexpr int         MOD  = 1000000007;
constexpr int         MOD2 = 998244353;
// clang-format on

//===----------------------------------------------------------------------===//
/* Utility Macros and Functions */

// Useful macros.
#define overload4(a, b, c, d, e, ...) e
#define overload3(a, b, c, d, ...) d
#define FOR_IMPL(i, a, b, s) for (int i = (a); (s) > 0 ? i < (b) : i > (b); i += (s))
#define FOR2(i, a) FOR_IMPL(i, 0, a, 1)
#define FOR3(i, a, b) FOR_IMPL(i, a, b, 1)
#define FOR4(i, a, b, s) FOR_IMPL(i, a, b, s)
#define FOR(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2)(__VA_ARGS__)
#define ROF(i, a, b) for (int i = (b) - 1; i >= (a); --i)

// Common macros
#define all(x) x.begin(), x.end()
#define sz(x) (int)x.size()
#define pb push_back
#define eb emplace_back
#define UNIQUE(v) sort(all(v)), v.erase(unique(all(v)), v.end())
#define LB(c, x) distance((c).begin(), lower_bound(all(c), (x)))
#define UB(c, x) distance((c).begin(), upper_bound(all(c), (x)))

// Bitwise operations (wrappers for GCC built-ins).
int popcnt(int x) {
  return __builtin_popcount(x);
}
int popcnt(ll x) {
  return __builtin_popcountll(x);
}
int topbit(int x) {
  return x == 0 ? -1 : 31 - __builtin_clz(x);
}
int topbit(ll x) {
  return x == 0 ? -1 : 63 - __builtin_clzll(x);
}
int lowbit(int x) {
  return x == 0 ? -1 : __builtin_ctz(x);
}
int lowbit(ll x) {
  return x == 0 ? -1 : __builtin_ctzll(x);
}

// Min/Max update functions.
template <class T, class S>
inline bool chmax(T& a, const S& b) {
  return (a < b ? a = b, 1 : 0);
}
template <class T, class S>
inline bool chmin(T& a, const S& b) {
  return (a > b ? a = b, 1 : 0);
}

// Generic Binary Search.
template <typename F>
ll binary_search(F&& predicate, ll left, ll right) {
  while (std::abs(left - right) > 1) {
    ll mid                          = left + (right - left) / 2; // Avoid overflow
    (predicate(mid) ? left : right) = mid;
  }
  return left;
}

template <typename F>
double binary_search_real(F&& predicate, double left, double right, size_t iterations = 100) {
  for ([[maybe_unused]] auto _ : views::iota(0u, iterations)) {
    double mid                      = left + (right - left) / 2;
    (predicate(mid) ? left : right) = mid;
  }
  return left + (right - left) / 2;
}

//===----------------------------------------------------------------------===//
/* Math utility functions and Modular Arithmetic */

ll power(ll base, ll exp) {
  ll res = 1;
  base %= MOD;
  while (exp > 0) {
    if (exp % 2 == 1)
      res = (res * base) % MOD;
    base = (base * base) % MOD;
    exp /= 2;
  }
  return res;
}

ll modInverse(ll n) {
  return power(n, MOD - 2);
}

ll gcd(ll a, ll b) {
  return b == 0 ? a : gcd(b, a % b);
}

template <int MOD>
struct modInt {
  U32 val;
  constexpr modInt(long long x = 0) : val((x %= MOD) < 0 ? x + MOD : x) {}
  modInt& operator+=(const modInt& p) {
    if ((val += p.val) >= MOD)
      val -= MOD;
    return *this;
  }
  modInt& operator-=(const modInt& p) {
    if ((val += MOD - p.val) >= MOD)
      val -= MOD;
    return *this;
  }
  modInt& operator*=(const modInt& p) {
    val = (unsigned long long)val * p.val % MOD;
    return *this;
  }
  modInt& operator/=(const modInt& p) {
    *this *= p.inverse();
    return *this;
  }

  bool operator==(const modInt& p) const { return val == p.val; }
  bool operator!=(const modInt& p) const { return val != p.val; }

  modInt operator-() const { return modInt(-val); }
  modInt operator+(const modInt& p) const { return modInt(*this) += p; }
  modInt operator-(const modInt& p) const { return modInt(*this) -= p; }
  modInt operator*(const modInt& p) const { return modInt(*this) *= p; }
  modInt operator/(const modInt& p) const { return modInt(*this) /= p; }

  modInt inverse() const {
    int a = val, b = MOD, u = 1, v = 0, t;
    while (b > 0) {
      t = a / b;
      swap(a -= t * b, b);
      swap(u -= t * v, v);
    }
    return modInt(u);
  }
  modInt pow(long long n) const {
    modInt ret(1), mul(val);
    while (n > 0) {
      if (n & 1)
        ret *= mul;
      mul *= mul;
      n >>= 1;
    }
    return ret;
  }
};

using mint = modInt<MOD>;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

// Immutable number theory cache with compile-time initialization potential.
struct NumberTheoryEngine {
  static constexpr size_t LIMIT = 1000001;
  array<int, LIMIT>       minimal_prime;
  array<int, LIMIT>       moebius;

  NumberTheoryEngine() {
    iota(minimal_prime.begin(), minimal_prime.end(), 0);
    fill(moebius.begin(), moebius.end(), 0);
    moebius[1] = 1;

    vector<int> prime_list;
    prime_list.reserve(80000); // Approximate prime count.

    for (size_t i = 2; i < LIMIT; ++i) {
      if (minimal_prime[i] == static_cast<int>(i)) {
        prime_list.push_back(i);
        moebius[i] = -1;
      }

      for (int prime : prime_list) {
        if (prime > minimal_prime[i] || static_cast<ll>(i) * prime >= static_cast<ll>(LIMIT))
          break;
        minimal_prime[i * prime] = prime;
        moebius[i * prime]       = (prime == minimal_prime[i]) ? 0 : -moebius[i];
      }
    }
  }

  // Factorization returning all squarefree divisors.
  [[nodiscard]] auto factorize_squarefree(int n) const -> vector<int> {
    if (n == 1)
      return {1};

    vector<int> prime_set;
    while (n > 1) {
      int p = minimal_prime[n];
      prime_set.push_back(p);
      do {
        n /= p;
      } while (n % p == 0);
    }

    // Generate all subsets as products.
    vector<int> divisors = {1};
    for (int prime : prime_set) {
      size_t current_size = divisors.size();
      divisors.reserve(current_size * 2);
      for (size_t i = 0; i < current_size; ++i) {
        divisors.push_back(divisors[i] * prime);
      }
    }
    return divisors;
  }
};

// Global immutable instance.
inline const NumberTheoryEngine NT_ENGINE;

// Value compression system using modern idioms.
class ValueCompressor {
public:
  struct CompressedValue {
    int         original;
    int         compressed_id;
    vector<int> occurrence_indices;
    vector<int> squarefree_factors;
  };

private:
  unordered_map<int, size_t> compression_map;
  vector<CompressedValue>    compressed_data;

public:
  // Compress the input sequence and build the mapping.
  void compress(span<const int> input_sequence) {
    compression_map.clear();
    compressed_data.clear();
    compression_map.reserve(input_sequence.size());

    for (size_t idx = 0; idx < input_sequence.size(); ++idx) {
      int value = input_sequence[idx];

      auto [iter, inserted] = compression_map.try_emplace(value, compressed_data.size());
      if (inserted) {
        compressed_data.push_back(
            {.original           = value,
             .compressed_id      = static_cast<int>(compressed_data.size() - 1),
             .occurrence_indices = {static_cast<int>(idx + 1)}, // 1-indexed
             .squarefree_factors = NT_ENGINE.factorize_squarefree(value)});
      } else {
        compressed_data[iter->second].occurrence_indices.push_back(static_cast<int>(idx + 1));
      }
    }
  }

  // Retrieve compressed ID if exists.
  [[nodiscard]] auto get_compressed_id(int value) const -> optional<size_t> {
    if (auto it = compression_map.find(value); it != compression_map.end()) {
      return it->second;
    }
    return nullopt;
  }

  [[nodiscard]] auto get_compressed_values() const -> span<const CompressedValue> { return compressed_data; }

  [[nodiscard]] auto unique_count() const -> size_t { return compressed_data.size(); }
};

// Coprimality analysis using inclusion-exclusion.
class CoprimalityAnalyzer {
private:
  vector<int> divisor_multiplicities;
  const int   range_limit;

public:
  explicit CoprimalityAnalyzer(int max_val, span<const int> initial_frequencies) :
      divisor_multiplicities(max_val + 1, 0), range_limit(max_val) {
    for (int divisor = 1; divisor <= range_limit; ++divisor) {
      for (int multiple = divisor; multiple <= range_limit; multiple += divisor) {
        divisor_multiplicities[divisor] += initial_frequencies[multiple];
      }
    }
  }

  // Optimized direct loop instead of transform_reduce.
  [[nodiscard]] inline int count_coprimes(const vector<int>& squarefree_factors) const {
    int sum = 0;
    for (int d : squarefree_factors) {
      sum += NT_ENGINE.moebius[d] * divisor_multiplicities[d];
    }
    return sum;
  }

  void modify_element_presence(const vector<int>& squarefree_factors, int delta) {
    for (int divisor : squarefree_factors) {
      divisor_multiplicities[divisor] += delta;
    }
  }
};

// Solution orchestrator using modern patterns.
class CoprimePairSolver {
private:
  // Problem specification.
  struct ProblemSpec {
    int         element_count;
    int         value_bound;
    vector<int> sequence;
    vector<int> value_histogram;
  } spec;

  // Core components.
  ValueCompressor                 compressor;
  unique_ptr<CoprimalityAnalyzer> analyzer;

  // Performance optimization caches.
  vector<int>         sequence_to_compressed_id; // Direct O(1) access to compressed IDs.
  vector<vector<int>> cached_squarefree_factors; // Pre-computed squarefree factors.
  vector<int>         cached_coprime_degrees;    // Pre-computed coprime degrees per unique value.
  vector<int>         promising_indices;         // Indices with degree >= 1 for faster search.
  vector<int>         cached_original_values;    // Original values per compressed ID.
  vector<vector<int>> cached_occurrences;        // Occurrence indices per compressed ID.

  // Solution type.
  using Quadruple     = array<int, 4>;
  using MaybeSolution = optional<Quadruple>;

  // Strategy pattern for different solving approaches.
  struct UnitValueStrategy {
    static auto execute(span<const int> unit_indices, span<const int> sequence) -> MaybeSolution {
      size_t unit_count = unit_indices.size();

      if (unit_count >= 4) {
        return Quadruple{unit_indices[0], unit_indices[1], unit_indices[2], unit_indices[3]};
      }

      if (unit_count == 3) {
        for (size_t i = 1; i < sequence.size(); ++i) {
          if (sequence[i] != 1) {
            return Quadruple{unit_indices[0], unit_indices[1], unit_indices[2], static_cast<int>(i)};
          }
        }
        return nullopt;
      }

      if (unit_count == 2) {
        vector<int> non_units;
        for (size_t i = 1; i < sequence.size() && non_units.size() < 2; ++i) {
          if (sequence[i] != 1) {
            non_units.push_back(static_cast<int>(i));
          }
        }
        if (non_units.size() >= 2) {
          return Quadruple{unit_indices[0], non_units[0], unit_indices[1], non_units[1]};
        }
      }

      return nullopt;
    }
  };

  // Optimized coprime search with cached access.
  auto filtered_coprime_search(int excluded_first, int excluded_second) -> pair<int, int> {
    // Iterate only over promising indices instead of all elements.
    for (int idx : promising_indices) {
      if (idx == excluded_first || idx == excluded_second)
        continue;

      int compressed_id = sequence_to_compressed_id[idx];

      // Recompute partners after removal (necessary due to dynamic graph changes).
      int potential_partners = analyzer->count_coprimes(cached_squarefree_factors[compressed_id]);

      if (spec.sequence[idx] == 1)
        potential_partners--;

      if (potential_partners >= 1) {
        for (int partner = 1; partner <= spec.element_count; ++partner) {
          if (partner == excluded_first || partner == excluded_second || partner == idx)
            continue;

          if (gcd(spec.sequence[idx], spec.sequence[partner]) == 1) {
            return {idx, partner};
          }
        }
      }
    }
    return {-1, -1};
  }

public:
  CoprimePairSolver() {
    cin >> spec.element_count >> spec.value_bound;
    spec.sequence.resize(spec.element_count + 1);
    spec.value_histogram.assign(spec.value_bound + 1, 0);

    for (int i = 1; i <= spec.element_count; ++i) {
      cin >> spec.sequence[i];
      spec.value_histogram[spec.sequence[i]]++;
    }

    // Initialize compression and analysis.
    span<const int> sequence_view(spec.sequence.data() + 1, spec.element_count);
    compressor.compress(sequence_view);
    analyzer = make_unique<CoprimalityAnalyzer>(spec.value_bound, spec.value_histogram);

    // Build performance caches.
    sequence_to_compressed_id.assign(spec.element_count + 1, -1);
    cached_squarefree_factors.resize(compressor.unique_count());
    cached_coprime_degrees.resize(compressor.unique_count());
    cached_original_values.resize(compressor.unique_count());
    cached_occurrences.resize(compressor.unique_count());

    // Populate compressed ID cache for O(1) access.
    for (int i = 1; i <= spec.element_count; ++i) {
      if (auto id = compressor.get_compressed_id(spec.sequence[i]); id.has_value()) {
        sequence_to_compressed_id[i] = static_cast<int>(*id);
      }
    }

    // Pre-compute all attributes for each unique value.
    for (size_t id = 0; id < compressor.unique_count(); ++id) {
      const auto& compressed        = compressor.get_compressed_values()[id];
      cached_squarefree_factors[id] = compressed.squarefree_factors;
      cached_original_values[id]    = compressed.original;
      cached_occurrences[id]        = compressed.occurrence_indices;

      // Pre-compute coprime degree for this unique value.
      int degree = analyzer->count_coprimes(cached_squarefree_factors[id]);
      if (compressed.original == 1) {
        degree--;
      }
      cached_coprime_degrees[id] = degree;
    }

    // Build list of promising indices for faster search.
    promising_indices.reserve(spec.element_count);
    for (int i = 1; i <= spec.element_count; ++i) {
      int compressed_id = sequence_to_compressed_id[i];
      if (compressed_id != -1 && cached_coprime_degrees[compressed_id] >= 1) {
        promising_indices.push_back(i);
      }
    }
  }

  void solve() {
    // Phase 1: Unit value optimization.
    if (auto unit_id = compressor.get_compressed_id(1); unit_id.has_value()) {
      const auto& unit_occurrences = cached_occurrences[*unit_id];

      if (auto result = UnitValueStrategy::execute(unit_occurrences, spec.sequence); result.has_value()) {
        output_solution(*result);
        return;
      }

      // Single unit special case.
      if (unit_occurrences.size() == 1) {
        if (auto solution = handle_single_unit_case(unit_occurrences[0]); solution.has_value()) {
          output_solution(*solution);
          return;
        }
        cout << 0 << "\n";
        return;
      }
    }

    // Phase 2: Duplicate value exploitation.
    for (size_t id = 0; id < compressor.unique_count(); ++id) {
      if (cached_occurrences[id].size() >= 2 && cached_original_values[id] != 1) {
        // Use pre-computed degree
        int coprime_candidates = cached_coprime_degrees[id];

        if (coprime_candidates >= 2) {
          vector<int> partners;
          for (int i = 1; i <= spec.element_count && partners.size() < 2; ++i) {
            if (gcd(cached_original_values[id], spec.sequence[i]) == 1) {
              partners.push_back(i);
            }
          }

          if (partners.size() >= 2) {
            output_solution({cached_occurrences[id][0], partners[0], cached_occurrences[id][1], partners[1]});
            return;
          }
        }
      }
    }

    // Phase 3: Graph-theoretic search with pruning.
    if (auto solution = execute_graph_search(); solution.has_value()) {
      output_solution(*solution);
      return;
    }

    cout << 0 << "\n";
  }

private:
  // Handle the special case with a single unit value present.
  auto handle_single_unit_case(int unit_index) -> MaybeSolution {
    // Find any coprime pair among non-unit values.
    for (size_t id = 0; id < compressor.unique_count(); ++id) {
      if (cached_original_values[id] == 1)
        continue;

      // Use pre-computed degree and adjust for non-unit context.
      int non_unit_coprimes = cached_coprime_degrees[id] - 1; // -1 because we exclude the unit.
      if (non_unit_coprimes < 1)
        continue;

      int first_index = cached_occurrences[id][0];

      for (int partner = 1; partner <= spec.element_count; ++partner) {
        if (partner == unit_index || partner == first_index)
          continue;

        if (gcd(cached_original_values[id], spec.sequence[partner]) == 1) {
          for (int fourth = 1; fourth <= spec.element_count; ++fourth) {
            if (fourth != unit_index && fourth != first_index && fourth != partner) {
              return Quadruple{first_index, partner, unit_index, fourth};
            }
          }
        }
      }
    }
    return nullopt;
  }

  // Execute optimized graph search based on coprimality degrees.
  auto execute_graph_search() -> MaybeSolution {
    // Build vertex list using pre-computed degrees.
    vector<tuple<int, int, int>> vertex_properties; // {degree, value_id, index}

    for (int i = 1; i <= spec.element_count; ++i) {
      int compressed_id = sequence_to_compressed_id[i];
      if (compressed_id == -1)
        continue;

      // Use pre-computed degree for O(1) access.
      int degree = cached_coprime_degrees[compressed_id];

      if (degree >= 1) {
        vertex_properties.emplace_back(degree, compressed_id, i);
      }
    }

    // Process by ascending degree.
    sort(vertex_properties.begin(), vertex_properties.end());

    constexpr int SEARCH_WIDTH = 30;

    for (const auto& [degree, value_id, primary_index] : vertex_properties) {
      // Special handling for leaf vertices.
      if (degree == 1) {
        if (auto solution = process_leaf_vertex(primary_index); solution.has_value()) {
          return solution;
        }
        continue;
      }

      // General search with bounded candidates.
      vector<int> candidate_partners;
      candidate_partners.reserve(SEARCH_WIDTH);

      for (int partner = 1; partner <= spec.element_count && static_cast<int>(candidate_partners.size()) < SEARCH_WIDTH; ++partner) {
        if (partner != primary_index && gcd(spec.sequence[primary_index], spec.sequence[partner]) == 1) {
          candidate_partners.push_back(partner);
        }
      }

      for (int secondary_index : candidate_partners) {
        // Use cached ID for O(1) access.
        int secondary_id = sequence_to_compressed_id[secondary_index];
        if (secondary_id == -1)
          continue;

        // Temporarily remove from active set using cached factors.
        analyzer->modify_element_presence(cached_squarefree_factors[value_id], -1);
        analyzer->modify_element_presence(cached_squarefree_factors[secondary_id], -1);

        auto [third, fourth] = filtered_coprime_search(primary_index, secondary_index);

        if (third != -1) {
          return Quadruple{primary_index, secondary_index, third, fourth};
        }

        // Restore to active set.
        analyzer->modify_element_presence(cached_squarefree_factors[value_id], +1);
        analyzer->modify_element_presence(cached_squarefree_factors[secondary_id], +1);
      }
    }

    return nullopt;
  }

  // Optimized processing for leaf vertices in the coprimality graph.
  auto process_leaf_vertex(int leaf_index) -> MaybeSolution {
    for (int partner = 1; partner <= spec.element_count; ++partner) {
      if (partner != leaf_index && gcd(spec.sequence[leaf_index], spec.sequence[partner]) == 1) {
        // Use cached IDs for O(1) access.
        int leaf_id    = sequence_to_compressed_id[leaf_index];
        int partner_id = sequence_to_compressed_id[partner];

        if (leaf_id == -1 || partner_id == -1)
          continue;

        // Use cached factors directly.
        analyzer->modify_element_presence(cached_squarefree_factors[leaf_id], -1);
        analyzer->modify_element_presence(cached_squarefree_factors[partner_id], -1);

        auto [third, fourth] = filtered_coprime_search(leaf_index, partner);

        if (third != -1) {
          return Quadruple{leaf_index, partner, third, fourth};
        }

        analyzer->modify_element_presence(cached_squarefree_factors[leaf_id], +1);
        analyzer->modify_element_presence(cached_squarefree_factors[partner_id], +1);
        break; // Leaf has only one partner.
      }
    }
    return nullopt;
  }

  void output_solution(const Quadruple& solution) {
    cout << solution[0] << " " << solution[1] << " " << solution[2] << " " << solution[3] << "\n";
  }
};

//===----------------------------------------------------------------------===//
/* Main function */

int main() {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int test_cases;
  cin >> test_cases;

  while (test_cases--) {
    CoprimePairSolver solver;
    solver.solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
/* End of Tester.cpp */