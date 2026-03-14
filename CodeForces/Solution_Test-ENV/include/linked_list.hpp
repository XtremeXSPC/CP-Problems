//===--------------------------------------------------------------------------===//
#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

// LinkedList for testing LLDB formatters.
template <typename T>
class LinkedList {
private:
  struct Node {
    T val;                       // formatter looks for 'val'
    std::unique_ptr<Node> next;  // formatter handles unique_ptr

    Node(T value) : val(std::move(value)), next(nullptr) {}
  };

  std::unique_ptr<Node> head;  // formatter looks for 'head'
  size_t count{};              // formatter looks for 'count' (not 'size')

  template <typename PtrType>
  static auto ptr_to_id(PtrType* ptr) -> std::string {
    std::ostringstream ss;
    ss << ptr;
    return ss.str();
  }

public:
  LinkedList() : head(nullptr), count(0) {}

  void push_front(T value) {
    auto new_node  = std::make_unique<Node>(std::move(value));
    new_node->next = std::move(head);
    head           = std::move(new_node);
    count++;
  }

  void print_to_console() const {
    Node* current = head.get();
    while (current) {
      std::cout << current->val << " -> ";
      current = current->next.get();
    }
    std::cout << "nullptr" << '\n';
  }

  [[nodiscard]] auto generateJson() const -> std::string;
};

template <typename T>
inline auto LinkedList<T>::generateJson() const -> std::string {
  std::ostringstream nodes_ss, edges_ss;

  bool first_node = true;
  bool first_edge = true;

  for (Node* current = head.get(); current != nullptr; current = current->next.get()) {
    if (!first_node) nodes_ss << ",";
    nodes_ss << "{\"id\":\"" << ptr_to_id(current) << "\",\"label\":\"" << current->val << "\"}";
    first_node = false;

    if (current->next) {
      if (!first_edge) edges_ss << ",";
      edges_ss << "{\"from\":\"" << ptr_to_id(current) << "\",\"to\":\"" << ptr_to_id(current->next.get()) << "\"}";
      first_edge = false;
    }
  }

  std::ostringstream final_json;
  final_json << "{\"kind\":{\"graph\":true},\"nodes\":[" << nodes_ss.str() << "],\"edges\":[" << edges_ss.str() << "]}";
  return final_json.str();
}

//===--------------------------------------------------------------------------===//
