//===----------------------------------------------------------------------===//
/**
 * @file: problem_D.cpp
 * @brief Codeforces Round 1047 Div. 3 - Problem D
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

// Helper function to group indices by their occurrence values.
inline auto groupIndicesByValue(const V_i& occurrences) -> VV_i {
    const int arraySize = occurrences.size();
    VV_i indexGroups(arraySize + 1);
    
    for (int idx = 0; idx < arraySize; ++idx) {
        indexGroups[occurrences[idx]].push_back(idx);
    }
    
    return indexGroups;
}

// Validates if the configuration is feasible based on divisibility constraints.
inline auto isValidConfiguration(const VV_i& indexGroups) -> bool {
    for (size_t value = 1; value < indexGroups.size(); ++value) {
        if (!indexGroups[value].empty()) {
            // Check divisibility constraint.
            if (indexGroups[value].size() % value != 0) {
                return false;
            }
        }
    }
    return true;
}

// Constructs the result array based on grouped indices and occurrence values.
inline auto constructResultArray(const VV_i& indexGroups, int arraySize) -> V_i {
    V_i result(arraySize, 0);
    int currentLabel = 1;
    
    for (size_t occurrenceCount = 1; occurrenceCount < indexGroups.size(); ++occurrenceCount) {
        if (indexGroups[occurrenceCount].empty()) continue;
        
        const auto& indices = indexGroups[occurrenceCount];
        const int groupSize = static_cast<int>(indices.size());
        
        // Process chunks of size occurrenceCount.
        for (int chunkStart = 0; chunkStart < groupSize; chunkStart += occurrenceCount) {
            // Assign same label to chunk elements.
            for (int offset = 0; offset < static_cast<int>(occurrenceCount); ++offset) {
                result[indices[chunkStart + offset]] = currentLabel;
            }
            ++currentLabel;
        }
    }
    
    return result;
}

// Function to solve a single test case.
void solve() {
    int sequenceLength;
    cin >> sequenceLength;
    
    V_i occurrenceArray(sequenceLength);
    for (auto& elem : occurrenceArray) {
        cin >> elem;
    }
    
    // Group indices by their occurrence values.
    auto indexGrouping = groupIndicesByValue(occurrenceArray);
    
    // Validate feasibility.
    if (!isValidConfiguration(indexGrouping)) {
        cout << "-1\n";
        return;
    }
    
    // Construct and output the result.
    auto resultSequence = constructResultArray(indexGrouping, sequenceLength);
    
    // Output using modern formatting.
    for (size_t pos = 0; pos < resultSequence.size(); ++pos) {
        if (pos > 0) cout << ' ';
        cout << resultSequence[pos];
    }
    cout << '\n';
    
    debug(occurrenceArray, resultSequence);
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
