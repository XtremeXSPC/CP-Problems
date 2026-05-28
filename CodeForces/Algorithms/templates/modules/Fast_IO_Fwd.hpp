#pragma once
#include "templates/core/IdiomAliases.hpp"
#include "templates/core/ScalarTypes.hpp"

template <I64 MOD>
struct ModInt;

namespace cp {
template <class T, class Tag>
class StrongType;
} // namespace cp

namespace fast_io {

template <class T>
concept FastIntegral = cp::Int<T> && !cp::Same<T, bool> && !cp::Same<T, char>;

template <class T>
concept FastFloating = cp::Float<T>;

template <class T>
inline void read_integer(T&);

template <class T>
inline void write_integer(T);

} // namespace fast_io
