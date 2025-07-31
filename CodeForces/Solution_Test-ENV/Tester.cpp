//===---------------------------------------------------------------------===//
/**
 * @file Tester.cpp
 * @brief Tester for coding data structures and algorithms.
 * @version 0.1
 * @date 2025-05-20
 *
 * @copyright Copyright MIT 2025
 *
 */
//===---------------------------------------------------------------------===//
/* Included library */

#include "include/graph.hpp"
#include "include/linked_list.hpp"
#include "include/tree.hpp"

#include <iostream>

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
  string_list.push_front("Haskell");
  string_list.push_front("Rust");
  string_list.push_front("OCaml");
  string_list.push_front("Erlang");
  string_list.push_front("TypeScript");
  string_list.push_front("Kotlin");
  string_list.push_front("Swift");
  string_list.push_front("Scala");

  // Print the linked list to console
  std::cout << "Program ready for debugging." << '\n';

  // Create a sample binary tree to test the Tree formatter
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

  std::cout << "Tree created and populated.\n" << '\n';

  // Create a graph of integers.
  // The name "my_std_graph" can be used directly in LLDB commands.
  StandardGraph<int> my_std_graph;

  // Add nodes using the helper function
  TestGraphNode<int>* node10  = create_node(my_std_graph, 10);
  TestGraphNode<int>* node20  = create_node(my_std_graph, 20);
  TestGraphNode<int>* node30  = create_node(my_std_graph, 30);
  TestGraphNode<int>* node40  = create_node(my_std_graph, 40);
  TestGraphNode<int>* node50  = create_node(my_std_graph, 50);
  TestGraphNode<int>* node60  = create_node(my_std_graph, 60);
  TestGraphNode<int>* node70  = create_node(my_std_graph, 70);
  TestGraphNode<int>* node80  = create_node(my_std_graph, 80);
  TestGraphNode<int>* node90  = create_node(my_std_graph, 90);
  TestGraphNode<int>* node100 = create_node(my_std_graph, 100);

  (void)node100; // Use node100 to avoid unused variable warning

  // Add edges
  add_edge(my_std_graph, node10, node20);
  add_edge(my_std_graph, node10, node30);
  add_edge(my_std_graph, node20, node40);
  add_edge(my_std_graph, node30, node40);
  add_edge(my_std_graph, node40, node50);
  add_edge(my_std_graph, node30, node10); // Edge for cycle
  add_edge(my_std_graph, node50, node60);
  add_edge(my_std_graph, node60, node70);
  add_edge(my_std_graph, node70, node80);
  add_edge(my_std_graph, node80, node90);
  add_edge(my_std_graph, node90, node10); // Another cycle
  add_edge(my_std_graph, node20, node60);
  add_edge(my_std_graph, node30, node70);
  add_edge(my_std_graph, node40, node80);

  std::cout << "StandardGraph created." << '\n';

  // ================================================================== //
  // 2. GENERATE AND PRINT THE JSON FOR ONE STRUCTURE
  // ================================================================== //

  // Declare a string to hold the JSON output.
  std::string json_to_visualize;

  // ----- CHOOSE WHICH STRUCTURE TO VISUALIZE -----
  // Uncomment ONLY ONE of the following lines at a time.

  // json_to_visualize = list.generateJson();
  // json_to_visualize = string_list.generateJson();
  json_to_visualize = my_binary_tree.generateJson();
  // json_to_visualize = generateGraphJson(my_std_graph);

  // Print the chosen JSON string to standard output.
  // The 'vscode-debug-visualizer' extension will capture this.
  std::cout << json_to_visualize << '\n';
  ;

  std::cout << "Program finished. Set a breakpoint on this line to see the visualization." << '\n';

  return 0;
}

//===---------------------------------------------------------------------===//
