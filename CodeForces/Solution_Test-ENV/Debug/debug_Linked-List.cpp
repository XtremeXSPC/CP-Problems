//===---------------------------------------------------------------------===//
/**
 * @file debug_Linked-List.cpp
 * @brief Tester for LLDB Data Formatters.
 * @version 0.1
 * @date 2025-07-27
 *
 * @copyright Copyright (c) 2025
 *
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <iostream>
#include <memory>
#include <string>

#include "../../Algorithms/debug.h"

//===---------------------------------------------------------------------===//
/* Definitions of Structures and Classes */

// A custom implementation of LinkedList for testing LLDB formatters.
template <typename T>
class LinkedList {
private:
  struct Node {
    T                     val;  // The formatter should find 'val'
    std::unique_ptr<Node> next; // The formatter should handle unique_ptr

    Node(T value) : val(std::move(value)), next(nullptr) {}
  };

  std::unique_ptr<Node> head;    // The formatter should find 'head'
  size_t                count{}; // The formatter should find 'count' (not 'size')

public:
  LinkedList() : head(nullptr), count(0) {}

  // Adds an element to the front of the list
  void push_front(T value) {
    auto new_node  = std::make_unique<Node>(std::move(value));
    new_node->next = std::move(head);
    head           = std::move(new_node);
    count++;
  }

  // A simple function to print the list to the console
  void print_to_console() const {
    Node* current = head.get();
    while (current) {
      std::cout << current->val << " -> ";
      current = current->next.get();
    }
    std::cout << "nullptr" << '\n';
  }
};

//===---------------------------------------------------------------------===//
/* Main function */
int main() {
  // The type name "LinkedList" should be recognized by our regex
  LinkedList<int> list;
  list.push_front(10);
  list.push_front(20);
  list.push_front(30);

  LinkedList<std::string> string_list;
  string_list.push_front("C++");
  string_list.push_front("Python");
  string_list.push_front("LLDB");

  std::cout << "Program ready for debugging." << '\n';
  std::cout << "Set a breakpoint on the next line to inspect the variables." << '\n';

  // Set a breakpoint here to test the formatters
  int breakpoint_line = 0; // <-- SET THE BREAKPOINT HERE

  // This line is only to use the variable and avoid compiler warnings
  (void)breakpoint_line;

  return 0;
}
//===---------------------------------------------------------------------===//
