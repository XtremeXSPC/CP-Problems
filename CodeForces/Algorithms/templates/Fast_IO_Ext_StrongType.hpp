#pragma once
#include "Fast_IO.hpp"

///@brief Optional Fast I/O extension for StrongType<T, Tag> when T is integral.

template <FastIntegral T, class Tag>
inline void read(::cp::StrongType<T, Tag>& x) {
  T value;
  read_integer(value);
  x = ::cp::StrongType<T, Tag>::from_raw(value);
}

template <FastIntegral T, class Tag>
inline void write(const ::cp::StrongType<T, Tag>& x) {
  write_integer(x.get());
}
