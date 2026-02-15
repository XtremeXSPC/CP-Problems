//===----------------------------------------------------------------------===//
//=========================== DEBUG WATCH MODULE =============================//

#ifndef MODERN_DEBUG_WATCH_H
#define MODERN_DEBUG_WATCH_H

#include "debug_core.h"

#if DEBUG_LEVEL > 0 && CP_DEBUG_ENABLE_WATCH
namespace modern_debug {

class WatchpointRegistry {
private:
  struct WatchEntry {
    std::string last_value;
    size_t change_count = 0;
    std::string last_location;
  };
  std::unordered_map<std::string, WatchEntry> watches;

public:
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

} // namespace modern_debug
#endif

#undef debug_watch
#if DEBUG_LEVEL >= 1 && CP_DEBUG_ENABLE_WATCH
  #define debug_watch(var) \
    do { \
      auto& _wfmt = modern_debug::get_thread_local_formatter(); \
      _wfmt.format_value(var); \
      std::string _loc = std::string(__FILE__) + ":" + std::to_string(__LINE__); \
      modern_debug::get_watchpoint_registry().check(#var, _wfmt.str(), _loc); \
    } while(0)
#else
  #define debug_watch(var) ((void)0)
#endif

#endif // MODERN_DEBUG_WATCH_H
