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
  #include "../Algorithms/PCH.h"
#else
  #include <bits/stdc++.h>
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

// Main solver class with value compression and Möbius optimization.
class QuadrupleFinder {
private:
  int num_elements, max_value;
  vi  array_values; // Original array (1-indexed)
  vi  frequency_counts;
  vi  mult_counts; // g[d] = count of active multiples of d

  // Value compression structures
  unordered_map<int, int> value_to_id;      // Maps value to compressed ID
  vi                      unique_vals;      // List of unique values
  vvi                     positions;        // positions[z] = list of indices with value unique_vals[z]
  vvi                     square_free_divs; // square_free_divs[z] = square-free divisors of unique_vals[z]

  // Get square-free divisors of a value.
  vi get_square_free_divisors(int value) {
    if (value == 1)
      return {1};

    vi  prime_factors;
    int temp = value;
    while (temp > 1) {
      int p = G_SIEVE.smallest_prime_div[temp];
      prime_factors.push_back(p);
      while (temp > 1 && temp % p == 0)
        temp /= p;
    }

    vi divs = {1};
    for (int p : prime_factors) {
      size_t current_size = divs.size();
      divs.reserve(current_size * 2);
      for (size_t i = 0; i < current_size; ++i) {
        divs.push_back(divs[i] * p);
      }
    }
    return divs;
  }

  // Count elements coprime with unique_vals[z] using inclusion-exclusion.
  int count_coprimes_for_value(int z) {
    int sum = 0;
    for (int d : square_free_divs[z]) {
      sum += G_SIEVE.mobius_fun_values[d] * mult_counts[d];
    }
    return sum;
  }

  // Update mult_counts when removing/adding an element at index idx.
  void update_mult_counts(int idx, int delta) {
    int z = value_to_id[array_values[idx]];
    for (int d : square_free_divs[z]) {
      mult_counts[d] += delta;
    }
  }

  // Find a coprime pair excluding two indices.
  pair<int, int> find_coprime_pair_excluding(int ban1, int ban2) {
    for (int r = 1; r <= num_elements; ++r) {
      if (r == ban1 || r == ban2)
        continue;

      int zr        = value_to_id[array_values[r]];
      int available = count_coprimes_for_value(zr);

      // Adjust for self if value is 1.
      if (unique_vals[zr] == 1)
        available--;

      // Only search if there are coprime partners available.
      if (available >= 1) {
        for (int s = 1; s <= num_elements; ++s) {
          if (s == ban1 || s == ban2 || s == r)
            continue;
          if (gcd(array_values[r], array_values[s]) == 1) {
            return {r, s};
          }
        }
      }
    }
    return {-1, -1};
  }

public:
  // Constructor reads input and initializes all structures.
  QuadrupleFinder() {
    cin >> num_elements >> max_value;
    array_values.resize(num_elements + 1);
    frequency_counts.assign(max_value + 1, 0);

    // Read input and count frequencies.
    for (int i = 1; i <= num_elements; ++i) {
      cin >> array_values[i];
      frequency_counts[array_values[i]]++;
    }

    // Build value compression.
    value_to_id.reserve(num_elements * 2);
    for (int i = 1; i <= num_elements; ++i) {
      int  x  = array_values[i];
      auto it = value_to_id.find(x);
      if (it == value_to_id.end()) {
        int z          = (int)unique_vals.size();
        value_to_id[x] = z;
        unique_vals.push_back(x);
        positions.push_back(vi());
        positions[z].push_back(i);
      } else {
        positions[it->second].push_back(i);
      }
    }

    // Precompute square-free divisors for each unique value.
    int U = (int)unique_vals.size();
    square_free_divs.resize(U);
    for (int z = 0; z < U; ++z) {
      square_free_divs[z] = get_square_free_divisors(unique_vals[z]);
    }

    // Initialize mult_counts: g[d] = count of multiples of d.
    mult_counts.assign(max_value + 1, 0);
    for (int d = 1; d <= max_value; ++d) {
      for (int k = d; k <= max_value; k += d) {
        mult_counts[d] += frequency_counts[k];
      }
    }
  }

  // Main solving function.
  void solve() {
    // Special handling for value 1.
    vi ones;
    if (value_to_id.count(1)) {
      int z = value_to_id[1];
      ones  = positions[z];
    }

    // Case: 4+ ones.
    if ((int)ones.size() >= 4) {
      cout << ones[0] << " " << ones[1] << " " << ones[2] << " " << ones[3] << "\n";
      return;
    }

    // Case: exactly 3 ones.
    if ((int)ones.size() == 3) {
      for (int i = 1; i <= num_elements; ++i) {
        if (array_values[i] != 1) {
          cout << ones[0] << " " << ones[1] << " " << ones[2] << " " << i << "\n";
          return;
        }
      }
      cout << 0 << "\n";
      return;
    }

    // Case: exactly 2 ones.
    if ((int)ones.size() == 2) {
      vi others;
      for (int i = 1; i <= num_elements && (int)others.size() < 2; ++i) {
        if (array_values[i] != 1)
          others.push_back(i);
      }
      if ((int)others.size() >= 2) {
        cout << ones[0] << " " << others[0] << " " << ones[1] << " " << others[1] << "\n";
        return;
      }
      cout << 0 << "\n";
      return;
    }

    // Process values with duplicates.
    int U = (int)unique_vals.size();
    for (int z = 0; z < U; ++z) {
      if (unique_vals[z] == 1)
        continue;

      if ((int)positions[z].size() >= 2) {
        int cop = count_coprimes_for_value(z);
        if (cop >= 2) {
          int v = unique_vals[z];
          vi  partners;
          for (int i = 1; i <= num_elements && (int)partners.size() < 2; ++i) {
            if (gcd(v, array_values[i]) == 1) {
              partners.push_back(i);
            }
          }
          if ((int)partners.size() >= 2) {
            cout << positions[z][0] << " " << partners[0] << " " << positions[z][1] << " " << partners[1] << "\n";
            return;
          }
        }
      }
    }

    // Case: exactly 1 one - special handling.
    if ((int)ones.size() == 1) {
      int one_idx = ones[0];

      for (int z = 0; z < U; ++z) {
        if (unique_vals[z] == 1)
          continue;

        int cop_no1 = count_coprimes_for_value(z) - 1; // Exclude the 1
        if (cop_no1 >= 1) {
          int i_idx = positions[z][0];

          // Find a coprime partner (not the 1).
          for (int t = 1; t <= num_elements; ++t) {
            if (t == one_idx || t == i_idx)
              continue;
            if (gcd(unique_vals[z], array_values[t]) == 1) {
              // Found coprime pair, now find any fourth element.
              for (int k = 1; k <= num_elements; ++k) {
                if (k != one_idx && k != i_idx && k != t) {
                  cout << i_idx << " " << t << " " << one_idx << " " << k << "\n";
                  return;
                }
              }
            }
          }
        }
      }
      cout << 0 << "\n";
      return;
    }

    // Compute degree for each unique value.
    vi deg_val(U, 0);
    for (int z = 0; z < U; ++z) {
      int fz = count_coprimes_for_value(z);
      if (unique_vals[z] == 1)
        fz -= 1;
      deg_val[z] = fz;
    }

    // Try leaf nodes first (degree = 1).
    for (int z = 0; z < U; ++z) {
      if (deg_val[z] == 1) {
        int leaf_i = positions[z][0];

        // Find the single coprime partner.
        for (int j = 1; j <= num_elements; ++j) {
          if (j == leaf_i)
            continue;
          if (gcd(array_values[leaf_i], array_values[j]) == 1) {
            // Found partner, remove both and find second pair.
            update_mult_counts(leaf_i, -1);
            update_mult_counts(j, -1);

            auto [r, s] = find_coprime_pair_excluding(leaf_i, j);

            if (r != -1) {
              cout << leaf_i << " " << j << " " << r << " " << s << "\n";
              return;
            }

            // Restore and continue.
            update_mult_counts(leaf_i, +1);
            update_mult_counts(j, +1);
            break;
          }
        }
      }
    }

    // General search with degree-based ordering.
    vi order_idx;
    order_idx.reserve(num_elements);
    for (int z = 0; z < U; ++z) {
      if (deg_val[z] >= 1) {
        for (int idx : positions[z])
          order_idx.push_back(idx);
      }
    }

    // Sort by degree of the value.
    sort(order_idx.begin(), order_idx.end(), [&](int i, int j) {
      int zi = value_to_id[array_values[i]];
      int zj = value_to_id[array_values[j]];
      if (deg_val[zi] != deg_val[zj])
        return deg_val[zi] < deg_val[zj];
      return i < j;
    });

    // Process vertices with limited candidates.
    const int MAX_CANDIDATES = 30;
    for (int ii : order_idx) {
      vi candidates;
      candidates.reserve(MAX_CANDIDATES);

      for (int t = 1; t <= num_elements && (int)candidates.size() < MAX_CANDIDATES; ++t) {
        if (t == ii)
          continue;
        if (gcd(array_values[ii], array_values[t]) == 1)
          candidates.push_back(t);
      }

      for (int jj : candidates) {
        // Remove this pair temporarily.
        update_mult_counts(ii, -1);
        update_mult_counts(jj, -1);

        auto [r, s] = find_coprime_pair_excluding(ii, jj);

        if (r != -1) {
          cout << ii << " " << jj << " " << r << " " << s << "\n";
          return;
        }

        // Restore counts.
        update_mult_counts(ii, +1);
        update_mult_counts(jj, +1);
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