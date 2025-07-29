//===---------------------------------------------------------------------===//
/**
 * @file debug_Linked-List.cpp
 * @brief Tester for LLDB Data Formatters.
 * @version 0.1
 * @date 2025-07-27
 *
 * @copyright Copyright MIT 2025
 *
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include <iostream>
#include <memory>
#include <string>

#include "include/tree.h"

//===---------------------------------------------------------------------===//
/* Definitions of Structures and Classes */

//===---------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  std::cout << "Creating a sample binary tree..." << '\n';

  // The variable we will inspect in LLDB
  Tree<int> my_binary_tree;

  // Insert some values to create a more complex, non-trivial tree
  my_binary_tree.insert(8); // root
  my_binary_tree.insert(3);
  my_binary_tree.insert(10);
  my_binary_tree.insert(1);
  my_binary_tree.insert(6);
  my_binary_tree.insert(4);
  my_binary_tree.insert(7);
  my_binary_tree.insert(14);
  my_binary_tree.insert(13);

  // Additional nodes for complexity
  my_binary_tree.insert(9);
  my_binary_tree.insert(2);
  my_binary_tree.insert(5);
  my_binary_tree.insert(12);
  my_binary_tree.insert(15);
  my_binary_tree.insert(11);
  my_binary_tree.insert(0);
  my_binary_tree.insert(16);
  my_binary_tree.insert(17);
  my_binary_tree.insert(18);

  std::cout << "Tree created and populated." << '\n';

  return 0; // Breakpoint here!
}

//===---------------------------------------------------------------------===//
