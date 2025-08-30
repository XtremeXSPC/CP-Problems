#pragma once
#include "Base.hpp"
#include "Types.hpp"

//===----------------------------------------------------------------------===//
/* Optimized I/O System */

// Fast I/O
namespace fast_io {
  static constexpr U32 BUFFER_SIZE = 1 << 17; // 128KB buffer
  alignas(64) char input_buffer[BUFFER_SIZE];
  alignas(64) char output_buffer[BUFFER_SIZE];
  alignas(64) char number_buffer[128];
  
  // Precomputed number strings for fast output:
  struct NumberLookup {
    char digits[10000][4];
    constexpr NumberLookup() : digits{} {
      for (I32 i = 0; i < 10000; ++i) {
        digits[i][3] = '0' + (i % 10);
        digits[i][2] = '0' + ((i / 10) % 10);
        digits[i][1] = '0' + ((i / 100) % 10);
        digits[i][0] = '0' + (i / 1000);
      }
    }
  };
  constexpr NumberLookup number_lookup;
  
  U32 input_pos = 0, input_end = 0, output_pos = 0;
  
  [[gnu::always_inline]] inline void load_input() {
    std::memmove(input_buffer, input_buffer + input_pos, input_end - input_pos);
    input_end = input_end - input_pos + 
                std::fread(input_buffer + input_end - input_pos, 1, 
                          BUFFER_SIZE - input_end + input_pos, stdin);
    input_pos = 0;
    if (input_end < BUFFER_SIZE) input_buffer[input_end++] = '\n';
  }
  
  [[gnu::always_inline]] inline void flush_output() {
    std::fwrite(output_buffer, 1, output_pos, stdout);
    output_pos = 0;
  }
  
  // Fast character reading:
  [[gnu::always_inline]] inline void read_char(char& c) {
    do {
      if (input_pos >= input_end) load_input();
      c = input_buffer[input_pos++];
    } while (std::isspace(c));
  }
  
  // Optimized integer reading with SIMD potential:
  template <typename T>
  [[gnu::always_inline]] inline void read_integer(T& x) {
    if (input_pos + 64 >= input_end) load_input();
    
    char c;
    do { c = input_buffer[input_pos++]; } while (c < '-');
    
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
  
  // Fast string reading:
  [[gnu::always_inline]] inline void read_string(std::string& s) {
    s.clear();
    char c;
    do {
      if (input_pos >= input_end) load_input();
      c = input_buffer[input_pos++];
    } while (std::isspace(c));
    
    do {
      s.push_back(c);
      if (input_pos >= input_end) load_input();
      c = input_buffer[input_pos++];
    } while (!std::isspace(c));
  }
  
  // Optimized integer writing:
  template <typename T>
  [[gnu::always_inline]] inline void write_integer(T x) {
    if (output_pos + 64 >= BUFFER_SIZE) flush_output();
    
    if (x < 0) {
      output_buffer[output_pos++] = '-';
      x = -x;
    }
    
    I32 digits = 0;
    T temp = x;
    do {
      number_buffer[digits++] = '0' + (temp % 10);
      temp /= 10;
    } while (temp > 0);
    
    // Reverse and copy:
    for (I32 i = digits - 1; i >= 0; --i) {
      output_buffer[output_pos++] = number_buffer[i];
    }
  }
  
  [[gnu::always_inline]] inline void write_char(char c) {
    if (output_pos >= BUFFER_SIZE) flush_output();
    output_buffer[output_pos++] = c;
  }
  
  [[gnu::always_inline]] inline void write_string(const std::string& s) {
    for (char c : s) write_char(c);
  }
  
  // Template-based readers:
  void read(I32& x) { read_integer(x); }
  void read(I64& x) { read_integer(x); }
  void read(U32& x) { read_integer(x); }
  void read(U64& x) { read_integer(x); }
  void read(char& x) { read_char(x); }
  void read(std::string& x) { read_string(x); }
  
  template <class T, class U>
  void read(std::pair<T, U>& p) { read(p.first); read(p.second); }
  
  template <class T>
  void read(VC<T>& v) { for (auto& x : v) read(x); }
  
  // Variadic read:
  template <class Head, class... Tail>
  void read(Head& head, Tail&... tail) {
    read(head);
    if constexpr (sizeof...(tail) > 0) read(tail...);
  }
  
  // Template-based writers:
  void write(I32 x) { write_integer(x); }
  void write(I64 x) { write_integer(x); }
  void write(U32 x) { write_integer(x); }
  void write(U64 x) { write_integer(x); }
  void write(char x) { write_char(x); }
  void write(const std::string& x) { write_string(x); }
  void write(const char* x) { write_string(std::string(x)); }
  
  template <class T, class U>
  void write(const std::pair<T, U>& p) {
    write(p.first); write(' '); write(p.second);
  }
  
  template <class T>
  void write(const VC<T>& v) {
    for (I64 i = 0; i < sz(v); ++i) {
      if (i) write(' ');
      write(v[i]);
    }
  }
  
  // Variadic write:
  template <class Head, class... Tail>
  void write(const Head& head, const Tail&... tail) {
    write(head);
    if constexpr (sizeof...(tail) > 0) {
      write(' ');
      write(tail...);
    }
  }
  
  void writeln() { write_char('\n'); }
  
  template <class... Args>
  void writeln(const Args&... args) {
    if constexpr (sizeof...(args) > 0) write(args...);
    write_char('\n');
  }
  
  // Destructor for automatic flushing:
  struct IOFlusher {
    ~IOFlusher() { flush_output(); }
  } io_flusher;
}

// Input/Output macros:
#define IN(...) fast_io::read(__VA_ARGS__)
#define OUT(...) fast_io::writeln(__VA_ARGS__)
#define FLUSH() fast_io::flush_output()

// Convenient input macros:
#define INT(...) I32 __VA_ARGS__; IN(__VA_ARGS__)
#define LL(...) I64 __VA_ARGS__; IN(__VA_ARGS__)
#define ULL(...) U64 __VA_ARGS__; IN(__VA_ARGS__)
#define STR(...) std::string __VA_ARGS__; IN(__VA_ARGS__)
#define CHR(...) char __VA_ARGS__; IN(__VA_ARGS__)
#define DBL(...) F64 __VA_ARGS__; IN(__VA_ARGS__)

#define VEC(type, name, size) VC<type> name(size); IN(name)
#define VV(type, name, h, w) VVC<type> name(h, VC<type>(w)); IN(name)

// Answer macros:
void YES(bool condition = true) { OUT(condition ? "YES" : "NO"); }
void NO(bool condition = true) { YES(!condition); }
void Yes(bool condition = true) { OUT(condition ? "Yes" : "No"); }
void No(bool condition = true) { Yes(!condition); }