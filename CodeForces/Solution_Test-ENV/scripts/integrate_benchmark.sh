#!/usr/bin/env bash
# =========================================================================== #
# integrate_benchmark.sh
# Integration script to use Python TestGen with existing benchmark system
# =========================================================================== #

set -euo pipefail

# Configuration
readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly TESTGEN_SCRIPT="${SCRIPT_DIR}/../../Test_Generator/TestGen.py"
readonly BENCHMARK_SCRIPT="${SCRIPT_DIR}/benchmark.sh"
readonly TEST_SUITES_CONFIG="${SCRIPT_DIR}/../../Test_Generator/test_suites.yaml"
readonly BENCHMARK_DIR="benchmarks"

# Colors for output
readonly C_GREEN=$'\033[0;32m'
readonly C_YELLOW=$'\033[0;33m'
readonly C_RED=$'\033[0;31m'
readonly C_BLUE=$'\033[0;34m'
readonly C_RESET=$'\033[0m'

# Logging functions
log_info() { echo -e "${C_BLUE}[INFO]${C_RESET} $1"; }
log_success() { echo -e "${C_GREEN}[SUCCESS]${C_RESET} $1"; }
log_warning() { echo -e "${C_YELLOW}[WARNING]${C_RESET} $1"; }
log_error() { echo -e "${C_RED}[ERROR]${C_RESET} $1"; }

# Check dependencies
check_dependencies() {
  local missing=()

  if ! command -v python3 >/dev/null 2>&1; then
    missing+=("python3")
  fi

  if ! command -v yq >/dev/null 2>&1; then
    log_warning "yq not found - YAML config parsing will be limited"
  fi

  if [[ ! -f "$TESTGEN_SCRIPT" ]]; then
    missing+=("testgen.py script at $TESTGEN_SCRIPT")
  fi

  if [[ ! -f "$BENCHMARK_SCRIPT" ]]; then
    missing+=("benchmark.sh script at $BENCHMARK_SCRIPT")
  fi

  if [[ ${#missing[@]} -gt 0 ]]; then
    log_error "Missing dependencies: ${missing[*]}"
    exit 1
  fi
}

# Generate test cases using Python TestGen
generate_test_case() {
  local generator="$1"
  local name="$2"
  local size="$3"
  local constraints="${4:-{\}}"
  local output_file="${BENCHMARK_DIR}/${name}.in"

  mkdir -p "$BENCHMARK_DIR"

  log_info "Generating test case: $name (generator: $generator, size: $size)"

  if python3 "$TESTGEN_SCRIPT" generate "$generator" \
    --size "$size" \
    --constraints "$constraints" \
    --output "$output_file"; then

    local file_size=$(stat -f%z "$output_file" 2>/dev/null || stat -c%s "$output_file" 2>/dev/null || echo "0")
    log_success "Generated: $output_file ($(numfmt --to=iec-i --suffix=B "$file_size" 2>/dev/null || echo "$file_size bytes"))"
    return 0
  else
    log_error "Failed to generate test case: $name"
    return 1
  fi
}

# Generate test suite from YAML config
generate_suite() {
  local problem_type="$1"
  local suite_name="$2"

  if ! command -v yq >/dev/null 2>&1; then
    log_error "yq is required for YAML parsing. Install with: brew install yq (macOS) or apt-get install yq (Linux)"
    exit 1
  fi

  log_info "Generating test suite: $problem_type/$suite_name"

  # Parse YAML and generate tests
  local generator_count=$(yq eval ".${problem_type}.${suite_name}.generators | length" "$TEST_SUITES_CONFIG")

  for ((i = 0; i < generator_count; i++)); do
    local gen_type=$(yq eval ".${problem_type}.${suite_name}.generators[$i].type" "$TEST_SUITES_CONFIG")
    local case_count=$(yq eval ".${problem_type}.${suite_name}.generators[$i].cases | length" "$TEST_SUITES_CONFIG")

    for ((j = 0; j < case_count; j++)); do
      local case_name=$(yq eval ".${problem_type}.${suite_name}.generators[$i].cases[$j].name" "$TEST_SUITES_CONFIG")
      local case_size=$(yq eval ".${problem_type}.${suite_name}.generators[$i].cases[$j].size" "$TEST_SUITES_CONFIG")
      local constraints=$(yq eval ".${problem_type}.${suite_name}.generators[$i].cases[$j].constraints // {}" "$TEST_SUITES_CONFIG" -o=json)

      local test_name="${suite_name}_${case_name}"
      generate_test_case "$gen_type" "$test_name" "$case_size" "$constraints"
    done
  done
}

# Run comprehensive benchmark with generated tests
run_comprehensive_benchmark() {
  local solution="$1"
  local problem_type="$2"

  log_info "Running comprehensive benchmark for: $solution"
  log_info "Problem type: $problem_type"

  # Generate standard test cases
  local generators=()
  case "$problem_type" in
  "graph")
    generators=("tree" "random_graph" "dag")
    ;;
  "string")
    generators=("string" "palindrome" "binary_string")
    ;;
  "array")
    generators=("array" "sorted_array" "permutation")
    ;;
  "dp")
    generators=("array" "matrix" "dp_path")
    ;;
  "geometry")
    generators=("points" "convex_polygon")
    ;;
  *)
    generators=("array")
    ;;
  esac

  # Generate test cases
  local test_cases=()
  for gen in "${generators[@]}"; do
    for size in "small" "medium" "large"; do
      local test_name="${gen}_${size}"
      if generate_test_case "$gen" "$test_name" "$size" "{}"; then
        test_cases+=("$test_name")
      fi
    done
  done

  # Run benchmark
  if [[ ${#test_cases[@]} -gt 0 ]]; then
    "$BENCHMARK_SCRIPT" bench "$solution" "${test_cases[@]}"
  else
    log_error "No test cases generated"
    exit 1
  fi
}

# Interactive mode for custom test generation
interactive_mode() {
  log_info "Interactive Test Generation Mode"

  # List available generators
  echo
  echo "Available generators:"
  python3 "$TESTGEN_SCRIPT" list
  echo

  # Get user input
  read -p "Enter generator type: " generator
  read -p "Enter test name: " name
  read -p "Enter size (tiny/small/medium/large/stress/custom): " size

  if [[ "$size" == "custom" ]]; then
    read -p "Enter custom size value: " custom_size
    extra_args="--custom-size $custom_size"
  else
    extra_args=""
  fi

  read -p "Enter constraints as JSON (or press Enter for defaults): " constraints
  if [[ -z "$constraints" ]]; then
    constraints="{}"
  fi

  # Generate test case
  generate_test_case "$generator" "$name" "$size" "$constraints"

  # Ask if user wants to benchmark
  read -p "Run benchmark with this test case? (y/n): " run_bench
  if [[ "$run_bench" == "y" ]]; then
    read -p "Enter solution file path: " solution
    if [[ -f "$solution" ]]; then
      "$BENCHMARK_SCRIPT" bench "$solution" "$name"
    else
      log_error "Solution file not found: $solution"
    fi
  fi
}

# Batch generation mode
batch_mode() {
  local generator="$1"
  local count="${2:-10}"
  local prefix="${3:-test}"

  log_info "Batch generation: $count test cases using $generator"

  for ((i = 0; i < count; i++)); do
    local sizes=("tiny" "small" "medium" "large")
    local size="${sizes[$((RANDOM % ${#sizes[@]}))]}"
    local test_name="${prefix}_${generator}_${i:03d}"

    generate_test_case "$generator" "$test_name" "$size" "{}"
  done

  log_success "Generated $count test cases"
}

# Compare solutions with diverse test cases
compare_with_diverse_tests() {
  local solutions=("$@")

  if [[ ${#solutions[@]} -lt 2 ]]; then
    log_error "Need at least 2 solutions to compare"
    exit 1
  fi

  log_info "Generating diverse test cases for comparison"

  # Generate variety of test cases
  local test_cases=()

  # Arrays
  generate_test_case "array" "cmp_array_random" "medium" '{"min_val": 1, "max_val": 1000000}'
  test_cases+=("cmp_array_random")

  generate_test_case "sorted_array" "cmp_sorted" "medium" '{"descending": false}'
  test_cases+=("cmp_sorted")

  generate_test_case "permutation" "cmp_permutation" "medium" '{}'
  test_cases+=("cmp_permutation")

  # Strings
  generate_test_case "string" "cmp_string" "large" '{"alphabet": "abc"}'
  test_cases+=("cmp_string")

  # Graphs
  generate_test_case "tree" "cmp_tree" "large" '{"weighted": true}'
  test_cases+=("cmp_tree")

  # Run comparison
  "$BENCHMARK_SCRIPT" compare "${solutions[@]}"
}

# Show help
show_help() {
  cat <<EOF
${C_BLUE}Advanced Test Generation & Benchmarking System${C_RESET}

${C_GREEN}Usage:${C_RESET}
  $(basename "$0") <command> [options]

${C_GREEN}Commands:${C_RESET}
  generate <generator> <name> <size> [constraints]
      Generate a single test case
      
  suite <problem_type> <suite_name>
      Generate test suite from YAML config
      
  batch <generator> [count] [prefix]
      Generate multiple test cases
      
  benchmark <solution> <problem_type>
      Run comprehensive benchmark with auto-generated tests
      
  compare <solution1> <solution2> [...]
      Compare solutions with diverse test cases
      
  interactive
      Interactive mode for test generation
      
  list
      List available generators
      
  help
      Show this help message

${C_GREEN}Examples:${C_RESET}
  # Generate a single test case
  $(basename "$0") generate tree my_tree large '{"weighted": true}'
  
  # Generate test suite from config
  $(basename "$0") suite graph_problems shortest_path
  
  # Batch generation
  $(basename "$0") batch array 20 test
  
  # Comprehensive benchmark
  $(basename "$0") benchmark solution.cpp graph
  
  # Compare solutions
  $(basename "$0") compare solution1.cpp solution2.cpp
  
  # Interactive mode
  $(basename "$0") interactive

${C_YELLOW}Configuration:${C_RESET}
  Test suites config: $TEST_SUITES_CONFIG
  Test output dir: $BENCHMARK_DIR/
  
${C_YELLOW}Environment Variables:${C_RESET}
  BENCHMARK_ITERATIONS   Number of benchmark iterations
  BENCHMARK_TIMEOUT      Timeout per test in seconds
EOF
}

# Main
main() {
  if [[ $# -eq 0 ]]; then
    show_help
    exit 0
  fi

  check_dependencies

  local command="$1"
  shift

  case "$command" in
  "generate")
    if [[ $# -lt 3 ]]; then
      log_error "Usage: generate <generator> <name> <size> [constraints]"
      exit 1
    fi
    generate_test_case "$1" "$2" "$3" "${4:-{\}}"
    ;;

  "suite")
    if [[ $# -lt 2 ]]; then
      log_error "Usage: suite <problem_type> <suite_name>"
      exit 1
    fi
    generate_suite "$1" "$2"
    ;;

  "batch")
    if [[ $# -lt 1 ]]; then
      log_error "Usage: batch <generator> [count] [prefix]"
      exit 1
    fi
    batch_mode "$@"
    ;;

  "benchmark")
    if [[ $# -lt 2 ]]; then
      log_error "Usage: benchmark <solution> <problem_type>"
      exit 1
    fi
    run_comprehensive_benchmark "$1" "$2"
    ;;

  "compare")
    if [[ $# -lt 2 ]]; then
      log_error "Usage: compare <solution1> <solution2> [...]"
      exit 1
    fi
    compare_with_diverse_tests "$@"
    ;;

  "interactive")
    interactive_mode
    ;;

  "list")
    python3 "$TESTGEN_SCRIPT" list
    ;;

  "help" | "-h" | "--help")
    show_help
    ;;

  *)
    log_error "Unknown command: $command"
    show_help
    exit 1
    ;;
  esac
}

main "$@"
