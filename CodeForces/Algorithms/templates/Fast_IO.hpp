#pragma once
#include "Types.hpp"

//===----------------------------------------------------------------------===//
/* High-Performance Buffered I/O */

#ifndef CP_FAST_IO_NAMESPACE_DEFINED
  #define CP_FAST_IO_NAMESPACE_DEFINED 1
#endif

namespace fast_io {

static constexpr U32 BUFFER_SIZE = 1U << 17; // 128 KB
alignas(64) inline char input_buffer[BUFFER_SIZE];
alignas(64) inline char output_buffer[BUFFER_SIZE];
alignas(64) inline char number_buffer[128];

inline U32 input_pos = 0;
inline U32 input_end = 0;
inline U32 output_pos = 0;

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
  if (input_pos + 64 >= input_end) load_input();

  char c;
  do {
    c = input_buffer[input_pos++];
  } while (c < '-');

  bool negative = false;
  if constexpr (std::is_signed_v<T>) {
    if (c == '-') {
      negative = true;
      c = input_buffer[input_pos++];
    }
  }

  x = 0;
  while (c >= '0') {
    x = x * 10 + (c - '0');
    c = input_buffer[input_pos++];
  }

  if constexpr (std::is_signed_v<T>) {
    if (negative) x = -x;
  }
}

inline void read_string(std::string& s) {
  s.clear();
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

inline void write_char(char c) {
  if (output_pos >= BUFFER_SIZE) flush_output();
  output_buffer[output_pos++] = c;
}

inline void write_string(std::string_view s) {
  for (char c : s) write_char(c);
}

inline void read(I32& x) { read_integer(x); }
inline void read(I64& x) { read_integer(x); }
inline void read(U32& x) { read_integer(x); }
inline void read(U64& x) { read_integer(x); }
inline void read(char& x) { read_char(x); }
inline void read(std::string& x) { read_string(x); }

template <class T, class U>
void read(std::pair<T, U>& p) {
  read(p.first);
  read(p.second);
}

template <class T>
void read(Vec<T>& v) {
  for (auto& x : v) read(x);
}

template <class Head, class... Tail>
void read(Head& head, Tail&... tail) {
  read(head);
  if constexpr (sizeof...(tail) > 0) read(tail...);
}

inline void write(I32 x) { write_integer(x); }
inline void write(I64 x) { write_integer(x); }
inline void write(U32 x) { write_integer(x); }
inline void write(U64 x) { write_integer(x); }
inline void write(char x) { write_char(x); }
inline void write(const std::string& x) { write_string(x); }
inline void write(const char* x) { write_string(x); }

template <class T, class U>
void write(const std::pair<T, U>& p) {
  write(p.first);
  write(' ');
  write(p.second);
}

template <class T>
void write(const Vec<T>& v) {
  for (I64 i = 0; i < static_cast<I64>(v.size()); ++i) {
    if (i) write(' ');
    write(v[i]);
  }
}

template <class Head, class... Tail>
void write(const Head& head, const Tail&... tail) {
  write(head);
  if constexpr (sizeof...(tail) > 0) {
    write(' ');
    write(tail...);
  }
}

inline void writeln() { write_char('\n'); }

template <class... Args>
void writeln(const Args&... args) {
  if constexpr (sizeof...(args) > 0) write(args...);
  write_char('\n');
}

struct IOFlusher {
  ~IOFlusher() { flush_output(); }
};
inline IOFlusher io_flusher;

} // namespace fast_io

#ifdef IN
  #undef IN
#endif
#ifdef OUT
  #undef OUT
#endif
#ifdef FLUSH
  #undef FLUSH
#endif

#define IN(...) fast_io::read(__VA_ARGS__)
#define OUT(...) fast_io::writeln(__VA_ARGS__)
#define FLUSH() fast_io::flush_output()
