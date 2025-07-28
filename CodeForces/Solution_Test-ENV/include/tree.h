//===--------------------------------------------------------------------------===//
#pragma once

#include <iostream>
#include <memory>

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

  std::unique_ptr<TreeNode<T>> root;
  size_t                       m_size;
};

//===--------------------------------------------------------------------------===//
