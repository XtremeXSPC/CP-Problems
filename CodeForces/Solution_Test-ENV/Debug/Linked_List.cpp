//===---------------------------------------------------------------------===//
/**
 * @file Linked_List.cpp
 * @brief Tester for LLDB Data Formatters.
 * @version 0.1
 * @date 2025-07-27
 *
 * @copyright Copyright MIT 2025
 *
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include "../include/linked_list.hpp" // Include the custom LinkedList header
#include <iostream>
#include <string>

// #include "../../Algorithms/libs/debug.h"

//===---------------------------------------------------------------------===//
/* Definitions of Structures and Classes */

//===---------------------------------------------------------------------===//
/* Main function */
auto main() -> int {
  // The type name "LinkedList" should be recognized by our regex
  LinkedList<int> list;
  list.push_front(10);
  list.push_front(20);
  list.push_front(30);
  list.push_front(40);
  list.push_front(50);
  list.push_front(60);
  list.push_front(70);
  list.push_front(80);
  list.push_front(90);

  LinkedList<std::string> string_list;
  string_list.push_front("C++");
  string_list.push_front("Python");
  string_list.push_front("LLDB");
  string_list.push_front("Rust");
  string_list.push_front("Go");
  string_list.push_front("JavaScript");
  string_list.push_front("TypeScript");
  string_list.push_front("Kotlin");
  string_list.push_front("Swift");
  string_list.push_front("Scala");

  std::cout << "Program ready for debugging." << '\n';
  std::cout << "Set a breakpoint on the next line to inspect the variables." << '\n';

  // Set a breakpoint here to test the formatters
  int breakpoint_line = 0;

  // This line is only to use the variable and avoid compiler warnings
  (void)breakpoint_line;

  return 0;
}

//===---------------------------------------------------------------------===//
