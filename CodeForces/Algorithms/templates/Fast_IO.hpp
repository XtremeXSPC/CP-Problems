#pragma once
#include "Types.hpp"

//===----------------------------------------------------------------------===//
/* High-Performance Buffered I/O */

#ifndef CP_FAST_IO_NAMESPACE_DEFINED
  #define CP_FAST_IO_NAMESPACE_DEFINED 1
#endif

template <I64 MOD>
struct ModInt;

namespace cp {
template <class T, class Tag>
class StrongType;
}

namespace fast_io {

static constexpr U32 BUFFER_SIZE = 1U << 17; // 128 KB
alignas(64) inline char input_buffer[BUFFER_SIZE];
alignas(64) inline char output_buffer[BUFFER_SIZE];
alignas(64) inline char number_buffer[128];

inline U32 input_pos = 0;
inline U32 input_end = 0;
inline U32 output_pos = 0;

template <class T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <class T>
concept FastIntegral = std::integral<remove_cvref_t<T>> && !std::same_as<remove_cvref_t<T>, bool>
    && !std::same_as<remove_cvref_t<T>, char>;

template <class T>
concept FastFloating = std::floating_point<remove_cvref_t<T>>;

/* ------------------------------- INPUT API -------------------------------- */

inline void load_input() {
  const U32 remaining = input_end - input_pos;
  std::memmove(input_buffer, input_buffer + input_pos, remaining);
  input_end = remaining + static_cast<U32>(
      std::fread(input_buffer + remaining, 1, BUFFER_SIZE - remaining, stdin));
  input_pos = 0;
  if (input_end < BUFFER_SIZE) input_buffer[input_end++] = '\n';
}

inline void flush_output() {
  if (output_pos == 0) return;
  std::fwrite(output_buffer, 1, output_pos, stdout);
  output_pos = 0;
}

inline void read_char(char& c) {
  do {
    if (input_pos >= input_end) load_input();
    c = input_buffer[input_pos++];
  } while (std::isspace(static_cast<unsigned char>(c)));
}

template <typename T>
inline void read_integer(T& x) {
  char c;
  do {
    if (input_pos >= input_end) load_input();
    c = input_buffer[input_pos++];
  } while (std::isspace(static_cast<unsigned char>(c)));

  bool negative = false;
  if constexpr (std::is_signed_v<T>) {
    if (c == '-') {
      negative = true;
      if (input_pos >= input_end) load_input();
      c = input_buffer[input_pos++];
    }
  }

  x = 0;
  while (c >= '0' && c <= '9') {
    x = x * 10 + (c - '0');
    if (input_pos >= input_end) load_input();
    c = input_buffer[input_pos++];
  }

  if constexpr (std::is_signed_v<T>) {
    if (negative) x = -x;
  }
}

inline void read_string(String& s) {
  s.clear();
  s.reserve(32);
  char c;
  do {
    if (input_pos >= input_end) load_input();
    c = input_buffer[input_pos++];
  } while (std::isspace(static_cast<unsigned char>(c)));

  do {
    s.push_back(c);
    if (input_pos >= input_end) load_input();
    c = input_buffer[input_pos++];
  } while (!std::isspace(static_cast<unsigned char>(c)));
}

template <typename T>
inline void read_floating(T& x) {
  String token;
  read_string(token);
  const char* ptr = token.c_str();
  char* end = nullptr;
  if constexpr (std::same_as<remove_cvref_t<T>, F32>) {
    x = std::strtof(ptr, &end);
  } else if constexpr (std::same_as<remove_cvref_t<T>, F64>) {
    x = std::strtod(ptr, &end);
  } else {
    x = std::strtold(ptr, &end);
  }
  if (end == ptr || *end != '\0') x = static_cast<T>(0);
}

template <FastIntegral T>
inline void read(T& x) {
  read_integer(x);
}
template <FastFloating T>
inline void read(T& x) {
  read_floating(x);
}
inline void read(char& x) { read_char(x); }
inline void read(String& x) { read_string(x); }

/* ------------------------------- OUTPUT API ------------------------------- */

template <typename T>
inline void write_integer(T x) {
  if (output_pos + 64 >= BUFFER_SIZE) flush_output();

  using UnsignedT = std::make_unsigned_t<T>;
  UnsignedT ux;
  if constexpr (std::is_signed_v<T>) {
    if (x < 0) {
      output_buffer[output_pos++] = '-';
      ux = static_cast<UnsignedT>(-(x + 1));
      ux += 1;
    } else {
      ux = static_cast<UnsignedT>(x);
    }
  } else {
    ux = static_cast<UnsignedT>(x);
  }

  I32 digits = 0;
  do {
    number_buffer[digits++] = static_cast<char>('0' + (ux % 10));
    ux /= 10;
  } while (ux > 0);

  for (I32 i = digits - 1; i >= 0; --i) {
    output_buffer[output_pos++] = number_buffer[i];
  }
}

template <typename T>
inline void write_floating(T x) {
  char local_buffer[128];
  const int n = std::snprintf(local_buffer, sizeof(local_buffer), "%.10Lf", static_cast<long double>(x));
  if (n <= 0) return;

  U32 len = static_cast<U32>(std::min(n, static_cast<int>(sizeof(local_buffer) - 1)));
  if (len >= BUFFER_SIZE) {
    flush_output();
    std::fwrite(local_buffer, 1, len, stdout);
    return;
  }
  if (output_pos + len >= BUFFER_SIZE) flush_output();
  std::memcpy(output_buffer + output_pos, local_buffer, len);
  output_pos += len;
}

inline void write_char(char c) {
  if (output_pos >= BUFFER_SIZE) flush_output();
  output_buffer[output_pos++] = c;
}

inline void write_string(std::string_view s) {
  for (char c : s) write_char(c);
}

template <FastIntegral T>
inline void write(T x) {
  write_integer(x);
}
template <FastFloating T>
inline void write(T x) {
  write_floating(x);
}
inline void write(char x) { write_char(x); }
inline void write(const String& x) { write_string(x); }
inline void write(const char* x) { write_string(x); }

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

#if CP_FAST_IO_ENABLE_MODINT
  #include "Fast_IO_Ext_ModInt.hpp"
#endif

#if CP_FAST_IO_ENABLE_STRONG_TYPE
  #include "Fast_IO_Ext_StrongType.hpp"
#endif

#include "IO_Composite.hpp"

template <class T>
void read(T&) = delete;

template <class T>
void write(const T&) = delete;

struct IOFlusher {
  ~IOFlusher() { flush_output(); }
};
inline IOFlusher io_flusher;

} // namespace fast_io

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
