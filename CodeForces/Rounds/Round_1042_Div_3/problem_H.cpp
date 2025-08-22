//===----------------------------------------------------------------------===//
/**
 * @file: problem_H.cpp
 * @brief: Codeforces Round 1042 (Div. 3) - Problem H
 * @author: Costantino Lombardi
 *
 * @status: NOT PASSED - TLE on test 9
 */
//===----------------------------------------------------------------------===//
/* Included library and Macros */

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
        smallest_prime_div[(size_t)i * p] = p;
        mobius_fun_values[(size_t)i * p]  = (p == smallest_prime_div[i]) ? 0 : -mobius_fun_values[i];
      }
    }
  }
};

// Global, const instance.
const SieveCache G_SIEVE;

// Manages coprime counting using the Sieve data.
class CoprimeCounter {
private:
  vi                     count_of_multiples;
  unordered_map<int, vi> divisor_cache;

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

public:
  // Initializes counts of multiples for all integers up to max_val.
  explicit CoprimeCounter(int max_val, const vi& frequencies) : count_of_multiples(max_val + 1, 0) {
    for (int d = 1; d <= max_val; ++d) {
      for (int k = d; k <= max_val; k += d) {
        count_of_multiples[d] += frequencies[k];
      }
    }
  }

  // Returns the count of integers coprime with 'value'.
  int query_coprime_count(int value) {
    int total = 0;
    for (int d : get_square_free_divs(value)) {
      total += G_SIEVE.mobius_fun_values[d] * count_of_multiples[d];
    }
    return total;
  }

  // Updates counts of multiples when adding or removing an integer 'value'.
  void update_counts_for_value(int value, int delta) {
    for (int d : get_square_free_divs(value)) {
      count_of_multiples[d] += delta;
    }
  }
};

// Main solver class orchestrating the search strategies.
class QuadrupleFinder {
private:
  int                     num_elements, max_value;
  vi                      problem_values;
  vi                      value_frequencies;
  unordered_map<int, vi>  value_positions;
  unordered_map<int, int> value_to_id;

  // Helper for the final backtracking strategy, using CoprimeCounter
  pair<int, int> find_disjoint_coprime_pair(CoprimeCounter& counter, int ignore_idx1, int ignore_idx2) {
    for (int r_idx = 1; r_idx <= num_elements; ++r_idx) {
      if (r_idx == ignore_idx1 || r_idx == ignore_idx2)
        continue;

      int val_r = problem_values[r_idx];
      // A number is coprime to itself if it's 1. query_coprime_count includes this.
      int self_coprime_adj = (val_r == 1) ? 1 : 0;
      if (counter.query_coprime_count(val_r) > self_coprime_adj) {
        // A coprime partner exists, now find it.
        for (int s_idx = 1; s_idx <= num_elements; ++s_idx) {
          if (s_idx == ignore_idx1 || s_idx == ignore_idx2 || s_idx == r_idx)
            continue;
          if (gcd(val_r, problem_values[s_idx]) == 1) {
            return {r_idx, s_idx};
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
    int distinct_id_counter = 0;
    for (int i = 1; i <= num_elements; ++i) {
      cin >> problem_values[i];
      value_frequencies[problem_values[i]]++;
      value_positions[problem_values[i]].push_back(i);
      if (value_to_id.find(problem_values[i]) == value_to_id.end()) {
        value_to_id[problem_values[i]] = distinct_id_counter++;
      }
    }
  }

  // Main dispatcher that tries strategies in order of complexity.
  void solve() {
    // Strategy 1: Trivial cases using '1's
    if (value_positions.count(1)) {
      const auto& ones_indices = value_positions.at(1);
      if (ones_indices.size() >= 4) {
        cout << ones_indices[0] << " " << ones_indices[1] << " " << ones_indices[2] << " " << ones_indices[3] << "\n";
        return;
      }
      if (ones_indices.size() >= 2) {
        vi others;
        for (int i = 1; i <= num_elements && others.size() < 2; ++i) {
          if (problem_values[i] != 1)
            others.push_back(i);
        }
        if (others.size() >= 2) {
          cout << ones_indices[0] << " " << others[0] << " " << ones_indices[1] << " " << others[1] << "\n";
          return;
        }
      }
    }

    // Strategy 2: Simplify problem using duplicate values
    for (const auto& [value, positions] : value_positions) {
      if (value == 1 || positions.size() < 2)
        continue;
      vi partners;
      for (int i = 1; i <= num_elements && partners.size() < 2; ++i) {
        if (i == positions[0] || i == positions[1])
          continue;
        if (gcd(value, problem_values[i]) == 1)
          partners.push_back(i);
      }
      if (partners.size() >= 2) {
        cout << positions[0] << " " << partners[0] << " " << positions[1] << " " << partners[1] << "\n";
        return;
      }
    }

    // Strategy 3: Heuristic search based on coprime degree
    CoprimeCounter counter(max_value, value_frequencies);

    // Pre-calculate degrees for each unique value
    vector<int> degrees_by_val_id(value_to_id.size());
    for (auto const& [val, id] : value_to_id) {
      int self_coprime_adj  = (value_frequencies[val] > 1 && val != 1) ? (value_frequencies[val] - 1) : 0;
      degrees_by_val_id[id] = counter.query_coprime_count(val) - self_coprime_adj;
    }

    vi order_idx(num_elements);
    iota(order_idx.begin(), order_idx.end(), 1);

    // Sort indices based on the degree of their values (ascending)
    sort(order_idx.begin(), order_idx.end(), [&](int i, int j) {
      int id_i = value_to_id[problem_values[i]];
      int id_j = value_to_id[problem_values[j]];
      if (degrees_by_val_id[id_i] != degrees_by_val_id[id_j]) {
        return degrees_by_val_id[id_i] < degrees_by_val_id[id_j];
      }
      return i < j;
    });

    for (int p_idx : order_idx) {
      vi candidates;
      // Heuristic: check only a limited number of partners for p_idx
      const int partner_limit = 30;
      for (int q_idx = 1; q_idx <= num_elements && (int)candidates.size() < partner_limit; ++q_idx) {
        if (p_idx == q_idx)
          continue;
        if (gcd(problem_values[p_idx], problem_values[q_idx]) == 1) {
          candidates.push_back(q_idx);
        }
      }

      for (int q_idx : candidates) {
        // Temporarily remove p and q
        counter.update_counts_for_value(problem_values[p_idx], -1);
        counter.update_counts_for_value(problem_values[q_idx], -1);

        auto second_pair = find_disjoint_coprime_pair(counter, p_idx, q_idx);

        if (second_pair.first != -1) {
          cout << p_idx << " " << q_idx << " " << second_pair.first << " " << second_pair.second << "\n";
          return;
        }

        // Backtrack: add p and q back for the next iteration
        counter.update_counts_for_value(problem_values[p_idx], +1);
        counter.update_counts_for_value(problem_values[q_idx], +1);
      }
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