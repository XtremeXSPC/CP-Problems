#!/usr/bin/env bash
# contest-helper.sh
# A wrapper script to streamline the competitive programming workflow using the enhanced Makefile.

set -euo pipefail

# ANSI colors
readonly C_RESET='\033[0m'
readonly C_BOLD='\033[1m'
readonly C_RED='\033[0;31m'
readonly C_GREEN='\033[0;32m'
readonly C_YELLOW='\033[0;33m'
readonly C_BLUE='\033[0;34m'
readonly C_CYAN='\033[0;36m'

# --- Configuration ---
# Location of the C++ templates
readonly TEMPLATE_DIR="templates/cpp"
# Standard directory for input files
readonly INPUT_DIR="input"
# Standard directory for output files
readonly OUTPUT_DIR="output"
# Standard directory for benchmark test cases
readonly BENCHMARK_DIR="benchmarks"
# Define the location of helper scripts relative to the project root
readonly SCRIPTS_DIR="scripts"

# --- Logging functions ---
log_info() { echo -e "${C_CYAN}[INFO]${C_RESET} $1"; }
log_success() { echo -e "${C_GREEN}[SUCCESS]${C_RESET} $1"; }
log_warning() { echo -e "${C_YELLOW}[WARNING]${C_RESET} $1"; }
log_error() { echo -e "${C_RED}[ERROR]${C_RESET} $1"; }
log_header() { echo -e "\n${C_BOLD}${C_BLUE}==== $1 ====${C_RESET}"; }

# --- Helper functions ---
check_makefile() {
  if [[ ! -f "Makefile" ]]; then
    log_error "Makefile not found in the current directory."
    log_info "Please run this script from your project's root directory."
    exit 1
  fi
}

# --- Core Commands ---

# Initialize a new project structure
cmd_init() {
  log_header "Initializing Project Structure and IDE Config"

  # Create standard directories
  mkdir -p "$INPUT_DIR" "$OUTPUT_DIR" "$BENCHMARK_DIR" ".ide-config"
  log_info "Created standard directories: $INPUT_DIR/, $OUTPUT_DIR/, $BENCHMARK_DIR/"

  # Initialize IDE configuration templates using the Makefile
  log_info "Running 'make init-ide-config' to set up templates..."
  make init-ide-config

  # Configure IDE for the current platform
  log_info "Running 'make config-ide' for platform-specific setup..."
  make config-ide

  log_success "Project initialized successfully."
}

# Create a new problem from a template
cmd_new() {
  if [[ $# -eq 0 ]]; then
    log_error "Usage: $0 new <ProblemName> [template]"
    log_info "Available templates: default, pbds"
    return 1
  fi

  local problem_name="$1"
  local template_name="${2:-default}"
  local source_file="${problem_name}.cpp"
  local template_file="${TEMPLATE_DIR}/${template_name}.cpp"
  local input_file="${INPUT_DIR}/${problem_name}.in"

  log_header "Creating New Problem: $problem_name"

  if [[ ! -f "$template_file" ]]; then
    log_error "Template '${template_name}' not found at '${template_file}'"
    return 1
  fi

  if [[ -f "$source_file" ]]; then
    log_warning "Source file '$source_file' already exists. Skipping."
  else
    # Copy template and replace placeholder
    sed "s/__FILE_NAME__/${source_file}/g" "$template_file" >"$source_file"
    log_success "Created source file: $source_file (from ${template_name} template)"
  fi

  if [[ -f "$input_file" ]]; then
    log_warning "Input file '$input_file' already exists. Skipping."
  else
    touch "$input_file"
    log_success "Created empty input file: $input_file"
  fi
}

# Build and run a solution
cmd_run() {
  if [[ $# -eq 0 ]]; then
    log_error "Usage: $0 run <ProblemName>"
    return 1
  fi
  local problem_name="$1"
  log_header "Building and Running: $problem_name"
  make run TARGET_NAME="$problem_name"
}

# Build and test a solution with I/O redirection
cmd_test() {
  if [[ $# -eq 0 ]]; then
    log_error "Usage: $0 test <ProblemName>"
    return 1
  fi
  local problem_name="$1"
  log_header "Testing with I/O Redirection: $problem_name"
  make test TARGET_NAME="$problem_name"
}

# Run benchmark on a solution
cmd_bench() {
  if [[ $# -eq 0 ]]; then
    log_error "Usage: $0 bench <ProblemName> [test_types...]"
    log_info "Example: $0 bench MySolution small large"
    return 1
  fi
  local problem_name="$1"
  shift
  local bench_tests="${*:-small medium large}"

  log_header "Benchmarking: $problem_name"
  log_info "Using test cases: $bench_tests"

  if [[ ! -f "${SCRIPTS_DIR}/benchmark.sh" ]]; then
    log_error "benchmark.sh script not found in '${SCRIPTS_DIR}/'"
    return 1
  fi

  make benchmark BENCH_TARGET="$problem_name" BENCH_TESTS="$bench_tests"
}

# Show help message
show_help() {
  cat <<EOF
${C_BOLD}Competitive Programming Helper Script${C_RESET}
Streamlines contest workflow by wrapping Makefile commands.

${C_CYAN}Usage:${C_RESET}
  $(basename "$0") <command> [options]

${C_CYAN}Commands:${C_RESET}
  ${C_GREEN}init${C_RESET}                     - Initialize project structure and IDE configuration.
  ${C_GREEN}new <Name> [tpl]${C_RESET}          - Create new problem source file from template (default, pbds).
  ${C_GREEN}run <Name>${C_RESET}                - Build and run a solution, reading from 'input/<Name>.in'.
  ${C_GREEN}test <Name>${C_RESET}               - Run with I/O redirection ('input/<Name>.in' -> 'output/<Name>.out').
  ${C_GREEN}bench <Name> [tests...]${C_RESET} - Benchmark a solution (e.g., 'small', 'large').
  ${C_GREEN}config-ide${C_RESET}              - (Re)configure the IDE for the current platform.
  ${C_GREEN}help${C_RESET}                    - Show this help message.

${C_CYAN}Makefile Passthrough:${C_RESET}
  You can still use make directly for more control (e.g., 'make clean', 'make contest').
  This script is a convenience wrapper for the most common tasks.

${C_CYAN}Example Workflow:${C_RESET}
  1. $(basename "$0") init
  2. $(basename "$0") new A
  3. # Write code in A.cpp, add input to input/A.in
  4. $(basename "$0") test A
  5. $(basename "$0") bench A
EOF
}

# --- Main entry point ---
main() {
  if [[ $# -eq 0 ]]; then
    show_help
    exit 0
  fi

  local command="$1"
  shift

  # Before running any command (except help), check for Makefile
  if [[ "$command" != "help" ]]; then
    check_makefile
  fi

  case "$command" in
  "init")
    cmd_init "$@"
    ;;
  "new")
    cmd_new "$@"
    ;;
  "run")
    cmd_run "$@"
    ;;
  "test")
    cmd_test "$@"
    ;;
  "bench" | "benchmark")
    cmd_bench "$@"
    ;;
  "config-ide")
    make config-ide
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
