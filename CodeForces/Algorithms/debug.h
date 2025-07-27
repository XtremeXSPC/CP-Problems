#ifndef ALGO_DEBUG_H
#define ALGO_DEBUG_H

#include <algorithm>
#include <bitset>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

//===---------------------------------------------------------------------===//
//============================= CONFIGURATION  ==============================//
// clang-format off

// Define `LOCAL` with a value to control debug level, e.g., g++ -DLOCAL=2
// If only -DLOCAL is used (no value), it defaults to 1.
#ifdef LOCAL
  #define DEBUG_ENABLED
  // Assign the value of the LOCAL macro (from the -D flag) to LOCAL_LEVEL
  #define LOCAL_LEVEL LOCAL
#else
  // If the -DLOCAL flag is not provided at all, disable debugging.
  #define LOCAL_LEVEL 0
#endif

//===---------------------------------------------------------------------===//
//=============================== ANSI COLORS ===============================//

// To make terminal output more readable.
#if LOCAL_LEVEL > 0
  #define RESET   "\033[0m"
  #define RED     "\033[31m"
  #define GREEN   "\033[32m"
  #define YELLOW  "\033[33m"
  #define BLUE    "\033[34m"
  #define MAGENTA "\033[35m"
  #define CYAN    "\033[36m"
#else
  #define RESET   ""
  #define RED     ""
  #define GREEN   ""
  #define YELLOW  ""
  #define BLUE    ""
  #define MAGENTA ""
  #define CYAN    ""
#endif

// clang-format on
//===---------------------------------------------------------------------===//
//====================== DEBUGGER CORE IMPLEMENTATION =======================//

#if LOCAL_LEVEL > 0
namespace AlgoDebug {

  // Function to get thread-local visited pointers set
  // This avoids ODR violations by using a function-local static variable
  inline std::set<const void*>& get_visited_pointers() {
    thread_local std::set<const void*> visited_pointers;
    return visited_pointers;
  }

  // Forward declarations to resolve template dependencies
  template <typename T>
  auto print_helper(const T& value) -> decltype(std::begin(value), void());

  template <typename T, typename... Args>
  void print_recursive_helper(T first, Args... rest);

  // Base cases for fundamental types
  inline void print_helper(int number) {
    std::cerr << number;
  }
  inline void print_helper(long number) {
    std::cerr << number;
  }
  inline void print_helper(long long number) {
    std::cerr << number;
  }
  inline void print_helper(unsigned number) {
    std::cerr << number;
  }
  inline void print_helper(unsigned long number) {
    std::cerr << number;
  }
  inline void print_helper(unsigned long long number) {
    std::cerr << number;
  }
  inline void print_helper(float decimal) {
    std::cerr << decimal;
  }
  inline void print_helper(double decimal) {
    std::cerr << decimal;
  }
  inline void print_helper(long double decimal) {
    std::cerr << decimal;
  }
  inline void print_helper(char character) {
    std::cerr << '\'' << character << '\'';
  }
  inline void print_helper(const char* str) {
    std::cerr << '"' << str << '"';
  }
  inline void print_helper(const std::string& str) {
    std::cerr << '"' << str << '"';
  }
  inline void print_helper(bool flag) {
    std::cerr << (flag ? "true" : "false");
  }
  template <size_t N>
  void print_helper(const std::bitset<N>& bits) {
    std::cerr << bits;
  }

  // Raw pointer printing
  template <typename T>
  void print_helper(T* ptr) {
    if (ptr == nullptr) {
      std::cerr << "nullptr";
      return;
    }
    // If the pointer has already been visited in this call, print a cycle warning and stop.
    if (get_visited_pointers().count(ptr)) {
      std::cerr << RED << "[CYCLE DETECTED AT " << ptr << "]" << CYAN;
      return;
    }
    get_visited_pointers().insert(ptr);
    std::cerr << "&";   // Indicates it's a pointer
    print_helper(*ptr); // Dereference and print the value
  }

  // Smart pointer printing
  template <typename T>
  void print_helper(const std::unique_ptr<T>& ptr) {
    print_helper(ptr.get());
  }
  template <typename T>
  void print_helper(const std::shared_ptr<T>& ptr) {
    print_helper(ptr.get());
  }

  // std::pair printing
  template <typename T, typename U>
  void print_helper(const std::pair<T, U>& pair_obj) {
    std::cerr << '{';
    print_helper(pair_obj.first);
    std::cerr << ", ";
    print_helper(pair_obj.second);
    std::cerr << '}';
  }

  // std::tuple printing
  template <typename T, std::size_t... Is>
  void print_tuple(const T& tuple_obj, std::index_sequence<Is...>) {
    std::cerr << "{";
    int separator_flag = 0;
    ((std::cerr << (separator_flag++ ? ", " : ""), print_helper(std::get<Is>(tuple_obj))), ...);
    std::cerr << "}";
  }
  template <typename... Args>
  void print_helper(const std::tuple<Args...>& tuple_obj) {
    print_tuple(tuple_obj, std::make_index_sequence<sizeof...(Args)>());
  }

  // Matrix (vector of vector) printing
  template <typename T>
  void print_helper(const std::vector<std::vector<T>>& matrix) {
    std::cerr << "{\n";
    for (const auto& row : matrix) {
      std::cerr << "  "; // Indent rows
      print_helper(row);
      std::cerr << "\n";
    }
    std::cerr << "}";
  }

  // Generic template for iterable containers (vector, set, map, list, etc.)
  template <typename T, typename = decltype(std::begin(std::declval<T>())), typename = std::enable_if_t<!std::is_same_v<T, std::string>>>
  void print_iterable(const T& iterable) {
    std::cerr << '{';
    auto it = std::begin(iterable);
    if (it != std::end(iterable)) {
      print_helper(*it);
      ++it;
    }
    while (it != std::end(iterable)) {
      std::cerr << ", ";
      print_helper(*it);
      ++it;
    }
    std::cerr << '}';
  }

  // SFINAE to call the printer for iterables.
  // This is now the primary template for iterable types.
  template <typename T>
  auto print_helper(const T& value) -> decltype(std::begin(value), void()) {
    // Exclude strings to avoid printing them as a char array
    if constexpr (!std::is_same_v<T, std::string>) {
      print_iterable(value);
    }
  }

  // Specializations for non-iterable containers (copied for printing)
  template <typename T, typename C>
  void print_helper(std::queue<T, C> queue_copy) {
    std::cerr << "{";
    bool first = true;
    while (!queue_copy.empty()) {
      if (!first)
        std::cerr << ", ";
      print_helper(queue_copy.front());
      queue_copy.pop();
      first = false;
    }
    std::cerr << "}";
  }

  template <typename T, typename C>
  void print_helper(std::stack<T, C> stack_copy) {
    std::vector<T> temp;
    while (!stack_copy.empty()) {
      temp.push_back(stack_copy.top());
      stack_copy.pop();
    }
    std::reverse(temp.begin(), temp.end());
    print_helper(temp);
  }

  template <typename T, typename Container, typename Compare>
  void print_helper(std::priority_queue<T, Container, Compare> pq_copy) {
    std::vector<T> temp;
    while (!pq_copy.empty()) {
      temp.push_back(pq_copy.top());
      pq_copy.pop();
    }
    print_helper(temp);
  }

  // Variadic recursive function to handle multiple arguments
  inline void print_recursive_helper() {
    std::cerr << "]" << std::endl;
  }

  template <typename T, typename... V>
  void print_recursive_helper(T first, V... rest) {
    print_helper(first);
    if (sizeof...(rest))
      std::cerr << ", ";
    print_recursive_helper(rest...);
  }

  //===------------------------------ TREES ------------------------------===//

  // Visual Binary Tree printing - OLD
  // SFINAE to detect if a type T has 'value', 'left', and 'right' members
  template <typename T, typename = void>
  struct has_tree_members : std::false_type {};
  template <typename T>
  struct has_tree_members<
      T,
      std::void_t<decltype(std::declval<T>().value), decltype(std::declval<T>().left), decltype(std::declval<T>().right)>>
      : std::true_type {};

  // Generic Visual Tree Printing
  // Helper traits to detect common member names
  template <typename T, typename = void>
  struct has_member_value : std::false_type {};
  template <typename T>
  struct has_member_value<T, std::void_t<decltype(std::declval<T>().value)>> : std::true_type {};
  template <typename T, typename = void>
  struct has_member_val : std::false_type {};
  template <typename T>
  struct has_member_val<T, std::void_t<decltype(std::declval<T>().val)>> : std::true_type {};
  template <typename T, typename = void>
  struct has_member_key : std::false_type {};
  template <typename T>
  struct has_member_key<T, std::void_t<decltype(std::declval<T>().key)>> : std::true_type {};
  template <typename T, typename = void>
  struct has_member_data : std::false_type {};
  template <typename T>
  struct has_member_data<T, std::void_t<decltype(std::declval<T>().data)>> : std::true_type {};
  template <typename T, typename = void>
  struct has_member_left : std::false_type {};
  template <typename T>
  struct has_member_left<T, std::void_t<decltype(std::declval<T>().left)>> : std::true_type {};
  template <typename T, typename = void>
  struct has_member_right : std::false_type {};
  template <typename T>
  struct has_member_right<T, std::void_t<decltype(std::declval<T>().right)>> : std::true_type {};
  template <typename T, typename = void>
  struct has_member_children : std::false_type {};
  template <typename T>
  struct has_member_children<T, std::void_t<decltype(std::begin(std::declval<T>().children))>> : std::true_type {};

  // Helper to get raw pointer from raw, unique_ptr, or shared_ptr
  template <typename T>
  T* get_raw_pointer(T* ptr) {
    return ptr;
  }
  template <typename T>
  T* get_raw_pointer(const std::unique_ptr<T>& ptr) {
    return ptr.get();
  }
  template <typename T>
  T* get_raw_pointer(const std::shared_ptr<T>& ptr) {
    return ptr.get();
  }

  template <typename T>
  void print_node_structure(T* node, const std::string& prefix, bool is_tail) {
    if (!node)
      return;

    std::cerr << prefix << (is_tail ? "└── " : "├── ");

    // Generic value printing: tries common names
    if constexpr (has_member_value<T>::value) {
      print_helper(node->value);
    } else if constexpr (has_member_val<T>::value) {
      print_helper(node->val);
    } else if constexpr (has_member_key<T>::value) {
      print_helper(node->key);
    } else if constexpr (has_member_data<T>::value) {
      print_helper(node->data);
    } else {
      std::cerr << "(Custom Node)";
    } // Fallback
    std::cerr << "\n";

    std::string child_prefix = prefix + (is_tail ? "    " : "│   ");

    // Generic children printing: checks for N-ary structure first, then binary
    if constexpr (has_member_children<T>::value) { // N-ary trees (e.g., vector<Node*>)
      auto& children = node->children;
      for (auto it = std::begin(children); it != std::end(children); ++it) {
        bool is_last_child = (std::next(it) == std::end(children));
        print_node_structure(get_raw_pointer(*it), child_prefix, is_last_child);
      }
    } else if constexpr (has_member_left<T>::value && has_member_right<T>::value) { // Binary trees
      T* left_child  = get_raw_pointer(node->left);
      T* right_child = get_raw_pointer(node->right);

      if (left_child && right_child) {
        print_node_structure(right_child, child_prefix, false);
        print_node_structure(left_child, child_prefix, true);
      } else if (right_child) {
        print_node_structure(right_child, child_prefix, true);
      } else if (left_child) {
        print_node_structure(left_child, child_prefix, true);
      }
    }
  }

  template <typename T>
  void pretty_print_tree(T* node) {
    if (!node) {
      std::cerr << "(empty tree)\n";
      return;
    }
    print_node_structure(node, "", true);
  }

  // Overload for smart pointers in pretty_print_tree
  template <typename T>
  void pretty_print_tree(const std::shared_ptr<T>& node_ptr) {
    pretty_print_tree(node_ptr.get()); // Extract raw pointer and call the original function
  }
  template <typename T>
  void pretty_print_tree(const std::unique_ptr<T>& node_ptr) {
    pretty_print_tree(node_ptr.get());
  }

  // NEW: Generic VERBOSE Visual Tree Printing
  template <typename T>
  void print_node_structure_verbose(T* node, const std::string& prefix, bool is_tail) {
    if (!node)
      return;

    std::cerr << prefix << (is_tail ? "└── " : "├── ");

    // Print node address
    std::cerr << "[" << node << "] ";

    // Generic value printing: tries common names
    if constexpr (has_member_value<T>::value) {
      print_helper(node->value);
    } else if constexpr (has_member_val<T>::value) {
      print_helper(node->val);
    } else if constexpr (has_member_key<T>::value) {
      print_helper(node->key);
    } else if constexpr (has_member_data<T>::value) {
      print_helper(node->data);
    } else {
      std::cerr << "(Custom Node)";
    } // Fallback
    std::cerr << "\n";

    std::string child_prefix = prefix + (is_tail ? "    " : "│   ");

    // Generic children printing
    if constexpr (has_member_children<T>::value) { // N-ary trees
      auto& children = node->children;
      for (auto it = std::begin(children); it != std::end(children); ++it) {
        bool is_last_child = (std::next(it) == std::end(children));
        print_node_structure_verbose(get_raw_pointer(*it), child_prefix, is_last_child);
      }
    } else if constexpr (has_member_left<T>::value && has_member_right<T>::value) { // Binary trees
      T* left_child  = get_raw_pointer(node->left);
      T* right_child = get_raw_pointer(node->right);

      if (left_child && right_child) {
        print_node_structure_verbose(right_child, child_prefix, false);
        print_node_structure_verbose(left_child, child_prefix, true);
      } else if (right_child) {
        print_node_structure_verbose(right_child, child_prefix, true);
      } else if (left_child) {
        print_node_structure_verbose(left_child, child_prefix, true);
      }
    }
  }

  template <typename T>
  void pretty_print_tree_verbose(T* node) {
    if (!node) {
      std::cerr << "(empty tree)\n";
      return;
    }
    print_node_structure_verbose(node, "", true);
  }

  // Overloads for smart pointers
  template <typename T>
  void pretty_print_tree_verbose(const std::shared_ptr<T>& node_ptr) {
    pretty_print_tree_verbose(node_ptr.get());
  }
  template <typename T>
  void pretty_print_tree_verbose(const std::unique_ptr<T>& node_ptr) {
    pretty_print_tree_verbose(node_ptr.get());
  }

} // namespace AlgoDebug
#endif // LOCAL_LEVEL > 0

//===---------------------------------------------------------------------===//
//============================ MAIN DEBUG MACROS ============================//
// clang-format off

#if LOCAL_LEVEL >= 1
  #define _debug_print(level_color, ...)                                                                                                     \
    do {                                                                                                                                     \
      AlgoDebug::get_visited_pointers().clear(); /* Clears visited pointers before each print */                                                    \
      std::cerr << level_color << "[" << __FILE__ << ":" << __LINE__ << " (" << __func__ << ")] " << RESET << MAGENTA << #__VA_ARGS__        \
                << " = " << CYAN << "[";                                                                                                     \
      AlgoDebug::print_recursive_helper(__VA_ARGS__);                                                                                        \
      std::cerr << RESET << std::flush;                                                                                                      \
    } while (0)
  #define debug(...) _debug_print(YELLOW, __VA_ARGS__)
#else
  #define debug(...) ((void)0)
#endif

#if LOCAL_LEVEL >= 2
  #define debug2(...) _debug_print(BLUE, __VA_ARGS__)
#else
  #define debug2(...) ((void)0)
#endif

// Macro for tree printing
#if LOCAL_LEVEL > 0
  #define debug_tree(root)                                                                                                                   \
    do {                                                                                                                                     \
      std::cerr << YELLOW << "\n[" << __FILE__ << ":" << __LINE__ << " (" << __func__ << ")] " << RESET << MAGENTA << #root << " Tree:\n"     \
                << CYAN;                                                                                                                     \
      AlgoDebug::pretty_print_tree(root);                                                                                                    \
      std::cerr << RESET << std::flush;                                                                                                      \
    } while (0)
#else
  #define debug_tree(...) ((void)0)
#endif

// Macro for tree printing - Level Verbose
#if LOCAL_LEVEL > 0
  #define debug_tree_verbose(root)                                                                                                           \
    do {                                                                                                                                     \
      std::cerr << YELLOW << "\n[" << __FILE__ << ":" << __LINE__ << " (" << __func__ << ")] " << RESET << MAGENTA << #root << " Verbose Tree:\n" \
                << CYAN;                                                                                                                     \
      AlgoDebug::pretty_print_tree_verbose(root);                                                                                            \
      std::cerr << RESET << std::flush;                                                                                                      \
    } while (0)
#else
  #define debug_tree_verbose(...) ((void)0)
#endif

#if LOCAL_LEVEL > 0
  #define debug_line() std::cerr << BLUE << "/====--------------------------------------------------====/" << RESET << "\n";
  #define debug_if(cond, ...) if (cond) { debug(__VA_ARGS__); }
  #define my_assert(condition)                                                                                                               \
    if (!(condition)) {                                                                                                                      \
      std::cerr << RED << "Assertion Failed: (" << #condition << ") at " << __FILE__ << ":" << __LINE__ << " in " << __func__ << RESET       \
                << "\n";                                                                                                                     \
      abort();                                                                                                                               \
    }
#else
  #define debug_line() ((void)0)
  #define debug_if(cond, ...) ((void)0)
  #define my_assert(condition) ((void)0)
#endif

#if LOCAL_LEVEL >= 2
  #define COUNT_CALLS(func_name)                                                                                                             \
    static int func_name##_call_count = 0;                                                                                                   \
    func_name##_call_count++;                                                                                                                \
    debug2(#func_name, "call number", func_name##_call_count);
#else
  #define COUNT_CALLS(func_name)
#endif

// clang-format on
//===---------------------------------------------------------------------===//
//============================= UTILITY: TIMER ==============================//

#if LOCAL_LEVEL > 0
class Timer {
public:
  Timer() : start_time(std::chrono::high_resolution_clock::now()) {}
  ~Timer() {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cerr << GREEN << "\n[Timer] Execution Time: " << duration.count() << " ms" << RESET << std::endl;
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};

inline void init_debug_log() {
  if (!freopen("debug_output.txt", "w", stderr)) {
    std::cerr << "Error redirecting stderr to file" << std::endl;
  }
}
#else
class Timer {};
inline void init_debug_log() {
}
#endif

#endif // ALGO_DEBUG_H

//===---------------------------------------------------------------------===//
