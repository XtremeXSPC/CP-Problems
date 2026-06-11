//===----------------------------------------------------------------------===//
/**
 * @file: problem_C_R1578D1_sub.cpp
 * @generated: 2026-06-11 20:46:48
 * @source: problem_C_R1578D1.cpp
 * @author: C.L.
 *
 * @brief: Codeforces Round XXX (Div. X) - Problem_C_R1578D1
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

template <class T, bool = std::is_integral_v<remove_cvref_t<T>> && !std::is_same_v<remove_cvref_t<T>, bool>>
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

template <class... Ts>
struct loop {
  using T = loop_t<Ts...>;

  template <class X>
  [[gnu::always_inline]] static constexpr T val(X x) noexcept {
    return x;
  }
};

template <class R>
[[gnu::always_inline]] constexpr I64 sz64(const R& x) noexcept(noexcept(std::ssize(x))) {
  return std::ssize(x);
}

template <class R>
[[gnu::always_inline]] constexpr I32 sz32(const R& x) noexcept(noexcept(std::ssize(x))) {
  return std::ssize(x);
}

} // namespace cp

//===----------------------------------------------------------------------===//
/* Advanced Macro System */

// Advanced FOR loop system:
#define CP_LOOP(...) cp::loop<__VA_ARGS__>
#define CP_VAL(x, ...) CP_LOOP(__VA_ARGS__)::val(x)

#define FOR1(a) \
  for (auto _ = CP_VAL(0, decltype(a)); _ < CP_VAL(a, decltype(a)); ++_)
#define FOR2(i, a) \
  for (auto i = CP_VAL(0, decltype(a)); i < CP_VAL(a, decltype(a)); ++i)
#define FOR3(i, a, b) \
  for (auto i = CP_VAL(a, decltype(a), decltype(b)); i < CP_VAL(b, decltype(a), decltype(b)); ++i)
#define FOR4(i, a, b, c)                                          \
  for (auto i = CP_VAL(a, decltype(a), decltype(b), decltype(c)); \
       i < CP_VAL(b, decltype(a), decltype(b), decltype(c));      \
       i += CP_VAL(c, decltype(a), decltype(b), decltype(c)))
#define FOR1_R(a) \
  for (auto _ = CP_VAL(a, decltype(a)); _-- > 0;)
#define FOR2_R(i, a) \
  for (auto i = CP_VAL(a, decltype(a)); i-- > 0;)
#define FOR3_R(i, a, b) \
  for (auto i = CP_VAL(b, decltype(a), decltype(b)); i-- > CP_VAL(a, decltype(a), decltype(b));)

// Overload resolution for FOR macros:
#define overload4(a, b, c, d, e, ...) e
#define overload3(a, b, c, d, ...) d
#define FOR(...) overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)(__VA_ARGS__)
#define FOR_R(...) overload3(__VA_ARGS__, FOR3_R, FOR2_R, FOR1_R)(__VA_ARGS__)
#define ROF(...) FOR_R(__VA_ARGS__)
#define FORD3(i, a, b) \
  for (auto i = CP_VAL(a, decltype(a), decltype(b)); i >= CP_VAL(b, decltype(a), decltype(b)); --i)
#define FORD4(i, a, b, c)                                         \
  for (auto i = CP_VAL(a, decltype(a), decltype(b), decltype(c)); \
       i >= CP_VAL(b, decltype(a), decltype(b), decltype(c));     \
       i -= CP_VAL(c, decltype(a), decltype(b), decltype(c)))
#define FORD(...) overload4(__VA_ARGS__, FORD4, FORD3)(__VA_ARGS__)

// Range-based iteration macros:
#define ALL(x) std::ranges::begin(x), std::ranges::end(x)
#define RALL(x) std::ranges::rbegin(x), std::ranges::rend(x)

// Container utility macros:
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) cp::sz64(x)
#define isz(x) cp::sz32(x)
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

constexpr I32 INF = 1 << 29;

enum Tag : I32 { BAD,
                 PATH,
                 PINT,
                 FD,
                 FS,
                 NIL };

struct Prof {
  Tag t = BAD;
  I32 a = 0, b = 0;
};

Prof bad() { return {BAD, 0, 0}; }
Prof path(I32 len) { return {PATH, len, 0}; }
Prof pint(I32 a, I32 b) { return {PINT, a, b}; }
Prof fd(I32 d) { return {FD, min(d, INF), 0}; }
Prof fs(I32 s) { return s == 0 ? fd(0) : Prof{FS, s, 0}; }
Prof nil() { return {NIL, 0, 0}; }

Prof shift(const Prof& p) {
  switch (p.t) {
  case PATH:
    return path(p.a + 1);
  case PINT:
    return fd(1);
  case FD:
    return fd(p.a + 1);
  case FS:
    return fs(p.a - 1);
  default:
    return bad();
  }
}

I32 free_top(const Prof& p) {
  switch (p.t) {
  case PATH:
    return INF;
  case FD:
    return p.a + 1;
  case PINT:
    return 1;
  default:
    return -1;
  }
}

struct Dec {
  I32 up   = -1;
  I32 side = 0;
  I32 len  = 0;
  bool both_path = false;
};

Prof merge2(const Prof& A, const Prof& B, Dec& dec) {
  if (A.t == BAD || B.t == BAD)
    return bad();
  if (A.t == PATH && B.t == PATH) {
    dec = {0, 0, 0, true};
    return pint(A.a, B.a);
  }

  I32 best = INF;
  Dec take;
  const Prof* p[2] = {&A, &B};

  FOR(up, 2) {
    const Prof& U = *p[up];
    const Prof& S = *p[1 - up];
    const I32 lim = free_top(U);
    if (lim < 0)
      continue;

    if (S.t == PATH) {
      if (S.a <= lim && 0 < best) {
        best = 0;
        take = {up, 0, 0, false};
      }
      if (S.a - 1 < best) {
        best = S.a - 1;
        take = {up, 1, 0, false};
      }
    } else if (S.t == PINT) {
      if (S.a <= lim - 1 && S.b < best) {
        best = S.b;
        take = {up, 2, S.a, false};
      }
      if (S.b <= lim - 1 && S.a < best) {
        best = S.a;
        take = {up, 2, S.b, false};
      }
    }
  }

  if (take.up < 0)
    return bad();
  dec = take;
  return fs(best);
}

Prof merge_all(const Vec<Prof>& ps, Dec& dec) {
  Vec<Prof> q;
  for (const Prof& p : ps) {
    if (p.t != NIL)
      q.push_back(p);
  }

  if (q.empty())
    return path(1);
  if (isz(q) == 1)
    return shift(q[0]);
  if (isz(q) == 2)
    return merge2(q[0], q[1], dec);
  return bad();
}

bool top_one(const Prof& p) {
  return p.t == PATH || p.t == FD || p.t == PINT;
}

bool top_fit(const Prof& A, const Prof& B) {
  const bool hasA = A.t != NIL;
  const bool hasB = B.t != NIL;
  if (!hasA && !hasB)
    return true;
  if (!hasA)
    return top_one(B);
  if (!hasB)
    return top_one(A);
  if (A.t == PATH && B.t == PATH)
    return true;
  if (A.t == FD && B.t == PATH)
    return B.a <= A.a;
  if (B.t == FD && A.t == PATH)
    return A.a <= B.a;
  return false;
}

struct Solver {
  I32 n = 0, m = 0;
  Vec2D<I32> adj, cyc, att;
  VecI32 deg, par, dep, ord;
  VecI32 cid, pos, ext, entry;
  Vec<Prof> down, up;
  VecI32 X, Y;
  bool fail = false;

  Prof beyond_att(I32 v) {
    const I32 h = ext[v];
    return par[h] == v ? down[h] : up[v];
  }

  Prof beyond(I32 u, I32 v) {
    return par[v] == u ? down[v] : up[u];
  }

  void build() {
    par.assign(n + 1, 0);
    dep.assign(n + 1, -1);
    cid.assign(n + 1, -1);
    pos.assign(n + 1, 0);
    ord.clear();
    ord.reserve(n);

    VecI32 it(n + 1, 0), st{1};
    dep[1] = 0;

    while (!st.empty()) {
      const I32 v = st.back();
      if (it[v] == 0)
        ord.push_back(v);

      if (it[v] == isz(adj[v])) {
        st.pop_back();
        continue;
      }

      const I32 to = adj[v][it[v]++];
      if (to == par[v])
        continue;

      if (dep[to] == -1) {
        dep[to] = dep[v] + 1;
        par[to] = v;
        st.push_back(to);
      } else if (dep[to] < dep[v]) {
        VecI32 vs;
        for (I32 x = v; x != to; x = par[x])
          vs.push_back(x);
        vs.push_back(to);
        reverse(all(vs));

        const I32 id = isz(cyc);
        if (isz(vs) % 2)
          fail = true;
        FOR(i, isz(vs)) {
          if (cid[vs[i]] != -1)
            fail = true;
          cid[vs[i]] = id;
          pos[vs[i]] = i;
        }
        cyc.push_back(std::move(vs));
        entry.push_back(to);
      }
    }

    ext.assign(n + 1, 0);
    att.assign(isz(cyc), {});
    FOR(v, 1, n + 1) {
      if (cid[v] < 0)
        continue;

      const VecI32& vs = cyc[cid[v]];
      const I32 L      = isz(vs);
      const I32 p      = pos[v];
      const I32 prev   = vs[(p - 1 + L) % L];
      const I32 next   = vs[(p + 1) % L];

      for (const I32 to : adj[v]) {
        if (to != prev && to != next)
          ext[v] = to;
      }
      if (ext[v])
        att[cid[v]].push_back(v);
    }
  }

  bool phi_fit(I32 c, I32 root, I32 dir, I32& s) {
    const VecI32& vs = cyc[c];
    const I32 L      = isz(vs);
    const I32 k      = L / 2;
    const I32 p      = pos[root];
    const I32 low    = ((p + dir) % L + L) % L;
    const I32 top1   = ((p + dir * k) % L + L) % L;
    const I32 top2   = ((p + dir * (k + 1)) % L + L) % L;

    Prof A = nil(), B = nil();
    s = 0;

    for (const I32 v : att[c]) {
      if (v == root)
        continue;
      const I32 q     = pos[v];
      const Prof prof = beyond_att(v);
      if (prof.t == BAD)
        return false;

      if (q == low) {
        if (prof.t != PATH)
          return false;
        s = prof.a;
      } else if (q == top1) {
        A = prof;
      } else if (q == top2) {
        B = prof;
      } else {
        return false;
      }
    }

    return top_fit(A, B);
  }

  Prof best_phi(I32 c, I32 root, I32* out = nullptr) {
    if (isz(att[c]) > 4)
      return bad();

    I32 best = INF, take = 0;
    FOR(d, 2) {
      const I32 dir = (d == 0 ? 1 : -1);
      I32 s         = 0;
      if (phi_fit(c, root, dir, s) && s < best) {
        best = s;
        take = dir;
      }
    }

    if (take == 0)
      return bad();
    if (out)
      *out = take;
    return fs(best);
  }

  void calc() {
    down.assign(n + 1, bad());
    up.assign(n + 1, nil());
    Dec dec;

    for (I32 i = n - 1; i >= 0; --i) {
      const I32 v = ord[i];
      if (cid[v] >= 0) {
        if (entry[cid[v]] == v)
          down[v] = best_phi(cid[v], v);
        continue;
      }

      Vec<Prof> ps;
      for (const I32 to : adj[v]) {
        if (to != par[v])
          ps.push_back(down[to]);
      }
      down[v] = merge_all(ps, dec);
    }

    FOR(i, 1, n) {
      const I32 v = ord[i];
      const I32 p = par[v];

      if (cid[v] >= 0 && cid[v] == (p ? cid[p] : -2)) {
        up[v] = bad();
        continue;
      }
      if (cid[p] >= 0) {
        up[v] = best_phi(cid[p], p);
        continue;
      }

      Vec<Prof> ps;
      for (const I32 to : adj[p]) {
        if (to != v && to != par[p])
          ps.push_back(down[to]);
      }
      if (p != ord[0])
        ps.push_back(up[p]);
      up[v] = merge_all(ps, dec);
    }
  }

  struct Job {
    I32 u, from, col, row, dir;
  };

  Vec<Job> jobs;

  void push(I32 u, I32 from, I32 col, I32 row, I32 dir) {
    jobs.push_back({u, from, col, row, dir});
  }

  void place_cycle(I32 c, I32 p, I32 d, I32 col, I32 row, I32 dir) {
    const VecI32& vs = cyc[c];
    const I32 L = isz(vs);
    const I32 k = L / 2;

    FOR(t, L) {
      const I32 v = vs[((p + d * t) % L + L) % L];
      if (t == 0) {
        X[v] = col;
        Y[v] = row;
      } else if (t <= k) {
        X[v] = 1 - col;
        Y[v] = row + (t - 1) * dir;
      } else {
        X[v] = col;
        Y[v] = row + (2 * k - t) * dir;
      }
    }
  }

  void expand_cycle(I32 root, I32 col, I32 row, I32 dir) {
    const I32 c = cid[root];
    const I32 p = pos[root];
    const VecI32& vs = cyc[c];
    const I32 L = isz(vs);
    const I32 k = L / 2;

    I32 best = INF, take = 0;
    FOR(d, 2) {
      const I32 od = (d == 0 ? 1 : -1);
      I32 s        = 0;
      if (phi_fit(c, root, od, s) && s < best) {
        best = s;
        take = od;
      }
    }

    place_cycle(c, p, take, col, row, dir);

    const I32 low  = ((p + take) % L + L) % L;
    const I32 top1 = ((p + take * k) % L + L) % L;
    const I32 top2 = ((p + take * (k + 1)) % L + L) % L;
    const I32 a = vs[low], b = vs[top1], e = vs[top2];

    if (ext[a] && a != root)
      push(ext[a], a, 1 - col, row - dir, -dir);
    if (ext[b])
      push(ext[b], b, 1 - col, row + k * dir, dir);
    if (ext[e])
      push(ext[e], e, col, row + k * dir, dir);
  }

  void expand_tree(I32 u, I32 from, I32 col, I32 row, I32 dir) {
    X[u] = col;
    Y[u] = row;

    VecI32 ch;
    Vec<Prof> ps;
    for (const I32 v : adj[u]) {
      if (v == from)
        continue;
      ch.push_back(v);
      ps.push_back(beyond(u, v));
    }

    if (ch.empty())
      return;
    if (isz(ch) == 1) {
      push(ch[0], u, col, row + dir, dir);
      return;
    }

    Dec dec;
    merge2(ps[0], ps[1], dec);

    if (dec.both_path) {
      push(ch[0], u, col, row + dir, dir);
      push(ch[1], u, 1 - col, row, dir);
      return;
    }

    const I32 U = ch[dec.up];
    const I32 S = ch[1 - dec.up];
    push(U, u, col, row + dir, dir);

    if (dec.side == 0) {
      push(S, u, 1 - col, row, dir);
    } else if (dec.side == 1) {
      push(S, u, 1 - col, row, -dir);
    } else {
      X[S] = 1 - col;
      Y[S] = row;

      I32 a = -1, b = -1;
      for (const I32 v : adj[S]) {
        if (v == u)
          continue;
        (a < 0 ? a : b) = v;
      }

      const I32 len        = beyond(S, a).a;
      const I32 up_child   = (len == dec.len ? a : b);
      const I32 down_child = (up_child == a ? b : a);
      push(up_child, S, 1 - col, row + dir, dir);
      push(down_child, S, 1 - col, row - dir, -dir);
    }
  }

  void run() {
    while (!jobs.empty()) {
      const Job j = jobs.back();
      jobs.pop_back();

      if (cid[j.u] >= 0)
        expand_cycle(j.u, j.col, j.row, j.dir);
      else
        expand_tree(j.u, j.from, j.col, j.row, j.dir);
    }
  }

  bool valid() {
    Set<PairI32> seen;
    FOR(v, 1, n + 1) {
      if (X[v] < 0 || X[v] > 1)
        return false;
      if (Y[v] < -200000 || Y[v] > 200000)
        return false;
      if (!seen.insert({X[v], Y[v]}).second)
        return false;
    }

    FOR(v, 1, n + 1) {
      for (const I32 to : adj[v]) {
        if (v < to && abs(X[v] - X[to]) + abs(Y[v] - Y[to]) != 1)
          return false;
      }
    }
    return true;
  }

  void reset() {
    jobs.clear();
    X.assign(n + 1, -1);
    Y.assign(n + 1, 0);
  }

  bool accept() {
    run();
    return valid();
  }

  bool solve_case() {
    X.assign(n + 1, 0);
    Y.assign(n + 1, 0);

    if (n == 1)
      return true;

    FOR(v, 1, n + 1) {
      if (deg[v] >= 4)
        fail = true;
    }
    if (fail)
      return false;

    build();
    if (fail)
      return false;

    calc();

    Dec dec;
    FOR(v, 1, n + 1) {
      if (cid[v] >= 0 || deg[v] > 2)
        continue;

      Vec<Prof> ps;
      for (const I32 to : adj[v])
        ps.push_back(beyond(v, to));
      if (merge_all(ps, dec).t == BAD)
        continue;

      reset();
      push(v, 0, 0, 0, 1);
      if (accept())
        return true;
    }

    FOR(c, isz(cyc)) {
      const VecI32& vs = cyc[c];
      const I32 L      = isz(vs);
      const I32 k      = L / 2;
      if (isz(att[c]) > 4)
        continue;

      FOR(i, L) {
        bool fits = true;
        Prof A = nil(), B = nil();

        for (const I32 v : att[c]) {
          const I32 d     = (pos[v] - i + L) % L;
          const Prof prof = beyond_att(v);
          if (prof.t == BAD) {
            fits = false;
            break;
          }

          if (d == 0 || d == 1) {
            if (prof.t != PATH) {
              fits = false;
              break;
            }
          } else if (d == k) {
            A = prof;
          } else if (d == k + 1) {
            B = prof;
          } else {
            fits = false;
            break;
          }
        }

        if (!fits || !top_fit(A, B))
          continue;

        reset();
        place_cycle(c, i, 1, 0, 0, 1);

        const I32 a = vs[i];
        const I32 b = vs[(i + 1) % L];
        const I32 x = vs[(i + k) % L];
        const I32 y = vs[(i + k + 1) % L];

        if (ext[a])
          push(ext[a], a, 0, -1, -1);
        if (ext[b])
          push(ext[b], b, 1, -1, -1);
        if (ext[x])
          push(ext[x], x, 1, k, 1);
        if (ext[y])
          push(ext[y], y, 0, k, 1);

        if (accept())
          return true;
      }
    }

    return false;
  }
};

void solve() {
  INT(n, m);

  Solver S;
  S.n = n;
  S.m = m;
  S.adj.assign(n + 1, {});
  S.deg.assign(n + 1, 0);

  FOR(m) {
    INT(u, v);
    S.adj[u].push_back(v);
    S.adj[v].push_back(u);
    ++S.deg[u];
    ++S.deg[v];
  }

  if (!S.solve_case()) {
    OUT("No");
    return;
  }

  OUT("Yes");
  FOR(i, 1, n + 1)
      cout << S.X[i] << ' ' << S.Y[i] << '\n';
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
