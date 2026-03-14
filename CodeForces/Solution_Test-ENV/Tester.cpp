//===---------------------------------------------------------------------===//
/**
 * @file Tester.cpp
 * @brief Tester for Pretty_LLDB custom formatters.
 * @version 0.2
 * @date 2025-05-20
 *
 * @copyright Copyright MIT 2025
 */
//===---------------------------------------------------------------------===//
/* Included Libraries */

#include "include/graph.hpp"
#include "include/linked_list.hpp"
#include "include/tree.hpp"

#include <iostream>

//===---------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
  LinkedList<int> int_list;
  for (int v : {1, 3, 5, 7, 9, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120})
    int_list.push_front(v);

  LinkedList<std::string> string_list;
  for (const char* s : {"C++", "Python", "Haskell", "Rust", "OCaml",
                        "Erlang", "TypeScript", "Kotlin", "Swift", "Scala",
                        "Go", "Zig", "Nim", "Julia", "Elixir"})
    string_list.push_front(s);

  LinkedList<double> double_list;
  for (double v : {3.14, 2.71, 1.41, 1.73, 0.57, 1.61, 2.30, 1.20, 0.69, 1.00})
    double_list.push_front(v);

  std::cout << "LinkedLists ready.\n";

  Tree<int> my_binary_tree;
  for (int v : {8, 3, 10, 1, 6, 4, 7, 14, 13, 9, 2, 5, 12, 15, 11, 0, 16, 17, 18, 19, 20})
    my_binary_tree.insert(v);

  Tree<double> double_tree;
  for (double v : {5.5, 2.2, 8.8, 1.1, 3.3, 6.6, 9.9, 0.5, 1.8, 4.4, 7.7, 10.0, 0.1})
    double_tree.insert(v);

  std::cout << "Trees ready.\n";

  StandardGraph<int> my_std_graph;

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
  TestGraphNode<int>* node110 = create_node(my_std_graph, 110);
  TestGraphNode<int>* node120 = create_node(my_std_graph, 120);

  add_edge(my_std_graph, node10,  node20);
  add_edge(my_std_graph, node10,  node30);
  add_edge(my_std_graph, node20,  node40);
  add_edge(my_std_graph, node30,  node40);
  add_edge(my_std_graph, node40,  node50);
  add_edge(my_std_graph, node30,  node10);  // back-edge (cycle)
  add_edge(my_std_graph, node50,  node60);
  add_edge(my_std_graph, node60,  node70);
  add_edge(my_std_graph, node70,  node80);
  add_edge(my_std_graph, node80,  node90);
  add_edge(my_std_graph, node90,  node10);  // back-edge (cycle)
  add_edge(my_std_graph, node20,  node60);
  add_edge(my_std_graph, node30,  node70);
  add_edge(my_std_graph, node40,  node80);
  add_edge(my_std_graph, node90,  node100);
  add_edge(my_std_graph, node100, node110);
  add_edge(my_std_graph, node110, node120);
  add_edge(my_std_graph, node120, node50);  // cross-edge
  add_edge(my_std_graph, node100, node20);  // cross-edge

  std::cout << "StandardGraph ready.\n";

  std::string json_to_visualize = int_list.generateJson();
  // std::string json_to_visualize = string_list.generateJson();
  // std::string json_to_visualize = double_list.generateJson();
  // std::string json_to_visualize = my_binary_tree.generateJson();
  // std::string json_to_visualize = double_tree.generateJson();
  // std::string json_to_visualize = generateGraphJson(my_std_graph);

  std::cout << json_to_visualize << '\n';

  std::cout << "Set a breakpoint here to inspect structures in LLDB.\n";

  return 0;
}

//===---------------------------------------------------------------------===//
