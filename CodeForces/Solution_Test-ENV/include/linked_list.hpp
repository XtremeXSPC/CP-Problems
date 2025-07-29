//===--------------------------------------------------------------------------===//
#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#pragma once

#include <iostream>
#include <memory>

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

#endif // LINKED_LIST_HPP
//===--------------------------------------------------------------------------===//
