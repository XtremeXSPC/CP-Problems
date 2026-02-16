# Competitive Programming Template Library - Reference Guide

## Table of Contents

1. [Architecture Overview](#1-architecture-overview)
2. [Preamble.hpp - Compiler Setup and Portability](#2-preamblehpp---compiler-setup-and-portability)
3. [Types.hpp - The Type System](#3-typeshpp---the-type-system)
4. [Constants.hpp - Mathematical and Sentinel Constants](#4-constantshpp---mathematical-and-sentinel-constants)
5. [Macros.hpp - Loop System, Containers, and N-D Vectors](#5-macroshpp---loop-system-containers-and-n-d-vectors)
6. [Math.hpp - Mathematical Utilities](#6-mathhpp---mathematical-utilities)
7. [Bit_Ops.hpp - Bitwise Operations](#7-bit_opshpp---bitwise-operations)
8. [IO.hpp - Lightweight I/O](#8-iohpp---lightweight-io)
9. [Fast_IO.hpp - High-Performance Buffered I/O](#9-fast_iohpp---high-performance-buffered-io)
10. [Mod_Int.hpp - Modular Arithmetic](#10-mod_inthpp---modular-arithmetic)
11. [PBDS.hpp - Policy-Based Data Structures](#11-pbdshpp---policy-based-data-structures)
12. [Containers.hpp - Container Utilities and Algorithms](#12-containershpp---container-utilities-and-algorithms)
13. [Base.hpp - The Module Orchestrator](#13-basehpp---the-module-orchestrator)
14. [Starter Templates (cpp/)](#14-starter-templates-cpp)
15. [Quick Reference Cheat Sheet](#15-quick-reference-cheat-sheet)

---

## 1. Architecture Overview

The template library follows a **modular, opt-in architecture**. Rather than
including everything unconditionally (which would bloat compile times and pollute
the namespace), the system uses preprocessor feature flags to pull in only the
modules you need.

### Dependency Graph

```text
Preamble.hpp          (compiler pragmas, includes, debug stubs)
    |
    v
Types.hpp             (fundamental type aliases, container aliases)
    |
    +---> Constants.hpp   (infinity, MOD, PI, POW10)
    +---> Macros.hpp      (FOR loops, container macros, N-D vectors)
    +---> Math.hpp        (division, power, chmin/chmax, RNG)
    +---> Bit_Ops.hpp     (popcount, clz, ctz, bit iteration)
    +---> Fast_IO.hpp     (128KB buffered fread/fwrite I/O)
    +---> IO.hpp          (cin/cout wrappers, input macros)
    +---> Mod_Int.hpp     (ModInt<MOD> class)
    +---> PBDS.hpp        (ordered_set, ordered_multiset, gp_hash_table)
    +---> Containers.hpp  (binary search, argsort, cumsum, pop_val)
    |
    v
Base.hpp              (module orchestrator -- reads #define flags)
```

### Feature Flags

Define these **before** including `Base.hpp`:

| Flag              | Modules Activated                            |
| ----------------- | -------------------------------------------- |
| `NEED_CORE`       | Types, Constants, Macros, Math               |
| `NEED_IO`         | Types, Macros, IO (cin/cout-based)           |
| `NEED_FAST_IO`    | Types, Macros, Fast_IO + IO                  |
| `NEED_BIT_OPS`    | Types, Bit_Ops                               |
| `NEED_MOD_INT`    | Types, Constants, Mod_Int                    |
| `NEED_PBDS`       | Types, PBDS                                  |
| `NEED_CONTAINERS` | Types, Macros, Containers                    |
| `NEED_NDVEC`      | (within Macros) N-D vector factory functions |

Flags compose freely. A typical competitive programming solution needs:

```cpp
#define NEED_CORE
#define NEED_IO
#include "templates/Base.hpp"
```

---

## 2. Preamble.hpp - Compiler Setup and Portability

This header is the first thing included by every module chain. It performs three
critical tasks:

### 2.1 Compiler Optimizations

For GCC, it enables aggressive optimization pragmas:

```text
Ofast, unroll-loops, fast-math, O3, inline-functions
```

Architecture-specific SIMD targeting (AVX2, BMI, POPCNT on x86_64; NEON on
AArch64) is gated behind the `CP_ENABLE_ARCH_TARGET_PRAGMAS` define to maintain
judge portability. Enable it only when you control the target machine.

### 2.2 Portable Header Inclusion

The header detects whether `<bits/stdc++.h>` is available (GCC-specific). If it
is, it includes it. Otherwise, it falls back to an explicit list of ~35 standard
headers covering algorithms, containers, I/O, numerics, and ranges.

This means the templates work on both GCC (Codeforces, AtCoder) and Clang
(local development on macOS) without modification.

### 2.3 Debug Subsystem

When compiled with `-DLOCAL`, the preamble includes `debug.h` and enables:

- `debug(...)` -- print variables with labels
- `debug_if(...)` -- conditional debug output
- `debug_tree(...)` -- tree structure visualization
- `my_assert(...)` -- assertions that are stripped in production

Optional subsystems (`CP_DEBUG_ENABLE_PERF`, `CP_DEBUG_ENABLE_MEMORY`,
`CP_DEBUG_ENABLE_WATCH`, `CP_DEBUG_ENABLE_TREE`) provide performance counters,
memory tracking, variable watches, and tree visualizers respectively.

When `LOCAL` is not defined, all debug macros expand to no-ops with zero runtime
cost.

---

## 3. Types.hpp - The Type System

### 3.1 Fundamental Integer Types

The library replaces verbose standard names with terse, unambiguous aliases:

| Alias  | Underlying Type       | Bits | Range                          |
| ------ | --------------------- | ---- | ------------------------------ |
| `I8`   | `std::int8_t`         | 8    | -128 to 127                    |
| `I16`  | `std::int16_t`        | 16   | -32768 to 32767                |
| `I32`  | `std::int32_t`        | 32   | ~-2.1e9 to ~2.1e9              |
| `I64`  | `std::int64_t`        | 64   | ~-9.2e18 to ~9.2e18            |
| `U8`   | `std::uint8_t`        | 8    | 0 to 255                       |
| `U16`  | `std::uint16_t`       | 16   | 0 to 65535                     |
| `U32`  | `std::uint32_t`       | 32   | 0 to ~4.3e9                    |
| `U64`  | `std::uint64_t`       | 64   | 0 to ~1.8e19                   |
| `I128` | `__int128` (if avail) | 128  | Extremely large range          |
| `U128` | `unsigned __int128`   | 128  | Extremely large unsigned range |

### 3.2 Floating-Point Types

| Alias  | Underlying Type | Precision  |
| ------ | --------------- | ---------- |
| `F32`  | `float`         | ~7 digits  |
| `F64`  | `double`        | ~15 digits |
| `F80`  | `long double`   | ~18 digits |
| `F128` | `__float128`    | ~33 digits |

### 3.3 Container Aliases

The most frequently used containers have short aliases:

```cpp
Vec<T>              // std::vector<T>
Vec2<T>             // Vec<Vec<T>>
Vec3<T>             // Vec<Vec<Vec<T>>>
Deque<T>            // std::deque<T>
Set<T>              // std::set<T>
Map<K, V>           // std::map<K, V>
UnorderedMap<K, V>  // std::unordered_map<K, V>
UnorderedSet<T>     // std::unordered_set<T>
Stack<T>            // std::stack<T, std::deque<T>>
Queue<T>            // std::queue<T, std::deque<T>>
PriorityQueue<T>    // max-heap
MinPriorityQueue<T> // min-heap (std::greater<T>)
```

### 3.4 Specialized Shorthand

For rapid prototyping:

```cpp
VI    // Vec<I32>       VLL   // Vec<I64>
VVI   // Vec<VI>        VVLL  // Vec<VLL>
VB    // Vec<bool>      VS    // Vec<string>
PII   // Pair<I32,I32>  PLL   // Pair<I64,I64>
VPII  // Vec<PII>       VPLL  // Vec<PLL>
```

### 3.5 Deprecation Policy

Legacy aliases (`ll`, `ull`, `ld`, `VC`, `VVC`, `VVVC`) are marked
`[[deprecated]]`. They still compile but produce warnings, guiding migration to
the canonical names.

---

## 4. Constants.hpp - Mathematical and Sentinel Constants

### 4.1 Mathematical Constants

```cpp
constexpr F80 PI   = 3.14159265358979323846...L;
constexpr F80 E    = 2.71828182845904523536...L;
constexpr F80 PHI  = 1.61803398874989484820...L;  // Golden ratio
constexpr F80 LN2  = 0.69314718055994530941...L;
constexpr F80 EPS  = 1e-9L;                       // Standard epsilon
constexpr F80 DEPS = 1e-12L;                      // Double epsilon
```

### 4.2 Infinity System

The template provides a **type-safe infinity system** via a constexpr variable
template:

```cpp
template <class T>
constexpr T infinity = std::numeric_limits<T>::max() / 4;
```

With specializations chosen to be safely addable without overflow:

| Type  | Value                     | Property                    |
| ----- | ------------------------- | --------------------------- |
| `I32` | 1,010,000,000             | `INF32 + INF32` < `INT_MAX` |
| `I64` | 2,020,000,000,000,000,000 | `INF64 + INF64` safe        |
| `U32` | 2,020,000,000             | Unsigned safe sentinel      |
| `U64` | 4,040,000,000,000,000,000 | Unsigned safe sentinel      |
| `F64` | 1e18                      | Floating-point infinity     |
| `F80` | 1e18L                     | Long double infinity        |

The named constants `INF32`, `INF64`, and `LINF` (legacy alias for `INF64`)
provide convenient access.

**Why these values?** In competitive programming, you frequently initialize
distances or costs to "infinity" and then compare or add them. If infinity is
`INT_MAX`, adding two infinities causes undefined behavior. These values are
chosen so that `infinity<T> + infinity<T>` remains representable.

### 4.3 Powers of Ten

```cpp
constexpr I64 POW10[] = {1, 10, 100, ..., 1000000000000000000};
```

A precomputed lookup table for 10^k (k = 0..18). Useful for digit extraction
and formatting without calling `pow()`.

### 4.4 Modular Arithmetic Constants

```cpp
constexpr I64 MOD  = 1'000'000'007;  // Most common prime modulus
constexpr I64 MOD2 = 998'244'353;    // NTT-friendly prime
constexpr I64 MOD3 = 1'000'000'009;  // Alternative prime modulus
```

---

## 5. Macros.hpp - Loop System, Containers, and N-D Vectors

### 5.1 The FOR Loop System

The `FOR` macro uses variadic macro overloading to provide a single, flexible
loop construct:

```cpp
FOR(10)           // repeat 10 times (anonymous iterator _)
FOR(i, n)         // for i in [0, n)
FOR(i, a, b)      // for i in [a, b)
FOR(i, a, b, c)   // for i in [a, b) with step c
```

Reverse variants:

```cpp
FOR_R(n)          // for i in [n-1, 0] (decreasing)
FOR_R(i, n)       // for i in [n-1, 0] (decreasing, named)
FOR_R(i, a, b)    // for i in [b-1, a] (decreasing)
```

**Implementation detail:** The overloading works by counting arguments with
`overload4(__VA_ARGS__, FOR4, FOR3, FOR2, FOR1)`. The fifth positional parameter
selects the correct variant based on argument count.

**All loop variables are `I64`**, preventing subtle truncation bugs when working
with large values.

### 5.2 Container Shorthand Macros

```cpp
all(x)      // x.begin(), x.end()        -- for STL algorithms
rall(x)     // x.rbegin(), x.rend()      -- reverse iteration
ALL(x)      // std::ranges version of all()
RALL(x)     // std::ranges version of rall()
sz(x)       // (I64)x.size()             -- avoids signed/unsigned warnings
len(x)      // alias for sz(x)
pb          // push_back
eb          // emplace_back
fi          // first   (pair member)
se          // second  (pair member)
mp          // make_pair
mt          // make_tuple
elif        // else if
```

### 5.3 Aggregate Operations

```cpp
UNIQUE(x)   // sort + deduplicate + shrink_to_fit
LB(c, x)    // index of lower_bound(x) in sorted container c
UB(c, x)    // index of upper_bound(x) in sorted container c
SUM(x)      // std::accumulate with 0LL initial value
MIN(x)      // minimum element value
MAX(x)      // maximum element value
```

**Example -- coordinate compression:**

```cpp
VLL coords = {100, 5, 42, 5, 100};
UNIQUE(coords);  // coords = {5, 42, 100}
I64 idx = LB(coords, 42);  // idx = 1
```

### 5.4 Multi-Dimensional Vector Factory (NEED_NDVEC)

When `NEED_NDVEC` is defined (within `NEED_CORE` or independently), you gain
template functions and macros for creating N-dimensional vectors:

```cpp
// Template functions:
auto v1 = make_nd_vec<I64>(n);              // 1D, uninitialized
auto v1 = make_nd_vec<I64>(n, 0);           // 1D, filled with 0
auto v2 = make_vec2<I64>(n, m);             // 2D
auto v2 = make_vec2<I64>(n, m, -1);         // 2D, filled with -1
auto v3 = make_vec3<I64>(n, m, k);          // 3D
auto v4 = make_vec4<I64>(a, b, c, d, 0);   // 4D, filled with 0

// Macro shortcuts:
vv(I64, dp, n, m, 0);      // Vec2<I64> dp(n, Vec<I64>(m, 0))
vvv(I64, dp, n, m, k, 0);  // 3D with fill value
vvvv(I64, dp, a, b, c, d, 0);  // 4D with fill value
```

These are indispensable for DP problems requiring multi-dimensional state
tables.

---

## 6. Math.hpp - Mathematical Utilities

### 6.1 Integer Division Variants

Standard C++ division truncates toward zero, which is often undesirable for
competitive programming (e.g., handling negative dividends in modular
arithmetic):

```cpp
div_floor(a, b)   // floor division (rounds toward -infinity)
div_ceil(a, b)    // ceiling division (rounds toward +infinity)
mod_floor(a, b)   // always-non-negative modulus
divmod(a, b)      // returns pair {quotient, remainder}
```

**Example:**

```cpp
div_floor(-7, 3)  // = -3  (not -2 as in C++ default)
div_ceil(7, 3)    // = 3   (not 2)
mod_floor(-7, 3)  // = 2   (not -1)
```

### 6.2 Exponentiation

```cpp
power(base, exp)            // base^exp without modulus (binary exponentiation)
mod_pow(base, exp, mod)     // base^exp % mod (uses __int128 to avoid overflow)
```

`mod_pow` uses `__int128` multiplication internally to handle the intermediate
product without overflow, even for 64-bit bases and moduli.

### 6.3 chmin / chmax

These are the competitive programmer's most beloved utility -- conditionally
update a value and report whether it changed:

```cpp
I64 ans = infinity<I64>;
if (chmin(ans, cost)) {
    // ans was updated; cost was strictly less
    best_path = current_path;
}

I64 mx = 0;
chmax(mx, a[i]);  // mx = max(mx, a[i])
```

Both accept an optional comparator:

```cpp
chmin(val, candidate, std::greater<>{});  // reverse comparison
```

### 6.4 Random Number Generation

A seeded `mt19937_64` RNG, seeded from the high-resolution clock:

```cpp
I64 x = rnd<I64>(1, 100);    // random integer in [1, 100]
F64 y = rnd<F64>(0.0, 1.0);  // random double in [0.0, 1.0]
```

Useful for randomized algorithms, stress testing, and hash seed generation.

### 6.5 Variadic min/max

```cpp
I64 result = _min(a, b, c, d);  // minimum of 4 values
I64 result = _max(a, b, c);     // maximum of 3 values
```

These use template recursion and accept any number of arguments of the same
type.

---

## 7. Bit_Ops.hpp - Bitwise Operations

### 7.1 Core Bit Functions

All functions are `constexpr`, `always_inline`, and handle both 32-bit and
64-bit types automatically via `if constexpr`:

```cpp
popcount(x)       // number of set bits
leading_zeros(x)  // number of leading zeros
trailing_zeros(x) // number of trailing zeros (= position of lowest set bit)
bit_width(x)      // position of highest set bit + 1 (= floor(log2(x)) + 1)
bit_floor(x)      // largest power of 2 <= x
bit_ceil(x)       // smallest power of 2 >= x
parity_sign(x)    // (-1)^popcount(x), useful for inclusion-exclusion
```

**Type safety:** All functions use `std::remove_cv_t` and
`std::make_unsigned_t` internally to handle signed types correctly, preventing
undefined behavior from `__builtin_clz` on negative values.

### 7.2 Bit Query Utilities

```cpp
kth_bit<T>(k)        // returns T(1) << k
has_kth_bit(x, k)    // tests if bit k is set: (x >> k) & 1
```

### 7.3 Bit Iteration Ranges

These enable range-based `for` loops over set bits and subsets:

**Iterating over set bits:**

```cpp
I64 mask = 0b10110;
for (I32 bit : bit_range<I64>(mask)) {
    // bit = 1, 2, 4 (positions of set bits, low to high)
}
```

Internally, each `operator++` performs `current &= current - 1` (Kernighan's
bit-clearing trick), making this O(popcount) rather than O(bit_width).

**Iterating over submasks:**

```cpp
I64 mask = 0b111;
for (I64 sub : subset_range<I64>(mask)) {
    // sub = 0b000, 0b001, 0b010, 0b011, 0b100, 0b101, 0b110, 0b111
    // (all 2^popcount(mask) subsets of mask)
}
```

This is the canonical O(3^n) bitmask DP pattern. The `subset_range` iterator
uses the formula `subset = (subset - 1) & original` to enumerate all subsets in
decreasing order.

### 7.4 Practical Applications

**Segment tree size:** `bit_ceil(n)` gives the next power of 2 for segment tree
sizing.

**Sparse table levels:** `bit_width(n)` gives the number of levels needed.

**Bitmask DP transitions:** `bit_range` and `subset_range` replace manual
bit-manipulation loops.

---

## 8. IO.hpp - Lightweight I/O

This module wraps `std::cin`/`std::cout` with convenience features while
remaining simple and portable.

### 8.1 Automatic Setup

An `IOSetup` global object automatically calls:

```cpp
std::ios_base::sync_with_stdio(false);
std::cin.tie(nullptr);
std::cout.tie(nullptr);
std::cout << std::fixed << std::setprecision(10);
```

This is done at static initialization time -- you never need to write these
lines in `main()`.

### 8.2 Variadic Read/Write

```cpp
IN(a, b, c);     // reads three variables from stdin
OUT(x, y, z);    // prints three values space-separated, followed by newline
```

These support fundamental types, `std::string`, `std::pair`, and `Vec<T>`.
Vectors are read element-by-element and printed space-separated.

### 8.3 Typed Input Macros

These declare and read in a single statement:

```cpp
INT(n, m);        // I32 n, m; IN(n, m);
LL(n);            // I64 n; IN(n);
ULL(x);           // U64 x; IN(x);
STR(s);           // string s; IN(s);
CHR(c);           // char c; IN(c);
DBL(x);           // F64 x; IN(x);
VEC(I64, a, n);   // Vec<I64> a(n); IN(a);
VV(I64, g, n, m); // Vec2<I64> g(n, Vec<I64>(m)); IN(g);
```

### 8.4 Answer Macros

```cpp
YES();            // prints "YES"
NO();             // prints "NO"
YES(condition);   // prints "YES" if condition is true, "NO" otherwise
Yes(condition);   // prints "Yes" / "No" (capitalized variant)
```

### 8.5 Fallback Compatibility

When `Fast_IO.hpp` is **not** included, the `IO.hpp` module creates a
`fast_io` namespace that delegates to `cp_io`, ensuring that code using
`fast_io::read()` or `IN()/OUT()` macros works regardless of which I/O backend
is active.

---

## 9. Fast_IO.hpp - High-Performance Buffered I/O

For problems with large input (>1M integers), `std::cin` becomes a bottleneck
even with `sync_with_stdio(false)`. This module provides a raw `fread`/`fwrite`
buffered I/O system.

### 9.1 Architecture

```text
stdin --> [128KB input buffer] --> read functions
                                      |
                              parse integers/strings
                                      |
write functions --> [128KB output buffer] --> stdout (flushed on exit)
```

Both buffers are `alignas(64)` for cache-line alignment. The `IOFlusher`
destructor guarantees output is flushed even if the program exits abnormally.

### 9.2 Activation

```cpp
#define NEED_FAST_IO
#include "templates/Base.hpp"
```

This defines `CP_FAST_IO_NAMESPACE_DEFINED`, preventing `IO.hpp` from creating
its own `fast_io` namespace. The `IN()`, `OUT()`, and `FLUSH()` macros
automatically route to the buffered implementation.

### 9.3 Integer Parsing

The `read_integer<T>` function:

1. Ensures at least 64 bytes are in the buffer (one `load_input()` call covers
   most integers).
2. Skips whitespace by scanning for characters `>= '-'`.
3. Handles negative signs for signed types via `if constexpr`.
4. Accumulates digits with `x = x * 10 + (c - '0')`.

This is significantly faster than `scanf`/`cin` because it avoids virtual
function calls, locale handling, and per-character synchronization.

### 9.4 Integer Writing

The `write_integer<T>` function handles negative numbers safely using unsigned
arithmetic (`-(x + 1) + 1` to avoid UB on `INT_MIN`), then writes digits in
reverse via a small stack buffer.

### 9.5 Performance Considerations

- Typical speedup: **3-5x** over `cin/cout` for large inputs.
- The 128KB buffer size is chosen to match typical L1 cache sizes.
- For interactive problems (where you need to flush after each query), call
  `FLUSH()` explicitly or use the standard `IO.hpp` module instead.

---

## 10. Mod_Int.hpp - Modular Arithmetic

### 10.1 The ModInt Class

`ModInt<MOD>` wraps an unsigned 64-bit value and overloads all arithmetic
operators to work modulo `MOD`:

```cpp
mint a = 5, b = 3;
mint c = a + b;      // 8
mint d = a * b;      // 15
mint e = a / b;      // 5 * inverse(3) mod 10^9+7
mint f = a.pow(100); // 5^100 mod 10^9+7
```

### 10.2 Type Aliases

```cpp
using mint  = ModInt<MOD>;    // mod 10^9 + 7
using mint2 = ModInt<MOD2>;   // mod 998244353
```

### 10.3 Operations

| Operation      | Method                  | Complexity |
| -------------- | ----------------------- | ---------- |
| Addition       | `a + b`, `a += b`       | O(1)       |
| Subtraction    | `a - b`, `a -= b`       | O(1)       |
| Multiplication | `a * b`, `a *= b`       | O(1)       |
| Division       | `a / b`, `a /= b`       | O(log MOD) |
| Power          | `a.pow(exp)`            | O(log exp) |
| Inverse        | `a.inverse()`           | O(log MOD) |
| Negation       | `-a`                    | O(1)       |
| Comparison     | `a == b`, `a != b`      | O(1)       |
| I/O            | `cin >> a`, `cout << a` | O(digits)  |

### 10.4 Inverse Computation

The modular inverse uses the **Extended Euclidean Algorithm**. It computes
`u` such that `value * u ≡ 1 (mod MOD)` by maintaining the invariant through
the GCD computation. If `gcd(value, MOD) != 1`, the inverse does not exist and
an assertion fires (in debug mode).

### 10.5 Overflow Safety

Multiplication uses `(U64)value * other.value % MOD`. For MOD values up to
~2^31, this fits in U64 without overflow. For larger moduli, you would need
`__int128` multiplication (which is used in `mod_pow` in Math.hpp).

### 10.6 Usage Patterns

**Combinatorics:**

```cpp
Vec<mint> fact(N + 1), inv_fact(N + 1);
fact[0] = 1;
FOR(i, 1, N + 1) fact[i] = fact[i - 1] * i;
inv_fact[N] = fact[N].inverse();
FOR_R(i, N) inv_fact[i] = inv_fact[i + 1] * (i + 1);

auto C = [&](I64 n, I64 r) -> mint {
    if (r < 0 || r > n) return 0;
    return fact[n] * inv_fact[r] * inv_fact[n - r];
};
```

---

## 11. PBDS.hpp - Policy-Based Data Structures

### 11.1 What Are PBDS?

GCC's Policy-Based Data Structures extend the STL with data structures not
available in the standard library. The most important for competitive
programming is the **order-statistics tree** -- a balanced BST that supports
O(log n) order-statistic queries.

### 11.2 Available Types

```cpp
ordered_set<T>        // set with order statistics
ordered_multiset<T>   // multiset with order statistics (uses less_equal)
ordered_map<K, V>     // map with order statistics
gp_hash_table<K, V>   // open-addressing hash table (faster than unordered_map)
```

### 11.3 Order-Statistics Operations

```cpp
ordered_set<I32> os;
os.insert(1); os.insert(3); os.insert(5); os.insert(7);

os.find_by_order(2);  // iterator to 3rd element (0-indexed) = 5
os.order_of_key(5);   // number of elements strictly less than 5 = 2
```

These two operations are what make `ordered_set` invaluable -- they are
equivalent to "find the k-th smallest element" and "count elements less than x",
both in O(log n).

### 11.4 The gp_hash_table

The `gp_hash_table` uses open addressing with linear probing, which is
significantly faster than `std::unordered_map` (which uses chaining) due to
better cache locality. Use it when hash map performance is critical.

### 11.5 Portability

PBDS require GCC's `<ext/pb_ds/*>` headers. The `Preamble.hpp` detects their
availability and defines `PBDS_AVAILABLE`. On compilers without PBDS support,
the module is safely skipped.

---

## 12. Containers.hpp - Container Utilities and Algorithms

### 12.1 Binary Search

The library provides two binary search variants that use a **predicate-based
interface** rather than searching for a specific value:

```cpp
// Integer binary search: finds the largest `left` where predicate(mid) is true
I64 ans = binary_search([&](I64 mid) {
    return check(mid);  // return true if mid is feasible
}, lo, hi);

// Real-valued binary search: 100 iterations of bisection
F64 ans = binary_search_real([&](F64 mid) {
    return check(mid);
}, lo, hi, 100);
```

The integer variant maintains the invariant that `predicate(left) = true` and
`predicate(right) = false`, converging when `left + 1 == right`.

### 12.2 argsort and rearrange

```cpp
Vec<I64> v = {30, 10, 20};
auto idx = argsort(v);           // idx = {1, 2, 0} (indices that sort v)
auto sorted = rearrange(v, idx); // sorted = {10, 20, 30}
```

`argsort` is stable (ties are broken by original index). The `reverse` parameter
produces a descending sort order.

### 12.3 Prefix Sums (cumsum)

```cpp
Vec<I64> a = {1, 2, 3, 4, 5};
auto ps = cumsum(a);           // ps = {0, 1, 3, 6, 10, 15}
auto ps2 = cumsum(a, false);   // ps2 = {1, 3, 6, 10, 15}
```

With `include_zero = true` (default), the result has length `n + 1` with
`ps[0] = 0`, which is the conventional form for range-sum queries:
`sum(a[l..r]) = ps[r+1] - ps[l]`.

### 12.4 Container Concatenation

```cpp
Vec<I64> a = {1, 2}, b = {3, 4}, c = {5};
concat(a, b, c);  // a = {1, 2, 3, 4, 5}
```

Uses fold expressions for variadic argument support.

### 12.5 Typed Accumulation

```cpp
Vec<mint> v = {1, 2, 3, 4, 5};
mint total = sum_as<mint>(v);  // sum under modular arithmetic
```

### 12.6 pop_val

A family of overloaded functions that extract and return the front/back/top
element from a container in a single call:

```cpp
auto x = pop_val(vec);         // pops from back of vector
auto y = pop_val(deq);         // pops from front of deque
auto z = pop_val(pq);          // pops from top of priority queue
auto w = pop_val(queue);       // pops from front of queue
```

All variants are move-aware and assert non-emptiness in debug mode.

### 12.7 String Conversion

```cpp
string s = "abcz?";
auto v = string_to_ints(s, 'a');  // v = {0, 1, 2, 25, -1}
```

Converts characters to integer offsets from a base character. The `'?'`
character is mapped to -1 as a sentinel.

---

## 13. Base.hpp - The Module Orchestrator

`Base.hpp` is the single entry point you include in your solution files. It
reads the `NEED_*` feature flags and includes the appropriate modules, handling
dependency resolution automatically.

**Dependency resolution example:** If you define `NEED_FAST_IO` without
`NEED_CORE`, `Base.hpp` still includes `Types.hpp` and `Macros.hpp` because
`Fast_IO.hpp` depends on them.

```cpp
#define NEED_CORE
#define NEED_FAST_IO
#define NEED_BIT_OPS
#include "templates/Base.hpp"
```

This gives you: Preamble + Types + Constants + Macros + Math + Fast_IO + IO +
Bit_Ops. Everything else is excluded.

---

## 14. Starter Templates (cpp/)

The `templates/cpp/` directory contains ready-to-use solution files:

### 14.1 base.cpp (Recommended Starting Point)

Minimal template using the modular header system:

```cpp
#define NEED_CORE
#define NEED_IO
#include "templates/Base.hpp"

void solve() {
    // Your solution here
}

auto main() -> int {
    INT(T);
    FOR(T) solve();
    return 0;
}
```

### 14.2 default.cpp (Standalone, Lightweight)

A self-contained file with inline type definitions and basic macros. Does not
use the modular header system. Suitable for copying directly to an online judge.

### 14.3 advanced.cpp (Full-Featured Standalone)

A self-contained monolithic file that includes everything: all types, constants,
macros, Fast_IO, Bit_Ops, Math utilities, ModInt, PBDS, and container
utilities. Use this when you need the full toolkit in a single file (e.g., for
online judges that do not support multiple files).

### 14.4 pbds.cpp (PBDS-Focused)

A standalone template with PBDS support, basic types, and standard macros. Use
this when the problem specifically requires order-statistics trees.

---

## 15. Quick Reference Cheat Sheet

### I/O

```cpp
INT(n, m);              // declare + read I32
LL(x);                  // declare + read I64
STR(s);                 // declare + read string
VEC(I64, a, n);         // declare + read vector
IN(a, b, c);            // read into existing variables
OUT(ans);               // print with newline
YES(cond);              // print YES/NO
```

### Loops

```cpp
FOR(n)                  // repeat n times
FOR(i, n)               // i in [0, n)
FOR(i, a, b)            // i in [a, b)
FOR(i, a, b, step)      // i in [a, b) with step
FOR_R(i, n)             // i in [n-1, 0]
FOR_R(i, a, b)          // i in [b-1, a]
```

### Containers

```cpp
sz(v)                   // size as I64
all(v) / rall(v)        // begin/end iterators
UNIQUE(v)               // sort + deduplicate
SUM(v) / MIN(v) / MAX(v)
LB(v, x) / UB(v, x)    // lower/upper bound index
```

### Math

```cpp
chmin(a, b) / chmax(a, b)      // conditional update
div_floor(a, b) / div_ceil(a, b)
mod_pow(base, exp, mod)
power(base, exp)
rnd<I64>(lo, hi)                // random integer
```

### Bits

```cpp
popcount(x)             // count set bits
trailing_zeros(x)       // lowest set bit position
bit_width(x)            // highest set bit + 1
bit_ceil(x)             // next power of 2
bit_range<I64>(mask)    // iterate set bits
subset_range<I64>(mask) // iterate all submasks
kth_bit<I64>(k)         // 1 << k
has_kth_bit(x, k)       // test bit k
```

### Modular Arithmetic

```cpp
mint a = 42;
a.pow(n);               // a^n mod MOD
a.inverse();            // modular inverse
mint b = a / mint(3);   // division via inverse
```

### Feature Flags Reference

```cpp
#define NEED_CORE       // Types + Constants + Macros + Math
#define NEED_IO         // Lightweight cin/cout I/O
#define NEED_FAST_IO    // Buffered fread/fwrite I/O
#define NEED_BIT_OPS    // Bitwise operations
#define NEED_MOD_INT    // ModInt class
#define NEED_PBDS       // Policy-based data structures
#define NEED_CONTAINERS // Container utilities
#define NEED_NDVEC      // N-dimensional vector macros
```

---
