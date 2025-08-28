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
#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC optimize("Ofast,unroll-loops,fast-math,O3")
  // Apple Silicon optimizations:
  
  #ifdef __aarch64__
    #pragma GCC target("+simd")
  #endif
#endif

#ifdef __clang__
  #pragma clang optimize on
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-result"
    // Target-specific optimizations for Clang
    #ifdef __aarch64__
      #pragma clang attribute push (__attribute__((target("neon"))), apply_to=function)
    #elif defined(__x86_64__)
      #pragma clang attribute push (__attribute__((target("sse4.2,popcnt"))), apply_to=function)
    #endif
#endif

// Smart header selection based on compiler and flags:
#if defined(USE_PCH_FALLBACK) || defined(USE_CLANG_SANITIZE)
  // Explicitly requested PCH fallback
  #include "../Algorithms/PCH.h"
#elif defined(__clang__)
  // Using Clang, need PCH fallback
  #include "../Algorithms/PCH.h"
#elif __has_include(<bits/stdc++.h>)
  // GCC with bits/stdc++.h available
  #include <bits/stdc++.h>
#else
  // Final fallback
  #include "../Algorithms/PCH.h"
#endif

// Debug macro:
#ifdef LOCAL
  #include "../Algorithms/debug.h"
#else
  #define debug(...) 42
#endif
// clang-format on

//===----------------------------------------------------------------------===//
/* Type Aliases and Constants */

// Type aliases
using ll     = long long;
using ull    = unsigned long long;
using ld     = long double;
using pii    = std::pair<int, int>;
using pll    = std::pair<long long, long long>;
using vi     = std::vector<int>;
using vui    = std::vector<unsigned int>;
using vll    = std::vector<long long>;
using vvi    = std::vector<std::vector<int>>;
using vvll   = std::vector<std::vector<long long>>;
using vs     = std::vector<std::string>;
using vpii   = std::vector<std::pair<int, int>>;
using vpll   = std::vector<std::pair<long long, long long>>;
using map_ll = std::map<long long, long long>;

// Mathematical constants
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

// clang-format off
// Restore original compiler settings.
#ifdef __clang__
    #ifdef __aarch64__
      #pragma clang attribute pop
    #elif defined(__x86_64__)
      #pragma clang attribute pop
    #endif
    #pragma clang diagnostic pop
#endif

//===----------------------------------------------------------------------===//