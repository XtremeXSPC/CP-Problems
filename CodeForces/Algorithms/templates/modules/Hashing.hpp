#pragma once
#include "templates/core/TypeTraits.hpp"
#ifdef CP_USE_ADVANCED
#include "templates/advanced/Concepts.hpp"
#endif

//===----------------------------------------------------------------------===//
/* Randomized Hash Utilities (anti-collision for unordered containers) */

namespace cp::hashing {

[[gnu::always_inline]] inline U64 splitmix64(U64 x) noexcept {
  x += 0x9e37'79b9'7f4a'7c15ULL;
  x = (x ^ (x >> 30)) * 0xbf58'476d'1ce4'e5b9ULL;
  x = (x ^ (x >> 27)) * 0x94d0'49bb'1331'11ebULL;
  return x ^ (x >> 31);
}

[[gnu::always_inline]] inline U64& fixed_random_seed_storage() noexcept {
#ifdef CP_SEED
  static U64 seed = static_cast<U64>(CP_SEED);
#else
  static U64 seed = static_cast<U64>(std::chrono::steady_clock::now().time_since_epoch().count());
#endif
  return seed;
}

[[gnu::always_inline]] inline U64 fixed_random_seed() noexcept { return fixed_random_seed_storage(); }

inline void reseed(U64 seed) noexcept { fixed_random_seed_storage() = seed; }

[[nodiscard]] constexpr inline U64 hash_combine(U64 lhs, U64 rhs) noexcept {
  return lhs ^ (rhs + 0x9e37'79b9'7f4a'7c15ULL + (lhs << 6) + (lhs >> 2));
}

template <class T>
[[gnu::always_inline]] inline U64 raw_hash(const T& value) noexcept {
  using U = cp::remove_cvref_t<T>;
#ifdef CP_USE_ADVANCED
  if constexpr (Integral<U>)
    return static_cast<U64>(value);
  else if constexpr (Enum<U>)
    return static_cast<U64>(static_cast<std::underlying_type_t<U>>(value));
  else if constexpr (Hashable<U>)
    return static_cast<U64>(std::hash<U>{}(value));
  else {
    static_assert(Hashable<U>, "raw_hash(): type is not hashable; provide std::hash specialization.");
    return 0;
  }
#else
  if constexpr (std::is_integral_v<U>)
    return static_cast<U64>(value);
  else if constexpr (std::is_enum_v<U>)
    return static_cast<U64>(static_cast<std::underlying_type_t<U>>(value));
  else
    return static_cast<U64>(std::hash<U>{}(value));
#endif
}

template <class T>
struct SplitMixHash {
  size_t operator()(const T& value) const noexcept {
    return static_cast<size_t>(splitmix64(raw_hash(value) + fixed_random_seed()));
  }
};

template <class T, class U>
struct PairHash {
  size_t operator()(const std::pair<T, U>& value) const noexcept {
    U64 lhs = static_cast<U64>(SplitMixHash<T>{}(value.first));
    U64 rhs = static_cast<U64>(SplitMixHash<U>{}(value.second));
    return static_cast<size_t>(splitmix64(hash_combine(lhs, rhs) + fixed_random_seed()));
  }
};

template <class K, class V, class Hash = SplitMixHash<K>>
using FastHashMap = std::unordered_map<K, V, Hash>;

template <class K, class Hash = SplitMixHash<K>>
using FastHashSet = std::unordered_set<K, Hash>;

template <class K1, class K2, class V, class Hash = PairHash<K1, K2>>
using FastHashMap2 = std::unordered_map<std::pair<K1, K2>, V, Hash>;

} // namespace cp::hashing
