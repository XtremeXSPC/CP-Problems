#pragma once
#include "templates/core/IdiomAliases.hpp"
#include "templates/core/TypeTraits.hpp"

//===----------------------------------------------------------------------===//
/* Core Concepts for Type-Safe CP Templates */
// Enum / Predicate / Hashable live in core/IdiomAliases.hpp (always-on); this
// advanced layer extends that vocabulary with the richer, rarely-used concepts.

namespace cp {

template <class T>
concept Integral = std::integral<remove_cvref_t<T>> || detail::is_extended_integral_v<remove_cvref_t<T>>;

// The #if HAS_INT128 arm splits this concept expression; clang-format would
// otherwise reflow it into a less readable operand-aligned form.
// clang-format off
template <class T>
concept SignedIntegral = Integral<T> && (std::is_signed_v<remove_cvref_t<T>>
#if HAS_INT128
  || std::same_as<remove_cvref_t<T>, I128>
#endif
);
// clang-format on

template <class T>
concept UnsignedIntegral = Integral<T> && !SignedIntegral<T>;

template <class T>
concept NonBoolIntegral = Integral<T> && !std::same_as<remove_cvref_t<T>, bool>;

template <class T>
concept Floating = std::floating_point<remove_cvref_t<T>>;

template <class T>
concept Arithmetic = Integral<T> || Floating<T>;

template <class T>
concept IndexLike = NonBoolIntegral<T>;

template <class R>
concept Range = std::ranges::range<remove_cvref_t<R>>;

template <class R>
concept SizedRange = Range<R> && requires(remove_cvref_t<R> r) { std::ranges::size(r); };

template <class T>
concept StreamReadable = requires(std::istream& is, T& value) {
  { is >> value } -> std::same_as<std::istream&>;
};

template <class T>
concept StreamWritable = requires(std::ostream& os, const T& value) {
  { os << value } -> std::same_as<std::ostream&>;
};

} // namespace cp
