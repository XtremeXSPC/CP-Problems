#!/usr/bin/env bash

# =========================================================================== #
# benchmark.sh
# Performance benchmarking utility for competitive programming solutions

set -euo pipefail

# =========================================================================== #
# ------------------------------ Configuration ------------------------------ #

# ANSI colors with graceful degradation
if [[ -t 1 ]] && command -v tput >/dev/null 2>&1 && (($(tput colors 2>/dev/null || echo 0) >= 8)); then
  readonly C_RESET=$'\033[0m'
  readonly C_BOLD=$'\033[1m'
  readonly C_RED=$'\033[0;31m'
  readonly C_GREEN=$'\033[0;32m'
  readonly C_YELLOW=$'\033[0;33m'
  readonly C_BLUE=$'\033[0;34m'
  readonly C_CYAN=$'\033[0;36m'
else
  readonly C_RESET=''
  readonly C_BOLD=''
  readonly C_RED=''
  readonly C_GREEN=''
  readonly C_YELLOW=''
  readonly C_BLUE=''
  readonly C_CYAN=''
fi

# Configuration
readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly BENCHMARK_DIR="benchmarks"
readonly RESULTS_DIR="benchmark-results"

# Default settings
DEFAULT_ITERATIONS=10
DEFAULT_TIMEOUT=5
DEFAULT_MEMORY_LIMIT=512 # MB

# =========================================================================== #
# --------------------------------- Funtions -------------------------------- #

# Logging functions
log_info() { echo -e "${C_CYAN}[INFO]${C_RESET} $1"; }
log_success() { echo -e "${C_GREEN}[SUCCESS]${C_RESET} $1"; }
log_warning() { echo -e "${C_YELLOW}[WARNING]${C_RESET} $1"; }
log_error() { echo -e "${C_RED}[ERROR]${C_RESET} $1"; }
log_header() { echo -e "${C_BOLD}${C_BLUE}==== $1 ====${C_RESET}"; }

# Utility functions
command_exists() {
  command -v "$1" >/dev/null 2>&1
}

get_file_size() {
  if [[ -f "$1" ]]; then
    if command_exists stat; then
      # Try GNU stat first (Linux)
      stat -c%s "$1" 2>/dev/null || stat -f%z "$1" 2>/dev/null || echo "0"
    else
      ls -l "$1" | awk '{print $5}'
    fi
  else
    echo "0"
  fi
}

format_time() {
  local time_ms="$1"
  if (($(echo "$time_ms >= 1000" | bc -l))); then
    printf "%.3fs" "$(echo "scale=3; $time_ms/1000" | bc -l)"
  else
    printf "%.0fms" "$time_ms"
  fi
}

format_memory() {
  local memory_kb="$1"
  if ((memory_kb >= 1024)); then
    printf "%.2fMB" "$(echo "scale=2; $memory_kb/1024" | bc -l)"
  else
    printf "${memory_kb}KB"
  fi
}

# Check dependencies
check_dependencies() {
  local missing_deps=()

  # Essential tools
  command_exists bc || missing_deps+=("bc")
  command_exists timeout || missing_deps+=("timeout")

  # Performance monitoring tools
  if ! command_exists time && ! command_exists /usr/bin/time; then
    missing_deps+=("time")
  fi

  # Memory profiling (optional but recommended)
  if ! command_exists valgrind; then
    log_warning "valgrind not found - memory profiling will be limited"
  fi

  if [[ ${#missing_deps[@]} -gt 0 ]]; then
    log_error "Missing dependencies: ${missing_deps[*]}"
    log_info "Please install missing tools and retry"
    exit 1
  fi
}

# Create benchmark test case
create_test_case() {
  local name="$1"
  local size="$2"
  local generator="${3:-}"

  log_info "Creating test case: $name (size: $size)"

  mkdir -p "$BENCHMARK_DIR"
  local input_file="$BENCHMARK_DIR/${name}.in"

  if [[ -n "$generator" ]] && [[ -f "$generator" ]]; then
    # Use custom generator
    if ! make TARGET_NAME="generator" SRCDIR="$(dirname "$generator")" clean all >/dev/null 2>&1; then
      log_error "Failed to build generator: $generator"
      return 1
    fi

    "./bin/generator" "$size" >"$input_file"
  else
    # Generate basic test case based on common competitive programming patterns
    case "$size" in
    "small")
      generate_small_test >"$input_file"
      ;;
    "medium")
      generate_medium_test >"$input_file"
      ;;
    "large")
      generate_large_test >"$input_file"
      ;;
    "stress")
      generate_stress_test >"$input_file"
      ;;
    *)
      # Custom size - generate array of that size
      generate_custom_test "$size" >"$input_file"
      ;;
    esac
  fi

  local file_size=$(get_file_size "$input_file")
  log_success "Generated test case: $name ($(format_memory $((file_size / 1024))) file size)"
}

# Optimized test generation using awk for better performance
generate_small_test() {
  echo "100"
  awk 'BEGIN{srand(); for(i=1;i<=100;i++) print int(rand()*1000)+1}'
}

generate_medium_test() {
  echo "10000"
  awk 'BEGIN{srand(); for(i=1;i<=10000;i++) print int(rand()*100000)+1}'
}

generate_large_test() {
  echo "100000"
  awk 'BEGIN{srand(); for(i=1;i<=100000;i++) print int(rand()*1000000)+1}'
}

generate_stress_test() {
  echo "1000000"
  awk 'BEGIN{srand(); for(i=1;i<=1000000;i++) print int(rand()*1000000000)+1}'
}

generate_custom_test() {
  local n="$1"
  echo "$n"
  awk -v n="$n" 'BEGIN{srand(); for(i=1;i<=n;i++) print int(rand()*1000000)+1}'
}

# Benchmark single run
benchmark_single_run() {
  local solution="$1"
  local test_case="$2"
  local timeout_sec="${3:-$DEFAULT_TIMEOUT}"

  local input_file="$BENCHMARK_DIR/${test_case}.in"
  local output_file="/tmp/benchmark_output_$$"
  local time_file="/tmp/benchmark_time_$$"

  #Log files for capturing hidden errors
  local build_log="/tmp/benchmark_build_log_$$"
  local stderr_log="/tmp/benchmark_stderr_log_$$"

  # Cleanup trap to ensure temp files are removed
  trap 'rm -f "$output_file" "$time_file" "$build_log" "$stderr_log"' RETURN

  if [[ ! -f "$input_file" ]]; then
    log_error "Test case not found: $input_file"
    return 1
  fi

  # Build solution if needed, now with verbose error logging
  local target_name
  target_name="$(basename "$solution" .cpp)"
  if [[ ! -f "bin/$target_name" ]] || [[ "$solution" -nt "bin/$target_name" ]]; then
    # The 'make' command now logs to a file. If it fails, we print the log.
    if ! make TARGET_NAME="$target_name" SRCDIR="$(dirname "$solution")" BUILD_TYPE=release clean all >"$build_log" 2>&1; then
      log_error "Failed to build solution: $solution"
      echo -e "${C_RED}--- Build Log ---${C_RESET}"
      cat "$build_log"
      echo -e "${C_RED}--- End Build Log ---${C_RESET}"
      # Returning a specific status for the main loop to handle
      echo "build_error:0:0:0:0"
      return
    fi
  fi

  # Use GNU time if available
  local time_cmd="time"
  [[ -f "/usr/bin/time" ]] && time_cmd="/usr/bin/time"

  # Run with timeout and collect metrics
  local start_time
  start_time=$(date +%s%3N)
  local exit_code=0

  # The execution now logs stderr to a file.
  # We check the exit code of 'timeout' itself.
  if timeout "${timeout_sec}s" $time_cmd -f "time_real:%e\nmem_max:%M\nexit_code:%x" -o "$time_file" \
    "./bin/$target_name" <"$input_file" >"$output_file" 2>"$stderr_log"; then
    local end_time
    end_time=$(date +%s%3N)
    local wall_time=$((end_time - start_time))

    local cpu_time_sec
    cpu_time_sec=$(grep "time_real:" "$time_file" 2>/dev/null | cut -d: -f2 || echo "0")
    local memory_kb
    memory_kb=$(grep "mem_max:" "$time_file" 2>/dev/null | cut -d: -f2 || echo "0")
    local cpu_time_ms
    cpu_time_ms=$(echo "scale=0; $cpu_time_sec * 1000" | bc -l 2>/dev/null || echo "$wall_time")
    local output_size
    output_size=$(get_file_size "$output_file")

    echo "success:$wall_time:$cpu_time_ms:$memory_kb:$output_size"
  else
    exit_code=$?
    if [[ $exit_code -eq 124 ]]; then
      echo "timeout:$((timeout_sec * 1000)):0:0:0"
    else
      # If it's any other error, it's a crash (Runtime Error)
      echo "runtime_error:0:0:0:0"
      # Print the captured stderr content to help debugging
      if [[ -s "$stderr_log" ]]; then
        echo -e "${C_RED}--- Program Stderr ---${C_RESET}" >&2
        cat "$stderr_log" >&2
        echo -e "${C_RED}--- End Stderr ---${C_RESET}" >&2
      fi
    fi
  fi
}

# Run comprehensive benchmark
run_benchmark() {
  local solution="$1"
  local test_cases=("${@:2}")
  local iterations="${BENCHMARK_ITERATIONS:-$DEFAULT_ITERATIONS}"
  local timeout="${BENCHMARK_TIMEOUT:-$DEFAULT_TIMEOUT}"

  log_header "Benchmarking Solution: $(basename "$solution")"

  # Create results directory
  mkdir -p "$RESULTS_DIR"
  local timestamp
  timestamp=$(date +%Y%m%d-%H%M%S)
  local results_file="$RESULTS_DIR/benchmark-$(basename "$solution" .cpp)-$timestamp.json"

  # Initialize JSON results
  cat >"$results_file" <<EOF
{
    "solution": "$solution",
    "timestamp": "$(date -Iseconds)",
    "iterations": $iterations,
    "timeout": $timeout,
    "test_cases": [
EOF

  local first_test=true

  for test_case in "${test_cases[@]}"; do
    if [[ "$first_test" = false ]]; then
      echo "        ," >>"$results_file"
    fi
    first_test=false

    log_info "Testing with: $test_case"

    # Arrays to store results
    local wall_times=()
    local cpu_times=()
    local memory_usages=()
    local output_sizes=()
    local successful_runs=0
    local timeout_runs=0
    local error_runs=0
    local build_errors=0

    # Progress indicator
    echo -n "  Progress: "

    # Run multiple iterations
    for ((i = 1; i <= iterations; i++)); do
      local result=$(benchmark_single_run "$solution" "$test_case" "$timeout")
      local status=$(echo "$result" | cut -d: -f1)

      case "$status" in
      "success")
        local wall_time=$(echo "$result" | cut -d: -f2)
        local cpu_time=$(echo "$result" | cut -d: -f3)
        local memory=$(echo "$result" | cut -d: -f4)
        local output_size=$(echo "$result" | cut -d: -f5)

        wall_times+=("$wall_time")
        cpu_times+=("$cpu_time")
        memory_usages+=("$memory")
        output_sizes+=("$output_size")
        ((successful_runs++))
        echo -n "✓"
        ;;
      "timeout")
        ((timeout_runs++))
        echo -n "T"
        ;;
      "runtime_error")
        ((error_runs++))
        echo -n "E"
        ;;
      "build_error")
        ((build_errors++))
        echo -n "B"
        break # Stop if build fails
        ;;
      esac
    done

    echo " ($successful_runs/$iterations successful)"

    # Calculate statistics
    if [[ $successful_runs -gt 0 ]]; then
      # Calculate averages and other statistics
      local avg_wall_time=$(printf '%s\n' "${wall_times[@]}" | awk '{sum+=$1} END {print sum/NR}')
      local avg_cpu_time=$(printf '%s\n' "${cpu_times[@]}" | awk '{sum+=$1} END {print sum/NR}')
      local avg_memory=$(printf '%s\n' "${memory_usages[@]}" | awk '{sum+=$1} END {print sum/NR}')
      local max_memory=$(printf '%s\n' "${memory_usages[@]}" | sort -n | tail -1)
      local min_wall_time=$(printf '%s\n' "${wall_times[@]}" | sort -n | head -1)
      local max_wall_time=$(printf '%s\n' "${wall_times[@]}" | sort -n | tail -1)

      # Calculate standard deviation
      local stddev_wall_time=$(printf '%s\n' "${wall_times[@]}" |
        awk -v avg="$avg_wall_time" '{sum+=($1-avg)^2} END {print sqrt(sum/NR)}')

      echo "    Results: $(format_time "$avg_wall_time") avg (±$(format_time "$stddev_wall_time")), $(format_time "$min_wall_time")-$(format_time "$max_wall_time") range, $(format_memory "$max_memory") peak memory"

      # Add to JSON results
      cat >>"$results_file" <<EOF
        {
            "test_case": "$test_case",
            "iterations": $iterations,
            "successful_runs": $successful_runs,
            "timeout_runs": $timeout_runs,
            "error_runs": $error_runs,
            "build_errors": $build_errors,
            "wall_time_ms": {
                "average": $avg_wall_time,
                "std_dev": $stddev_wall_time,
                "minimum": $min_wall_time,
                "maximum": $max_wall_time,
                "values": [$(
        IFS=','
        echo "${wall_times[*]}"
      )]
            },
            "cpu_time_ms": {
                "average": $avg_cpu_time,
                "values": [$(
        IFS=','
        echo "${cpu_times[*]}"
      )]
            },
            "memory_kb": {
                "average": $avg_memory,
                "maximum": $max_memory,
                "values": [$(
        IFS=','
        echo "${memory_usages[*]}"
      )]
            }
        }
EOF
    else
      echo "    Results: All runs failed!"

      # Add failed test case to JSON
      cat >>"$results_file" <<EOF
        {
            "test_case": "$test_case",
            "iterations": $iterations,
            "successful_runs": 0,
            "timeout_runs": $timeout_runs,
            "error_runs": $error_runs,
            "build_errors": $build_errors,
            "status": "failed"
        }
EOF
    fi
  done

  # Close JSON
  cat >>"$results_file" <<EOF
    ]
}
EOF

  log_success "Benchmark results saved to: $results_file"

  # Generate human-readable summary
  generate_benchmark_summary "$results_file"
}

# Generate benchmark summary report
generate_benchmark_summary() {
  local results_file="$1"
  local summary_file="${results_file%.json}.txt"

  log_info "Generating summary report: $summary_file"

  # Extract data and create summary using jq if available, otherwise use basic parsing
  if command_exists jq; then
    generate_summary_with_jq "$results_file" >"$summary_file"
  else
    generate_summary_basic "$results_file" >"$summary_file"
  fi

  log_success "Summary report generated: $summary_file"

  # Display summary to console
  echo
  log_header "Benchmark Summary"
  cat "$summary_file"
}

generate_summary_with_jq() {
  local results_file="$1"

  cat <<EOF
COMPETITIVE PROGRAMMING SOLUTION BENCHMARK REPORT
=================================================

Solution: $(jq -r '.solution' "$results_file")
Date: $(jq -r '.timestamp' "$results_file")
Iterations per test: $(jq -r '.iterations' "$results_file")
Timeout: $(jq -r '.timeout' "$results_file")s

TEST CASE RESULTS:
EOF

  # Process each test case
  jq -r '.test_cases[] | 
        "
Test Case: \(.test_case)
Success Rate: \(.successful_runs)/\(.iterations) (\((.successful_runs * 100 / .iterations) | floor)%)
" + 
        (if .successful_runs > 0 then
            "Average Time: \(.wall_time_ms.average)ms (±\(.wall_time_ms.std_dev)ms)
Best Time: \(.wall_time_ms.minimum)ms
Worst Time: \(.wall_time_ms.maximum)ms
Peak Memory: \(.memory_kb.maximum)KB
" else "Status: FAILED (all runs failed)
" end) + 
"----------------------------------------"' "$results_file"
}

generate_summary_basic() {
  local results_file="$1"

  echo "COMPETITIVE PROGRAMMING SOLUTION BENCHMARK REPORT"
  echo "================================================="
  echo
  echo "Solution: $(grep '"solution"' "$results_file" | cut -d'"' -f4)"
  echo "Date: $(grep '"timestamp"' "$results_file" | cut -d'"' -f4)"
  echo
  echo "Note: Install 'jq' for detailed summary report"
}

# Compare multiple solutions
compare_solutions() {
  local solutions=("$@")
  local test_cases=("small" "medium" "large")

  if [[ ${#solutions[@]} -lt 2 ]]; then
    log_error "Need at least 2 solutions to compare"
    return 1
  fi

  log_header "Comparing Solutions"

  # Ensure test cases exist
  for test_case in "${test_cases[@]}"; do
    if [[ ! -f "$BENCHMARK_DIR/${test_case}.in" ]]; then
      create_test_case "$test_case" "$test_case"
    fi
  done

  # Run benchmarks for each solution
  for solution in "${solutions[@]}"; do
    run_benchmark "$solution" "${test_cases[@]}"
  done

  # Generate comparison report
  log_info "Generating comparison report..."
  local comparison_file="$RESULTS_DIR/comparison-$(date +%Y%m%d-%H%M%S).txt"

  echo "SOLUTION COMPARISON REPORT" >"$comparison_file"
  echo "==========================" >>"$comparison_file"
  echo "Date: $(date)" >>"$comparison_file"
  echo "Solutions: ${solutions[*]}" >>"$comparison_file"
  echo >>"$comparison_file"

  # Detailed comparison if jq is available
  if command_exists jq; then
    for test_case in "${test_cases[@]}"; do
      echo "Test Case: $test_case" >>"$comparison_file"
      echo "$(printf '%-20s %-15s %-15s %-15s\n' "Solution" "Avg Time" "Peak Memory" "Success Rate")" >>"$comparison_file"
      echo "$(printf '%-20s %-15s %-15s %-15s\n' "--------" "--------" "-----------" "------------")" >>"$comparison_file"

      for solution in "${solutions[@]}"; do
        local solution_name="$(basename "$solution" .cpp)"
        local latest_result=$(ls -t "$RESULTS_DIR"/benchmark-"$solution_name"-*.json 2>/dev/null | head -1)

        if [[ -n "$latest_result" ]] && command_exists jq; then
          local avg_time=$(jq -r --arg tc "$test_case" '.test_cases[] | select(.test_case == $tc) | .wall_time_ms.average // "N/A"' "$latest_result")
          local peak_mem=$(jq -r --arg tc "$test_case" '.test_cases[] | select(.test_case == $tc) | .memory_kb.maximum // "N/A"' "$latest_result")
          local success_rate=$(jq -r --arg tc "$test_case" '.test_cases[] | select(.test_case == $tc) | "\(.successful_runs)/\(.iterations)"' "$latest_result")

          if [[ "$avg_time" != "N/A" ]]; then
            avg_time="$(format_time "$avg_time")"
          fi
          if [[ "$peak_mem" != "N/A" ]]; then
            peak_mem="$(format_memory "$peak_mem")"
          fi

          echo "$(printf '%-20s %-15s %-15s %-15s\n' "$solution_name" "$avg_time" "$peak_mem" "$success_rate")" >>"$comparison_file"
        else
          echo "$(printf '%-20s %-15s %-15s %-15s\n' "$solution_name" "N/A" "N/A" "N/A")" >>"$comparison_file"
        fi
      done
      echo >>"$comparison_file"
    done
  else
    # Simple comparison without jq
    for test_case in "${test_cases[@]}"; do
      echo "Test Case: $test_case" >>"$comparison_file"
      for solution in "${solutions[@]}"; do
        echo "  $(basename "$solution" .cpp): Check individual reports" >>"$comparison_file"
      done
      echo >>"$comparison_file"
    done
  fi

  log_success "Comparison report saved: $comparison_file"
  cat "$comparison_file"
}

# Profile memory usage with valgrind
profile_solution() {
  local solution="$1"
  local test_case="$2"

  if ! command_exists valgrind; then
    log_error "valgrind not found. Please install valgrind for profiling."
    exit 1
  fi

  local target_name="$(basename "$solution" .cpp)"
  local input_file="$BENCHMARK_DIR/${test_case}.in"

  if [[ ! -f "$input_file" ]]; then
    log_error "Test case not found: $input_file"
    exit 1
  fi

  log_info "Building solution in debug mode..."
  if ! make TARGET_NAME="$target_name" SRCDIR="$(dirname "$solution")" BUILD_TYPE=debug clean all; then
    log_error "Failed to build solution"
    exit 1
  fi

  log_info "Running memory profiling with valgrind..."
  valgrind --tool=memcheck \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --verbose \
    --log-file="$RESULTS_DIR/valgrind-$target_name-$(date +%Y%m%d-%H%M%S).log" \
    "./bin/$target_name" <"$input_file"

  log_success "Profiling complete. Check results in $RESULTS_DIR/"
}

show_help() {
  cat <<EOF
${C_BOLD}Performance Benchmark Script${C_RESET}
Benchmark competitive programming solutions for performance analysis

${C_CYAN}Usage:${C_RESET}
  $(basename "$0") <command> [options]

${C_CYAN}Commands:${C_RESET}
  ${C_GREEN}create-tests${C_RESET}           - Create standard test cases (small, medium, large)
  ${C_GREEN}bench <solution> [tests...]${C_RESET}  - Benchmark solution with specified test cases
  ${C_GREEN}compare <sol1> <sol2> [...]${C_RESET} - Compare multiple solutions
  ${C_GREEN}profile <solution> <test>${C_RESET}   - Detailed profiling with valgrind
  ${C_GREEN}help${C_RESET}                   - Show this help

${C_CYAN}Options:${C_RESET}
  ${C_YELLOW}BENCHMARK_ITERATIONS=N${C_RESET}   - Number of runs per test (default: $DEFAULT_ITERATIONS)
  ${C_YELLOW}BENCHMARK_TIMEOUT=N${C_RESET}      - Timeout per run in seconds (default: $DEFAULT_TIMEOUT)
  ${C_YELLOW}BENCHMARK_MEMORY_LIMIT=N${C_RESET} - Memory limit in MB (default: $DEFAULT_MEMORY_LIMIT)

${C_CYAN}Examples:${C_RESET}
  $(basename "$0") create-tests                    # Create standard test cases
  $(basename "$0") bench solution.cpp              # Benchmark with all test cases
  $(basename "$0") bench solution.cpp small medium # Benchmark with specific tests
  $(basename "$0") compare solution1.cpp solution2.cpp # Compare two solutions
  
  BENCHMARK_ITERATIONS=50 $(basename "$0") bench solution.cpp # Run 50 iterations

${C_CYAN}Test Cases:${C_RESET}
  ${C_YELLOW}small${C_RESET}   - 100 elements
  ${C_YELLOW}medium${C_RESET}  - 10,000 elements  
  ${C_YELLOW}large${C_RESET}   - 100,000 elements
  ${C_YELLOW}stress${C_RESET}  - 1,000,000 elements

${C_CYAN}Output:${C_RESET}
  • Results saved to: ${C_YELLOW}benchmark-results/${C_RESET}
  • JSON data for detailed analysis
  • Human-readable summary reports
  • Comparison reports for multiple solutions
EOF
}

main() {
  if [[ $# -eq 0 ]]; then
    show_help
    exit 0
  fi

  # Check dependencies
  check_dependencies

  local command="$1"
  shift

  case "$command" in
  "create-tests" | "create")
    log_header "Creating Standard Test Cases"
    create_test_case "small" "small"
    create_test_case "medium" "medium"
    create_test_case "large" "large"
    create_test_case "stress" "stress"
    log_success "All test cases created in $BENCHMARK_DIR/"
    ;;
  "bench" | "benchmark")
    if [[ $# -eq 0 ]]; then
      log_error "Please specify solution file"
      echo "Usage: $(basename "$0") bench <solution.cpp> [test_cases...]"
      exit 1
    fi

    local solution="$1"
    shift

    if [[ ! -f "$solution" ]]; then
      log_error "Solution file not found: $solution"
      exit 1
    fi

    # Use provided test cases or default ones
    local test_cases=("$@")
    if [[ ${#test_cases[@]} -eq 0 ]]; then
      test_cases=("small" "medium" "large")

      # Create test cases if they don't exist
      for test_case in "${test_cases[@]}"; do
        if [[ ! -f "$BENCHMARK_DIR/${test_case}.in" ]]; then
          create_test_case "$test_case" "$test_case"
        fi
      done
    fi

    run_benchmark "$solution" "${test_cases[@]}"
    ;;
  "compare")
    if [[ $# -lt 2 ]]; then
      log_error "Please specify at least 2 solutions to compare"
      echo "Usage: $(basename "$0") compare <solution1.cpp> <solution2.cpp> [...]"
      exit 1
    fi

    compare_solutions "$@"
    ;;
  "profile")
    if [[ $# -lt 2 ]]; then
      log_error "Please specify solution and test case"
      echo "Usage: $(basename "$0") profile <solution.cpp> <test_case>"
      exit 1
    fi

    profile_solution "$1" "$2"
    ;;
  "help" | "-h" | "--help")
    show_help
    ;;
  *)
    log_error "Unknown command: $command"
    echo "Run '$(basename "$0") help' for usage information"
    exit 1
    ;;
  esac
}

# Run main function
main "$@"

# =========================================================================== #
# End of script
