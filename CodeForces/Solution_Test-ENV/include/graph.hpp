//===--------------------------------------------------------------------------===//
#pragma once

#include <sstream>
#include <string>
#include <vector>

// Graph node with public members for easy LLDB introspection.
template <typename T>
struct TestGraphNode {
  T                              value;
  std::vector<TestGraphNode<T>*> neighbors;

  TestGraphNode(T val) : value(val) {}
};

template <typename T>
struct StandardGraph {
  int num_nodes;
  int num_edges;

  // Primary container inspected by the LLDB formatter script.
  std::vector<TestGraphNode<T>*> nodes;

  StandardGraph() : num_nodes(0), num_edges(0) {}

  ~StandardGraph() {
    for (auto* node : nodes) {
      delete node;
    }
  }
};

template <typename T>
inline std::string ptr_to_id(T* ptr) {
  std::ostringstream ss;
  ss << ptr;
  return ss.str();
}

template <typename T>
auto generateGraphJson(const StandardGraph<T>& graph) -> std::string {
  std::ostringstream nodes_ss, edges_ss;
  bool               first_node = true;
  bool               first_edge = true;

  for (TestGraphNode<T>* node : graph.nodes) {
    if (!first_node) nodes_ss << ",";
    nodes_ss << "{\"id\":\"" << ptr_to_id(node) << "\",\"label\":\"" << node->value << "\"}";
    first_node = false;

    for (TestGraphNode<T>* neighbor : node->neighbors) {
      if (!first_edge) edges_ss << ",";
      edges_ss << "{\"from\":\"" << ptr_to_id(node) << "\",\"to\":\"" << ptr_to_id(neighbor) << "\"}";
      first_edge = false;
    }
  }

  std::ostringstream final_json;
  final_json << "{\"kind\":{\"graph\":true},\"nodes\":[" << nodes_ss.str() << "],\"edges\":[" << edges_ss.str() << "]}";
  return final_json.str();
}

template <typename T>
auto create_node(StandardGraph<T>& graph, T value) -> TestGraphNode<T>* {
  auto* new_node = new TestGraphNode<T>(value);
  graph.nodes.push_back(new_node);
  graph.num_nodes++;
  return new_node;
}

template <typename T>
void add_edge(StandardGraph<T>& graph, TestGraphNode<T>* from, TestGraphNode<T>* to) {
  if (!from || !to) return;
  // Does not check for duplicate edges by design.
  from->neighbors.push_back(to);
  graph.num_edges++;
}

//===--------------------------------------------------------------------------===//
