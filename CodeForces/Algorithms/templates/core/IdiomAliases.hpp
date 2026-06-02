#pragma once
#include "ScalarTypes.hpp"

//===----------------------------------------------------------------------===//
/* Library Function Aliases */

namespace cp {

template <bool B, class T, class F>
using Conditional = std::conditional_t<B, T, F>;

template <class T>
using cvref_t = std::remove_cvref_t<T>;

template <class T, class U>
concept Same = std::same_as<cvref_t<T>, cvref_t<U>>;

// clang-format off
template <class T>
concept Int = std::integral<cvref_t<T>>
#if HAS_INT128
  || std::same_as<cvref_t<T>, I128>
  || std::same_as<cvref_t<T>, U128>
#endif
    ;

template <class T>
concept Float = std::floating_point<cvref_t<T>>;

template <class T>
concept Signed = Int<T> && (std::is_signed_v<cvref_t<T>>
#if HAS_INT128
  || std::same_as<cvref_t<T>, I128>
#endif
);
// clang-format on

template <class T>
concept Unsigned = Int<T> && !Signed<T>;

template <class T>
concept Enum = std::is_enum_v<cvref_t<T>>;

template <class F, class... Args>
concept Predicate = std::predicate<F, Args...>;

template <class T>
concept Hashable = requires(const cvref_t<T>& value) {
  { std::hash<cvref_t<T>>{}(value) } -> std::convertible_to<std::size_t>;
};

} // namespace cp
