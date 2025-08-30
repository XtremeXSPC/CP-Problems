#pragma once
#include "Types.hpp"

//===----------------------------------------------------------------------===//
/* Advanced Macro System */

// Multi-dimensional vector creation macros:
#define make_vec(type, name, ...) VC<type> name(__VA_ARGS__)
#define vv(type, name, h, ...) \
  VC<VC<type>> name(h, VC<type>(__VA_ARGS__))
#define vvv(type, name, h, w, ...) \
  VC<VC<VC<type>>> name(h, VC<VC<type>>(w, VC<type>(__VA_ARGS__)))
#define vvvv(type, name, a, b, c, ...) \
  VC<VC<VC<VC<type>>>> name(a, VC<VC<VC<type>>>(b, VC<VC<type>>(c, VC<type>(__VA_ARGS__))))

// Advanced FOR loop system:
#define FOR1(a) for (I64 _ = 0; _ < (a); ++_)
#define FOR2(i, a) for (I64 i = 0; i < (a); ++i)
#define FOR3(i, a, b) for (I64 i = (a); i < (b); ++i)
#define FOR4(i, a, b, c) for (I64 i = (a); i < (b); i += (c))
#define FOR1_R(a) for (I64 i = (a) - 1; i >= 0; --i)
#define FOR2_R(i, a) for (I64 i = (a) - 1; i >= 0; --i)
#define FOR3_R(i, a, b) for (I64 i = (b) - 1; i >= (a); --i)

#define overload4(a, b, c, d, e, ...) e
#define overload3(a, b, c, d, ...) d
#define FOR(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define FOR_R(...) overload3(__VA_ARGS__, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)

// Range-based iteration:
#define REP(i, n) for (I64 i : std::views::iota(0LL, (I64)(n)))
#define RREP(i, n) for (I64 i : std::views::iota(0LL, (I64)(n)) | std::views::reverse)
#define ALL(x) std::ranges::begin(x), std::ranges::end(x)
#define RALL(x) std::ranges::rbegin(x), std::ranges::rend(x)

// Container utility macros:
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) (I64)(x).size()
#define len(x) sz(x)
#define pb push_back
#define eb emplace_back
#define mp make_pair
#define mt make_tuple
#define fi first
#define se second
#define elif else if

// Advanced container operations:
#define UNIQUE(x) (std::ranges::sort(x), x.erase(std::ranges::unique(x).begin(), x.end()), x.shrink_to_fit())
#define LB(c, x) (I64)std::distance((c).begin(), std::ranges::lower_bound(c, x))
#define UB(c, x) (I64)std::distance((c).begin(), std::ranges::upper_bound(c, x))
#define SUM(x) std::accumulate(all(x), 0LL)
#define MIN(x) *std::ranges::min_element(x)
#define MAX(x) *std::ranges::max_element(x)