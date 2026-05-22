#pragma once
// ContainerAliases.hpp is supplied by the caller at global scope.
// Including it here would scope Vec/Pair/Tuple into the surrounding
// namespace when this header is pulled inside cp_io / fast_io.
#ifndef CP_IO_COMPOSITE_CONTEXT
#include "IO.hpp"
#define CP_IO_COMPOSITE_READ cp_io::read
#define CP_IO_COMPOSITE_WRITE cp_io::write
#else
#define CP_IO_COMPOSITE_READ read
#define CP_IO_COMPOSITE_WRITE write
#endif

template <class T, class U>
inline void read(Pair<T, U>& p) {
  CP_IO_COMPOSITE_READ(p.first);
  CP_IO_COMPOSITE_READ(p.second);
}

template <class T>
inline void read(Vec<T>& v) {
  for (auto& x : v)
    CP_IO_COMPOSITE_READ(x);
}

template <typename... Args>
inline void read(std::tuple<Args...>& t) {
  std::apply([](auto&... args) { (CP_IO_COMPOSITE_READ(args), ...); }, t);
}

template <class T, class U>
inline void write(const Pair<T, U>& p) {
  CP_IO_COMPOSITE_WRITE(p.first);
  CP_IO_COMPOSITE_WRITE(' ');
  CP_IO_COMPOSITE_WRITE(p.second);
}

template <class T>
inline void write(const Vec<T>& v) {
  for (I64 i = 0; i < as<I64>(v.size()); ++i) {
    if (i)
      CP_IO_COMPOSITE_WRITE(' ');
    CP_IO_COMPOSITE_WRITE(v[i]);
  }
}

template <typename... Args>
inline void write(const std::tuple<Args...>& t) {
  I32 i = 0;
  std::apply(
      [&i](const auto&... args) {
        ((i++ > 0 ? (CP_IO_COMPOSITE_WRITE(' '), 0) : 0, CP_IO_COMPOSITE_WRITE(args)), ...);
      },
      t);
}

#undef CP_IO_COMPOSITE_READ
#undef CP_IO_COMPOSITE_WRITE
