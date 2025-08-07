//===--------------------------------------------------------------------------===//
#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

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

  // Helper function to get a unique ID from a pointer's address
  template <typename PtrType>
  static auto ptr_to_id(PtrType* ptr) -> std::string {
    std::stringstream ss;
    ss << ptr;
    return ss.str();
  }

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

  // ================================================================== //
  // NEW: Function to generate the JSON for vscode-debug-visualizer
  // ================================================================== //
  /**
   * @brief Generates a JSON string representing the list for visualization.
   * @return A string in the format expected by vscode-debug-visualizer.
   */
  [[nodiscard]] auto generateJson() const -> std::string;
};

// Helper function to get a unique ID from a pointer's address
template <typename T>
inline auto LinkedList<T>::generateJson() const -> std::string {
  std::stringstream nodes_ss, edges_ss;
  bool              first_node = true;
  bool              first_edge = true;

  // Iterate through the linked list and build the JSON strings
  for (Node* current = head.get(); current != nullptr; current = current->next.get()) {
    // "Comma-before" logic: add a comma before every element except the first.
    if (!first_node) {
      nodes_ss << ",";
    }
    nodes_ss << "{\"id\":\"" << ptr_to_id(current) << "\",\"label\":\"" << current->val << "\"}";
    first_node = false;

    if (current->next) {
      if (!first_edge) {
        edges_ss << ",";
      }
      edges_ss << "{\"from\":\"" << ptr_to_id(current) << "\",\"to\":\"" << ptr_to_id(current->next.get()) << "\"}";
      first_edge = false;
    }
  }

  // Assemble the final JSON object
  std::stringstream final_json;
  final_json << "{\"kind\":{\"graph\":true},\"nodes\":[" << nodes_ss.str() << "],\"edges\":[" << edges_ss.str() << "]}";
  return final_json.str();
}

#endif // LINKED_LIST_HPP
//===--------------------------------------------------------------------------===//
