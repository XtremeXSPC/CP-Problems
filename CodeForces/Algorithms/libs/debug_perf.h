//===----------------------------------------------------------------------===//
//=========================== DEBUG PERF MODULE ==============================//

#ifndef MODERN_DEBUG_PERF_H
#define MODERN_DEBUG_PERF_H

#include "debug_core.h"

#if DEBUG_LEVEL > 0
namespace modern_debug {

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

} // namespace modern_debug
#endif // DEBUG_LEVEL > 0

#undef PERF_START
#undef PERF_END
#undef PERF_SCOPE

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

#endif // MODERN_DEBUG_PERF_H
