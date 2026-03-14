//===--------------------------------------------------------------------------===//
#pragma once

#include <memory>
#include <sstream>
#include <string>

// Note: member names 'value', 'left', 'right' are referenced by the LLDB script.
template <typename T>
struct TreeNode {
  T                            value;
  std::unique_ptr<TreeNode<T>> left;
  std::unique_ptr<TreeNode<T>> right;

  TreeNode(const T& val) : value(val), left(nullptr), right(nullptr) {}
};

// Note: member names 'root' and 'm_size' are referenced by the LLDB script.
template <typename T>
class Tree {
public:
  Tree() : root(nullptr), m_size(0) {}

  void insert(const T& value) { _insert(root, value); }

  [[nodiscard]] auto generateJson() const -> std::string {
    std::ostringstream nodes_ss, edges_ss;
    bool               first_node = true;
    bool               first_edge = true;

    _generateJson(root.get(), nodes_ss, edges_ss, first_node, first_edge);

    std::ostringstream final_json;
    final_json << "{\"kind\":{\"tree\":true},\"nodes\":[" << nodes_ss.str() << "],\"edges\":[" << edges_ss.str() << "]}";
    return final_json.str();
  }

private:
  void _insert(std::unique_ptr<TreeNode<T>>& node, const T& value) {
    if (!node) {
      node = std::make_unique<TreeNode<T>>(value);
      m_size++;
    } else if (value < node->value) {
      _insert(node->left, value);
    } else {
      _insert(node->right, value);
    }
  }

  static auto ptr_to_id(TreeNode<T>* ptr) -> std::string;

  void _generateJson(TreeNode<T>* node, std::ostringstream& nodes_ss, std::ostringstream& edges_ss,
                     bool& first_node, bool& first_edge) const {
    if (!node) return;

    if (!first_node) nodes_ss << ",";
    first_node = false;
    nodes_ss << "{\"id\":\"" << ptr_to_id(node) << "\",\"label\":\"" << node->value << "\"}";

    if (node->left) {
      if (!first_edge) edges_ss << ",";
      edges_ss << "{\"from\":\"" << ptr_to_id(node) << "\",\"to\":\"" << ptr_to_id(node->left.get()) << "\"}";
      first_edge = false;
    }
    if (node->right) {
      if (!first_edge) edges_ss << ",";
      edges_ss << "{\"from\":\"" << ptr_to_id(node) << "\",\"to\":\"" << ptr_to_id(node->right.get()) << "\"}";
      first_edge = false;
    }

    _generateJson(node->left.get(),  nodes_ss, edges_ss, first_node, first_edge);
    _generateJson(node->right.get(), nodes_ss, edges_ss, first_node, first_edge);
  }

  std::unique_ptr<TreeNode<T>> root;
  size_t                       m_size;
};

template <typename T>
inline auto Tree<T>::ptr_to_id(TreeNode<T>* ptr) -> std::string {
  std::ostringstream ss;
  ss << ptr;
  return ss.str();
}

//===--------------------------------------------------------------------------===//
