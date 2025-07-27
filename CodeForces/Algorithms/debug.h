#ifndef ALGO_DEBUG_H
#define ALGO_DEBUG_H

#include <algorithm>
#include <bitset>
#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

//===---------------------------------------------------------------------===//
//============================= CONFIGURATION  ==============================//

// Define the `LOCAL` macro via compiler flag, (Example: g++ -DLOCAL)
#ifdef LOCAL
// If LOCAL is defined, debugging is enabled.
#define DEBUG_ENABLED
#endif

//===---------------------------------------------------------------------===//
//=============================== ANSI COLORS ===============================//

// To make terminal output more readable.
#ifdef DEBUG_ENABLED
#define RESET "\033[0m"
#define RED "\033[31m"     /* Red */
#define GREEN "\033[32m"   /* Green */
#define YELLOW "\033[33m"  /* Yellow */
#define BLUE "\033[34m"    /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m"    /* Cyan */
#else
#define RESET ""
#define RED ""
#define GREEN ""
#define YELLOW ""
#define BLUE ""
#define MAGENTA ""
#define CYAN ""
#endif

//===---------------------------------------------------------------------===//
//====================== DEBUGGER CORE IMPLEMENTATION =======================//

// Forward declaration for the main printing function
void _print_recursive();

// Forward declarations for template functions to resolve dependencies
template <typename T, typename... V>
void _print_recursive(T t, V... v);
template <typename T>
void _print(const T& x);

// Base cases for printing fundamental types
inline void _print(int x) {
  std::cerr << x;
}
inline void _print(long x) {
  std::cerr << x;
}
inline void _print(long long x) {
  std::cerr << x;
}
inline void _print(unsigned x) {
  std::cerr << x;
}
inline void _print(unsigned long x) {
  std::cerr << x;
}
inline void _print(unsigned long long x) {
  std::cerr << x;
}
inline void _print(float x) {
  std::cerr << x;
}
inline void _print(double x) {
  std::cerr << x;
}
inline void _print(long double x) {
  std::cerr << x;
}
inline void _print(char x) {
  std::cerr << '\'' << x << '\'';
}
inline void _print(const char* x) {
  std::cerr << '"' << x << '"';
}
inline void _print(const std::string& x) {
  std::cerr << '"' << x << '"';
}
inline void _print(bool x) {
  std::cerr << (x ? "true" : "false");
}
inline void _print(std::bitset<8> b) {
  std::cerr << b;
}
template <size_t N>
void _print(const std::bitset<N>& b) {
  std::cerr << b;
}

// Template for printing a std::pair
template <typename T, typename U>
void _print(const std::pair<T, U>& x) {
  std::cerr << '{';
  _print(x.first);
  std::cerr << ", ";
  _print(x.second);
  std::cerr << '}';
}

// Helper for printing a std::tuple
template <typename T, std::size_t... Is>
void print_tuple(const T& t, std::index_sequence<Is...>) {
  std::cerr << "{";
  int f = 0;
  // C++17 fold expression
  ((std::cerr << (f++ ? ", " : ""), _print(std::get<Is>(t))), ...);
  std::cerr << "}";
}

// Template for printing a std::tuple
template <typename... Args>
void _print(const std::tuple<Args...>& t) {
  print_tuple(t, std::make_index_sequence<sizeof...(Args)>());
}

// Template for printing iterable containers (vector, set, map, etc.)
template <typename T>
void _print(const T& x) {
  int f = 0;
  std::cerr << '{';
  for (const auto& i : x) {
    std::cerr << (f++ ? ", " : "");
    _print(i);
  }
  std::cerr << "}";
}

// Specializations for non-iterable containers (pass by value to copy)
template <typename T>
void _print(std::queue<T> q) {
  std::cerr << "{";
  bool first = true;
  while (!q.empty()) {
    if (!first)
      std::cerr << ", ";
    _print(q.front());
    q.pop();
    first = false;
  }
  std::cerr << "}";
}

template <typename T>
void _print(std::stack<T> s) {
  std::vector<T> temp;
  while (!s.empty()) {
    temp.push_back(s.top());
    s.pop();
  }
  std::reverse(temp.begin(), temp.end());
  _print(temp);
}

template <typename T, typename Container, typename Compare>
void _print(std::priority_queue<T, Container, Compare> pq) {
  std::vector<T> temp;
  while (!pq.empty()) {
    temp.push_back(pq.top());
    pq.pop();
  }
  _print(temp);
}

// Recursive variadic function to handle multiple arguments.
inline void _print_recursive() {
  std::cerr << "]" << std::endl;
}

template <typename T, typename... V>
void _print_recursive(T t, V... v) {
  _print(t);
  if (sizeof...(v)) {
    std::cerr << ", ";
  }
  _print_recursive(v...);
}

//===---------------------------------------------------------------------===//
//============================ MAIN DEBUG MACROS ============================//

#ifdef DEBUG_ENABLED
// Main debug macro: prints file, line, function, and variables.
// __VA_ARGS__ is a variadic macro that captures all passed arguments.
// #__VA_ARGS__ converts them into a string literal.
#define debug(...)                                                                                                                         \
  std::cerr << YELLOW << "[" << __FILE__ << ":" << __LINE__ << " (" << __func__ << ")] " << RESET << MAGENTA << #__VA_ARGS__ << " = "      \
            << CYAN;                                                                                                                       \
  std::cerr << "[";                                                                                                                        \
  _print_recursive(__VA_ARGS__);                                                                                                           \
  std::cerr << RESET;

// Macro for printing a separator line.
#define debug_line() std::cerr << BLUE << "-------------------------------------------------" << RESET << std::endl;
#else
// If debugging is disabled, all macros do nothing.
// The compiler will optimize them away completely.
#define debug(...) 42
#define debug_line() 42
#endif

//===---------------------------------------------------------------------===//
//============================= UTILITY: TIMER ==============================//

#ifdef DEBUG_ENABLED
class Timer {
public:
  Timer() : start_time(std::chrono::high_resolution_clock::now()) {}

  ~Timer() {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cerr << GREEN << "\n[Timer] Execution Time: " << duration.count() << " ms" << RESET << std::endl;
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};
#endif

#endif // ALGO_DEBUG_H

//===---------------------------------------------------------------------===//s