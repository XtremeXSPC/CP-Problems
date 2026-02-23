#pragma once
#include "Types.hpp"

//===----------------------------------------------------------------------===//
/* Core Concepts for Type-Safe CP Templates */

namespace cp {

template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <class T>
concept Integral = std::integral<remove_cvref_t<T>>;

template <class T>
concept SignedIntegral = Integral<T> && std::is_signed_v<remove_cvref_t<T>>;

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

template <class T>
concept Enum = std::is_enum_v<remove_cvref_t<T>>;

template <class F, class... Args>
concept Predicate = std::predicate<F, Args...>;

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

template <class T>
concept Hashable = requires(const remove_cvref_t<T>& value) {
  { std::hash<remove_cvref_t<T>>{}(value) } -> std::convertible_to<std::size_t>;
};

} // namespace cp
