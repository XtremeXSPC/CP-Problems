#ifndef CP_MODULES_SPLAY_TREE_MONOIDS_HPP
#define CP_MODULES_SPLAY_TREE_MONOIDS_HPP

#include "_Common.hpp"

//===--------------------- COMMON MONOIDS DEFINITIONS ---------------------===//

/**
 * @brief Range sum query with range addition.
 */
struct SumAdd {
  using Val  = I64;
  using Lazy = I64;
  static Val  e()                              { return 0; }
  static Lazy le()                             { return 0; }
  static Val  op(Val a, Val b)                 { return a + b; }
  static Val  mapping(Lazy f, Val x, I32 n)    { return x + f * n; }
  static Lazy composition(Lazy f, Lazy g)      { return f + g; }
};

/**
 * @brief Range minimum query with range addition.
 */
struct MinAdd {
  using Val  = I64;
  using Lazy = I64;
  static Val  e()                              { return infinity<I64>; }
  static Lazy le()                             { return 0; }
  static Val  op(Val a, Val b)                 { return std::min(a, b); }
  static Val  mapping(Lazy f, Val x, [[maybe_unused]] I32 n) { return x + f; }
  static Lazy composition(Lazy f, Lazy g)      { return f + g; }
};

/**
 * @brief Range maximum query with range addition.
 */
struct MaxAdd {
  using Val  = I64;
  using Lazy = I64;
  static Val  e()                              { return -infinity<I64>; }
  static Lazy le()                             { return 0; }
  static Val  op(Val a, Val b)                 { return std::max(a, b); }
  static Val  mapping(Lazy f, Val x, [[maybe_unused]] I32 n) { return x + f; }
  static Lazy composition(Lazy f, Lazy g)      { return f + g; }
};

/**
 * @brief Range sum query with range assignment.
 *
 * Uses std::optional<I64> as the lazy type; std::nullopt denotes no pending update.
 */
struct SumAssign {
  using Val  = I64;
  using Lazy = std::optional<I64>;
  static Val  e()                              { return 0; }
  static Lazy le()                             { return std::nullopt; }
  static Val  op(Val a, Val b)                 { return a + b; }
  static Val  mapping(Lazy f, Val x, I32 n)    { return f.has_value() ? *f * n : x; }
  static Lazy composition(Lazy f, Lazy g)      { return f.has_value() ? f : g; }
};

#endif
