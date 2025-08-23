//===----------------------------------------------------------------------===//
/**
 * @file: problem_H.cpp
 * @brief: Codeforces Round 1042 (Div. 3) - Problem H
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library and Macros */

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
/* Modern C++ Data Structures */

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

// Global immutable instance
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

  [[nodiscard]] auto count_coprimes(span<const int> squarefree_factors) const -> int {
    return transform_reduce(squarefree_factors.begin(), squarefree_factors.end(), 0, plus<>{}, [this](int d) {
      return NT_ENGINE.moebius[d] * divisor_multiplicities[d];
    });
  }

  void modify_element_presence(span<const int> squarefree_factors, int delta) {
    for (int divisor : squarefree_factors) {
      divisor_multiplicities[divisor] += delta;
    }
  }
};

// Solution orchestrator using modern patterns.
class CoprimePairSolver {
private:
  // Problem specification
  struct ProblemSpec {
    int         element_count;
    int         value_bound;
    vector<int> sequence;
    vector<int> value_histogram;
  } spec;

  // Core components
  ValueCompressor                 compressor;
  unique_ptr<CoprimalityAnalyzer> analyzer;

  // Solution type
  using Quadruple     = array<int, 4>;
  using MaybeSolution = optional<Quadruple>;

  // Strategy pattern for different solving approaches
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

  // Advanced search with coprimality filtering
  auto filtered_coprime_search(int excluded_first, int excluded_second) -> pair<int, int> {
    for (size_t idx = 1; idx < spec.sequence.size(); ++idx) {
      if (static_cast<int>(idx) == excluded_first || static_cast<int>(idx) == excluded_second)
        continue;

      auto compressed_id = compressor.get_compressed_id(spec.sequence[idx]);
      if (!compressed_id)
        continue;

      const auto& compressed         = compressor.get_compressed_values()[*compressed_id];
      int         potential_partners = analyzer->count_coprimes(compressed.squarefree_factors);

      if (compressed.original == 1)
        potential_partners--;

      if (potential_partners >= 1) {
        for (size_t partner = 1; partner < spec.sequence.size(); ++partner) {
          if (partner == static_cast<size_t>(excluded_first) || partner == static_cast<size_t>(excluded_second) || partner == idx)
            continue;

          if (gcd(spec.sequence[idx], spec.sequence[partner]) == 1) {
            return {static_cast<int>(idx), static_cast<int>(partner)};
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

    // Initialize compression and analysis
    span<const int> sequence_view(spec.sequence.data() + 1, spec.element_count);
    compressor.compress(sequence_view);
    analyzer = make_unique<CoprimalityAnalyzer>(spec.value_bound, spec.value_histogram);
  }

  void solve() {
    // Phase 1: Unit value optimization
    if (auto unit_id = compressor.get_compressed_id(1); unit_id.has_value()) {
      const auto& unit_data = compressor.get_compressed_values()[*unit_id];

      if (auto result = UnitValueStrategy::execute(unit_data.occurrence_indices, spec.sequence); result.has_value()) {
        output_solution(*result);
        return;
      }

      // Single unit special case
      if (unit_data.occurrence_indices.size() == 1) {
        if (auto solution = handle_single_unit_case(unit_data.occurrence_indices[0]); solution.has_value()) {
          output_solution(*solution);
          return;
        }
        cout << 0 << "\n";
        return;
      }
    }

    // Phase 2: Duplicate value exploitation
    for (const auto& compressed : compressor.get_compressed_values()) {
      if (compressed.occurrence_indices.size() >= 2 && compressed.original != 1) {
        int coprime_candidates = analyzer->count_coprimes(compressed.squarefree_factors);

        if (coprime_candidates >= 2) {
          vector<int> partners;
          for (int i = 1; i <= spec.element_count && partners.size() < 2; ++i) {
            if (gcd(compressed.original, spec.sequence[i]) == 1) {
              partners.push_back(i);
            }
          }

          if (partners.size() >= 2) {
            output_solution({compressed.occurrence_indices[0], partners[0], compressed.occurrence_indices[1], partners[1]});
            return;
          }
        }
      }
    }

    // Phase 3: Graph-theoretic search with pruning
    if (auto solution = execute_graph_search(); solution.has_value()) {
      output_solution(*solution);
      return;
    }

    cout << 0 << "\n";
  }

private:
  // Handle the special case with a single unit value present.
  auto handle_single_unit_case(int unit_index) -> MaybeSolution {
    // Find any coprime pair among non-unit values
    for (const auto& compressed : compressor.get_compressed_values()) {
      if (compressed.original == 1)
        continue;

      int non_unit_coprimes = analyzer->count_coprimes(compressed.squarefree_factors) - 1;
      if (non_unit_coprimes < 1)
        continue;

      int first_index = compressed.occurrence_indices[0];

      for (int partner = 1; partner <= spec.element_count; ++partner) {
        if (partner == unit_index || partner == first_index)
          continue;

        if (gcd(compressed.original, spec.sequence[partner]) == 1) {
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

  // Execute a graph search based on coprimality degrees.
  auto execute_graph_search() -> MaybeSolution {
    // Compute coprimality degrees
    vector<tuple<int, int, int>> vertex_properties; // {degree, value_id, index}

    for (const auto& compressed : compressor.get_compressed_values()) {
      int degree = analyzer->count_coprimes(compressed.squarefree_factors);
      if (compressed.original == 1)
        degree--;

      if (degree >= 1) {
        for (int idx : compressed.occurrence_indices) {
          vertex_properties.emplace_back(degree, compressed.compressed_id, idx);
        }
      }
    }

    // Process by ascending degree
    sort(vertex_properties.begin(), vertex_properties.end());

    constexpr int SEARCH_WIDTH = 30;

    for (const auto& [degree, value_id, primary_index] : vertex_properties) {
      // Special handling for leaf vertices
      if (degree == 1) {
        if (auto solution = process_leaf_vertex(primary_index); solution.has_value()) {
          return solution;
        }
        continue;
      }

      // General search with bounded candidates
      vector<int> candidate_partners;
      candidate_partners.reserve(SEARCH_WIDTH);

      for (int partner = 1; partner <= spec.element_count && static_cast<int>(candidate_partners.size()) < SEARCH_WIDTH; ++partner) {
        if (partner != primary_index && gcd(spec.sequence[primary_index], spec.sequence[partner]) == 1) {
          candidate_partners.push_back(partner);
        }
      }

      for (int secondary_index : candidate_partners) {
        if (static_cast<size_t>(value_id) >= compressor.get_compressed_values().size())
          continue;
        const auto& primary_compressed = compressor.get_compressed_values()[value_id];
        auto        secondary_id       = compressor.get_compressed_id(spec.sequence[secondary_index]);

        if (!secondary_id)
          continue;
        const auto& secondary_compressed = compressor.get_compressed_values()[*secondary_id];

        // Temporarily remove from active set
        analyzer->modify_element_presence(primary_compressed.squarefree_factors, -1);
        analyzer->modify_element_presence(secondary_compressed.squarefree_factors, -1);

        auto [third, fourth] = filtered_coprime_search(primary_index, secondary_index);

        if (third != -1) {
          return Quadruple{primary_index, secondary_index, third, fourth};
        }

        // Restore to active set
        analyzer->modify_element_presence(primary_compressed.squarefree_factors, +1);
        analyzer->modify_element_presence(secondary_compressed.squarefree_factors, +1);
      }
    }

    return nullopt;
  }

  // Specialized processing for leaf vertices (degree == 1) in the coprimality graph.
  auto process_leaf_vertex(int leaf_index) -> MaybeSolution {
    for (int partner = 1; partner <= spec.element_count; ++partner) {
      if (partner != leaf_index && gcd(spec.sequence[leaf_index], spec.sequence[partner]) == 1) {
        auto leaf_id    = compressor.get_compressed_id(spec.sequence[leaf_index]);
        auto partner_id = compressor.get_compressed_id(spec.sequence[partner]);

        if (!leaf_id || !partner_id)
          continue;

        const auto& leaf_compressed    = compressor.get_compressed_values()[*leaf_id];
        const auto& partner_compressed = compressor.get_compressed_values()[*partner_id];

        analyzer->modify_element_presence(leaf_compressed.squarefree_factors, -1);
        analyzer->modify_element_presence(partner_compressed.squarefree_factors, -1);

        auto [third, fourth] = filtered_coprime_search(leaf_index, partner);

        if (third != -1) {
          return Quadruple{leaf_index, partner, third, fourth};
        }

        analyzer->modify_element_presence(leaf_compressed.squarefree_factors, +1);
        analyzer->modify_element_presence(partner_compressed.squarefree_factors, +1);
        break; // Leaf has only one partner
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