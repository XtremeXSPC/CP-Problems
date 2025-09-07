//===----------------------------------------------------------------------===//
/**
 * @file: problem_E.cpp
 * @brief Codeforces Round 1047 Div. 3 - Problem E
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

// Helper functions for problem logic.
inline auto computeFrequencyDistribution(const V_i& elements) -> V_i {
    const int arraySize = static_cast<int>(elements.size());
    V_i frequencies(arraySize + 1, 0);
    
    for (const auto& value : elements) {
        if (value <= arraySize) {
            frequencies[value]++;
        }
    }
    
    return frequencies;
}

// Finds the MEX boundary value.
inline auto findMexBound(const V_i& frequencies) -> int {
    int mexValue = 0;
    const int maxValue = static_cast<int>(frequencies.size());
    
    while (mexValue < maxValue && frequencies[mexValue] > 0) {
        ++mexValue;
    }
    
    return mexValue;
}

// Calculates sum for single operation case.
inline auto calculateSingleOperationSum(const V_i& frequencies, int mexBound, int arraySize) -> ll {
    ll uniqueSum = 0;
    int uniqueCount = 0;
    
    for (int value = 0; value < mexBound; ++value) {
        if (frequencies[value] == 1) {
            ++uniqueCount;
            uniqueSum += value;
        }
    }
    
    return uniqueSum + static_cast<ll>(arraySize - uniqueCount) * mexBound;
}

// Finds the first duplicate value within MEX boundary.
inline auto findFirstDuplicate(const V_i& frequencies, int mexBound) -> int {
    for (int value = 0; value < mexBound; ++value) {
        if (frequencies[value] >= 2) {
            return value;
        }
    }
    return -1;
}

// Computes final sum based on duplicates and operations.
inline auto computeFinalSum(int firstDup, int mexBound, int arraySize, ll operations) -> ll {
    if (firstDup != -1) {
        // Case: Early duplicate found.
        ll triangularSum = static_cast<ll>(firstDup) * (firstDup - 1) / 2;
        ll remainingElements = arraySize - firstDup;
        
        if (operations % 2 == 0) {
            return triangularSum + remainingElements * firstDup;
        } else {
            return triangularSum + remainingElements * (firstDup + 1);
        }
    } else {
        // Case: No early duplicates.
        ll triangularSum = static_cast<ll>(mexBound) * (mexBound - 1) / 2;
        ll remainingElements = arraySize - mexBound;
        
        if (remainingElements == 0) {
            return triangularSum;
        } else if (remainingElements == 1) {
            return triangularSum + mexBound;
        } else {
            if (operations % 2 == 0) {
                return triangularSum + remainingElements * (mexBound + 1);
            } else {
                return triangularSum + remainingElements * mexBound;
            }
        }
    }
}

// Function to solve a single test case.
void solve() {
    int arraySize;
    ll operationCount;
    cin >> arraySize >> operationCount;
    
    V_i sequence(arraySize);
    for (auto& element : sequence) {
        cin >> element;
    }
    
    // Build frequency distribution.
    auto freqDist = computeFrequencyDistribution(sequence);
    
    // Find MEX boundary.
    int mexLimit = findMexBound(freqDist);
    
    // Handle single operation case.
    if (operationCount == 1) {
        ll result = calculateSingleOperationSum(freqDist, mexLimit, arraySize);
        cout << result << '\n';
        return;
    }
    
    // Find first duplicate for multi-operation case.
    int firstDuplicateValue = findFirstDuplicate(freqDist, mexLimit);
    
    // Compute and output final sum.
    ll finalSum = computeFinalSum(firstDuplicateValue, mexLimit, arraySize, operationCount);
    cout << finalSum << '\n';
    
    debug(sequence, freqDist, mexLimit, firstDuplicateValue, finalSum);
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
