#pragma once
#include "Fast_IO.hpp"

///@brief Optional Fast I/O extension for ModInt.

template <I64 MOD>
inline void read(::ModInt<MOD>& x) {
  I64 value;
  read_integer(value);
  x = ::ModInt<MOD>(value);
}

template <I64 MOD>
inline void write(const ::ModInt<MOD>& x) {
  write_integer(static_cast<I64>(x));
}
