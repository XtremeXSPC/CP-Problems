//===---------------------------------------------------------------------===//
/* competitive.hpp - Wrapper header for "Competitive Programming */
#pragma once

// Disable clangd parsing for system headers
#ifdef __clang__
// Provide minimal declarations for intellisense
#include <algorithm>
#include <climits>
#include <cmath>
#include <cstring>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

// Minimal pb_ds declarations for intellisense
namespace __gnu_pbds {
  template <typename Key, typename Mapped = void>
  class cc_hash_table {};

  template <typename T>
  class tree {};

  template <typename T>
  class rb_tree_tag {};

  template <typename T>
  class tree_order_statistics_node_update {};
} // namespace __gnu_pbds

#else
// Real includes when compiling with GCC
#pragma GCC system_header

// Suppress all warnings/errors in this block
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Weverything"

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#pragma GCC diagnostic pop
#endif

using namespace std;

#ifndef __clang__
using namespace __gnu_pbds;

// Useful typedefs for competitive programming
typedef long long          ll;
typedef unsigned long long ull;
typedef pair<int, int>     pii;
typedef pair<ll, ll>       pll;
typedef vector<int>        vi;
typedef vector<ll>         vll;
typedef vector<pii>        vpii;
typedef vector<pll>        vpll;
typedef vector<string>     vs;

// Policy-based data structures
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;
typedef tree<ll, null_type, less<ll>, rb_tree_tag, tree_order_statistics_node_update>   ordered_set_ll;

// Common macros
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) (int)(x).size()
#define pb push_back
#define mp make_pair
#define fi first
#define se second

// Fast I/O
#define fastio                                                                                                                             \
  ios_base::sync_with_stdio(false);                                                                                                        \
  cin.tie(NULL);                                                                                                                           \
  cout.tie(NULL)

#endif

//===---------------------------------------------------------------------===//