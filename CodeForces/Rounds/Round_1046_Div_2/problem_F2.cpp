//===----------------------------------------------------------------------===//
/**
 * @file: problem_F2.cpp
 * @brief Codeforces Round 1046 (Div. 2) - Problem F2
 * @author: Costantino Lombardi
 *
 * @status: In Progress
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

// Class to interact with the editor oracle.
class EditorOracle {
public:
  static constexpr int ARTICLE_SIZE = 100'000;
  static constexpr int QUERY_BUDGET = 12'500; // Half of total budget
  static constexpr int SEPARATOR    = 119;    // Strategic separator value

  // Query the editor with an article.
  [[nodiscard]] static int submitArticle(const vi& article) {
    cout << "? " << article.size();
    for (const auto& word : article) {
      cout << ' ' << word;
    }
    cout << '\n';
    cout.flush();

    int lines;
    cin >> lines;

    if (!cin || lines == -1) {
      exit(0);
    }

    return lines;
  }

  // Report the discovered width.
  static void reportWidth(int width) {
    cout << "! " << width << '\n';
    cout.flush();
  }

  // Find candidate widths based on quotient for small widths.
  [[nodiscard]] static int findSmallWidth(int quotient) {
    for (int w = 1; w < SEPARATOR; ++w) {
      if ((QUERY_BUDGET + w - 1) / w == quotient) {
        return w;
      }
    }
    return -1; // Should not happen.
  }

  // Find candidate widths for large widths based on lines.
  [[nodiscard]] static pii findLargeWidthRange(int lines) {
    int minWidth = ARTICLE_SIZE;
    int maxWidth = 0;

    for (int w = SEPARATOR; w <= ARTICLE_SIZE; ++w) {
      int words_per_line = w / SEPARATOR;
      if ((QUERY_BUDGET + words_per_line - 1) / words_per_line == lines) {
        minWidth = min(minWidth, w);
        maxWidth = max(maxWidth, w);
      }
    }

    return {minWidth, maxWidth};
  }

  // Build distinguishing query pattern.
  [[nodiscard]] static vi buildQuery(int left, int right) {
    vi query;
    query.reserve(2 * (right - left + 1));

    // Create pairs: (left, 1), (left, 2), ..., (left, right-left+1).
    for (int offset = 1; offset <= right - left + 1; ++offset) {
      query.push_back(left);
      query.push_back(offset);
    }

    return query;
  }

  // Calculate width from observed lines.
  [[nodiscard]] static int deduceWidth(int observedLines, int querySize, int left) {
    // Formula: width = querySize - observedLines + left.
    return querySize - observedLines + left;
  }
};

// Function to solve a single test case.
void solve() {
  // First query: probe with SEPARATOR-sized words to determine if W < SEPARATOR.
  const vi  probeArticle(EditorOracle::QUERY_BUDGET, EditorOracle::SEPARATOR);
  const int probeResult = EditorOracle::submitArticle(probeArticle);

  if (probeResult == 0) {
    // W < SEPARATOR: use small width strategy.
    const vi  uniformArticle(EditorOracle::QUERY_BUDGET, 1);
    const int quotient = EditorOracle::submitArticle(uniformArticle);

    const int width = EditorOracle::findSmallWidth(quotient);
    EditorOracle::reportWidth(width);
  } else {
    // W >= SEPARATOR: use large width strategy.
    auto [leftBound, rightBound] = EditorOracle::findLargeWidthRange(probeResult);

    // If unique width found, report it.
    if (leftBound == rightBound) {
      EditorOracle::reportWidth(leftBound);
      return;
    }

    // Second query: distinguishing pattern.
    const vi  distinguishingQuery = EditorOracle::buildQuery(leftBound, rightBound);
    const int observedLines       = EditorOracle::submitArticle(distinguishingQuery);

    // Deduce the exact width.
    const int width = EditorOracle::deduceWidth(observedLines, distinguishingQuery.size(), leftBound);
    EditorOracle::reportWidth(width);
  }
}

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  cin >> t;

  for ([[maybe_unused]] auto _ : views::iota(0, t)) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
