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

using namespace std;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

// Initialize random number generator
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// Sampling constant K
const int K = 100;

// Helper function to remove two distinct indices from a vector in O(1) using swap-and-pop
void remove_two(vector<int>& V, int idx1, int idx2) {
  if (idx1 == idx2) return;
  int i1 = max(idx1, idx2);
  int i2 = min(idx1, idx2);
  
  // Remove element at i1 (larger index) first.
  // If it's not the last element, copy the last element to position i1.
  if (i1 < V.size() - 1) {
    V[i1] = V.back();
  }
  V.pop_back();
  
  // Remove element at i2 (smaller index).
  // i2 is still valid because i2 < i1.
  // If it's not the (new) last element, copy the (new) last element to position i2.
  if (i2 < V.size() - 1) {
    V[i2] = V.back();
  }
  V.pop_back();
}

void solve() {
  int n, m;
  // Read N and M.
  if (!(cin >> n >> m)) return;
  
  // Use 1-based indexing for convenience with required output.
  // Use int because M <= 10^6.
  vector<int> a(n + 1);
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
  }
  
  if (n < 4) {
    cout << "0\n";
    return;
  }
  
  // Vector of unused indices (1-based)
  vector<int> unused(n);
  iota(unused.begin(), unused.end(), 1); // 1, 2, ..., n
  vector<pair<int, int>> pairs;
  
  // Randomized Greedy Matching Algorithm
  while (unused.size() >= 2) {
    int N_unused = unused.size();
    
    // 1. Randomly choose an index i from unused
    // Create uniform distribution for range [0, N_unused - 1]
    uniform_int_distribution<int> dist(0, N_unused - 1);
    int idx_i = dist(rng);
    int i = unused[idx_i];
    
    // 2. Attempt to find a partner j
    bool found_partner = false;
    
    if (N_unused <= K + 1) {
      // Case A: Few elements remaining. Check all.
      for (int idx_j = 0; idx_j < N_unused; ++idx_j) {
        if (idx_i == idx_j) continue;
        int j = unused[idx_j];
        
        // std::gcd is available from C++17
        if (std::gcd(a[i], a[j]) == 1) {
          // Match found
          pairs.push_back({i, j});
          found_partner = true;
          
          // Remove i and j from unused
          remove_two(unused, idx_i, idx_j);
          
          // Check termination
          if (pairs.size() == 2) goto solution_found;
          break; // Stop searching for i
        }
      }
    } else {
      // Case B: Many elements remaining. Sample K times using rejection sampling.
      unordered_set<int> sampled_indices;
      for (int k = 0; k < K; ++k) {
        int idx_j;
        // Random sampling ensuring idx_j is distinct from idx_i and not already sampled
        do {
          idx_j = dist(rng);
        } while (idx_j == idx_i || sampled_indices.count(idx_j));
        sampled_indices.insert(idx_j);
        
        int j = unused[idx_j];
        if (std::gcd(a[i], a[j]) == 1) {
          // Match found
          pairs.push_back({i, j});
          found_partner = true;
          
          // Remove i and j from unused
          remove_two(unused, idx_i, idx_j);
          
          // Check termination
          if (pairs.size() == 2) goto solution_found;
          break; // Stop searching for i
        }
      }
    }
    
    // 3. If we didn't find a partner, remove only i
    if (!found_partner) {
      // idx_i is still valid because we haven't removed anything in this iteration if found_partner is false
      // Swap-and-pop to remove i
      if (idx_i < unused.size() - 1) {
        unused[idx_i] = unused.back();
      }
      unused.pop_back();
    }
  }
  
  // If loop terminates without finding 2 pairs
  cout << "0\n";
  return;

solution_found:
  cout << pairs[0].first << " " << pairs[0].second << " "
       << pairs[1].first << " " << pairs[1].second << "\n";
}

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;

  while (T--) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
/* End of Tester.cpp */