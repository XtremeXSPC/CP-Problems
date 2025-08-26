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

// Precomputed sieve data for prime factorization and Mobius function.
namespace MathUtilities {
  static constexpr int32_t MAX_VALUE = 1000005;

  struct SieveData {
    array<int32_t, MAX_VALUE> smallest_factor{};
    array<int8_t, MAX_VALUE>  mobius_values{};
    vector<int32_t>           prime_numbers;

    SieveData() {
      // Initialize smallest prime factor array
      iota(smallest_factor.begin(), smallest_factor.end(), 0);
      fill(mobius_values.begin(), mobius_values.end(), 0);
      mobius_values[1] = 1;

      prime_numbers.reserve(80000);

      for (int32_t current = 2; current < MAX_VALUE; ++current) {
        if (smallest_factor[current] == current) {
          prime_numbers.push_back(current);
          mobius_values[current] = -1;
        }

        for (const auto& prime : prime_numbers) {
          int64_t product = static_cast<int64_t>(prime) * current;
          if (product >= MAX_VALUE || prime > smallest_factor[current]) {
            break;
          }
          smallest_factor[product] = prime;
          mobius_values[product]   = (prime == smallest_factor[current]) ? 0 : -mobius_values[current];
        }
      }
    }

    vector<int32_t> extract_prime_factors(int32_t number) const {
      vector<int32_t> factors;
      while (number > 1) {
        int32_t prime = smallest_factor[number];
        factors.push_back(prime);
        while (number % prime == 0) {
          number /= prime;
        }
      }
      return factors;
    }

    vector<int32_t> generate_squarefree_divisors(const vector<int32_t>& primes) const {
      vector<int32_t> divisors{1};
      for (const auto& prime : primes) {
        size_t current_size = divisors.size();
        divisors.reserve(current_size * 2);
        for (size_t idx = 0; idx < current_size; ++idx) {
          divisors.push_back(divisors[idx] * prime);
        }
      }
      return divisors;
    }
  };

  inline const SieveData& get_sieve() {
    static const SieveData sieve_instance;
    return sieve_instance;
  }
} // namespace MathUtilities

// Main class to find coprime quadruples in the array.
class CoprimeQuadrupleSearcher {
private:
  struct ElementInfo {
    int32_t         value;
    vector<int32_t> positions;
    vector<int32_t> prime_factors;
    vector<int32_t> squarefree_divisors;
  };

  int32_t                        array_size;
  int32_t                        max_value;
  vector<int32_t>                elements;
  vector<int32_t>                frequency_count;
  vector<int32_t>                divisibility_count;
  unordered_map<int32_t, size_t> value_to_index;
  vector<ElementInfo>            unique_elements;

  void read_input_data() {
    cin >> array_size >> max_value;
    elements.resize(array_size + 1);
    frequency_count.assign(max_value + 1, 0);

    for (int32_t idx = 1; idx <= array_size; ++idx) {
      cin >> elements[idx];
      ++frequency_count[elements[idx]];
    }
  }

  // Precompute counts of elements divisible by each integer up to max_value.
  void compute_divisibility_counts() {
    divisibility_count.assign(max_value + 1, 0);
    for (int32_t divisor = 1; divisor <= max_value; ++divisor) {
      for (int32_t multiple = divisor; multiple <= max_value; multiple += divisor) {
        divisibility_count[divisor] += frequency_count[multiple];
      }
    }
  }

  // Build data for unique elements including prime factors and square-free divisors.
  void build_unique_element_data() {
    const auto& sieve = MathUtilities::get_sieve();
    value_to_index.reserve(array_size * 2);

    for (int32_t idx = 1; idx <= array_size; ++idx) {
      int32_t current_value = elements[idx];

      if (value_to_index.find(current_value) == value_to_index.end()) {
        size_t new_index              = unique_elements.size();
        value_to_index[current_value] = new_index;

        ElementInfo info;
        info.value = current_value;
        info.positions.push_back(idx);

        if (current_value == 1) {
          info.squarefree_divisors = {1};
        } else {
          info.prime_factors       = sieve.extract_prime_factors(current_value);
          info.squarefree_divisors = sieve.generate_squarefree_divisors(info.prime_factors);
        }

        unique_elements.push_back(std::move(info));
      } else {
        unique_elements[value_to_index[current_value]].positions.push_back(idx);
      }
    }
  }

  // Calculate count of elements coprime to the element at given index.
  int32_t calculate_coprime_count(size_t element_index, const vector<int32_t>& div_counts) const {
    const auto& sieve = MathUtilities::get_sieve();
    int32_t     total = 0;

    for (const auto& divisor : unique_elements[element_index].squarefree_divisors) {
      total += sieve.mobius_values[divisor] * div_counts[divisor];
    }

    return total;
  }

  // Update divisibility counts when adding or removing an element.
  void update_divisibility_for_element(int32_t position, int32_t delta) {
    size_t elem_index = value_to_index[elements[position]];
    for (const auto& divisor : unique_elements[elem_index].squarefree_divisors) {
      divisibility_count[divisor] += delta;
    }
  }

  // Find a coprime pair excluding two specified positions.
  pair<int32_t, int32_t> find_coprime_pair(int32_t exclude1, int32_t exclude2) {
    const auto& sieve = MathUtilities::get_sieve();

    for (int32_t first_idx = 1; first_idx <= array_size; ++first_idx) {
      if (first_idx == exclude1 || first_idx == exclude2)
        continue;

      size_t  elem_index    = value_to_index[elements[first_idx]];
      int32_t coprime_count = calculate_coprime_count(elem_index, divisibility_count);

      if (elements[first_idx] == 1) {
        coprime_count -= 1;
      }

      if (coprime_count >= 1) {
        for (int32_t second_idx = 1; second_idx <= array_size; ++second_idx) {
          if (second_idx == exclude1 || second_idx == exclude2 || second_idx == first_idx) {
            continue;
          }

          if (std::gcd(elements[first_idx], elements[second_idx]) == 1) {
            return {first_idx, second_idx};
          }
        }
      }
    }

    return {-1, -1};
  }

  // Output the found solution.
  void output_solution(int32_t p1, int32_t p2, int32_t p3, int32_t p4) { cout << p1 << " " << p2 << " " << p3 << " " << p4 << "\n"; }

  // Handle cases involving the value 1.
  bool handle_unit_values() {
    if (value_to_index.find(1) == value_to_index.end()) {
      return false;
    }

    const auto& unit_positions = unique_elements[value_to_index[1]].positions;
    size_t      unit_count     = unit_positions.size();

    if (unit_count >= 4) {
      output_solution(unit_positions[0], unit_positions[1], unit_positions[2], unit_positions[3]);
      return true;
    }

    if (unit_count == 3) {
      for (int32_t idx = 1; idx <= array_size; ++idx) {
        if (find(unit_positions.begin(), unit_positions.end(), idx) == unit_positions.end()) {
          output_solution(unit_positions[0], unit_positions[1], unit_positions[2], idx);
          return true;
        }
      }
      cout << "0\n";
      return true;
    }

    if (unit_count == 2) {
      vector<int32_t> non_units;
      for (int32_t idx = 1; idx <= array_size && non_units.size() < 2; ++idx) {
        if (find(unit_positions.begin(), unit_positions.end(), idx) == unit_positions.end()) {
          non_units.push_back(idx);
        }
      }

      if (non_units.size() >= 2) {
        output_solution(unit_positions[0], non_units[0], unit_positions[1], non_units[1]);
        return true;
      }

      cout << "0\n";
      return true;
    }

    return false;
  }

  // Find solution using elements with duplicate values.
  bool find_duplicate_value_solution() {
    for (size_t idx = 0; idx < unique_elements.size(); ++idx) {
      const auto& elem = unique_elements[idx];

      if (elem.value == 1 || elem.positions.size() < 2) {
        continue;
      }

      int32_t coprimes = calculate_coprime_count(idx, divisibility_count);

      if (coprimes >= 2) {
        vector<int32_t> partners;
        for (int32_t pos = 1; pos <= array_size && partners.size() < 2; ++pos) {
          if (std::gcd(elem.value, elements[pos]) == 1) {
            partners.push_back(pos);
          }
        }

        if (partners.size() >= 2) {
          output_solution(elem.positions[0], partners[0], elem.positions[1], partners[1]);
          return true;
        }
      }
    }

    return false;
  }

  // Handle case with exactly one occurrence of the value 1.
  bool handle_single_unit_case() {
    if (value_to_index.find(1) == value_to_index.end() || unique_elements[value_to_index[1]].positions.size() != 1) {
      return false;
    }

    int32_t unit_pos = unique_elements[value_to_index[1]].positions[0];

    for (size_t idx = 0; idx < unique_elements.size(); ++idx) {
      const auto& elem = unique_elements[idx];
      if (elem.value == 1)
        continue;

      int32_t adjusted_coprimes = calculate_coprime_count(idx, divisibility_count) - 1;

      if (adjusted_coprimes >= 1) {
        int32_t first_pos = elem.positions[0];

        for (int32_t partner_pos = 1; partner_pos <= array_size; ++partner_pos) {
          if (partner_pos == unit_pos || partner_pos == first_pos)
            continue;

          if (std::gcd(elem.value, elements[partner_pos]) == 1) {
            for (int32_t fourth_pos = 1; fourth_pos <= array_size; ++fourth_pos) {
              if (fourth_pos != unit_pos && fourth_pos != first_pos && fourth_pos != partner_pos) {
                output_solution(first_pos, partner_pos, unit_pos, fourth_pos);
                return true;
              }
            }
          }
        }
      }
    }

    cout << "0\n";
    return true;
  }

  // Search for solution using degree ordering heuristic.
  bool search_with_degree_ordering() {
    vector<int32_t> coprime_degrees(unique_elements.size());

    for (size_t idx = 0; idx < unique_elements.size(); ++idx) {
      int32_t degree = calculate_coprime_count(idx, divisibility_count);
      if (unique_elements[idx].value == 1) {
        degree -= 1;
      }
      coprime_degrees[idx] = degree;
    }

    // Try leaf vertices first
    for (size_t idx = 0; idx < unique_elements.size(); ++idx) {
      if (coprime_degrees[idx] == 1) {
        int32_t first_pos = unique_elements[idx].positions[0];

        for (int32_t second_pos = 1; second_pos <= array_size; ++second_pos) {
          if (second_pos == first_pos)
            continue;

          if (std::gcd(elements[first_pos], elements[second_pos]) == 1) {
            update_divisibility_for_element(first_pos, -1);
            update_divisibility_for_element(second_pos, -1);

            auto [third, fourth] = find_coprime_pair(first_pos, second_pos);

            if (third != -1) {
              output_solution(first_pos, second_pos, third, fourth);
              return true;
            }

            update_divisibility_for_element(first_pos, 1);
            update_divisibility_for_element(second_pos, 1);

            cout << "0\n";
            return true;
          }
        }
      }
    }

    // Build ordered list of all positions
    vector<int32_t> ordered_positions;
    ordered_positions.reserve(array_size);

    for (size_t idx = 0; idx < unique_elements.size(); ++idx) {
      if (coprime_degrees[idx] >= 1) {
        for (int32_t pos : unique_elements[idx].positions) {
          ordered_positions.push_back(pos);
        }
      }
    }

    // Sort positions by degree of their values.
    sort(ordered_positions.begin(), ordered_positions.end(), [this, &coprime_degrees](int32_t a, int32_t b) {
      size_t idx_a = value_to_index[elements[a]];
      size_t idx_b = value_to_index[elements[b]];

      if (coprime_degrees[idx_a] != coprime_degrees[idx_b]) {
        return coprime_degrees[idx_a] < coprime_degrees[idx_b];
      }
      return a < b;
    });

    // Try combinations with bounded search
    for (int32_t first_pos : ordered_positions) {
      vector<int32_t> candidates;
      candidates.reserve(30);

      for (int32_t second_pos = 1; second_pos <= array_size && candidates.size() < 30; ++second_pos) {
        if (second_pos != first_pos && std::gcd(elements[first_pos], elements[second_pos]) == 1) {
          candidates.push_back(second_pos);
        }
      }

      for (int32_t second_pos : candidates) {
        update_divisibility_for_element(first_pos, -1);
        update_divisibility_for_element(second_pos, -1);

        auto [third, fourth] = find_coprime_pair(first_pos, second_pos);

        if (third != -1) {
          output_solution(first_pos, second_pos, third, fourth);
          return true;
        }

        update_divisibility_for_element(first_pos, 1);
        update_divisibility_for_element(second_pos, 1);
      }
    }

    cout << "0\n";
    return true;
  }

public:
  void solve_test_case() {
    read_input_data();
    compute_divisibility_counts();
    build_unique_element_data();

    if (handle_unit_values())
      return;
    if (find_duplicate_value_solution())
      return;
    if (handle_single_unit_case())
      return;

    search_with_degree_ordering();
  }
};

//===----------------------------------------------------------------------===//
/* Main function */

int main() {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int32_t test;
  cin >> test;

  while (test--) {
    CoprimeQuadrupleSearcher searcher;
    searcher.solve_test_case();
  }

  return 0;
}

//===----------------------------------------------------------------------===//