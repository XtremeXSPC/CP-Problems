//===----------------------------------------------------------------------===//
/**
 * @file: problem_G.cpp
 * @brief Codeforces Round 1042 (Div. 3) - Problem G
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library */

#include <bits/stdc++.h>
using namespace std;

// Debug macro:
#ifdef LOCAL
#include "debug.h"
#else
#define debug(...) 42
#endif

// Type aliases
using ll     = long long;
using vi     = vector<int>;
using pii    = pair<int, int>;
using vll    = vector<ll>;
using vpii   = vector<pii>;
using map_ll = map<ll, ll>;

// Constants
constexpr ll  MOD  = 1e9 + 7;
constexpr int INF  = 1e9;
constexpr ll  LINF = 1e18;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

// Memoization table for the H function
map_ll memo_H;

/**
 * @brief Calculates (base^exp) % MOD using binary exponentiation.
 * @param base The base of the power.
 * @param exp The exponent.
 * @return The result of (base^exp) % MOD.
 */
ll power(ll base, ll exp) {
  ll res = 1;
  base %= MOD;
  while (exp > 0) {
    if (exp % 2 == 1)
      res = (res * base) % MOD;
    base = (base * base) % MOD;
    exp /= 2;
  }
  return res;
}

/**
 * @brief Calculates the product P(k).
 * @param k The input value.
 * @return The calculated product modulo MOD.
 */
ll get_P(ll k) {
  if (k == 0)
    return 1;
  ll res = 1;
  for (ll p = 0; p < 60; ++p) {
    ll term = 1LL << p;
    if (term > k)
      break;
    ll count = (k / term + 1) / 2;
    if (count == 0)
      continue;
    res = (res * power(p + 1, count)) % MOD;
  }
  return res;
}

/**
 * @brief Calculates the value H(g) using memoization.
 * @param g The input value.
 * @return The calculated H(g) modulo MOD.
 */
ll get_H(ll g) {
  if (g <= 1)
    return 1;
  if (memo_H.count(g))
    return memo_H[g];
  ll h_prev        = get_H(g - 1);
  ll res           = (g - 1) * h_prev % MOD * h_prev % MOD;
  return memo_H[g] = res;
}

// Function to solve a single test case
void solve() {
  int n;
  ll  k;
  cin >> n >> k;
  set<ll> s;
  for (int i = 0; i < n; ++i) {
    ll val;
    cin >> val;
    s.insert(val);
  }

  ll g = 1;
  while (s.count(g)) {
    g++;
  }

  vll events;
  events.push_back(g);
  for (ll val : s) {
    if (val >= g) {
      events.push_back(val);
    }
  }

  ll ans = 1;

  for (size_t i = 0; i < events.size(); ++i) {
    if (k == 0)
      break;
    ll current_g = events[i];

    if (current_g > 1) {
      ll ops_needed = -1;
      if (current_g - 1 < 31) {
        ops_needed = (1LL << (current_g - 1)) - 1;
      }

      if (ops_needed != -1 && k >= ops_needed) {
        k -= ops_needed;
        ans = (ans * get_H(current_g)) % MOD;
      } else {
        ans = (ans * get_P(k)) % MOD;
        k   = 0;
        break;
      }
    }

    if (k == 0)
      break;

    if (i + 1 < events.size()) {
      k--;
      ans = (ans * events[i + 1]) % MOD;
    } else {
      ans = (ans * get_P(k)) % MOD;
      k   = 0;
    }
  }

  cout << ans << "\n";
}

//===----------------------------------------------------------------------===//
/* Main function */

int main() {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  cin >> t;
  while (t--) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//