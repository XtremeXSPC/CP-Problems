//===----------------------------------------------------------------------===//
/**
 * @file: problem_I_sub.cpp
 * @generated: 2026-06-10 14:56:47
 * @source: problem_I.cpp
 * @author: C.L.
 *
 * @brief: Codeforces Round 1100 (Div. 1) - Problem I
 */
//===----------------------------------------------------------------------===//
/* Included library and Compiler Optimizations */

#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define CP_IO_PROFILE_SIMPLE

//===----------------------------------------------------------------------===//
/* Compiler Pragmas */

#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC optimize("O3,unroll-loops,inline-functions")
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wunused-result"
  #if defined(__x86_64__) && !defined(__MINGW32__) && !defined(__MINGW64__)
    #pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt,sse4.2,fma")
  #endif
  #if defined(__aarch64__) && !defined(__MINGW32__) && !defined(__MINGW64__)
    #pragma GCC target("+simd,+crypto,+fp16")
  #endif
#endif

#ifdef __clang__
  #pragma clang optimize on
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-result"
#endif

//===----------------------------------------------------------------------===//
/* Standard Library Includes */

#include <bits/stdc++.h>

//===----------------------------------------------------------------------===//
/* Toolchain Feature Detection */

#ifndef HAS_INT128
  #ifdef __SIZEOF_INT128__
    #define HAS_INT128 1
  #else
    #define HAS_INT128 0
  #endif
#endif

#ifndef HAS_FLOAT128
  #ifdef __FLOAT128__
    #define HAS_FLOAT128 1
  #else
    #define HAS_FLOAT128 0
  #endif
#endif

#if defined(__has_include) && __has_include(<ext/pb_ds/assoc_container.hpp>) \
 && __has_include(<ext/pb_ds/tree_policy.hpp>)
  #ifndef PBDS_AVAILABLE
    #define PBDS_AVAILABLE 1
  #endif
#elif !defined(PBDS_AVAILABLE)
  #define PBDS_AVAILABLE 0
#endif

//===----------------------------------------------------------------------===//
/* Debug and Assertion Policy */

#ifdef LOCAL
  #include "debug.h"
#else
  #define debug(...)
  #define debug_if(...)
  #define debug_tree(...)
  #define debug_tree_verbose(...)
  #define debug_line()
  #define my_assert(cond) ((void)0)
  #define COUNT_CALLS(...)
#endif

#ifdef __clang__
  #pragma clang diagnostic pop
#endif

#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC diagnostic pop
#endif

//===----------------------------------------------------------------------===//
/* Scalar Type Aliases */

using I8  = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;
using U8  = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

#if HAS_INT128
__extension__ using I128 = __int128;
__extension__ using U128 = unsigned __int128;
#else
using I128 = std::int64_t;
using U128 = std::uint64_t;
#endif

using F32 = float;
using F64 = double;
using F80 = long double;

#if HAS_FLOAT128
using F128 = __float128;
#else
using F128 = long double;
#endif

template <typename T>
using Limits = std::numeric_limits<T>;

using Size = std::size_t;
using Diff = std::ptrdiff_t;
using Byte = std::byte;

//===----------------------------------------------------------------------===//
/* Extended Type Traits */

namespace cp {

template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

namespace detail {

template <class T>
inline constexpr bool is_extended_integral_v = false;

template <class T>
struct make_unsigned {
  using type = std::make_unsigned_t<T>;
};

template <>
struct make_unsigned<bool> {
  using type = U8;
};

#if HAS_INT128
template <>
inline constexpr bool is_extended_integral_v<I128> = true;
template <>
inline constexpr bool is_extended_integral_v<U128> = true;

template <>
struct make_unsigned<I128> {
  using type = U128;
};
template <>
struct make_unsigned<U128> {
  using type = U128;
};
#endif

} // namespace detail

template <class T>
using make_unsigned_t = typename detail::make_unsigned<remove_cvref_t<T>>::type;

namespace detail {

template <class T, bool = std::is_integral_v<remove_cvref_t<T>> &&
                          !std::is_same_v<remove_cvref_t<T>, bool>>
struct loop_arg {
  using type = remove_cvref_t<T>;
};

template <class T>
struct loop_arg<T, true> {
  using type = std::make_signed_t<remove_cvref_t<T>>;
};

} // namespace detail

template <class... Ts>
using loop_t = std::common_type_t<I32, typename detail::loop_arg<Ts>::type...>;

} // namespace cp

//===----------------------------------------------------------------------===//
/* Advanced Macro System */

// Advanced FOR loop system:
#define FOR1(a) for (cp::loop_t<decltype(a)> _ = 0, _end = (a); _ < _end; ++_)
#define FOR2(i, a) for (cp::loop_t<decltype(a)> i = 0, _end = (a); i < _end; ++i)
#define FOR3(i, a, b) for (cp::loop_t<decltype(a), decltype(b)> i = (a), _end = (b); i < _end; ++i)
#define FOR4(i, a, b, c) for (cp::loop_t<decltype(a), decltype(b), decltype(c)> i = (a), _end = (b), _step = (c); i < _end; i += _step)
#define FOR1_R(a) for (cp::loop_t<decltype(a)> _ = (a); _-- > 0;)
#define FOR2_R(i, a) for (cp::loop_t<decltype(a)> i = (a); i-- > 0;)
#define FOR3_R(i, a, b) for (cp::loop_t<decltype(a), decltype(b)> i = (b), _end = (a); i-- > _end;)

// Overload resolution for FOR macros:
#define overload4(a, b, c, d, e, ...) e
#define overload3(a, b, c, d, ...) d
#define FOR(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define FOR_R(...) overload3(__VA_ARGS__, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)
#define ROF(...) FOR_R(__VA_ARGS__)
#define FORD3(i, a, b) for (cp::loop_t<decltype(a), decltype(b)> i = (a), _end = (b); i >= _end; --i)
#define FORD4(i, a, b, c) for (cp::loop_t<decltype(a), decltype(b), decltype(c)> i = (a), _end = (b), _step = (c); i >= _end; i -= _step)
#define FORD(...) overload4(__VA_ARGS__, FORD4, FORD3)(__VA_ARGS__)

// Range-based iteration macros:
#define ALL(x) std::ranges::begin(x), std::ranges::end(x)
#define RALL(x) std::ranges::rbegin(x), std::ranges::rend(x)

// Container utility macros:
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) (I64)(x).size()
#define isz(x) (I32)(x).size()
#define len(x) sz(x)
#define eb emplace_back
#define elif else if

// Advanced container operations:
#define UNIQUE(x) (std::ranges::sort(x), x.erase(std::ranges::unique(x).begin(), x.end()), x.shrink_to_fit())
#define LB(c, x) (I64) std::distance((c).begin(), std::ranges::lower_bound(c, x))
#define UB(c, x) (I64) std::distance((c).begin(), std::ranges::upper_bound(c, x))
#define SUM(x) std::accumulate(all(x), std::iter_value_t<decltype((x).begin())>{})
#define MIN(x)                                       \
  ([&]() -> decltype(auto) {                         \
    auto&& _cp_min_range = (x);                      \
    if (std::ranges::empty(_cp_min_range)) {         \
      my_assert(false && "MIN(): empty range.");     \
      std::abort();                                  \
    }                                                \
    return *std::ranges::min_element(_cp_min_range); \
  }())
#define MAX(x)                                       \
  ([&]() -> decltype(auto) {                         \
    auto&& _cp_max_range = (x);                      \
    if (std::ranges::empty(_cp_max_range)) {         \
      my_assert(false && "MAX(): empty range.");     \
      std::abort();                                  \
    }                                                \
    return *std::ranges::max_element(_cp_max_range); \
  }())

// Y-combinator for recursive lambdas:
template <class F>
struct YCombinator {
  F fn;

  template <class... Args>
  decltype(auto) operator()(Args&&... args) const {
    return fn(*this, std::forward<Args>(args)...);
  }
};

template <class F>
YCombinator(F) -> YCombinator<F>;

template <class F>
[[gnu::always_inline]] constexpr auto fix(F&& fn) {
  return YCombinator<std::decay_t<F>>{std::forward<F>(fn)};
}

// Type-safe cast alias:
template <typename To>
[[gnu::always_inline]] constexpr To as(auto&& x) noexcept {
  return static_cast<To>(std::forward<decltype(x)>(x));
}

//===----------------------------------------------------------------------===//
/* Container and Utility Aliases */

using String     = std::string;
using StringView = std::string_view;

template <class T>
using Vec = std::vector<T>;
template <class T>
using Deque = std::deque<T>;
template <class T>
using List = std::list<T>;
template <class T, Size N>
using Array = std::array<T, N>;
template <Size N>
using BitSet = std::bitset<N>;

template <class T>
using Set = std::set<T>;
template <class T>
using MultiSet = std::multiset<T>;
template <class K, class V>
using Map = std::map<K, V>;
template <class K, class V>
using MultiMap = std::multimap<K, V>;
template <class T>
using UnorderedSet = std::unordered_set<T>;
template <class K, class V>
using UnorderedMap = std::unordered_map<K, V>;

template <class T, class Compare>
using OrderedSetBy = std::set<T, Compare>;
template <class T, class Compare>
using OrderedMultiSetBy = std::multiset<T, Compare>;
template <class K, class V, class Compare>
using OrderedMapBy = std::map<K, V, Compare>;
template <class K, class V, class Compare>
using OrderedMultiMapBy = std::multimap<K, V, Compare>;
template <class T, class Hash, class Eq = std::equal_to<T>>
using HashedSetBy = std::unordered_set<T, Hash, Eq>;
template <class K, class V, class Hash, class Eq = std::equal_to<K>>
using HashedMapBy = std::unordered_map<K, V, Hash, Eq>;

template <class T>
using Stack = std::stack<T, std::deque<T>>;
template <class T>
using Queue = std::queue<T, std::deque<T>>;
template <class T>
using PriorityQueue = std::priority_queue<T, std::vector<T>>;
template <class T>
using MinPriorityQueue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

template <class T, class Container>
using StackIn = std::stack<T, Container>;
template <class T, class Container>
using QueueIn = std::queue<T, Container>;
template <class T, class Container, class Compare>
using PriorityQueueBy = std::priority_queue<T, Container, Compare>;
template <class T, class Container = std::vector<T>>
using MinPriorityQueueIn = std::priority_queue<T, Container, std::greater<T>>;

template <class T, class U>
using Pair = std::pair<T, U>;
template <class... Args>
using Tuple = std::tuple<Args...>;
template <class T>
using Optional = std::optional<T>;
template <class... Ts>
using Variant = std::variant<Ts...>;
template <class Signature>
using Function = std::function<Signature>;
template <class T>
using Span = std::span<T>;

template <class T>
using Vec2D = Vec<Vec<T>>;
template <class T>
using Vec3D = Vec<Vec2D<T>>;
template <class T>
using Vec4D = Vec<Vec3D<T>>;

using VecU8   = Vec<U8>;
using VecU16  = Vec<U16>;
using VecU32  = Vec<U32>;
using VecU64  = Vec<U64>;
using VecI8   = Vec<I8>;
using VecI16  = Vec<I16>;
using VecI32  = Vec<I32>;
using VecI64  = Vec<I64>;
using VecF64  = Vec<F64>;
using VecF80  = Vec<F80>;
using VecBool = Vec<bool>;
using VecStr  = Vec<String>;

template <class T, class U>
using VecPair = Vec<Pair<T, U>>;

using PairI32    = Pair<I32, I32>;
using PairI64    = Pair<I64, I64>;
using PairF80    = Pair<F80, F80>;
using VecPairI32 = Vec<PairI32>;
using VecPairI64 = Vec<PairI64>;

//===----------------------------------------------------------------------===//
/* Lightweight Stopwatch Utility */

struct Stopwatch {
  using Clock = std::chrono::high_resolution_clock;
  Clock::time_point start;

  Stopwatch() : start(Clock::now()) {}
  void reset() { start = Clock::now(); }
  [[gnu::always_inline]] F64 elapsed() const { return std::chrono::duration<F64>(Clock::now() - start).count(); }
  [[gnu::always_inline]] bool within(F64 limit) const { return elapsed() < limit; }
};

//===----------------------------------------------------------------------===//
/* Lightweight I/O Utilities */

namespace cp_io {

inline void setup() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cout << std::fixed << std::setprecision(10);
}

struct IOSetup {
  IOSetup() { setup(); }
};

inline IOSetup io_setup;

template <class T>
void read(T& x) { std::cin >> x; }

template <class T>
void write(const T& x) { std::cout << x; }

#define CP_IO_COMPOSITE_CONTEXT 1
template <class T, class U>
inline void read(Pair<T, U>& p) {
  read(p.first);
  read(p.second);
}

template <class T>
inline void read(Vec<T>& v) {
  for (auto& x : v)
    read(x);
}

template <typename... Args>
inline void read(std::tuple<Args...>& t) {
  std::apply([](auto&... args) { (read(args), ...); }, t);
}

template <class T, class U>
inline void write(const Pair<T, U>& p) {
  write(p.first);
  write(' ');
  write(p.second);
}

template <class T>
inline void write(const Vec<T>& v) {
  for (I64 i = 0; i < sz(v); ++i) {
    if (i)
      write(' ');
    write(v[i]);
  }
}

template <typename... Args>
inline void write(const std::tuple<Args...>& t) {
  I32 i = 0;
  std::apply([&i](const auto&... args) { ((i++ > 0 ? (write(' '), 0) : 0, write(args)), ...); }, t);
}
#undef CP_IO_COMPOSITE_CONTEXT

template <class Head, class... Tail>
  requires(sizeof...(Tail) > 0)
inline void read(Head& head, Tail&... tail) {
  read(head);
  read(tail...);
}

template <class Head, class... Tail>
  requires(sizeof...(Tail) > 0)
inline void write(const Head& head, const Tail&... tail) {
  write(head);
  write(' ');
  write(tail...);
}

inline void writeln() { write('\n'); }

template <class... Args>
inline void writeln(const Args&... args) {
  if constexpr (sizeof...(args) > 0)
    write(args...);
  writeln();
}

} // namespace cp_io

namespace fast_io {
#define CP_IO_COMPAT_FAST_IO_NAMESPACE_DEFINED 1

template <class T>
inline void read_integer(T& x) { cp_io::read(x); }
inline void read_char(char& x) { cp_io::read(x); }
inline void read_string(std::string& x) { cp_io::read(x); }

template <class T>
inline void write_integer(T x) { cp_io::write(x); }
inline void write_char(char c) { std::cout.put(c); }
inline void write_string(const std::string& s) { cp_io::write(s); }
inline void flush_output() { std::cout.flush(); }

using cp_io::read;
using cp_io::write;
using cp_io::writeln;
} // namespace fast_io

#define CP_IO_IMPL_READ(...) cp_io::read(__VA_ARGS__)
#define CP_IO_IMPL_WRITELN(...) cp_io::writeln(__VA_ARGS__)
#define CP_IO_IMPL_FLUSH() std::cout.flush()

//===----------------------------------------------------------------------===//
/* Shared I/O Macro and Answer Helper Definitions */

#define IN(...) CP_IO_IMPL_READ(__VA_ARGS__)
#define OUT(...) CP_IO_IMPL_WRITELN(__VA_ARGS__)
#define FLUSH() CP_IO_IMPL_FLUSH()

#define CP_IO_DECL_MACROS_DEFINED 1
#define INT(...)   \
  I32 __VA_ARGS__; \
  IN(__VA_ARGS__)
#define LL(...)    \
  I64 __VA_ARGS__; \
  IN(__VA_ARGS__)
#define ULL(...)   \
  U64 __VA_ARGS__; \
  IN(__VA_ARGS__)
#define STR(...)           \
  std::string __VA_ARGS__; \
  IN(__VA_ARGS__)
#define CHR(...)    \
  char __VA_ARGS__; \
  IN(__VA_ARGS__)
#define DBL(...)   \
  F64 __VA_ARGS__; \
  IN(__VA_ARGS__)

#define CP_IO_ANSWER_HELPERS_DEFINED 1
inline void YES(bool condition = true) {
  if (condition)
    OUT("YES");
  else
    OUT("NO");
}
inline void NO(bool condition = true) { YES(!condition); }
inline void Yes(bool condition = true) {
  if (condition)
    OUT("Yes");
  else
    OUT("No");
}
inline void No(bool condition = true) { Yes(!condition); }

//===----------------------------------------------------------------------===//
/* Main Solver Function */

using namespace std;

constexpr I64 NEG = Limits<I64>::min() / 4;

struct Tab {
  I32 lo = 0;
  VecI64 a;

  [[nodiscard]] I32 hi() const { return lo + a.size() - 1; }

  [[nodiscard]] I64 get(I32 c) const {
    if (c < lo || c > hi())
      return NEG;
    return a[c - lo];
  }
};

void solve() {
  INT(n, k);
  const I32 cap = k - 1;

  VecI64 w(n + 1);
  FOR(i, 1, n + 1)
  IN(w[i]);

  Vec2D<I32> adj(n + 1);
  FOR(n - 1) {
    INT(u, v);
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  auto conv = [&](const Tab& A, const Tab& B, I32 lo, I32 hi) -> Tab {
    Tab C{lo, VecI64(hi - lo + 1, NEG)};

    FOR(j, B.lo, B.hi() + 1) {
      const I64 bj = B.a[j - B.lo];
      if (bj == NEG)
        continue;

      const I32 from = max(lo, j + A.lo);
      const I32 to   = min(hi, j + A.hi());
      FOR(c, from, to + 1) {
        const I64 av = A.a[c - j - A.lo];
        if (av == NEG)
          continue;
        C.a[c - lo] = max(C.a[c - lo], av + bj);
      }
    }
    return C;
  };

  auto close = [&](const Tab& A, I64 val, I32 lo, I32 hi) -> Tab {
    Tab B{lo, VecI64(hi - lo + 1, NEG)};

    FOR(c, lo, hi + 1) {
      I64 best = (c == 0 ? 0 : NEG);
      best     = max(best, A.get(c));

      if (c >= 1) {
        const I64 prev = A.get(c - 1);
        if (prev != NEG)
          best = max(best, prev + val * c);
      }
      B.a[c - lo] = best;
    }
    return B;
  };

  VecI32 par(n + 1, 0), ord, sub(n + 1, 1);
  Vec2D<I32> ch(n + 1);
  ord.reserve(n);

  VecI32 st{1};
  par[1] = -1;
  while (!st.empty()) {
    const I32 u = st.back();
    st.pop_back();
    ord.push_back(u);

    for (const I32 v : adj[u]) {
      if (v == par[u])
        continue;
      par[v] = u;
      ch[u].push_back(v);
      st.push_back(v);
    }
  }
  for (I32 id = n - 1; id > 0; --id) {
    sub[par[ord[id]]] += sub[ord[id]];
  }

  auto h_lo = [&](I32 u) -> I32 { return max(0, cap - sub[u] + 1); };
  auto h_hi = [&](I32 u) -> I32 { return min(n - sub[u], cap); };

  Vec<Tab> down(n + 1), up(n + 1);
  for (I32 id = n - 1; id >= 0; --id) {
    const I32 u = ord[id];
    Tab cur{0, {0}};
    I32 got = 0;
    for (const I32 v : ch[u]) {
      got += sub[v];
      cur = conv(cur, down[v], 0, min(got, cap));
    }
    down[u] = close(cur, w[u], 0, min(sub[u], cap));
  }

  VecI64 ans(n + 1);
  up[1] = Tab{0, {0}};

  for (const I32 u : ord) {
    const VecI32& kids = ch[u];
    const I32 m = kids.size();
    Tab all{0, {0}};

    if (m > 0) {
      Vec<Tab> seg(4 * m);
      VecI32 mass(4 * m, 0);

      auto build = [&](auto&& self, I32 p, I32 l, I32 r) -> void {
        if (l == r) {
          seg[p]  = down[kids[l]];
          mass[p] = sub[kids[l]];
          return;
        }
        const I32 mid = (l + r) / 2;
        self(self, 2 * p, l, mid);
        self(self, 2 * p + 1, mid + 1, r);
        mass[p] = mass[2 * p] + mass[2 * p + 1];
        seg[p]  = conv(seg[2 * p], seg[2 * p + 1], 0, min(mass[p], cap));
      };

      auto dfs = [&](auto&& self, I32 p, I32 l, I32 r, const Tab& out) -> void {
        if (l == r) {
          const I32 v = kids[l];
          up[v] = close(out, w[u], h_lo(v), h_hi(v));
          return;
        }
        const I32 mid = (l + r) / 2;
        self(self, 2 * p, l, mid,
             conv(out, seg[2 * p + 1], max(0, cap - mass[2 * p] - 1), cap));
        self(self, 2 * p + 1, mid + 1, r,
             conv(out, seg[2 * p], max(0, cap - mass[2 * p + 1] - 1), cap));
      };

      build(build, 1, 0, m - 1);

      Tab base{max(0, cap - mass[1] - 1), VecI64()};
      base.a.assign(cap - base.lo + 1, NEG);
      FOR(c, max(base.lo, up[u].lo), min(cap, up[u].hi()) + 1) {
        base.a[c - base.lo] = up[u].a[c - up[u].lo];
      }

      dfs(dfs, 1, 0, m - 1, base);
      all = std::move(seg[1]);
    }

    I64 best = NEG;
    FOR(j, max(all.lo, cap - up[u].hi()), min(all.hi(), cap - up[u].lo) + 1) {
      const I64 x = all.a[j - all.lo];
      const I64 y = up[u].a[cap - j - up[u].lo];
      if (x != NEG && y != NEG)
        best = max(best, x + y);
    }
    ans[u] = w[u] * k + best;
    up[u]  = Tab{};
  }

  FOR(i, 1, n + 1) {
    cout << ans[i] << " \n"[i == n];
  }
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Stopwatch timer;
#endif

  INT(T);
  FOR(T)
  solve();

  return 0;
}

//===----------------------------------------------------------------------===//
