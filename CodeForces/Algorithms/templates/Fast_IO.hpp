#pragma once
#include "Macros.hpp"
#include "ScalarTypes.hpp"

//===----------------------------------------------------------------------===//
/* High-Performance Buffered I/O */

#ifndef CP_FAST_IO_NAMESPACE_DEFINED
#define CP_FAST_IO_NAMESPACE_DEFINED 1
#endif
#ifdef CP_IO_COMPAT_FAST_IO_NAMESPACE_DEFINED
#error "Fast_IO.hpp must be included before IO.hpp when both I/O backends are used."
#endif

#include "Fast_IO_Fwd.hpp"
// Pull ContainerAliases into the global namespace BEFORE opening fast_io,
// but only when composite I/O is enabled.
#if CP_IO_ENABLE_COMPOSITE
#include "ContainerAliases.hpp"
#endif

namespace fast_io {

static constexpr U32 BUFFER_SIZE = 1U << 17; // 128 KB
alignas(64) inline char input_buffer[BUFFER_SIZE];
alignas(64) inline char output_buffer[BUFFER_SIZE];
alignas(64) inline char number_buffer[128];

inline U32  input_pos   = 0;
inline U32  input_end   = 0;
inline U32  output_pos  = 0;
inline bool input_eof   = false;
inline bool input_error = false;

template <class T>
concept FastIntegral = std::integral<std::remove_cvref_t<T>> && !std::same_as<std::remove_cvref_t<T>, bool>
                       && !std::same_as<std::remove_cvref_t<T>, char>;

template <class T>
concept FastFloating = std::floating_point<std::remove_cvref_t<T>>;

/* ------------------------------- INPUT API -------------------------------- */

inline void load_input() {
  if (input_eof && input_pos >= input_end) {
    input_pos = input_end = 0;
    return;
  }

  const U32 remaining = input_end - input_pos;
  std::memmove(input_buffer, input_buffer + input_pos, remaining);
  const U32 capacity   = BUFFER_SIZE - remaining;
  const U32 bytes_read = as<U32>(std::fread(input_buffer + remaining, 1, capacity, stdin));
  input_error          = input_error || (std::ferror(stdin) != 0);
  my_assert(!input_error && "Fast_IO: fread failed.");
  input_end = remaining + bytes_read;
  input_pos = 0;
  if (bytes_read < capacity) {
    input_eof = true;
    if (input_end < BUFFER_SIZE)
      input_buffer[input_end++] = '\n';
  }
}

inline char next_input_char() {
  if (input_pos >= input_end)
    load_input();
  my_assert(input_pos < input_end && "Fast_IO: unexpected end of input.");
  return input_buffer[input_pos++];
}

inline void flush_output() {
  if (output_pos == 0)
    return;
  std::fwrite(output_buffer, 1, output_pos, stdout);
  output_pos = 0;
}

inline void read_char(char& c) {
  do {
    c = next_input_char();
  } while (std::isspace(as<unsigned char>(c)));
}

template <typename T>
inline void read_integer(T& x) {
  char c;
  do {
    c = next_input_char();
  } while (std::isspace(as<unsigned char>(c)));

  bool negative = false;
  if constexpr (std::is_signed_v<T>) {
    if (c == '-') {
      negative = true;
      c        = next_input_char();
    }
  }

  x              = 0;
  bool has_digit = false;
  while (c >= '0' && c <= '9') {
    has_digit = true;
    x         = x * 10 + (c - '0');
    c         = next_input_char();
  }
  my_assert(has_digit && "Fast_IO: expected an integer token.");

  if constexpr (std::is_signed_v<T>) {
    if (negative)
      x = -x;
  }
}

inline void read_string(std::string& s) {
  s.clear();
  s.reserve(32);
  char c;
  do {
    c = next_input_char();
  } while (std::isspace(as<unsigned char>(c)));

  do {
    s.push_back(c);
    c = next_input_char();
  } while (!std::isspace(as<unsigned char>(c)));
}

template <typename T>
inline void read_floating(T& x) {
  std::string token;
  read_string(token);
  const char* ptr = token.c_str();
  char*       end = nullptr;
  if constexpr (std::same_as<std::remove_cvref_t<T>, F32>) {
    x = std::strtof(ptr, &end);
  } else if constexpr (std::same_as<std::remove_cvref_t<T>, F64>) {
    x = std::strtod(ptr, &end);
  } else {
    x = std::strtold(ptr, &end);
  }
  if (end == ptr || *end != '\0') {
    my_assert(false && "read_floating(): failed to parse floating-point token.");
    x = as<T>(0);
  }
}

template <FastIntegral T>
inline void read(T& x) {
  read_integer(x);
}

template <FastFloating T>
inline void read(T& x) {
  read_floating(x);
}

inline void read(char& x) {
  read_char(x);
}

inline void read(std::string& x) {
  read_string(x);
}

/* ------------------------------- OUTPUT API ------------------------------- */

template <typename T>
inline void write_integer(T x) {
  if (output_pos + 64 >= BUFFER_SIZE)
    flush_output();

  using UnsignedT = std::make_unsigned_t<T>;
  UnsignedT ux;
  if constexpr (std::is_signed_v<T>) {
    if (x < 0) {
      output_buffer[output_pos++] = '-';
      ux                          = as<UnsignedT>(-(x + 1));
      ux += 1;
    } else {
      ux = as<UnsignedT>(x);
    }
  } else {
    ux = as<UnsignedT>(x);
  }

  I32 digits = 0;
  do {
    number_buffer[digits++] = as<char>('0' + (ux % 10));
    ux /= 10;
  } while (ux > 0);

  for (I32 i = digits - 1; i >= 0; --i) {
    output_buffer[output_pos++] = number_buffer[i];
  }
}

#ifndef CP_FLOAT_PRECISION
#define CP_FLOAT_PRECISION 10
#endif

template <typename T>
inline void write_floating(T x) {
  char      local_buffer[128];
  const int n = std::snprintf(local_buffer, sizeof(local_buffer), "%.*Lf", CP_FLOAT_PRECISION, as<long double>(x));
  if (n <= 0)
    return;

  U32 len = as<U32>(std::min(n, as<int>(sizeof(local_buffer) - 1)));
  if (len >= BUFFER_SIZE) {
    flush_output();
    std::fwrite(local_buffer, 1, len, stdout);
    return;
  }
  if (output_pos + len >= BUFFER_SIZE)
    flush_output();
  std::memcpy(output_buffer + output_pos, local_buffer, len);
  output_pos += len;
}

inline void write_char(char c) {
  if (output_pos >= BUFFER_SIZE)
    flush_output();
  output_buffer[output_pos++] = c;
}

inline void write_string(std::string_view s) {
  const char* data      = s.data();
  U32         remaining = as<U32>(s.size());
  while (remaining > 0) {
    if (output_pos >= BUFFER_SIZE)
      flush_output();
    U32 space = BUFFER_SIZE - output_pos;
    U32 chunk = (remaining < space) ? remaining : space;
    std::memcpy(output_buffer + output_pos, data, chunk);
    output_pos += chunk;
    data += chunk;
    remaining -= chunk;
  }
}

template <FastIntegral T>
inline void write(T x) {
  write_integer(x);
}

template <FastFloating T>
inline void write(T x) {
  write_floating(x);
}

inline void write(char x) {
  write_char(x);
}

inline void write(const std::string& x) {
  write_string(x);
}

inline void write(const char* x) {
  write_string(x);
}

#ifndef CP_FAST_IO_ENABLE_MODINT
#ifdef NEED_MOD_INT
#define CP_FAST_IO_ENABLE_MODINT 1
#else
#define CP_FAST_IO_ENABLE_MODINT 0
#endif
#endif

#ifndef CP_FAST_IO_ENABLE_STRONG_TYPE
#define CP_FAST_IO_ENABLE_STRONG_TYPE 0
#endif

#if CP_IO_ENABLE_COMPOSITE
#define CP_IO_COMPOSITE_CONTEXT 1
#include "IO_Composite.hpp"
#undef CP_IO_COMPOSITE_CONTEXT
#endif

template <class Head, class... Tail>
  requires(sizeof...(Tail) > 0)
inline void read(Head& head, Tail&... tail) {
  read(head);
  read(tail...);
}

template <class Head, class... Tail>
  requires(sizeof...(Tail) > 0)
inline void write(const Head& head, const Tail&... tail) {
  write(head);
  write(' ');
  write(tail...);
}

inline void writeln() {
  write('\n');
}

template <class... Args>
inline void writeln(const Args&... args) {
  if constexpr (sizeof...(args) > 0)
    write(args...);
  writeln();
}

// Single-arg fallbacks: error on types lacking a concrete overload.
template <class T>
void read(T&) = delete;

template <class T>
void write(const T&) = delete;

struct IOFlusher {
  ~IOFlusher() { flush_output(); }
};

inline IOFlusher io_flusher;

} // namespace fast_io

#if CP_FAST_IO_ENABLE_MODINT
#include "Fast_IO_Ext_ModInt.hpp"
#endif

#if CP_FAST_IO_ENABLE_STRONG_TYPE
#include "Fast_IO_Ext_StrongType.hpp"
#endif

#ifdef CP_IO_IMPL_READ
#undef CP_IO_IMPL_READ
#endif
#ifdef CP_IO_IMPL_WRITELN
#undef CP_IO_IMPL_WRITELN
#endif
#ifdef CP_IO_IMPL_FLUSH
#undef CP_IO_IMPL_FLUSH
#endif

#define CP_IO_IMPL_READ(...) fast_io::read(__VA_ARGS__)
#define CP_IO_IMPL_WRITELN(...) fast_io::writeln(__VA_ARGS__)
#define CP_IO_IMPL_FLUSH() fast_io::flush_output()

#include "IO_Defs.hpp"
