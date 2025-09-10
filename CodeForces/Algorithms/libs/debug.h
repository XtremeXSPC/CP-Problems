//===----------------------------------------------------------------------===//
//========================= MODERN DEBUG SYSTEM C++23 ========================//
/*
 * Advanced debugging utility for competitive programming with C++23 features.
 * Features: Reflection-based printing, std::format integration, performance
 * profiling, concept constraints, memory tracking and advanced tree
 * visualization.
 *
 * Highly configurable via DEBUG_LEVEL for granular control.
 */
//===----------------------------------------------------------------------===//
// clang-format off
 
#ifndef MODERN_DEBUG_H
#define MODERN_DEBUG_H

// C++23 feature detection and includes.
#if __cplusplus >= 202302L
  #define HAS_CPP23_FEATURES
  #if __has_include(<print>)
    #include <print>
    #define HAS_STD_PRINT
  #endif
  #if __has_include(<format>)
    #include <format>
    #define HAS_STD_FORMAT
  #endif
  #if __has_include(<source_location>)
    #include <source_location>
    #define HAS_SOURCE_LOCATION
  #endif
  #if __has_include(<stacktrace>)
    // Test if stacktrace is actually usable, not just available
    #if !defined(_GLIBCXX_STACKTRACE_DISABLED) && defined(__GNUC__) && __GNUC__ >= 13
      // Try to detect if libbacktrace support is available
      #if defined(__has_attribute)
        #if __has_attribute(__gnu_inline__) || defined(__linux__)
          // Additional runtime check could be added here
          #include <stacktrace>
          #define HAS_STACKTRACE
        #endif
      #else
        #include <stacktrace>
        #define HAS_STACKTRACE
      #endif
    #endif
  #endif
#endif

// Fallback includes for broader compatibility.
#include <algorithm>
#include <bitset>
#include <chrono>
#include <cmath>
#include <concepts>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <map>
#include <memory>
#include <queue>
#include <ranges>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

//===----------------------------------------------------------------------===//
//============================== CONFIGURATION ===============================//

// Debug level configuration with enhanced granularity.
#ifdef LOCAL
  #define DEBUG_ENABLED
  #ifndef DEBUG_LEVEL
    #define DEBUG_LEVEL LOCAL
    #if !defined(LOCAL) || LOCAL == 1
      #undef DEBUG_LEVEL
      #define DEBUG_LEVEL 1
    #endif
  #endif
#else
  #ifndef DEBUG_LEVEL
    #define DEBUG_LEVEL 0
  #endif
#endif

// Feature toggles.
#if DEBUG_LEVEL > 0
  #define ENABLE_DEBUG_COLORS
  #define ENABLE_PERFORMANCE_TRACKING
  #define ENABLE_MEMORY_TRACKING
  #if DEBUG_LEVEL >= 2
    #define ENABLE_VERBOSE_DEBUG
    #define ENABLE_CALL_TRACING
  #endif
  #if DEBUG_LEVEL >= 3
    #define ENABLE_DEEP_INSPECTION
    #define ENABLE_STACKTRACE_ON_ERROR
  #endif
#endif

//===----------------------------------------------------------------------===//
//========================= MODERN ANSI COLOR SYSTEM =========================//

#ifdef ENABLE_DEBUG_COLORS
  namespace colors {
    // Standard colors.
    constexpr std::string_view RESET   = "\033[0m";
    constexpr std::string_view RED     = "\033[31m";
    constexpr std::string_view GREEN   = "\033[32m";
    constexpr std::string_view YELLOW  = "\033[33m";
    constexpr std::string_view BLUE    = "\033[34m";
    constexpr std::string_view MAGENTA = "\033[35m";
    constexpr std::string_view CYAN    = "\033[36m";
    constexpr std::string_view WHITE   = "\033[37m";
    
    // Bright variants.
    constexpr std::string_view BRIGHT_RED     = "\033[91m";
    constexpr std::string_view BRIGHT_GREEN   = "\033[92m";
    constexpr std::string_view BRIGHT_YELLOW  = "\033[93m";
    constexpr std::string_view BRIGHT_BLUE    = "\033[94m";
    constexpr std::string_view BRIGHT_MAGENTA = "\033[95m";
    constexpr std::string_view BRIGHT_CYAN    = "\033[96m";
    
    // Styles.
    constexpr std::string_view BOLD      = "\033[1m";
    constexpr std::string_view DIM       = "\033[2m";
    constexpr std::string_view ITALIC    = "\033[3m";
    constexpr std::string_view UNDERLINE = "\033[4m";
    
    // Backgrounds.
    constexpr std::string_view BG_RED    = "\033[41m";
    constexpr std::string_view BG_GREEN  = "\033[42m";
    constexpr std::string_view BG_YELLOW = "\033[43m";
    constexpr std::string_view BG_BLUE   = "\033[44m";
  }
#else
  namespace colors {
    constexpr std::string_view RESET = "";
    constexpr std::string_view RED = "";
    constexpr std::string_view GREEN = "";
    constexpr std::string_view YELLOW = "";
    constexpr std::string_view BLUE = "";
    constexpr std::string_view MAGENTA = "";
    constexpr std::string_view CYAN = "";
    constexpr std::string_view WHITE = "";
    constexpr std::string_view BRIGHT_RED = "";
    constexpr std::string_view BRIGHT_GREEN = "";
    constexpr std::string_view BRIGHT_YELLOW = "";
    constexpr std::string_view BRIGHT_BLUE = "";
    constexpr std::string_view BRIGHT_MAGENTA = "";
    constexpr std::string_view BRIGHT_CYAN = "";
    constexpr std::string_view BOLD = "";
    constexpr std::string_view DIM = "";
    constexpr std::string_view ITALIC = "";
    constexpr std::string_view UNDERLINE = "";
    constexpr std::string_view BG_RED = "";
    constexpr std::string_view BG_GREEN = "";
    constexpr std::string_view BG_YELLOW = "";
    constexpr std::string_view BG_BLUE = "";
  }
#endif

//===----------------------------------------------------------------------===//
//====================== C++23 CONCEPTS AND CONSTRAINTS ======================//

#if DEBUG_LEVEL > 0
namespace modern_debug {
  
  // Type trait concepts for better template constraints.
  template<typename T>
  concept Integral = std::integral<T>;
  
  template<typename T>
  concept FloatingPoint = std::floating_point<T>;
  
  template<typename T>
  concept Arithmetic = Integral<T> || FloatingPoint<T>;
  
  template<typename T>
  concept StringLike = std::convertible_to<T, std::string_view> ||
                       std::same_as<T, std::string> ||
                       std::same_as<T, const char*>;
  
  template<typename T>
  concept Printable = requires(T t) {
    std::cout << t;
  };
  
  template<typename T>
  concept Iterable = requires(T t) {
    std::ranges::begin(t);
    std::ranges::end(t);
  } && !StringLike<T>;
  
  template<typename T>
  concept PairLike = requires(T t) {
    t.first;
    t.second;
  };
  
  template<typename T>
  concept TupleLike = requires(T t) {
    std::get<0>(t);
    std::tuple_size<T>::value;
  } && !PairLike<T>;
  
  template<typename T>
  concept OptionalLike = requires(T t) {
    t.has_value();
    t.value();
  };
  
  template<typename T>
  concept SmartPointer = requires(T t) {
    t.get();
    t.operator->();
  };
  
  template<typename T>
  concept HasTreeStructure = requires(T t) {
    t.left;
    t.right;
  } || requires(T t) {
    t.children;
  };
  
  template<typename T>
  concept Container = Iterable<T> && requires(T t) {
    typename T::value_type;
    t.size();
  };
  
  template<typename T>
  concept Matrix = Container<T> && requires(T t) {
    typename T::value_type::value_type;
    t[0].size();
  } && !StringLike<T>;

  // Source location wrapper for better debugging info.
  #ifdef HAS_SOURCE_LOCATION
    using source_location = std::source_location;
  #else
    struct source_location {
      static constexpr source_location current() noexcept { return {}; }
      constexpr const char* file_name() const noexcept { return __FILE__; }
      constexpr const char* function_name() const noexcept { return __func__; }
      constexpr std::uint_least32_t line() const noexcept { return __LINE__; }
    };
  #endif

  //===--------------------------------------------------------------------===//
  //==================== PERFORMANCE AND MEMORY TRACKING =====================//
  
  #ifdef ENABLE_PERFORMANCE_TRACKING
  class PerformanceTracker {
  private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::unordered_map<std::string, std::chrono::nanoseconds> timings;
    std::unordered_map<std::string, size_t> call_counts;
    
  public:
    PerformanceTracker() : start_time(std::chrono::high_resolution_clock::now()) {}
    
    void start_timer(const std::string& label) {
      timings[label + "_start"] = std::chrono::high_resolution_clock::now().time_since_epoch();
    }
    
    void end_timer(const std::string& label) {
      auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
      auto start = timings[label + "_start"];
      timings[label] += (now - start);
      call_counts[label]++;
    }
    
    void print_stats() const {
      std::cerr << colors::BRIGHT_BLUE << "\n╭─-───── Performance Statistics ─────-─╮\n" << colors::RESET;
      for (const auto& [label, duration] : timings) {
        if (label.ends_with("_start")) continue;
        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        auto count = call_counts.at(label);
        std::cerr << colors::CYAN << "│ " << std::left << std::setw(20) << label 
                  << ": " << colors::YELLOW << ms << "μs" 
                  << colors::DIM << " (" << count << " calls)" << colors::RESET << "\n";
      }
      std::cerr << colors::BRIGHT_BLUE << "╰────────────-───────-─────────────────╯\n" << colors::RESET;
    }
    
    ~PerformanceTracker() {
      if constexpr (DEBUG_LEVEL >= 2) {
        print_stats();
      }
    }
  };
  
  inline PerformanceTracker& get_performance_tracker() {
    static PerformanceTracker tracker;
    return tracker;
  }
  #endif
  
  #ifdef ENABLE_MEMORY_TRACKING
  class MemoryTracker {
  private:
    size_t allocated_bytes = 0;
    size_t peak_bytes = 0;
    std::unordered_map<std::string, size_t> allocations_by_type;
    
  public:
    void record_allocation(size_t bytes, const std::string& type = "unknown") {
      allocated_bytes += bytes;
      peak_bytes = std::max(peak_bytes, allocated_bytes);
      allocations_by_type[type] += bytes;
    }
    
    void record_deallocation(size_t bytes) {
      allocated_bytes = (allocated_bytes >= bytes) ? allocated_bytes - bytes : 0;
    }
    
    void print_stats() const {
      std::cerr << colors::BRIGHT_GREEN << "\n╭─-───── Memory Statistics ─────-─╮\n" << colors::RESET;
      std::cerr << colors::GREEN << "│ Current: " << colors::YELLOW << (allocated_bytes / 1024.0) << " KB\n" << colors::RESET;
      std::cerr << colors::GREEN << "│ Peak:    " << colors::YELLOW << (peak_bytes / 1024.0) << " KB\n" << colors::RESET;
      std::cerr << colors::BRIGHT_GREEN << "╰──────────-───────────-──────────╯\n" << colors::RESET;
    }
  };
  
  inline MemoryTracker& get_memory_tracker() {
    static MemoryTracker tracker;
    return tracker;
  }
  #endif

  //===--------------------------------------------------------------------===//
  //======================= ADVANCED FORMATTING SYSTEM =======================//
  
  // Thread-local visited pointers for cycle detection.
  thread_local std::unordered_set<const void*> visited_pointers;

  // RAII guard for cycle detection.
  class VisitGuard {
    const void* ptr;
    bool inserted;
  public:
    VisitGuard(const void* p) : ptr(p), inserted(visited_pointers.insert(p).second) {}
    ~VisitGuard() { if (inserted) visited_pointers.erase(ptr); }
    bool is_cycle() const { return !inserted; }
  };

  // Modern formatter with concept-based dispatch organized by template specificity.
  class ModernFormatter {
  private:
    std::ostringstream output;
    size_t depth = 0;
    static constexpr size_t MAX_DEPTH = 10;
    static constexpr size_t MAX_CONTAINER_ELEMENTS = 100;
    
  public:
    // Fundamental types - highest specificity.
    void format_value(bool value) {
      output << colors::BOLD;
      if (value) {
        output << colors::BRIGHT_GREEN << "true";
      } else {
        output << colors::BRIGHT_RED << "false";
      }
      output << colors::RESET;
    }
    
    void format_value(char c) {
      if (std::isprint(c)) {
        output << colors::GREEN << "'" << c << "'" << colors::RESET;
      } else {
        output << colors::DIM << "'\\x" << std::hex << static_cast<unsigned char>(c) << std::dec << "'" << colors::RESET;
      }
    }
    
    void format_value(std::nullptr_t) {
      output << colors::DIM << "nullptr" << colors::RESET;
    }
    
    // Numeric types.
    template<Integral T>
    void format_value(const T& value) 
      requires (!std::same_as<T, char> && !std::same_as<T, bool>) {
      if constexpr (sizeof(T) == 1) {
        if constexpr (std::is_signed_v<T>) {
          output << static_cast<int>(value);
        } else {
          output << static_cast<unsigned int>(value);
        }
      } else {
        output << value;
      }
    }
    
    template<FloatingPoint T>
    void format_value(const T& value) {
      if (std::isnan(value)) {
        output << colors::RED << "NaN" << colors::RESET;
      } else if (std::isinf(value)) {
        output << colors::RED << (value > 0 ? "+∞" : "-∞") << colors::RESET;
      } else {
        output << std::fixed << std::setprecision(6) << value;
      }
    }
    
    // String types - must come before pointer types.
    template<StringLike T>
    void format_value(const T& str) 
      requires (!std::same_as<T, char>) {
      std::string_view sv = str;
      output << colors::GREEN << "\"" << sv << "\"" << colors::RESET;
    }
    
    // Bitset - specialized container.
    template<size_t N>
    void format_value(const std::bitset<N>& bits) {
      output << colors::CYAN << "0b" << bits << colors::RESET;
    }
    
    // Matrix containers - more specific than general containers.
    template<Matrix T>
    void format_value(const T& matrix) {
      if (depth >= MAX_DEPTH) {
        output << colors::DIM << "[...depth limit...]" << colors::RESET;
        return;
      }
      depth++;
      format_matrix(matrix);
      depth--;
    }
    
    // General iterable containers.
    template<Container T>
    void format_value(const T& container) 
      requires (!Matrix<T> && !StringLike<T>) {
      if (depth >= MAX_DEPTH) {
        output << colors::DIM << "[...depth limit...]" << colors::RESET;
        return;
      }
      depth++;
      format_container_impl(container);
      depth--;
    }
    
    // Pair types.
    template<PairLike T>
    void format_value(const T& pair) {
      output << colors::MAGENTA << "{" << colors::RESET;
      format_value(pair.first);
      output << colors::DIM << ", " << colors::RESET;
      format_value(pair.second);
      output << colors::MAGENTA << "}" << colors::RESET;
    }
    
    // Tuple types.
    template<TupleLike T>
    void format_value(const T& tuple) 
      requires (!PairLike<T>) {
      output << colors::MAGENTA << "{" << colors::RESET;
      format_tuple_impl(tuple, std::make_index_sequence<std::tuple_size_v<T>>{});
      output << colors::MAGENTA << "}" << colors::RESET;
    }
    
    // Optional-like types.
    template<OptionalLike T>
    void format_value(const T& opt) {
      if (opt.has_value()) {
        output << colors::BRIGHT_BLUE << "Some(" << colors::RESET;
        format_value(opt.value());
        output << colors::BRIGHT_BLUE << ")" << colors::RESET;
      } else {
        output << colors::DIM << "None" << colors::RESET;
      }
    }
    
    // Smart pointers.
    template<SmartPointer T>
    void format_value(const T& ptr) {
      format_value(ptr.get());
    }
    
    // Raw pointers with cycle detection.
    template<typename T>
    void format_value(T* ptr) 
      requires (!StringLike<T*>) {
      if (!ptr) {
        format_value(nullptr);
        return;
      }
      
      VisitGuard guard(ptr);
      if (guard.is_cycle()) {
        output << colors::RED << "[CYCLE@" << ptr << "]" << colors::RESET;
        return;
      }
      
      output << colors::CYAN << "&(" << colors::RESET;
      format_value(*ptr);
      output << colors::CYAN << ")" << colors::RESET;
    }
    
    // Non-copyable containers.
    template<typename T, typename Container>
    void format_value(std::queue<T, Container> queue_copy) {
      output << colors::BLUE << "queue{" << colors::RESET;
      size_t count = 0;
      while (!queue_copy.empty() && count < MAX_CONTAINER_ELEMENTS) {
        if (count > 0) output << colors::DIM << ", " << colors::RESET;
        format_value(queue_copy.front());
        queue_copy.pop();
        count++;
      }
      if (!queue_copy.empty()) output << colors::DIM << ", ..." << colors::RESET;
      output << colors::BLUE << "}" << colors::RESET;
    }
    
    template<typename T, typename Container>
    void format_value(std::stack<T, Container> stack_copy) {
      std::vector<T> temp;
      while (!stack_copy.empty()) {
        temp.push_back(stack_copy.top());
        stack_copy.pop();
      }
      output << colors::BLUE << "stack{" << colors::RESET;
      for (size_t i = 0; i < temp.size() && i < MAX_CONTAINER_ELEMENTS; ++i) {
        if (i > 0) output << colors::DIM << ", " << colors::RESET;
        format_value(temp[temp.size() - 1 - i]);
      }
      if (temp.size() > MAX_CONTAINER_ELEMENTS) output << colors::DIM << ", ..." << colors::RESET;
      output << colors::BLUE << "}" << colors::RESET;
    }
    
    // Custom structures - constrained to avoid conflicts.
    template<typename T>
    void format_value(const T& value) 
      requires (std::is_class_v<T> && !std::is_polymorphic_v<T> && 
                !Integral<T> && !FloatingPoint<T> && !StringLike<T> && 
                !Container<T> && !Matrix<T> && !PairLike<T> && !TupleLike<T> && 
                !OptionalLike<T> && !SmartPointer<T> && !Printable<T> &&
                !std::same_as<T, char> && !std::same_as<T, bool> && 
                !std::same_as<T, std::nullptr_t>) {
      output << colors::CYAN << "{";
      
      if constexpr (requires { value.to; value.weight; }) {
        output << "to: " << value.to << ", weight: " << value.weight;
      } else if constexpr (requires { value.first; value.second; }) {
        output << "first: " << value.first << ", second: " << value.second;
      } else {
        output << "custom_struct";
      }
      
      output << "}" << colors::RESET;
    }
    
    // Fallback for remaining printable types.
    template<Printable T>
    void format_value(const T& value) 
      requires (!Integral<T> && !FloatingPoint<T> && !StringLike<T> && 
                !Container<T> && !Matrix<T> && !PairLike<T> && !TupleLike<T> && 
                !OptionalLike<T> && !SmartPointer<T> &&
                !std::same_as<T, char> && !std::same_as<T, bool> && 
                !std::same_as<T, std::nullptr_t> &&
                !(std::is_class_v<T> && !std::is_polymorphic_v<T>)) {
      output << colors::WHITE << value << colors::RESET;
    }
    
    // Final fallback for unprintable types
    template<typename T>
    void format_value(const T& value) 
      requires (!Printable<T> && !Integral<T> && !FloatingPoint<T> && 
                !StringLike<T> && !Container<T> && !Matrix<T> && 
                !PairLike<T> && !TupleLike<T> && !OptionalLike<T> && 
                !SmartPointer<T> && !std::same_as<T, char> && 
                !std::same_as<T, bool> && !std::same_as<T, std::nullptr_t> &&
                !(std::is_class_v<T> && !std::is_polymorphic_v<T>)) {
      output << colors::DIM << "[unprintable:" << typeid(T).name() << "@" << &value << "]" << colors::RESET;
    }
    
    std::string str() const { return output.str(); }
    void clear() { output.str(""); output.clear(); depth = 0; }
    
  private:
    template<typename T, size_t... Is>
    void format_tuple_impl(const T& tuple, std::index_sequence<Is...>) {
      size_t count = 0;
      auto format_element = [&](const auto& element) {
        if (count++ > 0) output << colors::DIM << ", " << colors::RESET;
        format_value(element);
      };
      (format_element(std::get<Is>(tuple)), ...);
    }
    
    template<Container T>
    void format_container_impl(const T& container) {
      output << colors::BLUE << "{" << colors::RESET;
      size_t count = 0;
      for (const auto& element : container) {
        if (count >= MAX_CONTAINER_ELEMENTS) {
          output << colors::DIM << ", ..." << colors::RESET;
          break;
        }
        if (count > 0) output << colors::DIM << ", " << colors::RESET;
        format_value(element);
        count++;
      }
      output << colors::BLUE << "}" << colors::RESET;
    }
    
    template<Matrix T>
    void format_matrix(const T& matrix) {
      output << colors::BLUE << "{\n" << colors::RESET;
      size_t row_count = 0;
      for (const auto& row : matrix) {
        if (row_count >= MAX_CONTAINER_ELEMENTS) {
          output << colors::DIM << "  ...\n" << colors::RESET;
          break;
        }
        output << "  ";
        format_container_impl(row);
        output << "\n";
        row_count++;
      }
      output << colors::BLUE << "}" << colors::RESET;
    }
  };

  //===--------------------------------------------------------------------===//
  //======================= TREE VISUALIZATION SYSTEM ========================//
  
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
  
  // Enhanced tree visualizer with Unicode box drawing.
  class TreeVisualizer {
  private:
    static constexpr std::string_view TREE_BRANCH     = "├── ";
    static constexpr std::string_view TREE_LAST       = "└── ";
    static constexpr std::string_view TREE_VERTICAL   = "│   ";
    static constexpr std::string_view TREE_SPACE      = "    ";
    static constexpr std::string_view TREE_TEE        = "┬";
    static constexpr std::string_view TREE_CORNER     = "╰";
    
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
      /*
      std::cerr << colors::BRIGHT_BLUE << "╰";
      for (size_t i = 0; i < title.length() + 6; ++i) {
        std::cerr << "─";
      }
      std::cerr << "╯\n" << colors::RESET;
      */
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
      
      ModernFormatter formatter;
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

  //===--------------------------------------------------------------------===//
  //===================== MAIN DEBUG INTERFACE FUNCTIONS =====================//
  
  template<typename... Args>
  void debug_print(std::string_view level_color, std::string_view expression, 
                  const source_location& loc, Args&&... args) {
    visited_pointers.clear();
    
    #ifdef ENABLE_PERFORMANCE_TRACKING
    auto& tracker = get_performance_tracker();
    tracker.start_timer("debug_print");
    #endif
    
    // Header with source location
    std::cerr << level_color << "┌─[" << colors::RESET 
              << colors::DIM << loc.file_name() << ":" << loc.line() 
              << " (" << loc.function_name() << ")" << colors::RESET 
              << level_color << "]\n" << colors::RESET;
    
    std::cerr << level_color << "│ " << colors::RESET 
              << colors::MAGENTA << expression << colors::CYAN << " = " << colors::RESET;
    
    if constexpr (sizeof...(args) == 1) {
      ModernFormatter formatter;
      (formatter.format_value(args), ...);
      std::cerr << formatter.str();
    } else if constexpr (sizeof...(args) > 1) {
      std::cerr << colors::BLUE << "{" << colors::RESET;
      size_t count = 0;
      auto print_single = [&count](const auto& arg) {
        if (count++ > 0) std::cerr << colors::DIM << ", " << colors::RESET;
        ModernFormatter formatter;
        formatter.format_value(arg);
        std::cerr << formatter.str();
      };
      (print_single(args), ...);
      std::cerr << colors::BLUE << "}" << colors::RESET;
    }
    
    std::cerr << "\n";
    std::cerr << level_color << "└";
    for (int i = 0; i < 50; ++i) {
      std::cerr << "─";
    }
    std::cerr << "\n" << colors::RESET;
    std::cerr.flush();
      
    #ifdef ENABLE_PERFORMANCE_TRACKING
    tracker.end_timer("debug_print");
    #endif
  }

  //===--------------------------------------------------------------------===//
  //===================== ERROR HANDLING AND ASSERTIONS ======================//
  
  void handle_assertion_failure(const char* condition, const source_location& loc) {
    #if defined(HAS_STACKTRACE) && defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
    try {
      auto trace = std::stacktrace::current();
      std::cerr << colors::BG_RED << colors::WHITE << " ASSERTION FAILED " << colors::RESET << "\n";
      std::cerr << colors::RED << "Condition: " << colors::YELLOW << condition << colors::RESET << "\n";
      std::cerr << colors::RED << "Location:  " << colors::CYAN << loc.file_name() << ":" << loc.line() << colors::RESET << "\n";
      std::cerr << colors::RED << "Function:  " << colors::MAGENTA << loc.function_name() << colors::RESET << "\n\n";
      
      std::cerr << colors::BRIGHT_RED << "Stack Trace:\n" << colors::RESET;
      for (const auto& frame : trace) {
        std::cerr << colors::DIM << "  " << frame << colors::RESET << "\n";
      }
    } catch (...) {
      // Fallback if stacktrace fails at runtime.
      std::cerr << colors::BG_RED << colors::WHITE << " ASSERTION FAILED " << colors::RESET << "\n";
      std::cerr << colors::RED << "Condition: " << colors::YELLOW << condition << colors::RESET << "\n";
      std::cerr << colors::RED << "Location:  " << colors::CYAN << loc.file_name() << ":" << loc.line() 
                << " in " << colors::MAGENTA << loc.function_name() << colors::RESET << "\n";
      std::cerr << colors::YELLOW << "Note: Stacktrace unavailable (runtime error)" << colors::RESET << "\n";
    }
    #else
    // Original fallback implementation.
    std::cerr << colors::BG_RED << colors::WHITE << " ASSERTION FAILED " << colors::RESET << "\n";
    std::cerr << colors::RED << "Condition: " << colors::YELLOW << condition << colors::RESET << "\n";
    std::cerr << colors::RED << "Location:  " << colors::CYAN << loc.file_name() << ":" << loc.line() 
              << " in " << colors::MAGENTA << loc.function_name() << colors::RESET << "\n";
    #endif
    
    std::abort();
  }

} // namespace modern_debug
#endif // DEBUG_LEVEL > 0

//===----------------------------------------------------------------------===//
//=========================== PUBLIC DEBUG MACROS ============================//

// Main debug macros with automatic source location.
#if DEBUG_LEVEL >= 1
  #define debug(...) \
    modern_debug::debug_print(colors::YELLOW, #__VA_ARGS__, \
                            modern_debug::source_location::current(), __VA_ARGS__)
  
  #define debug_info(...) \
    modern_debug::debug_print(colors::BRIGHT_BLUE, #__VA_ARGS__, \
                            modern_debug::source_location::current(), __VA_ARGS__)
  
  #define debug_warn(...) \
    modern_debug::debug_print(colors::BRIGHT_YELLOW, #__VA_ARGS__, \
                            modern_debug::source_location::current(), __VA_ARGS__)
  
  #define debug_error(...) \
    modern_debug::debug_print(colors::BRIGHT_RED, #__VA_ARGS__, \
                            modern_debug::source_location::current(), __VA_ARGS__)
#else
  #define debug(...) ((void)0)
  #define debug_info(...) ((void)0)
  #define debug_warn(...) ((void)0)
  #define debug_error(...) ((void)0)
#endif

// Conditional debug.
#if DEBUG_LEVEL >= 1
  #define debug_if(condition, ...) \
    do { if (condition) { debug(__VA_ARGS__); } } while(0)
#else
  #define debug_if(condition, ...) ((void)0)
#endif

// Level 2 debug (verbose).
#if DEBUG_LEVEL >= 2
  #define debug_verbose(...) \
    modern_debug::debug_print(colors::BRIGHT_CYAN, #__VA_ARGS__, \
                            modern_debug::source_location::current(), __VA_ARGS__)
  
  #define debug_trace() \
    std::cerr << colors::DIM << "TRACE: " << __FILE__ << ":" << __LINE__ \
              << " in " << __func__ << colors::RESET << "\n"
#else
  #define debug_verbose(...) ((void)0)
  #define debug_trace() ((void)0)
#endif

// Enhanced tree visualization macros.
#if DEBUG_LEVEL >= 1
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

// Modern assertion with better error reporting.
#if DEBUG_LEVEL >= 1
  #define modern_assert(condition) \
    do { \
      if (!(condition)) { \
        modern_debug::handle_assertion_failure(#condition, modern_debug::source_location::current()); \
      } \
    } while(0)
  
  // Legacy alias.
  #define my_assert(condition) modern_assert(condition)
#else
  #define modern_assert(condition) ((void)0)
  #define my_assert(condition) ((void)0)
#endif

// Performance tracking macros
#ifdef ENABLE_PERFORMANCE_TRACKING
  #define PERF_START(label) modern_debug::get_performance_tracker().start_timer(label)
  #define PERF_END(label) modern_debug::get_performance_tracker().end_timer(label)
  #define PERF_SCOPE(label) \
    struct _perf_guard_##__LINE__ { \
      _perf_guard_##__LINE__() { PERF_START(label); } \
      ~_perf_guard_##__LINE__() { PERF_END(label); } \
    } _perf_instance_##__LINE__
#else
  #define PERF_START(label) ((void)0)
  #define PERF_END(label) ((void)0)
  #define PERF_SCOPE(label) ((void)0)
#endif

// Call counting for debugging recursive functions.
#if DEBUG_LEVEL >= 2
  #define COUNT_CALLS(func_name) \
    do { \
      static thread_local int func_name##_call_count = 0; \
      func_name##_call_count++; \
      debug_verbose(#func_name " call count", func_name##_call_count); \
    } while(0)
#else
  #define COUNT_CALLS(func_name) ((void)0)
#endif

// Visual separator.
#if DEBUG_LEVEL >= 1
  #define debug_line() \
    std::cerr << colors::BRIGHT_BLUE \
              << "\n//===----------------------------------------------------------------------===//\n\n" \
              << colors::RESET
#else
  #define debug_line() ((void)0)
#endif

//===----------------------------------------------------------------------===//
//========================== MODERN TIMER WITH RAII ==========================//

#if DEBUG_LEVEL >= 1
class ModernTimer {
private:
  std::chrono::high_resolution_clock::time_point start_time;
  std::string label;
  
public:
  ModernTimer(std::string_view name = "Execution") 
    : start_time(std::chrono::high_resolution_clock::now()), label(name) {
    std::cerr << colors::GREEN << " Timer '" << label 
              << "' started\n" << colors::RESET;
  }
  
  ~ModernTimer() {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = end_time - start_time;
    
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    
    std::cerr << colors::GREEN << " Timer '" << label << "' finished: " 
              << colors::BRIGHT_YELLOW;
    
    if (ns >= 1'000'000'000) {
      std::cerr << (ns / 1'000'000'000.0) << " s";
    } else if (ns >= 1'000'000) {
      std::cerr << (ns / 1'000'000.0) << " ms";
    } else if (ns >= 1'000) {
      std::cerr << (ns / 1'000.0) << " μs";
    } else {
      std::cerr << ns << " ns";
    }
    
    std::cerr << colors::RESET << "\n";
  }
  
  void checkpoint(std::string_view checkpoint_name = "checkpoint") {
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time).count();
    std::cerr << colors::CYAN << " " <<label << " - " << checkpoint_name 
              << ": " << colors::YELLOW << elapsed << " μs" 
              << colors::RESET << "\n";
  }
};

// Legacy alias.
using Timer = ModernTimer;

// Debug file initialization.
inline void init_debug_log() {
  #if DEBUG_LEVEL >= 1
  if (!std::freopen("debug_output.txt", "w", stderr)) {
    std::cerr << colors::RED 
              << "Warning: Could not redirect stderr to debug file" 
              << colors::RESET << std::endl;
  } else {
    std::cerr << colors::BRIGHT_GREEN 
              << "Debug output redirected to debug_output.txt" 
              << colors::RESET << std::endl;
  }
  #endif
}

#else
// Minimal timer for non-debug builds.
class ModernTimer {
public:
  ModernTimer(std::string_view = "") {}
  void checkpoint(std::string_view = "") {}
};
using Timer = ModernTimer;
inline void init_debug_log() {}
#endif

#endif // MODERN_DEBUG_H

//===----------------------------------------------------------------------===//
