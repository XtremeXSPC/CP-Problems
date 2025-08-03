//===--------------------------------------------------------------------------===//
#ifndef TREE_HPP
#define TREE_HPP

#pragma once

#include <memory>
#include <set>
#include <sstream>
#include <string>

// The Node structure for our Tree.
// Note the member names: 'value', 'left', 'right'. Our script looks for these.
template <typename T>
struct TreeNode {
  T                            value;
  std::unique_ptr<TreeNode<T>> left;
  std::unique_ptr<TreeNode<T>> right;

  TreeNode(const T& val) : value(val), left(nullptr), right(nullptr) {}
};

// The Tree container class.
// Note the member names: 'root' and 'm_size'.
template <typename T>
class Tree {
public:
  Tree() : root(nullptr), m_size(0) {}

  // Public method to insert a value.
  void insert(const T& value) { _insert(root, value); }

  // ================================================================== //
  // NEW: Function to generate the JSON for vscode-debug-visualizer
  // ================================================================== //
  /**
   * @brief Generates a JSON string representing the tree for visualization.
   * @return A string in the format expected by vscode-debug-visualizer.
   */
  [[nodiscard]] auto generateJson() const -> std::string {
    std::stringstream      nodes_ss, edges_ss;
    std::set<TreeNode<T>*> visited;

    _generateJson(root.get(), nodes_ss, edges_ss, visited);

    // Assemble the final JSON object. Use `kind: tree` for the correct layout.
    std::stringstream final_json;
    final_json << "{\"kind\":{\"tree\":true},\"nodes\":[" << nodes_ss.str() << "],\"edges\":[" << edges_ss.str() << "]}";
    return final_json.str();
  }

private:
  // Recursive helper to find the right spot and insert the node.
  void _insert(std::unique_ptr<TreeNode<T>>& node, const T& value) {
    if (!node) {
      // Found the spot. Create the new node.
      node = std::make_unique<TreeNode<T>>(value);
      m_size++;
    } else if (value < node->value) {
      _insert(node->left, value);
    } else {
      _insert(node->right, value);
    }
  }

  // Helper function to get a unique ID from a pointer's address
  static auto ptr_to_id(TreeNode<T>* ptr) -> std::string;

  // Private recursive helper for JSON generation
  void _generateJson(TreeNode<T>* node, std::stringstream& nodes_ss, std::stringstream& edges_ss, std::set<TreeNode<T>*>& visited) const {
    if (!node || visited.count(node)) {
      return;
    }

    // Add the current node to the JSON nodes array
    if (!visited.empty()) {
      nodes_ss << ",";
    }
    visited.insert(node);
    nodes_ss << "{\"id\":\"" << ptr_to_id(node) << "\",\"label\":\"" << node->value << "\"}";

    if (node->left) {
      if (edges_ss.tellp() > 0) {
        edges_ss << ",";
      }
      edges_ss << "{\"from\":\"" << ptr_to_id(node) << "\",\"to\":\"" << ptr_to_id(node->left.get()) << "\"}";
    }
    if (node->right) {
      if (edges_ss.tellp() > 0) {
        edges_ss << ",";
      }
      edges_ss << "{\"from\":\"" << ptr_to_id(node) << "\",\"to\":\"" << ptr_to_id(node->right.get()) << "\"}";
    }

    // Recurse on children
    _generateJson(node->left.get(), nodes_ss, edges_ss, visited);
    _generateJson(node->right.get(), nodes_ss, edges_ss, visited);
  }

  std::unique_ptr<TreeNode<T>> root;
  size_t                       m_size;
};

// Helper function to get a unique ID from a pointer's address
template <typename T>
inline auto Tree<T>::ptr_to_id(TreeNode<T>* ptr) -> std::string {
  std::stringstream ss;
  ss << ptr;
  return ss.str();
}

#endif // TREE_HPP
//===--------------------------------------------------------------------------===//
