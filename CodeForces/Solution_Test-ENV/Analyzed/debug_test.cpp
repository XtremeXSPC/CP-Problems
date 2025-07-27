//===---------------------------------------------------------------------===//
/**
 * @file debug_test.cpp
 * @brief Tester for functions and macros in Algorithms/debug.h header file.
 * @version 0.1
 * @date 2025-07-27
 *
 * @copyright Copyright (c) 2025
 *
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <iostream>
#include <map>
#include <numbers>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

#ifdef LOCAL
#include "../../Algorithms/debug.h"
#else
#define debug(...) 42
#endif

//===---------------------------------------------------------------------===//
/* Function Definitions */

// Test function for basic data types
void test_basic_types() {
  debug_line();
  std::cout << "# Testing basic data types\n";
  int         a = 42;
  long long   b = 1234567890123LL;
  double      c = std::numbers::pi;
  bool        d = true;
  char        e = 'X';
  std::string f = "Hello, Debug!";

  // The debug macro can take multiple arguments of different types
  debug(a, b, c, d, e, f);
}

// Test function for standard STL containers
void test_stl_containers() {
  debug_line();
  std::cout << "\n# Testing STL Containers\n";

  std::vector<int> v = {1, 2, 3, 4, 5};
  debug(v);

  std::map<std::string, int> scores = {{"Alice", 95}, {"Bob", 88}};
  debug(scores);

  std::set<int> unique_numbers = {1, 5, 2, 1, 5, 3};
  debug(unique_numbers);

  std::pair<int, std::string> p = {10, "Points"};
  debug(p);

  std::tuple<int, char, double> t = {1, 'a', 2.5};
  debug(t);

  std::queue<int> q;
  q.push(10);
  q.push(20);
  debug(q);

  std::stack<int> s;
  s.push(30);
  s.push(40);
  debug(s);

  std::priority_queue<int> pq;
  pq.push(5);
  pq.push(15);
  pq.push(10);
  debug(pq);
}

// Test function for advanced printing features (matrices and pointers)
void test_advanced_printing() {
  debug_line();
  std::cout << "\n# Testing Advanced Printing\n";

  // Test matrix (2D vector) pretty printing
  std::vector<std::vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  debug(matrix);

  // Test pointer printing
  int  x        = 100;
  int* ptr      = &x;
  int* null_ptr = nullptr;
  debug(ptr, null_ptr);
}

// A recursive function to test COUNT_CALLS
auto fibonacci(int n) -> int {
  // This will print a message every time the function is called,
  // but only if compiled with LOCAL_LEVEL >= 2
  COUNT_CALLS(fibonacci);
  if (n <= 1)
    return n;
  return fibonacci(n - 1) + fibonacci(n - 2);
}

// Test function for the utility macros
void test_macros() {
  debug_line();
  std::cout << "\n# Testing Utility Macros\n";

  // 1. Test debug_if
  std::cout << "Testing debug_if (will print only for even numbers):\n";
  for (int i = 0; i < 5; ++i) {
    debug_if(i % 2 == 0, i, "is an even number");
  }

  // 2. Test COUNT_CALLS
  std::cout << "\nTesting COUNT_CALLS with fibonacci(4):\n";
  fibonacci(4);

  // 3. Test my_assert
  std::cout << "\nTesting my_assert (the program should NOT crash here):\n";
  int valid_value = 10;
  my_assert(valid_value > 0);
  std::cout << "Assertion passed, as expected.\n";

  // Uncomment the line below to see my_assert in action.
  // It will print a detailed error message and terminate the program.
  // std::cout << "\nTesting a failing my_assert (program will terminate):\n";
  // int invalid_value = -5;
  // my_assert(invalid_value > 0);
}

// Test function for the Timer and logging utilities
void test_utilities() {
  debug_line();
  std::cout << "\n# Testing Utilities (Timer)\n";

  // The Timer object will automatically print the elapsed time
  // when it goes out of scope at the end of this function.
  Timer t;

  // Simulate some work
  long long sum = 0;
  for (int i = 0; i < 10000000; ++i) {
    sum += i;
  }
  // "Use" the variable in a debug call to prevent the compiler warning.
  debug("Final sum from simulated work", sum);

  std::cout << "Simulated work finished.\n";
}

//===---------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // To test file logging, uncomment the line below.
  // All debug output will be redirected to "debug_output.txt".
  // init_debug_log();

  std::cout << "Starting debug header test...\n";

  // Run all test functions
  test_basic_types();
  test_stl_containers();
  test_advanced_printing();
  test_macros();
  test_utilities();

  debug_line();
  std::cout << "\nTest finished successfully!\n";

  return 0;
}

//===---------------------------------------------------------------------===//