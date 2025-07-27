#ifndef ALGO_DEBUG_H
#define ALGO_DEBUG_H

#include <algorithm>
#include <bitset>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

//===---------------------------------------------------------------------===//
//============================= CONFIGURATION  ==============================//

// Define `LOCAL` with a value to control debug level, e.g., g++ -DLOCAL=2
// If only -DLOCAL is used (no value), it defaults to 1.
#ifdef LOCAL
#define DEBUG_ENABLED
// Assign the value of the LOCAL macro (from the -D flag) to LOCAL_LEVEL
#define LOCAL_LEVEL LOCAL
#else
// If the -DLOCAL flag is not provided at all, disable debugging.
#define LOCAL_LEVEL 0
#endif

//===---------------------------------------------------------------------===//
//=============================== ANSI COLORS ===============================//

// To make terminal output more readable.
#if LOCAL_LEVEL > 0
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

#if LOCAL_LEVEL > 0
namespace AlgoDebug {

  // Forward declaration for the main printing function
  void print_helper_recursive();

  // Forward declarations for template functions to resolve dependencies
  template <typename T>
  void print_helper(const T& x);
  template <typename T, typename... V>
  void print_helper_recursive(T t, V... v);

  // Base cases for printing fundamental types
  inline void print_helper(int x) {
    std::cerr << x;
  }
  inline void print_helper(long x) {
    std::cerr << x;
  }
  inline void print_helper(long long x) {
    std::cerr << x;
  }
  inline void print_helper(unsigned x) {
    std::cerr << x;
  }
  inline void print_helper(unsigned long x) {
    std::cerr << x;
  }
  inline void print_helper(unsigned long long x) {
    std::cerr << x;
  }
  inline void print_helper(float x) {
    std::cerr << x;
  }
  inline void print_helper(double x) {
    std::cerr << x;
  }
  inline void print_helper(long double x) {
    std::cerr << x;
  }
  inline void print_helper(char x) {
    std::cerr << '\'' << x << '\'';
  }
  inline void print_helper(const char* x) {
    std::cerr << '"' << x << '"';
  }
  inline void print_helper(const std::string& x) {
    std::cerr << '"' << x << '"';
  }
  inline void print_helper(bool x) {
    std::cerr << (x ? "true" : "false");
  }
  inline void print_helper(std::bitset<8> b) {
    std::cerr << b;
  }
  template <size_t N>
  void print_helper(const std::bitset<N>& b) {
    std::cerr << b;
  }

  // Pointer printing
  template <typename T>
  void print_helper(T* p) {
    if (p == nullptr) {
      std::cerr << "nullptr";
    } else {
      std::cerr << p; /* Just print address */
    }
  }

  // Template for printing a std::pair
  template <typename T, typename U>
  void print_helper(const std::pair<T, U>& x) {
    std::cerr << '{';
    print_helper(x.first);
    std::cerr << ", ";
    print_helper(x.second);
    std::cerr << '}';
  }

  // Helper for printing a std::tuple
  template <typename T, std::size_t... Is>
  void print_tuple(const T& t, std::index_sequence<Is...>) {
    std::cerr << "{";
    int f = 0;
    // C++17 fold expression
    ((std::cerr << (f++ ? ", " : ""), print_helper(std::get<Is>(t))), ...);
    std::cerr << "}";
  }

  // Template for printing a std::tuple
  template <typename... Args>
  void print_helper(const std::tuple<Args...>& t) {
    print_tuple(t, std::make_index_sequence<sizeof...(Args)>());
  }

  // Pretty print for 2D vectors (matrices)
  template <typename T>
  void print_helper(const std::vector<std::vector<T>>& mat) {
    std::cerr << "{\n";
    for (const auto& row : mat) {
      std::cerr << "  "; // Indent rows
      print_helper(row);
      std::cerr << "\n";
    }
    std::cerr << "}";
  }

  // Template for printing iterable containers (vector, set, map, etc.)
  template <typename T>
  void print_helper(const T& x) {
    int f = 0;
    std::cerr << '{';
    for (const auto& i : x) {
      std::cerr << (f++ ? ", " : "");
      print_helper(i);
    }
    std::cerr << "}";
  }

  // Specializations for non-iterable containers (pass by value to copy)
  template <typename T, typename C>
  void print_helper(const std::queue<T, C>& q);
  template <typename T, typename C>
  void print_helper(const std::stack<T, C>& s);
  template <typename T, typename Container, typename Compare>
  void print_helper(const std::priority_queue<T, Container, Compare>& pq);

  template <typename T, typename C>
  void print_helper(const std::queue<T, C>& q_orig) {
    auto q = q_orig; // Make a copy
    std::cerr << "{";
    bool first = true;
    while (!q.empty()) {
      if (!first)
        std::cerr << ", ";
      print_helper(q.front());
      q.pop();
      first = false;
    }
    std::cerr << "}";
  }

  template <typename T, typename C>
  void print_helper(const std::stack<T, C>& s_orig) {
    auto           s = s_orig; // Make a copy
    std::vector<T> temp;
    while (!s.empty()) {
      temp.push_back(s.top());
      s.pop();
    }
    std::reverse(temp.begin(), temp.end());
    print_helper(temp);
  }

  template <typename T, typename Container, typename Compare>
  void print_helper(const std::priority_queue<T, Container, Compare>& pq_orig) {
    auto           pq = pq_orig; // Make a copy
    std::vector<T> temp;
    while (!pq.empty()) {
      temp.push_back(pq.top());
      pq.pop();
    }
    print_helper(temp);
  }

  // Recursive variadic function to handle multiple arguments.
  inline void print_recursive_helper() {
    std::cerr << "]" << std::endl;
  }

  template <typename T, typename... V>
  void print_recursive_helper(T t, V... v) {
    print_helper(t);
    if (sizeof...(v))
      std::cerr << ", ";
    print_recursive_helper(v...);
  }

} // namespace AlgoDebug
#endif // LOCAL_LEVEL > 0

//===---------------------------------------------------------------------===//
//============================ MAIN DEBUG MACROS ============================//

#if LOCAL_LEVEL >= 1
#define _debug_print(level_color, ...)                                                                                                     \
  do {                                                                                                                                     \
    std::cerr << level_color << "[" << __FILE__ << ":" << __LINE__ << " (" << __func__ << ")] " << RESET << MAGENTA << #__VA_ARGS__        \
              << " = " << CYAN << "[";                                                                                                     \
    AlgoDebug::print_recursive_helper(__VA_ARGS__);                                                                                        \
    std::cerr << RESET << std::flush;                                                                                                      \
  } while (0)
#define debug(...) _debug_print(YELLOW, __VA_ARGS__)
#else
#define debug(...) ((void)0)
#endif

#if LOCAL_LEVEL >= 2
#define debug2(...) _debug_print(BLUE, __VA_ARGS__)
#else
#define debug2(...) ((void)0)
#endif

#if LOCAL_LEVEL > 0
#define debug_line() std::cerr << BLUE << "/====--------------------------------------------------====/" << RESET << "\n";
#define debug_if(cond, ...)                                                                                                                \
  if (cond) {                                                                                                                              \
    debug(__VA_ARGS__);                                                                                                                    \
  }
#define my_assert(condition)                                                                                                               \
  if (!(condition)) {                                                                                                                      \
    std::cerr << RED << "Assertion Failed: (" << #condition << ") at " << __FILE__ << ":" << __LINE__ << " in " << __func__ << RESET       \
              << "\n";                                                                                                                     \
    abort();                                                                                                                               \
  }
#else
#define debug_line() 42
#define debug_if(cond, ...) 42
#define my_assert(condition) ((void)0)
#endif

#if LOCAL_LEVEL >= 2
#define COUNT_CALLS(func_name)                                                                                                             \
  static int func_name##_call_count = 0;                                                                                                   \
  func_name##_call_count++;                                                                                                                \
  debug2(#func_name, "call number", func_name##_call_count);
#else
#define COUNT_CALLS(func_name)
#endif

//===---------------------------------------------------------------------===//
//============================= UTILITY: TIMER ==============================//

#if LOCAL_LEVEL > 0
// Timer class
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

// Redirects stderr to a file for logging
inline void init_debug_log() {
  if (!freopen("debug_output.txt", "w", stderr)) {
    std::cerr << "Error redirecting stderr to file" << std::endl;
  }
}
#else
// Define empty Timer and init function for release builds
class Timer {};
inline void init_debug_log() {
}
#endif

#endif // ALGO_DEBUG_H

//===---------------------------------------------------------------------===//s