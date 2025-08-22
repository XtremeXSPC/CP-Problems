//===----------------------------------------------------------------------===//
/**
 * @file: problem_H.cpp
 * @brief: Codeforces Round 1042 (Div. 3) - Problem H
 * @author: Costantino Lombardi
 *
 * @status:
 */
//===----------------------------------------------------------------------===//
/* Included library and Macros */

// Compiler optimizations:
// #pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math,O3")

// clang-format off
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
/* Data Types and Function Definitions */

// Maximum value for sieve.
const int MAX_M = 1000001;

// Encapsulated in a struct to be initialized only once across test cases.
class SieveCache {
public:
  vi smallest_prime_div;
  vi mobius_fun_values;

  SieveCache() {
    smallest_prime_div.resize(MAX_M);
    mobius_fun_values.resize(MAX_M);

    iota(smallest_prime_div.begin(), smallest_prime_div.end(), 0);
    mobius_fun_values[1] = 1;
    vi primes;

    for (int i = 2; i < MAX_M; ++i) {
      if (smallest_prime_div[i] == i) {
        primes.push_back(i);
        mobius_fun_values[i] = -1;
      }
      for (int p : primes) {
        if (p > smallest_prime_div[i] || (ll)i * p >= MAX_M)
          break;
        smallest_prime_div[i * p] = p;
        mobius_fun_values[i * p]  = (p == smallest_prime_div[i]) ? 0 : -mobius_fun_values[i];
      }
    }
  }
};

// Global, const instance.
const SieveCache G_SIEVE;

// Helper class for square-free divisor operations.
class DivisorHelper {
private:
  unordered_map<int, vi> divisor_cache;

public:
  // Returns all square-free divisors of n.
  const vi& get_square_free_divs(int n) {
    if (divisor_cache.count(n))
      return divisor_cache.at(n);

    vi  prime_factors;
    int temp = n;
    while (temp > 1) {
      int p = G_SIEVE.smallest_prime_div[temp];
      prime_factors.push_back(p);
      while (temp > 1 && temp % p == 0)
        temp /= p;
    }

    vi divs = {1};
    for (int p : prime_factors) {
      divs.reserve(divs.size() * 2);
      for (size_t i = 0, current_size = divs.size(); i < current_size; ++i) {
        divs.push_back(divs[i] * p);
      }
    }
    return divisor_cache[n] = divs;
  }
};

// Main solver class orchestrating the search strategies.
class QuadrupleFinder {
private:
  int                    num_elements, max_value;
  vi                     problem_values;
  vi                     value_frequencies;
  vi                     count_of_multiples;
  unordered_map<int, vi> value_positions;
  DivisorHelper          div_helper;

  // Compute coprime count using inclusion-exclusion principle.
  int compute_coprime_count(int value) {
    int total = 0;
    for (int d : div_helper.get_square_free_divs(value)) {
      total += G_SIEVE.mobius_fun_values[d] * count_of_multiples[d];
    }
    return total;
  }

  // Update counts when removing/adding an element.
  void update_counts(int idx, int delta) {
    int value = problem_values[idx];
    for (int d : div_helper.get_square_free_divs(value)) {
      count_of_multiples[d] += delta;
    }
  }

  // Find a coprime pair excluding two indices.
  pair<int, int> find_second_pair(int ban1, int ban2) {
    for (int r = 1; r <= num_elements; ++r) {
      if (r == ban1 || r == ban2)
        continue;

      // Check if r has coprime partners available.
      int available_coprimes = compute_coprime_count(problem_values[r]);

      // Adjust for self if value is 1.
      if (problem_values[r] == 1)
        available_coprimes -= 1;

      if (available_coprimes >= 1) {
        for (int s = 1; s <= num_elements; ++s) {
          if (s == ban1 || s == ban2 || s == r)
            continue;
          if (gcd(problem_values[r], problem_values[s]) == 1) {
            return {r, s};
          }
        }
      }
    }
    return {-1, -1};
  }

public:
  // Constructor reads input and initializes data structures.
  QuadrupleFinder() {
    cin >> num_elements >> max_value;
    problem_values.resize(num_elements + 1);
    value_frequencies.assign(max_value + 1, 0);
    count_of_multiples.assign(max_value + 1, 0);

    for (int i = 1; i <= num_elements; ++i) {
      cin >> problem_values[i];
      value_frequencies[problem_values[i]]++;
      value_positions[problem_values[i]].push_back(i);
    }

    // Initialize count_of_multiples.
    for (int d = 1; d <= max_value; ++d) {
      for (int k = d; k <= max_value; k += d) {
        count_of_multiples[d] += value_frequencies[k];
      }
    }
  }

  // Main dispatcher that tries strategies in order of complexity.
  void solve() {
    // Strategy 1: Complete handling of all '1' cases
    if (value_positions.count(1)) {
      const auto& ones_indices = value_positions.at(1);

      // Case: 4+ ones - trivial solution
      if (ones_indices.size() >= 4) {
        cout << ones_indices[0] << " " << ones_indices[1] << " " << ones_indices[2] << " " << ones_indices[3] << "\n";
        return;
      }

      // Case: exactly 3 ones - take any other element
      if (ones_indices.size() == 3) {
        for (int i = 1; i <= num_elements; ++i) {
          if (problem_values[i] != 1) {
            cout << ones_indices[0] << " " << ones_indices[1] << " " << ones_indices[2] << " " << i << "\n";
            return;
          }
        }
        // Only 3 elements total, all are 1s - no solution
        cout << 0 << "\n";
        return;
      }

      // Case: exactly 2 ones - take any 2 other elements
      if (ones_indices.size() == 2) {
        vi others;
        for (int i = 1; i <= num_elements && others.size() < 2; ++i) {
          if (problem_values[i] != 1)
            others.push_back(i);
        }
        if (others.size() >= 2) {
          cout << ones_indices[0] << " " << others[0] << " " << ones_indices[1] << " " << others[1] << "\n";
          return;
        }
        // Less than 4 elements total - no solution
        cout << 0 << "\n";
        return;
      }

      // Case: exactly 1 one - most complex case
      if (ones_indices.size() == 1) {
        int one_idx = ones_indices[0];

        // Strategy: find any coprime pair among non-1 elements,
        // then pair them with the 1 and any remaining element
        for (int i = 1; i <= num_elements; ++i) {
          if (i == one_idx)
            continue;

          // Check if element i has coprime partners (excluding the 1)
          for (int j = i + 1; j <= num_elements; ++j) {
            if (j == one_idx)
              continue;

            if (gcd(problem_values[i], problem_values[j]) == 1) {
              // Found coprime pair (i,j). Now take 1 and any other element
              for (int k = 1; k <= num_elements; ++k) {
                if (k != one_idx && k != i && k != j) {
                  cout << i << " " << j << " " << one_idx << " " << k << "\n";
                  return;
                }
              }
            }
          }
        }
        // No solution found with the single 1 - continue to other strategies
      }
    }

    // Strategy 2: Exploit duplicate values efficiently
    for (const auto& [value, positions] : value_positions) {
      if (positions.size() < 2 || value == 1)
        continue;

      // Early check: does this value have enough coprime partners?
      int available_coprimes = compute_coprime_count(value);
      if (available_coprimes < 2)
        continue;

      // Use a set for O(1) lookup of position membership.
      unordered_set<int> position_set(positions.begin(), positions.end());

      // Find exactly 2 coprime partners efficiently.
      vi partners;
      partners.reserve(2);

      for (int i = 1; i <= num_elements && partners.size() < 2; ++i) {
        // Skip if this index belongs to the current value.
        if (position_set.count(i))
          continue;

        if (gcd(value, problem_values[i]) == 1) {
          partners.push_back(i);
        }
      }

      if (partners.size() >= 2) {
        cout << positions[0] << " " << partners[0] << " " << positions[1] << " " << partners[1] << "\n";
        return;
      }
    }

    // Strategy 3: Graph-based search with leaf optimization and smart ordering.
    using SearchResult = optional<array<int, 4>>;

    // Calculate degree for each index position (not value!).
    auto calculate_vertex_degrees = [&]() -> vi {
      vi vertex_degrees(num_elements + 1);
      for (int i = 1; i <= num_elements; ++i) {
        vertex_degrees[i] = compute_coprime_count(problem_values[i]);
        if (problem_values[i] == 1) {
          vertex_degrees[i]--; // Can't pair with itself.
        }
      }
      return vertex_degrees;
    };

    // Fast leaf node handler - these have exactly one coprime partner.
    auto process_leaf_nodes = [&](const vi& degrees) -> SearchResult {
      for (int leaf_idx = 1; leaf_idx <= num_elements; ++leaf_idx) {
        if (degrees[leaf_idx] != 1)
          continue;

        // Find the single coprime partner for this leaf.
        int partner_idx = -1;
        for (int j = 1; j <= num_elements; ++j) {
          if (j != leaf_idx && gcd(problem_values[leaf_idx], problem_values[j]) == 1) {
            partner_idx = j;
            break;
          }
        }

        if (partner_idx == -1)
          continue;

        // Temporarily remove this pair.
        update_counts(leaf_idx, -1);
        update_counts(partner_idx, -1);

        // Find second coprime pair.
        auto [r, s] = find_second_pair(leaf_idx, partner_idx);

        if (r != -1) {
          return array<int, 4>{leaf_idx, partner_idx, r, s};
        }

        // Restore and try next leaf.
        update_counts(leaf_idx, +1);
        update_counts(partner_idx, +1);
      }
      return {};
    };

    // General search with intelligent ordering and candidate limiting.
    auto general_search = [&](const vi& degrees) -> SearchResult {
      // Build active vertices list with their degrees.
      vector<pair<int, int>> active_vertices; // {degree, index}
      active_vertices.reserve(num_elements);

      for (int i = 1; i <= num_elements; ++i) {
        if (degrees[i] >= 1) {
          active_vertices.emplace_back(degrees[i], i);
        }
      }

      // Sort by degree (ascending) to prioritize constrained vertices.
      sort(active_vertices.begin(), active_vertices.end());

      // Process vertices starting from most constrained.
      const int MAX_CANDIDATES = 30; // Heuristic limit.

      for (const auto& [deg, first_idx] : active_vertices) {
        // Build candidate list efficiently.
        vi candidates;
        candidates.reserve(MAX_CANDIDATES);

        int first_val = problem_values[first_idx];

        // Collect coprime candidates up to limit.
        for (int j = 1; j <= num_elements && static_cast<int>(candidates.size()) < MAX_CANDIDATES; ++j) {
          if (j != first_idx && gcd(first_val, problem_values[j]) == 1) {
            candidates.push_back(j);
          }
        }

        // Try each candidate pair.
        for (int second_idx : candidates) {
          // Temporarily remove this pair.
          update_counts(first_idx, -1);
          update_counts(second_idx, -1);

          // Search for complementary pair.
          auto [third_idx, fourth_idx] = find_second_pair(first_idx, second_idx);

          if (third_idx != -1) {
            return array<int, 4>{first_idx, second_idx, third_idx, fourth_idx};
          }

          // Restore counts.
          update_counts(first_idx, +1);
          update_counts(second_idx, +1);
        }
      }

      return {};
    };

    // Execute two-phase search strategy.
    auto vertex_degrees = calculate_vertex_degrees();

    // Phase 1: Try leaf nodes first (most efficient).
    if (auto result = process_leaf_nodes(vertex_degrees); result.has_value()) {
      const auto& [p, q, r, s] = result.value();
      cout << p << " " << q << " " << r << " " << s << "\n";
      return;
    }

    // Phase 2: General search with degree-based ordering.
    if (auto result = general_search(vertex_degrees); result.has_value()) {
      const auto& [p, q, r, s] = result.value();
      cout << p << " " << q << " " << r << " " << s << "\n";
      return;
    }

    cout << 0 << "\n";
  }
};

//===----------------------------------------------------------------------===//
/* Main function */

int main() {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  cin >> t;
  while (t--) {
    QuadrupleFinder solver;
    solver.solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//