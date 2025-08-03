//===--------------------------------------------------------------------------===//
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <sstream>
#include <string>
#include <vector>
/**
 * @brief A graph node with public members for easy introspection.
 * @tparam T Type of the value.
 */
template <typename T>
struct TestGraphNode {
  // Public members by default, fully accessible from LLDB
  T                              value;
  std::vector<TestGraphNode<T>*> neighbors;

  TestGraphNode(T val) : value(val) {}
};

/**
 * @brief A simple graph representation using standard library containers.
 *        Designed to be easily debugged.
 * @tparam T Type of the value in nodes.
 */
template <typename T>
struct StandardGraph {
  // ----- Members that LLDB formatters will look for ----- //
  // These members are public and have names your formatter already recognizes.

  //
  int num_nodes;
  int num_edges;

  // Il contenitore principale che il tuo script Python cercherà.
  std::vector<TestGraphNode<T>*> nodes;

  /**
   * @brief Constructor initializes an empty graph.
   */
  StandardGraph() : num_nodes(0), num_edges(0) {}

  /**
   * @brief Destructor cleans up dynamically allocated nodes.
   */
  ~StandardGraph() {
    for (auto* node : nodes) {
      delete node;
    }
  }
};

// ==================================================================== //
// NEW: Function to generate the JSON for vscode-debug-visualizer
// ==================================================================== //

// Helper function to get a unique ID from a pointer's address
template <typename T>
static std::string ptr_to_id(T* ptr) {
  std::stringstream ss;
  ss << ptr;
  return ss.str();
}

/**
 * @brief Generates a JSON string representing the graph for visualization.
 * @param graph The graph to visualize.
 * @return A string in the format expected by vscode-debug-visualizer.
 */
template <typename T>
auto generateGraphJson(const StandardGraph<T>& graph) -> std::string {
  std::stringstream nodes_ss, edges_ss;
  bool              first_node = true;
  bool              first_edge = true;

  for (TestGraphNode<T>* node : graph.nodes) {
    // "Comma-before" logic
    if (!first_node) {
      nodes_ss << ",";
    }
    nodes_ss << "{\"id\":\"" << ptr_to_id(node) << "\",\"label\":\"" << node->value << "\"}";
    first_node = false;

    // Add edges for neighbors
    for (TestGraphNode<T>* neighbor : node->neighbors) {
      if (!first_edge) {
        edges_ss << ",";
      }
      edges_ss << "{\"from\":\"" << ptr_to_id(node) << "\",\"to\":\"" << ptr_to_id(neighbor) << "\"}";
      first_edge = false;
    }
  }

  // Assemble the final JSON object
  std::stringstream final_json;
  final_json << "{\"kind\":{\"graph\":true},\"nodes\":[" << nodes_ss.str() << "],\"edges\":[" << edges_ss.str() << "]}";
  return final_json.str();
}

// ----- Helper functions to build the graph in main() ----- //

/**
 * @brief Helper to create a new node and add it to the graph.
 */
template <typename T>
auto create_node(StandardGraph<T>& graph, T value) -> TestGraphNode<T>* {
  auto* new_node = new TestGraphNode<T>(value);
  graph.nodes.push_back(new_node);
  graph.num_nodes++;
  return new_node;
}

/**
 * @brief Helper to add a directed edge between two nodes.
 */
template <typename T>
void add_edge(StandardGraph<T>& graph, TestGraphNode<T>* from, TestGraphNode<T>* to) {
  if (!from || !to)
    return;
  // Simple, does not check for duplicates for testing purposes
  from->neighbors.push_back(to);
  graph.num_edges++;
}

#endif // GRAPH_HPP

//===--------------------------------------------------------------------------===//
