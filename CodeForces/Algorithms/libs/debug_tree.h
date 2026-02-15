//===----------------------------------------------------------------------===//
//============================ DEBUG TREE MODULE =============================//

#ifndef MODERN_DEBUG_TREE_H
#define MODERN_DEBUG_TREE_H

#include "debug_core.h"

#if DEBUG_LEVEL > 0 && CP_DEBUG_ENABLE_TREE
namespace modern_debug {

template<typename T>
concept BinaryTreeNode = requires(T node) {
  node.left;
  node.right;
};

template<typename T>
concept NaryTreeNode = requires(T node) {
  std::ranges::begin(node.children);
  std::ranges::end(node.children);
};

template<typename T>
concept TreeNode = BinaryTreeNode<T> || NaryTreeNode<T>;

class TreeVisualizer {
private:
  static constexpr std::string_view TREE_BRANCH   = "├── ";
  static constexpr std::string_view TREE_LAST     = "└── ";
  static constexpr std::string_view TREE_VERTICAL = "│   ";
  static constexpr std::string_view TREE_SPACE    = "    ";

  template<typename T>
  static auto get_node_value(const T& node) {
    if constexpr (requires { node.value; }) return node.value;
    else if constexpr (requires { node.val; }) return node.val;
    else if constexpr (requires { node.data; }) return node.data;
    else if constexpr (requires { node.key; }) return node.key;
    else return std::string("?");
  }

  template<typename T>
  static auto get_raw_ptr(T* ptr) { return ptr; }

  template<typename T>
  static auto get_raw_ptr(const std::unique_ptr<T>& ptr) { return ptr.get(); }

  template<typename T>
  static auto get_raw_ptr(const std::shared_ptr<T>& ptr) { return ptr.get(); }

public:
  template<TreeNode T>
  static void print_tree(const T* root, bool verbose = false, const std::string& title = "Tree") {
    if (!root) {
      std::cerr << colors::DIM << "(" << title << ": empty)\n" << colors::RESET;
      return;
    }

    std::cerr << colors::BRIGHT_BLUE << "╭─ " << title << " ─╮\n" << colors::RESET;
    print_node(root, "", true, verbose);
    std::cerr << "\n" << colors::RESET;
  }

  template<TreeNode T>
  static void print_tree(const std::unique_ptr<T>& root, bool verbose = false, const std::string& title = "Tree") {
    print_tree(root.get(), verbose, title);
  }

  template<TreeNode T>
  static void print_tree(const std::shared_ptr<T>& root, bool verbose = false, const std::string& title = "Tree") {
    print_tree(root.get(), verbose, title);
  }

private:
  template<TreeNode T>
  static void print_node(const T* node, const std::string& prefix, bool is_last, bool verbose) {
    if (!node) return;

    std::cerr << prefix << (is_last ? TREE_LAST : TREE_BRANCH);

    if (verbose) {
      std::cerr << colors::DIM << "[" << node << "] " << colors::RESET;
    }

    auto& formatter = get_thread_local_formatter();
    formatter.format_value(get_node_value(*node));
    std::cerr << formatter.str() << "\n";

    std::string child_prefix = prefix + std::string(is_last ? TREE_SPACE : TREE_VERTICAL);

    if constexpr (BinaryTreeNode<T>) {
      auto* left = get_raw_ptr(node->left);
      auto* right = get_raw_ptr(node->right);

      if (left && right) {
        print_node(right, child_prefix, false, verbose);
        print_node(left, child_prefix, true, verbose);
      } else if (right) {
        print_node(right, child_prefix, true, verbose);
      } else if (left) {
        print_node(left, child_prefix, true, verbose);
      }
    } else if constexpr (NaryTreeNode<T>) {
      auto& children = node->children;
      for (auto it = std::ranges::begin(children); it != std::ranges::end(children); ++it) {
        bool is_last_child = (std::next(it) == std::ranges::end(children));
        print_node(get_raw_ptr(*it), child_prefix, is_last_child, verbose);
      }
    }
  }
};

} // namespace modern_debug
#endif

#undef debug_tree
#undef debug_tree_verbose

#if DEBUG_LEVEL >= 1 && CP_DEBUG_ENABLE_TREE
  #define debug_tree(root, ...) \
    do { \
      std::cerr << colors::YELLOW << "\n┌─[" << __FILE__ << ":" << __LINE__ \
                << " (" << __func__ << ")]\n" << colors::RESET; \
      modern_debug::TreeVisualizer::print_tree(root, false, #root); \
      std::cerr.flush(); \
    } while(0)

  #define debug_tree_verbose(root, ...) \
    do { \
      std::cerr << colors::BRIGHT_YELLOW << "\n┌─[" << __FILE__ << ":" << __LINE__ \
                << " (" << __func__ << ")]\n" << colors::RESET; \
      modern_debug::TreeVisualizer::print_tree(root, true, #root " (verbose)"); \
      std::cerr.flush(); \
    } while(0)
#else
  #define debug_tree(root, ...) ((void)0)
  #define debug_tree_verbose(root, ...) ((void)0)
#endif

#endif // MODERN_DEBUG_TREE_H
