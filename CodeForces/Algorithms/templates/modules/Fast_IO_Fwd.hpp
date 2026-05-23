#pragma once
#include "templates/core/ScalarTypes.hpp"

template <I64 MOD>
struct ModInt;

namespace cp {
template <class T, class Tag>
class StrongType;
} // namespace cp

namespace fast_io {

template <class T>
concept FastIntegral = std::integral<std::remove_cvref_t<T>> && !std::same_as<std::remove_cvref_t<T>, bool>
                    && !std::same_as<std::remove_cvref_t<T>, char>;

template <class T>
concept FastFloating = std::floating_point<std::remove_cvref_t<T>>;

template <class T>
inline void read_integer(T&);

template <class T>
inline void write_integer(T);

} // namespace fast_io
