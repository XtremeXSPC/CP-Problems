//===----------------------------------------------------------------------===//
//========================= MODERN DEBUG SYSTEM C++23 ========================//
/*
 * Advanced debugging utility for competitive programming with C++23 features.
 * Features: Reflection-based printing, std::format integration, performance
 * profiling, concept constraints, memory tracking, advanced tree/graph
 * visualization, watchpoints, 1-indexed printing, and multi-variable name
 * splitting.
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
    #ifndef HAS_SOURCE_LOCATION
      #define HAS_SOURCE_LOCATION
    #endif
  #endif
  #if __has_include(<stacktrace>) && !defined(HAS_STACKTRACE)
    // Test if stacktrace is actually usable, not just available.
    #if !defined(_GLIBCXX_STACKTRACE_DISABLED) && defined(__GNUC__) && __GNUC__ >= 13
      // Try to detect if libbacktrace support is available.
      #if defined(__has_attribute)
        #if __has_attribute(__gnu_inline__) || defined(__linux__)
          // Additional runtime check could be added here.
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
#include <functional>
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
#include <variant>
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

// Compile-time validation of DEBUG_LEVEL range.
#if DEBUG_LEVEL < 0 || DEBUG_LEVEL > 3
  #warning "DEBUG_LEVEL must be 0-3. Values outside this range behave as level 3."
  #undef DEBUG_LEVEL
  #define DEBUG_LEVEL 3
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

  // Concept for variant-like types.
  template<typename T>
  concept VariantLike = requires(T t) {
    t.index();
    std::variant_size<T>::value;
  };

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
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;

    time_point global_start;
    std::unordered_map<std::string, time_point> pending_starts;
    std::unordered_map<std::string, std::chrono::nanoseconds> accumulated;
    std::unordered_map<std::string, size_t> call_counts;

  public:
    PerformanceTracker() : global_start(clock::now()) {}

    void start_timer(const std::string& label) {
      pending_starts[label] = clock::now();
    }

    void end_timer(const std::string& label) {
      auto now = clock::now();
      auto it = pending_starts.find(label);
      if (it == pending_starts.end()) return;
      accumulated[label] += (now - it->second);
      call_counts[label]++;
      pending_starts.erase(it);
    }

    void print_stats() const {
      if (accumulated.empty()) return;
      std::cerr << colors::BRIGHT_BLUE << "\n╭────────── Performance Statistics ──────────╮\n" << colors::RESET;
      for (const auto& [label, duration] : accumulated) {
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
        auto count = call_counts.at(label);
        std::cerr << colors::CYAN << "│ " << std::left << std::setw(24) << label
                  << ": " << colors::YELLOW << std::right << std::setw(8) << us << "μs"
                  << colors::DIM << " (" << count << " calls, avg "
                  << (count > 0 ? us / static_cast<long long>(count) : 0) << "μs)"
                  << colors::RESET << "\n";
      }
      auto total = std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - global_start).count();
      std::cerr << colors::CYAN << "│ " << std::left << std::setw(24) << "TOTAL WALL TIME"
                << ": " << colors::BRIGHT_YELLOW << std::right << std::setw(8) << total << "μs"
                << colors::RESET << "\n";
      std::cerr << colors::BRIGHT_BLUE << "╰───────────────────────────────────────────╯\n" << colors::RESET;
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
    size_t total_allocations = 0;
    std::unordered_map<std::string, size_t> allocations_by_type;

  public:
    void record_allocation(size_t bytes, const std::string& type = "unknown") {
      allocated_bytes += bytes;
      peak_bytes = std::max(peak_bytes, allocated_bytes);
      allocations_by_type[type] += bytes;
      total_allocations++;
    }

    void record_deallocation(size_t bytes) {
      allocated_bytes = (allocated_bytes >= bytes) ? allocated_bytes - bytes : 0;
    }

    void print_stats() const {
      std::cerr << colors::BRIGHT_GREEN << "\n╭────────── Memory Statistics ──────────╮\n" << colors::RESET;
      std::cerr << colors::GREEN << "│ Current:      " << colors::YELLOW << std::setw(10) << std::fixed << std::setprecision(2) << (allocated_bytes / 1024.0) << " KB\n" << colors::RESET;
      std::cerr << colors::GREEN << "│ Peak:         " << colors::YELLOW << std::setw(10) << std::fixed << std::setprecision(2) << (peak_bytes / 1024.0) << " KB\n" << colors::RESET;
      std::cerr << colors::GREEN << "│ Allocations:  " << colors::YELLOW << std::setw(10) << total_allocations << "\n" << colors::RESET;
      if (!allocations_by_type.empty()) {
        std::cerr << colors::GREEN << "│ By type:\n" << colors::RESET;
        for (const auto& [type, bytes] : allocations_by_type) {
          std::cerr << colors::DIM << "│   " << std::left << std::setw(16) << type
                    << ": " << colors::YELLOW << std::fixed << std::setprecision(2) << (bytes / 1024.0) << " KB\n" << colors::RESET;
        }
      }
      std::cerr << colors::BRIGHT_GREEN << "╰──────────────────────────────────────╯\n" << colors::RESET;
    }
  };

  inline MemoryTracker& get_memory_tracker() {
    static MemoryTracker tracker;
    return tracker;
  }
  #endif

  //===--------------------------------------------------------------------===//
  //========================== WATCHPOINT SYSTEM =============================//

  class WatchpointRegistry {
  private:
    struct WatchEntry {
      std::string last_value;
      size_t change_count = 0;
      std::string last_location;
    };
    std::unordered_map<std::string, WatchEntry> watches;

  public:
    // Returns true if the value changed.
    bool check(const std::string& name, const std::string& current_value,
               const std::string& location) {
      auto it = watches.find(name);
      if (it == watches.end()) {
        watches[name] = {current_value, 0, location};
        std::cerr << colors::BRIGHT_MAGENTA << "  WATCH " << colors::RESET
                  << colors::MAGENTA << name << colors::RESET
                  << colors::DIM << " initialized = " << colors::RESET
                  << current_value << "\n";
        return false;
      }
      auto& entry = it->second;
      if (entry.last_value != current_value) {
        entry.change_count++;
        std::cerr << colors::BRIGHT_MAGENTA << "  WATCH " << colors::RESET
                  << colors::MAGENTA << name << colors::RESET
                  << colors::DIM << " changed (#" << entry.change_count << "): " << colors::RESET
                  << colors::RED << entry.last_value << colors::RESET
                  << colors::DIM << " -> " << colors::RESET
                  << colors::GREEN << current_value << colors::RESET
                  << colors::DIM << "  [at " << location << "]" << colors::RESET << "\n";
        entry.last_value = current_value;
        entry.last_location = location;
        return true;
      }
      return false;
    }

    void print_summary() const {
      if (watches.empty()) return;
      std::cerr << colors::BRIGHT_MAGENTA << "\n╭────────── Watchpoint Summary ──────────╮\n" << colors::RESET;
      for (const auto& [name, entry] : watches) {
        std::cerr << colors::MAGENTA << "│ " << std::left << std::setw(20) << name
                  << colors::DIM << ": " << entry.change_count << " changes, final = "
                  << colors::RESET << entry.last_value << "\n";
      }
      std::cerr << colors::BRIGHT_MAGENTA << "╰───────────────────────────────────────╯\n" << colors::RESET;
    }
  };

  inline WatchpointRegistry& get_watchpoint_registry() {
    static WatchpointRegistry registry;
    return registry;
  }

  //===--------------------------------------------------------------------===//
  //======================= ADVANCED FORMATTING SYSTEM =======================//

  // Thread-local visited pointers for cycle detection.
  inline thread_local std::unordered_set<const void*> visited_pointers;

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
        output << colors::RED << (value > 0 ? "+inf" : "-inf") << colors::RESET;
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
      requires (!PairLike<T> && !VariantLike<T>) {
      output << colors::MAGENTA << "{" << colors::RESET;
      format_tuple_impl(tuple, std::make_index_sequence<std::tuple_size_v<T>>{});
      output << colors::MAGENTA << "}" << colors::RESET;
    }

    // Variant types.
    template<VariantLike T>
    void format_value(const T& var) {
      output << colors::BRIGHT_CYAN << "variant(" << colors::DIM << "index=" << var.index() << colors::BRIGHT_CYAN << ", " << colors::RESET;
      std::visit([this](const auto& val) { format_value(val); }, var);
      output << colors::BRIGHT_CYAN << ")" << colors::RESET;
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

    // Non-copyable container adapters.
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

    template<typename T, typename Container, typename Compare>
    void format_value(std::priority_queue<T, Container, Compare> pq_copy) {
      output << colors::BLUE << "pq{" << colors::RESET;
      size_t count = 0;
      while (!pq_copy.empty() && count < MAX_CONTAINER_ELEMENTS) {
        if (count > 0) output << colors::DIM << ", " << colors::RESET;
        format_value(pq_copy.top());
        pq_copy.pop();
        count++;
      }
      if (!pq_copy.empty()) output << colors::DIM << ", ..." << colors::RESET;
      output << colors::BLUE << "}" << colors::RESET;
    }

    // Custom structures - broad compile-time member detection.
    template<typename T>
    void format_value(const T& value)
      requires (std::is_class_v<T> && !std::is_polymorphic_v<T> &&
                !Integral<T> && !FloatingPoint<T> && !StringLike<T> &&
                !Container<T> && !Matrix<T> && !PairLike<T> && !TupleLike<T> &&
                !OptionalLike<T> && !SmartPointer<T> && !Printable<T> &&
                !VariantLike<T> &&
                !std::same_as<T, char> && !std::same_as<T, bool> &&
                !std::same_as<T, std::nullptr_t>) {
      output << colors::CYAN << "{";
      // Weighted edge: {to, weight} or {to, w}.
      if constexpr (requires { value.to; value.weight; }) {
        output << "to: "; format_value_inline(value.to);
        output << ", weight: "; format_value_inline(value.weight);
      } else if constexpr (requires { value.to; value.w; }) {
        output << "to: "; format_value_inline(value.to);
        output << ", w: "; format_value_inline(value.w);
      }
      // Graph edge: {u, v, w}.
      else if constexpr (requires { value.u; value.v; value.w; }) {
        output << "u: "; format_value_inline(value.u);
        output << ", v: "; format_value_inline(value.v);
        output << ", w: "; format_value_inline(value.w);
      }
      // Graph edge: {u, v}.
      else if constexpr (requires { value.u; value.v; }) {
        output << "u: "; format_value_inline(value.u);
        output << ", v: "; format_value_inline(value.v);
      }
      // Point / coordinate: {x, y, z}.
      else if constexpr (requires { value.x; value.y; value.z; }) {
        output << "x: "; format_value_inline(value.x);
        output << ", y: "; format_value_inline(value.y);
        output << ", z: "; format_value_inline(value.z);
      }
      // Point / coordinate: {x, y}.
      else if constexpr (requires { value.x; value.y; }) {
        output << "x: "; format_value_inline(value.x);
        output << ", y: "; format_value_inline(value.y);
      }
      // Interval: {l, r}.
      else if constexpr (requires { value.l; value.r; }) {
        output << "l: "; format_value_inline(value.l);
        output << ", r: "; format_value_inline(value.r);
      }
      // Interval: {lo, hi}.
      else if constexpr (requires { value.lo; value.hi; }) {
        output << "lo: "; format_value_inline(value.lo);
        output << ", hi: "; format_value_inline(value.hi);
      }
      // Keyed value: {id, val}.
      else if constexpr (requires { value.id; value.val; }) {
        output << "id: "; format_value_inline(value.id);
        output << ", val: "; format_value_inline(value.val);
      }
      // Keyed value: {key, value} (non-pair).
      else if constexpr (requires { value.key; value.value; }) {
        output << "key: "; format_value_inline(value.key);
        output << ", value: "; format_value_inline(value.value);
      }
      // Node-like: {id, cost/dist}.
      else if constexpr (requires { value.id; value.cost; }) {
        output << "id: "; format_value_inline(value.id);
        output << ", cost: "; format_value_inline(value.cost);
      }
      else if constexpr (requires { value.id; value.dist; }) {
        output << "id: "; format_value_inline(value.id);
        output << ", dist: "; format_value_inline(value.dist);
      }
      // Event-like: {time, type}.
      else if constexpr (requires { value.time; value.type; }) {
        output << "time: "; format_value_inline(value.time);
        output << ", type: "; format_value_inline(value.type);
      }
      // Pair-like via .first/.second members.
      else if constexpr (requires { value.first; value.second; }) {
        output << "first: "; format_value_inline(value.first);
        output << ", second: "; format_value_inline(value.second);
      }
      // Single-value wrappers.
      else if constexpr (requires { value.val; }) {
        output << "val: "; format_value_inline(value.val);
      }
      else if constexpr (requires { value.value; }) {
        output << "value: "; format_value_inline(value.value);
      }
      else {
        output << "struct@" << sizeof(T) << "B";
      }
      output << "}" << colors::RESET;
    }

    // Fallback for remaining printable types.
    template<Printable T>
    void format_value(const T& value)
      requires (!Integral<T> && !FloatingPoint<T> && !StringLike<T> &&
                !Container<T> && !Matrix<T> && !PairLike<T> && !TupleLike<T> &&
                !OptionalLike<T> && !SmartPointer<T> && !VariantLike<T> &&
                !std::same_as<T, char> && !std::same_as<T, bool> &&
                !std::same_as<T, std::nullptr_t> &&
                !(std::is_class_v<T> && !std::is_polymorphic_v<T>)) {
      output << colors::WHITE << value << colors::RESET;
    }

    // Final fallback for unprintable types.
    template<typename T>
    void format_value(const T& value)
      requires (!Printable<T> && !Integral<T> && !FloatingPoint<T> &&
                !StringLike<T> && !Container<T> && !Matrix<T> &&
                !PairLike<T> && !TupleLike<T> && !OptionalLike<T> &&
                !SmartPointer<T> && !VariantLike<T> &&
                !std::same_as<T, char> &&
                !std::same_as<T, bool> && !std::same_as<T, std::nullptr_t> &&
                !(std::is_class_v<T> && !std::is_polymorphic_v<T>)) {
      output << colors::DIM << "[unprintable:" << typeid(T).name() << "@" << &value << "]" << colors::RESET;
    }

    std::string str() const { return output.str(); }
    void clear() { output.str(""); output.clear(); depth = 0; }

  private:
    // Helper that formats a value directly into the output stream
    // without creating a sub-formatter (used by struct detection).
    template<typename T>
    void format_value_inline(const T& val) {
      ModernFormatter sub;
      sub.format_value(val);
      output << sub.str();
    }

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
  //=================== 1-INDEXED CONTAINER FORMATTING ======================//

  // Prints a container with 1-based indices: [1]=val, [2]=val, ...
  template<Container T>
  void format_1indexed(const T& container, std::string_view name) {
    std::cerr << colors::MAGENTA << name << colors::CYAN << " = " << colors::RESET;
    std::cerr << colors::BLUE << "{" << colors::RESET;
    size_t idx = 1;
    for (const auto& element : container) {
      if (idx > 1) std::cerr << colors::DIM << ", " << colors::RESET;
      std::cerr << colors::DIM << "[" << idx << "]=" << colors::RESET;
      ModernFormatter fmt;
      fmt.format_value(element);
      std::cerr << fmt.str();
      if (idx >= 100) {
        std::cerr << colors::DIM << ", ..." << colors::RESET;
        break;
      }
      idx++;
    }
    std::cerr << colors::BLUE << "}" << colors::RESET;
  }

  //===--------------------------------------------------------------------===//
  //================= RANGE / SUBRANGE CONTAINER FORMATTING ==================//

  template<typename T>
  void format_range(const T& container, size_t lo, size_t hi, std::string_view name) {
    hi = std::min(hi, container.size() - 1);
    if (lo > hi) {
      std::cerr << colors::MAGENTA << name << colors::DIM << "[" << lo << ".." << hi << "] = (empty range)" << colors::RESET;
      return;
    }
    std::cerr << colors::MAGENTA << name << colors::DIM << "[" << lo << ".." << hi << "]" << colors::CYAN << " = " << colors::RESET;
    std::cerr << colors::BLUE << "{" << colors::RESET;
    for (size_t i = lo; i <= hi; i++) {
      if (i > lo) std::cerr << colors::DIM << ", " << colors::RESET;
      ModernFormatter fmt;
      fmt.format_value(container[i]);
      std::cerr << fmt.str();
    }
    std::cerr << colors::BLUE << "}" << colors::RESET;
  }

  //===--------------------------------------------------------------------===//
  //===================== INTEGER BINARY REPRESENTATION =====================//

  template<Integral T>
  void format_bits(const T& value, std::string_view name, int num_bits = -1) {
    constexpr int type_bits = sizeof(T) * 8;
    if (num_bits < 0 || num_bits > type_bits) num_bits = type_bits;

    // Find the highest set bit to trim leading zeros intelligently.
    int highest = -1;
    for (int i = type_bits - 1; i >= 0; i--) {
      if ((value >> i) & 1) { highest = i; break; }
    }
    int display_bits = (num_bits > 0) ? num_bits : std::max(highest + 1, 1);

    std::cerr << colors::MAGENTA << name << colors::CYAN << " = " << colors::RESET;
    std::cerr << value << colors::DIM << " = " << colors::RESET;
    std::cerr << colors::CYAN << "0b";
    for (int i = display_bits - 1; i >= 0; i--) {
      std::cerr << (((value >> i) & 1) ? "1" : "0");
      if (i > 0 && i % 4 == 0) std::cerr << colors::DIM << "'" << colors::CYAN;
    }
    std::cerr << colors::RESET;
    std::cerr << colors::DIM << " (" << display_bits << " bits, popcount=" << __builtin_popcountll(static_cast<unsigned long long>(value)) << ")" << colors::RESET;
  }

  //===--------------------------------------------------------------------===//
  //=================== 2D GRID WITH ROW/COL INDICES ========================//

  template<Matrix T>
  void format_grid(const T& grid, std::string_view name) {
    if (grid.empty()) {
      std::cerr << colors::MAGENTA << name << colors::DIM << " = (empty grid)" << colors::RESET << "\n";
      return;
    }
    size_t rows = grid.size();
    size_t cols = grid[0].size();

    // Determine column width from the data.
    size_t max_width = 1;
    for (size_t r = 0; r < rows && r < 100; r++) {
      for (size_t c = 0; c < cols && c < 100; c++) {
        ModernFormatter fmt;
        fmt.format_value(grid[r][c]);
        // Strip ANSI codes for width calculation.
        std::string raw = fmt.str();
        std::string clean;
        bool in_esc = false;
        for (char ch : raw) {
          if (ch == '\033') { in_esc = true; continue; }
          if (in_esc) { if (ch == 'm') in_esc = false; continue; }
          clean += ch;
        }
        max_width = std::max(max_width, clean.size());
      }
    }
    int w = static_cast<int>(max_width);

    std::cerr << colors::MAGENTA << name << colors::DIM
              << " (" << rows << "x" << cols << "):" << colors::RESET << "\n";

    // Column header.
    std::cerr << colors::DIM << "     ";
    for (size_t c = 0; c < cols && c < 100; c++) {
      std::cerr << std::right << std::setw(w + 1) << c;
    }
    std::cerr << colors::RESET << "\n";

    // Rows with index.
    for (size_t r = 0; r < rows && r < 100; r++) {
      std::cerr << colors::DIM << std::right << std::setw(4) << r << " " << colors::RESET;
      for (size_t c = 0; c < cols && c < 100; c++) {
        ModernFormatter fmt;
        fmt.format_value(grid[r][c]);
        // Compute visible width for alignment.
        std::string raw = fmt.str();
        std::string clean;
        bool in_esc = false;
        for (char ch : raw) {
          if (ch == '\033') { in_esc = true; continue; }
          if (in_esc) { if (ch == 'm') in_esc = false; continue; }
          clean += ch;
        }
        int pad = w + 1 - static_cast<int>(clean.size());
        for (int i = 0; i < pad; i++) std::cerr << ' ';
        std::cerr << raw;
      }
      std::cerr << "\n";
    }
    if (rows > 100 || cols > 100) {
      std::cerr << colors::DIM << "  ... (truncated)" << colors::RESET << "\n";
    }
  }

  //===--------------------------------------------------------------------===//
  //=================== ADJACENCY LIST GRAPH VISUALIZATION ==================//

  // Prints an adjacency list (vector<vector<T>>) as a labeled graph.
  template<Container AdjList>
  void format_graph(const AdjList& adj, std::string_view name, bool zero_indexed = true) {
    size_t n = adj.size();
    size_t edge_count = 0;
    for (const auto& neighbors : adj) {
      if constexpr (Container<std::decay_t<decltype(neighbors)>>) {
        edge_count += neighbors.size();
      }
    }

    std::cerr << colors::BRIGHT_BLUE << "╭────────── Graph: " << name
              << " (" << n << " nodes, " << edge_count << " edges) ──────────╮\n" << colors::RESET;

    int base = zero_indexed ? 0 : 1;
    for (size_t i = 0; i < n; i++) {
      if constexpr (Container<std::decay_t<decltype(adj[i])>>) {
        if (adj[i].empty()) continue;

        std::cerr << colors::CYAN << "│ " << colors::BRIGHT_GREEN << std::setw(3)
                  << (i + base) << colors::RESET << colors::DIM << " -> " << colors::RESET;

        size_t count = 0;
        for (const auto& neighbor : adj[i]) {
          if (count > 0) std::cerr << colors::DIM << ", " << colors::RESET;
          ModernFormatter fmt;
          fmt.format_value(neighbor);
          std::cerr << fmt.str();
          count++;
          if (count >= 50) {
            std::cerr << colors::DIM << ", ..." << colors::RESET;
            break;
          }
        }
        std::cerr << "\n";
      }
    }
    std::cerr << colors::BRIGHT_BLUE << "╰";
    for (int i = 0; i < 50; i++) std::cerr << "─";
    std::cerr << "╯\n" << colors::RESET;
  }

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
  //=================== EXPRESSION NAME SPLITTING ===========================//

  // Splits a comma-separated expression string into individual names,
  // respecting template angle brackets, parentheses, and braces.
  inline std::vector<std::string> split_expression_names(std::string_view expr) {
    std::vector<std::string> names;
    int depth_angle = 0;
    int depth_paren = 0;
    int depth_brace = 0;
    size_t start = 0;

    for (size_t i = 0; i < expr.size(); i++) {
      char c = expr[i];
      switch (c) {
        case '<': depth_angle++; break;
        case '>': depth_angle--; break;
        case '(': depth_paren++; break;
        case ')': depth_paren--; break;
        case '{': depth_brace++; break;
        case '}': depth_brace--; break;
        case ',':
          if (depth_angle == 0 && depth_paren == 0 && depth_brace == 0) {
            std::string_view token = expr.substr(start, i - start);
            // Trim whitespace.
            while (!token.empty() && token.front() == ' ') token.remove_prefix(1);
            while (!token.empty() && token.back() == ' ') token.remove_suffix(1);
            names.emplace_back(token);
            start = i + 1;
          }
          break;
      }
    }
    // Last token.
    std::string_view token = expr.substr(start);
    while (!token.empty() && token.front() == ' ') token.remove_prefix(1);
    while (!token.empty() && token.back() == ' ') token.remove_suffix(1);
    names.emplace_back(token);
    return names;
  }

  //===--------------------------------------------------------------------===//
  //===================== MAIN DEBUG INTERFACE FUNCTIONS =====================//

  // Multi-variable debug print with individual name=value pairing.
  template<typename... Args>
  void debug_print_multi(std::string_view level_color, std::string_view expression,
                         const source_location& loc, Args&&... args) {
    visited_pointers.clear();

    #ifdef ENABLE_PERFORMANCE_TRACKING
    auto& tracker = get_performance_tracker();
    tracker.start_timer("debug_print");
    #endif

    // Header with source location.
    std::cerr << level_color << "┌─[" << colors::RESET
              << colors::DIM << loc.file_name() << ":" << loc.line()
              << " (" << loc.function_name() << ")" << colors::RESET
              << level_color << "]\n" << colors::RESET;

    if constexpr (sizeof...(args) == 0) {
      // No arguments: print expression text only (useful for checkpoints).
      std::cerr << level_color << "│ " << colors::RESET
                << colors::MAGENTA << expression << colors::RESET;
    } else if constexpr (sizeof...(args) == 1) {
      std::cerr << level_color << "│ " << colors::RESET
                << colors::MAGENTA << expression << colors::CYAN << " = " << colors::RESET;
      ModernFormatter formatter;
      (formatter.format_value(args), ...);
      std::cerr << formatter.str();
    } else {
      // Multi-variable: split names and pair with values.
      auto names = split_expression_names(expression);
      size_t idx = 0;
      std::cerr << level_color << "│ " << colors::RESET;
      auto print_named = [&](const auto& arg) {
        if (idx > 0) std::cerr << colors::DIM << ", " << colors::RESET;
        if (idx < names.size()) {
          std::cerr << colors::MAGENTA << names[idx] << colors::CYAN << " = " << colors::RESET;
        }
        ModernFormatter formatter;
        formatter.format_value(arg);
        std::cerr << formatter.str();
        idx++;
      };
      (print_named(args), ...);
    }

    std::cerr << "\n";
    std::cerr << level_color << "└";
    for (int i = 0; i < 50; ++i) std::cerr << "─";
    std::cerr << "\n" << colors::RESET;
    std::cerr.flush();

    #ifdef ENABLE_PERFORMANCE_TRACKING
    tracker.end_timer("debug_print");
    #endif
  }

  //===--------------------------------------------------------------------===//
  //===================== ERROR HANDLING AND ASSERTIONS ======================//

  inline void handle_assertion_failure(const char* condition, const source_location& loc) {
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

  // Assertion with value dumping: prints all provided values before aborting.
  template<typename... Args>
  void handle_assert_with_values(const char* condition, const char* value_expr,
                                 const source_location& loc, Args&&... args) {
    std::cerr << colors::BG_RED << colors::WHITE << " ASSERTION FAILED " << colors::RESET << "\n";
    std::cerr << colors::RED << "Condition: " << colors::YELLOW << condition << colors::RESET << "\n";
    std::cerr << colors::RED << "Location:  " << colors::CYAN << loc.file_name() << ":" << loc.line()
              << " in " << colors::MAGENTA << loc.function_name() << colors::RESET << "\n";

    if constexpr (sizeof...(args) > 0) {
      auto names = split_expression_names(std::string_view(value_expr));
      size_t idx = 0;
      std::cerr << colors::RED << "Values:    " << colors::RESET;
      auto print_one = [&](const auto& arg) {
        if (idx > 0) std::cerr << colors::DIM << ", " << colors::RESET;
        if (idx < names.size()) {
          std::cerr << colors::MAGENTA << names[idx] << colors::CYAN << " = " << colors::RESET;
        }
        ModernFormatter formatter;
        formatter.format_value(arg);
        std::cerr << formatter.str();
        idx++;
      };
      (print_one(args), ...);
      std::cerr << "\n";
    }

    #if defined(HAS_STACKTRACE) && defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
    try {
      auto trace = std::stacktrace::current();
      std::cerr << colors::BRIGHT_RED << "Stack Trace:\n" << colors::RESET;
      for (const auto& frame : trace) {
        std::cerr << colors::DIM << "  " << frame << colors::RESET << "\n";
      }
    } catch (...) {}
    #endif

    std::abort();
  }

} // namespace modern_debug
#endif // DEBUG_LEVEL > 0

//===----------------------------------------------------------------------===//
//=========================== PUBLIC DEBUG MACROS ============================//

// Main debug macros with automatic source location and multi-variable name splitting.
#if DEBUG_LEVEL >= 1
  #define debug(...) \
    modern_debug::debug_print_multi(colors::YELLOW, #__VA_ARGS__, \
                            modern_debug::source_location::current() __VA_OPT__(,) __VA_ARGS__)

  #define debug_info(...) \
    modern_debug::debug_print_multi(colors::BRIGHT_BLUE, #__VA_ARGS__, \
                            modern_debug::source_location::current() __VA_OPT__(,) __VA_ARGS__)

  #define debug_warn(...) \
    modern_debug::debug_print_multi(colors::BRIGHT_YELLOW, #__VA_ARGS__, \
                            modern_debug::source_location::current() __VA_OPT__(,) __VA_ARGS__)

  #define debug_error(...) \
    modern_debug::debug_print_multi(colors::BRIGHT_RED, #__VA_ARGS__, \
                            modern_debug::source_location::current() __VA_OPT__(,) __VA_ARGS__)
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
    modern_debug::debug_print_multi(colors::BRIGHT_CYAN, #__VA_ARGS__, \
                            modern_debug::source_location::current() __VA_OPT__(,) __VA_ARGS__)

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

  // Assertion with value dumping: debug_assert(cond, val1, val2, ...).
  #define debug_assert(condition, ...) \
    do { \
      if (!(condition)) { \
        modern_debug::handle_assert_with_values(#condition, #__VA_ARGS__, \
          modern_debug::source_location::current() __VA_OPT__(,) __VA_ARGS__); \
      } \
    } while(0)

  // Legacy alias.
  #define my_assert(condition) modern_assert(condition)
#else
  #define modern_assert(condition) ((void)0)
  #define debug_assert(condition, ...) ((void)0)
  #define my_assert(condition) ((void)0)
#endif

// Performance tracking macros - fixed with proper token pasting indirection.
#ifdef ENABLE_PERFORMANCE_TRACKING
  #define PERF_START(label) modern_debug::get_performance_tracker().start_timer(label)
  #define PERF_END(label) modern_debug::get_performance_tracker().end_timer(label)

  #define MODERN_DEBUG_CONCAT_IMPL(a, b) a##b
  #define MODERN_DEBUG_CONCAT(a, b) MODERN_DEBUG_CONCAT_IMPL(a, b)

  #ifdef __COUNTER__
    #define MODERN_DEBUG_UNIQUE_ID __COUNTER__
  #else
    #define MODERN_DEBUG_UNIQUE_ID __LINE__
  #endif

  #define PERF_SCOPE(label) \
    const auto MODERN_DEBUG_CONCAT(_perf_scope_, MODERN_DEBUG_UNIQUE_ID) = [&]() { \
      struct Guard { \
        const char* lbl; \
        Guard(const char* l) : lbl(l) { PERF_START(lbl); } \
        ~Guard() { PERF_END(lbl); } \
      }; \
      return Guard(label); \
    }()
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

// 1-indexed container printing macro.
#if DEBUG_LEVEL >= 1
  #define debug1(container) \
    do { \
      std::cerr << colors::YELLOW << "┌─[" << colors::RESET \
                << colors::DIM << __FILE__ << ":" << __LINE__ \
                << " (" << __func__ << ")" << colors::RESET \
                << colors::YELLOW << "]\n" << colors::RESET; \
      std::cerr << colors::YELLOW << "│ " << colors::RESET; \
      modern_debug::format_1indexed(container, #container); \
      std::cerr << "\n"; \
      std::cerr << colors::YELLOW << "└"; \
      for (int _i = 0; _i < 50; ++_i) std::cerr << "─"; \
      std::cerr << "\n" << colors::RESET; \
      std::cerr.flush(); \
    } while(0)
#else
  #define debug1(container) ((void)0)
#endif

// Subrange container printing macro: debug_range(container, lo, hi).
#if DEBUG_LEVEL >= 1
  #define debug_range(container, lo, hi) \
    do { \
      std::cerr << colors::YELLOW << "┌─[" << colors::RESET \
                << colors::DIM << __FILE__ << ":" << __LINE__ \
                << " (" << __func__ << ")" << colors::RESET \
                << colors::YELLOW << "]\n" << colors::RESET; \
      std::cerr << colors::YELLOW << "│ " << colors::RESET; \
      modern_debug::format_range(container, lo, hi, #container); \
      std::cerr << "\n"; \
      std::cerr << colors::YELLOW << "└"; \
      for (int _i = 0; _i < 50; ++_i) std::cerr << "─"; \
      std::cerr << "\n" << colors::RESET; \
      std::cerr.flush(); \
    } while(0)
#else
  #define debug_range(container, lo, hi) ((void)0)
#endif

// Integer binary representation macro: debug_bits(value) or debug_bits(value, num_bits).
#if DEBUG_LEVEL >= 1
  #define debug_bits(value, ...) \
    do { \
      std::cerr << colors::YELLOW << "┌─[" << colors::RESET \
                << colors::DIM << __FILE__ << ":" << __LINE__ \
                << " (" << __func__ << ")" << colors::RESET \
                << colors::YELLOW << "]\n" << colors::RESET; \
      std::cerr << colors::YELLOW << "│ " << colors::RESET; \
      modern_debug::format_bits(value, #value __VA_OPT__(,) __VA_ARGS__); \
      std::cerr << "\n"; \
      std::cerr << colors::YELLOW << "└"; \
      for (int _i = 0; _i < 50; ++_i) std::cerr << "─"; \
      std::cerr << "\n" << colors::RESET; \
      std::cerr.flush(); \
    } while(0)
#else
  #define debug_bits(value, ...) ((void)0)
#endif

// 2D grid printing with row/column indices.
#if DEBUG_LEVEL >= 1
  #define debug_grid(grid) \
    do { \
      std::cerr << colors::YELLOW << "┌─[" << colors::RESET \
                << colors::DIM << __FILE__ << ":" << __LINE__ \
                << " (" << __func__ << ")" << colors::RESET \
                << colors::YELLOW << "]\n" << colors::RESET; \
      modern_debug::format_grid(grid, #grid); \
      std::cerr << colors::YELLOW << "└"; \
      for (int _i = 0; _i < 50; ++_i) std::cerr << "─"; \
      std::cerr << "\n" << colors::RESET; \
      std::cerr.flush(); \
    } while(0)
#else
  #define debug_grid(grid) ((void)0)
#endif

// Adjacency list graph visualization.
#if DEBUG_LEVEL >= 1
  #define debug_graph(adj, ...) \
    do { \
      std::cerr << colors::YELLOW << "┌─[" << colors::RESET \
                << colors::DIM << __FILE__ << ":" << __LINE__ \
                << " (" << __func__ << ")" << colors::RESET \
                << colors::YELLOW << "]\n" << colors::RESET; \
      modern_debug::format_graph(adj, #adj __VA_OPT__(,) __VA_ARGS__); \
      std::cerr.flush(); \
    } while(0)
#else
  #define debug_graph(adj, ...) ((void)0)
#endif

// Watchpoint macro: debug_watch(variable) - tracks changes across calls.
#if DEBUG_LEVEL >= 1
  #define debug_watch(var) \
    do { \
      modern_debug::ModernFormatter _wfmt; \
      _wfmt.format_value(var); \
      std::string _loc = std::string(__FILE__) + ":" + std::to_string(__LINE__); \
      modern_debug::get_watchpoint_registry().check(#var, _wfmt.str(), _loc); \
    } while(0)
#else
  #define debug_watch(var) ((void)0)
#endif

//===----------------------------------------------------------------------===//
//========================== MODERN TIMER WITH RAII ==========================//

#if DEBUG_LEVEL >= 1
class [[nodiscard("ModernTimer will be destroyed immediately -- assign it to a variable")]] ModernTimer {
private:
  std::chrono::high_resolution_clock::time_point start_time;
  std::string label;

public:
  ModernTimer(std::string_view name = "Execution")
    : start_time(std::chrono::high_resolution_clock::now()), label(name) {
    std::cerr << colors::GREEN << " Timer '" << label
              << "' started\n" << colors::RESET;
  }

  ~ModernTimer() {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = end_time - start_time;

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();

    std::cerr << colors::GREEN << " Timer '" << label << "' finished: "
              << colors::BRIGHT_YELLOW;

    if (ns >= 1'000'000'000) {
      std::cerr << std::fixed << std::setprecision(3) << (ns / 1'000'000'000.0) << " s";
    } else if (ns >= 1'000'000) {
      std::cerr << std::fixed << std::setprecision(3) << (ns / 1'000'000.0) << " ms";
    } else if (ns >= 1'000) {
      std::cerr << std::fixed << std::setprecision(3) << (ns / 1'000.0) << " us";
    } else {
      std::cerr << ns << " ns";
    }

    std::cerr << colors::RESET << "\n";
  }

  // Non-copyable, non-movable to prevent accidental copies.
  ModernTimer(const ModernTimer&) = delete;
  ModernTimer& operator=(const ModernTimer&) = delete;
  ModernTimer(ModernTimer&&) = default;
  ModernTimer& operator=(ModernTimer&&) = delete;

  void checkpoint(std::string_view checkpoint_name = "checkpoint") {
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time).count();
    std::cerr << colors::CYAN << " " << label << " - " << checkpoint_name
              << ": " << colors::YELLOW << elapsed << " us"
              << colors::RESET << "\n";
  }
};

// Legacy alias.
using Timer = ModernTimer;

// Debug file initialization -- prints confirmation to stdout before redirecting stderr.
inline void init_debug_log(const char* filename = "debug_output.txt") {
  #if DEBUG_LEVEL >= 1
  std::cout << "Redirecting debug output to " << filename << std::endl;
  if (!std::freopen(filename, "w", stderr)) {
    // Redirect failed: restore stderr and warn.
    std::cout << "Warning: Could not redirect stderr to debug file" << std::endl;
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
inline void init_debug_log(const char* = "debug_output.txt") {}
#endif

#endif // MODERN_DEBUG_H

//===----------------------------------------------------------------------===//
