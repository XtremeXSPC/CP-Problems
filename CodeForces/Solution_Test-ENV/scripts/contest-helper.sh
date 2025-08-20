#!/usr/bin/env bash

# =========================================================================== #
# contest-helper.sh
# Enhanced wrapper script to streamline the competitive programming workflow
# using the enhanced Makefile with improved error handling and validation.

set -euo pipefail

# =========================================================================== #
# ------------------------------ Configuration ------------------------------ #
# =========================================================================== #
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

# Project structure configuration
readonly TEMPLATE_DIR="templates/cpp"
readonly INPUT_DIR="input"
readonly OUTPUT_DIR="output"
readonly BENCHMARK_DIR="benchmarks"
readonly SCRIPTS_DIR="scripts"
readonly IDE_CONFIG_DIR=".ide-config"

# Script metadata
readonly SCRIPT_NAME="$(basename "$0")"
readonly SCRIPT_VERSION="2.0"

# =========================================================================== #
# ---------------------------- Utility Functions ---------------------------- #
# =========================================================================== #

# Enhanced logging functions with consistent formatting
log_info() {
  echo -e "${C_CYAN}[INFO]${C_RESET} $*" >&2
}

log_success() {
  echo -e "${C_GREEN}[SUCCESS]${C_RESET} $*" >&2
}

log_warning() {
  echo -e "${C_YELLOW}[WARNING]${C_RESET} $*" >&2
}

log_error() {
  echo -e "${C_RED}[ERROR]${C_RESET} $*" >&2
}

log_header() {
  echo -e "\n${C_BOLD}${C_BLUE}/===----- $* -----===/${C_RESET}" >&2
}

# Enhanced error handling
die() {
  log_error "$1"
  exit "${2:-1}"
}

# Validation functions
validate_problem_name() {
  local name="$1"
  [[ -n "$name" ]] || die "Problem name cannot be empty"
  [[ "$name" =~ ^[A-Za-z0-9_-]+$ ]] || die "Problem name must contain only alphanumeric characters, hyphens, and underscores"
  [[ ${#name} -le 50 ]] || die "Problem name must be 50 characters or less"
}

validate_template() {
  local template="$1"
  local template_file="${TEMPLATE_DIR}/${template}.cpp"

  [[ -f "$template_file" ]] || die "Template '$template' not found at '$template_file'"
  [[ -r "$template_file" ]] || die "Template file '$template_file' is not readable"
}

validate_file_exists() {
  local file="$1"
  local description="${2:-File}"

  [[ -f "$file" ]] || die "$description '$file' not found"
  [[ -r "$file" ]] || die "$description '$file' is not readable"
}

# Enhanced Makefile validation
check_makefile() {
  if [[ ! -f "Makefile" ]]; then
    log_error "Makefile not found in the current directory."
    log_info "Please run this script from your project's root directory."
    log_info "Expected structure:"
    log_info "  project-root/"
    log_info "  ├── Makefile"
    log_info "  ├── $TEMPLATE_DIR/"
    log_info "  ├── $INPUT_DIR/"
    log_info "  └── $SCRIPTS_DIR/"
    exit 1
  fi

  # Validate Makefile has required targets
  local required_targets=("all" "run" "test" "config-ide")
  for target in "${required_targets[@]}"; do
    if ! grep -q "^${target}:" Makefile 2>/dev/null; then
      log_warning "Makefile target '$target' not found - some features may not work"
    fi
  done
}

# Safe directory creation with error handling
ensure_directory() {
  local dir="$1"
  local description="${2:-Directory}"

  if [[ ! -d "$dir" ]]; then
    if ! mkdir -p "$dir" 2>/dev/null; then
      die "Failed to create $description: $dir"
    fi
    log_success "Created $description: $dir"
  fi
}

# Enhanced template processing with validation
process_template() {
  local template_file="$1"
  local target_file="$2"
  local problem_name="$3"

  # Validate inputs
  validate_file_exists "$template_file" "Template file"
  [[ -n "$target_file" ]] || die "Target file path not specified"
  [[ -n "$problem_name" ]] || die "Problem name not specified"

  # Process template with safer substitution
  local temp_file
  temp_file=$(mktemp) || die "Failed to create temporary file"

  # Enhanced template processing - handle multiple placeholders
  sed -e "s/__FILE_NAME__/${target_file}/g" \
    -e "s/__PROBLEM_NAME__/${problem_name}/g" \
    -e "s/__DATE__/$(date +'%Y-%m-%d')/g" \
    -e "s/__TIME__/$(date +'%H:%M:%S')/g" \
    "$template_file" >"$temp_file" || {
    rm -f "$temp_file"
    die "Failed to process template"
  }

  # Atomic file creation
  if ! mv "$temp_file" "$target_file" 2>/dev/null; then
    rm -f "$temp_file"
    die "Failed to create target file: $target_file"
  fi
}

# =========================================================================== #
# ------------------------------ Core Commands ------------------------------ #
# =========================================================================== #

# Initialize project structure with enhanced validation
cmd_init() {
  log_header "Initializing Enhanced Project Structure and IDE Config"

  # Create standard directories with proper error handling
  local dirs=("$INPUT_DIR" "$OUTPUT_DIR" "$BENCHMARK_DIR" "$IDE_CONFIG_DIR")
  for dir in "${dirs[@]}"; do
    ensure_directory "$dir"
  done

  # Validate template directory exists
  if [[ ! -d "$TEMPLATE_DIR" ]]; then
    log_warning "Template directory '$TEMPLATE_DIR' not found"
    log_info "You may need to create templates manually or run this from the correct directory"
  fi

  # Initialize IDE configuration with better error handling
  log_info "Running 'make init-ide-config' to set up templates..."
  if ! make init-ide-config 2>/dev/null; then
    log_warning "Failed to initialize IDE config - continuing anyway"
  fi

  # Configure IDE for the current platform
  log_info "Running 'make config-ide' for platform-specific setup..."
  if ! make config-ide 2>/dev/null; then
    log_warning "Failed to configure IDE - you may need to run 'make config-ide' manually later"
  fi

  log_success "Project initialization completed successfully"
}

# Enhanced problem creation with better validation
cmd_new() {
  if [[ $# -eq 0 ]]; then
    log_error "Usage: $SCRIPT_NAME new <ProblemName> [template]"
    log_info "Available templates: default, pbds, graph, string"
    log_info "Example: $SCRIPT_NAME new Problem1 pbds"
    return 1
  fi

  local problem_name="$1"
  local template_name="${2:-default}"

  # Enhanced validation
  validate_problem_name "$problem_name"
  validate_template "$template_name"

  local source_file="${problem_name}.cpp"
  local template_file="${TEMPLATE_DIR}/${template_name}.cpp"
  local input_file="${INPUT_DIR}/${problem_name}.in"

  log_header "Creating New Problem: $problem_name"

  # Ensure directories exist
  ensure_directory "$INPUT_DIR"

  # Check for existing files and handle appropriately
  if [[ -f "$source_file" ]]; then
    log_warning "Source file '$source_file' already exists"
    read -p "Overwrite? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
      log_info "Skipping source file creation"
    else
      process_template "$template_file" "$source_file" "$problem_name"
      log_success "Created source file: $source_file (from $template_name template)"
    fi
  else
    process_template "$template_file" "$source_file" "$problem_name"
    log_success "Created source file: $source_file (from $template_name template)"
  fi

  # Create input file if it doesn't exist
  if [[ -f "$input_file" ]]; then
    log_info "Input file '$input_file' already exists"
  else
    if ! touch "$input_file" 2>/dev/null; then
      log_warning "Failed to create input file: $input_file"
    else
      log_success "Created empty input file: $input_file"
    fi
  fi

  log_info "Next steps:"
  log_info "  1. Edit $source_file with your solution"
  log_info "  2. Add test input to $input_file"
  log_info "  3. Run: $SCRIPT_NAME test $problem_name"
}

# Enhanced build and run with better error handling
cmd_run() {
  if [[ $# -eq 0 ]]; then
    log_error "Usage: $SCRIPT_NAME run <ProblemName> [args...]"
    log_info "Example: $SCRIPT_NAME run Problem1"
    return 1
  fi

  local problem_name="$1"
  shift
  local args=("$@")

  validate_problem_name "$problem_name"

  log_header "Building and Running: $problem_name"

  # Pass additional arguments to make
  local make_args=("run" "TARGET_NAME=$problem_name")
  [[ ${#args[@]} -gt 0 ]] && make_args+=("ARGS=${args[*]}")

  if ! make "${make_args[@]}"; then
    die "Build or execution failed for $problem_name"
  fi
}

# Enhanced test with validation
cmd_test() {
  if [[ $# -eq 0 ]]; then
    log_error "Usage: $SCRIPT_NAME test <ProblemName>"
    log_info "This runs the solution with input/$PROBLEM_NAME.in -> output/$PROBLEM_NAME.out"
    return 1
  fi

  local problem_name="$1"
  validate_problem_name "$problem_name"

  local input_file="${INPUT_DIR}/${problem_name}.in"
  validate_file_exists "$input_file" "Input file"

  log_header "Testing with I/O Redirection: $problem_name"

  # Ensure output directory exists
  ensure_directory "$OUTPUT_DIR"

  if ! make test "TARGET_NAME=$problem_name"; then
    die "Test execution failed for $problem_name"
  fi

  local output_file="${OUTPUT_DIR}/${problem_name}.out"
  if [[ -f "$output_file" ]]; then
    log_success "Test completed - output saved to $output_file"
    log_info "You can review the output with: cat $output_file"
  else
    log_warning "Expected output file $output_file was not created"
  fi
}

# Enhanced benchmark with validation
cmd_bench() {
  if [[ $# -eq 0 ]]; then
    log_error "Usage: $SCRIPT_NAME bench <ProblemName> [test_types...]"
    log_info "Available test types: small, medium, large, stress"
    log_info "Example: $SCRIPT_NAME bench Problem1 small large"
    return 1
  fi

  local problem_name="$1"
  shift
  local bench_tests=("$@")

  validate_problem_name "$problem_name"

  # Use default test types if none specified
  [[ ${#bench_tests[@]} -eq 0 ]] && bench_tests=("small" "medium" "large")

  log_header "Benchmarking: $problem_name"
  log_info "Using test cases: ${bench_tests[*]}"

  # Validate benchmark script exists
  local benchmark_script="${SCRIPTS_DIR}/benchmark.sh"
  validate_file_exists "$benchmark_script" "Benchmark script"
  [[ -x "$benchmark_script" ]] || die "Benchmark script is not executable: $benchmark_script"

  # Execute benchmark through Makefile
  if ! make benchmark "BENCH_TARGET=$problem_name" "BENCH_TESTS=${bench_tests[*]}"; then
    die "Benchmark execution failed for $problem_name"
  fi

  log_success "Benchmark completed for $problem_name"
  log_info "Check the benchmark-results/ directory for detailed reports"
}

cmd_list() {
  log_header "Project Status"

  # List available templates
  if [[ -d "$TEMPLATE_DIR" ]]; then
    log_info "Available templates:"
    find "$TEMPLATE_DIR" -name "*.cpp" -exec basename {} .cpp \; 2>/dev/null | sed 's/^/  /' || log_warning "No templates found"
  else
    log_warning "Template directory not found: $TEMPLATE_DIR"
  fi

  # List existing problems
  log_info "Existing problems:"
  local problems=()
  while IFS= read -r -d '' file; do
    problems+=("$(basename "$file" .cpp)")
  done < <(find . -maxdepth 1 -name "*.cpp" -print0 2>/dev/null)

  if [[ ${#problems[@]} -gt 0 ]]; then
    printf '  %s\n' "${problems[@]}"
  else
    log_info "  No problems found in current directory"
  fi

  # Show directory status
  echo
  log_info "Directory status:"
  for dir in "$INPUT_DIR" "$OUTPUT_DIR" "$BENCHMARK_DIR" "$IDE_CONFIG_DIR"; do
    if [[ -d "$dir" ]]; then
      local count
      count=$(find "$dir" -type f 2>/dev/null | wc -l)
      log_info "  $dir/: $count files"
    else
      log_warning "  $dir/: not found"
    fi
  done
}

# Enhanced debug command with robust error handling
cmd_debug() {
  log_header "Debug Information"

  # Test competitive programming detection
  log_info "Testing competitive programming header detection..."
  local cp_result=""
  local cp_exit_code=0

  if [[ -f "${SCRIPTS_DIR}/get_gcc_includes.sh" ]]; then
    # Use safer command execution that doesn't exit on error
    if cp_result=$("${SCRIPTS_DIR}/get_gcc_includes.sh" --check-competitive . 2>/dev/null); then
      log_success "Detection result: $cp_result"
    else
      cp_exit_code=$?
      log_warning "Detection command failed with exit code: $cp_exit_code"
      # Try again with debug output
      log_info "Retrying with debug output..."
      DEBUG=true "${SCRIPTS_DIR}/get_gcc_includes.sh" --check-competitive . || true
    fi
  else
    log_error "get_gcc_includes.sh not found in ${SCRIPTS_DIR}/"
  fi

  # Show which files were checked
  log_info "C++ files in current directory:"
  local cpp_files
  cpp_files=$(find . -maxdepth 1 -name "*.cpp" -o -name "*.cc" -o -name "*.cxx" 2>/dev/null) || true
  if [[ -n "$cpp_files" ]]; then
    echo "$cpp_files" | sed 's/^/  /'
  else
    log_info "  No C++ files found"
  fi

  # Check specific files for competitive headers
  local target_file="${TARGET_NAME:-Tester}.cpp"
  if [[ -f "$target_file" ]]; then
    log_info "Checking $target_file for competitive headers:"
    local header_check
    header_check=$(head -30 "$target_file" 2>/dev/null | grep -E "#include.*bits|#include.*ext/pb_ds" || true)
    if [[ -n "$header_check" ]]; then
      echo "$header_check" | sed 's/^/  Found: /'
    else
      log_info "  No competitive headers found in first 30 lines"
    fi

    # Show first few includes for reference
    log_info "First 30 lines of $target_file:"
    head -30 "$target_file" 2>/dev/null | sed 's/^/  /' || log_warning "  Could not read file"
  else
    log_info "Target file $target_file not found"
  fi

  # Test color support
  log_info "Color support test:"
  echo -e "  ${C_RED}Red${C_RESET} ${C_GREEN}Green${C_RESET} ${C_BLUE}Blue${C_RESET} ${C_CYAN}Cyan${C_RESET} ${C_YELLOW}Yellow${C_RESET} ${C_BOLD}Bold${C_RESET}"

  # Show color detection variables
  log_info "Color detection debug:"
  log_info "  TTY test: $(test -t 1 -o -t 2 && echo 'true' || echo 'false')"
  log_info "  N_COLORS: ${N_COLORS:-'not set'}"
  log_info "  TERM: ${TERM:-'not set'}"

  # Show environment info
  log_info "Environment:"
  echo -e "\t OS: $(uname -s 2>/dev/null || echo 'unknown')"
  echo -e "\t Shell: ${SHELL:-'not set'}"
  echo -e "\t Terminal: ${TERM:-'not set'}"
  echo -e "\t TTY: $(tty 2>/dev/null || echo 'not available')"
  echo -e "\t PWD: $PWD"

  # Check script permissions
  log_info "Script permissions:"
  if [[ -f "${SCRIPTS_DIR}/get_gcc_includes.sh" ]]; then
    echo -e "\t get_gcc_includes.sh: $(ls -la "${SCRIPTS_DIR}/get_gcc_includes.sh" | awk '{print $1 " " $3 ":" $4}')"
    echo -e "\t Executable: $([[ -x "${SCRIPTS_DIR}/get_gcc_includes.sh" ]] && echo 'yes' || echo 'no')"
  else
    log_warning "  get_gcc_includes.sh: not found"
  fi

  # Check available compilers
  log_info "Available compilers:"
  local compilers=("gcc" "g++" "clang" "clang++")
  for compiler in "${compilers[@]}"; do
    local compiler_path
    compiler_path=$(command -v "$compiler" 2>/dev/null || echo "not found")
    echo -e "\t $compiler: $compiler_path"
  done

  # Show Makefile status
  if [[ -f "Makefile" ]]; then
    echo -e "Running 'make debug-info' for detailed build configuration..."
    if make debug-info 2>/dev/null; then
      log_success "make debug-info completed successfully"
    else
      log_warning "make debug-info failed, trying with verbose output..."
      make debug-info || log_error "make debug-info failed completely"
    fi
  else
    log_warning "Makefile not found in current directory"
  fi

  log_success "Debug information collection completed"
}

cmd_list() {
  log_header "Project Status"

  # List available templates
  if [[ -d "$TEMPLATE_DIR" ]]; then
    log_info "Available templates:"
    find "$TEMPLATE_DIR" -name "*.cpp" -exec basename {} .cpp \; 2>/dev/null | sed 's/^/  /' || log_warning "No templates found"
  else
    log_warning "Template directory not found: $TEMPLATE_DIR"
  fi

  # List existing problems
  log_info "Existing problems:"
  local problems=()
  while IFS= read -r -d '' file; do
    problems+=("$(basename "$file" .cpp)")
  done < <(find . -maxdepth 1 -name "*.cpp" -print0 2>/dev/null)

  if [[ ${#problems[@]} -gt 0 ]]; then
    printf '  %s\n' "${problems[@]}"
  else
    log_info "  No problems found in current directory"
  fi

  # Show directory status
  echo
  log_info "Directory status:"
  for dir in "$INPUT_DIR" "$OUTPUT_DIR" "$BENCHMARK_DIR" "$IDE_CONFIG_DIR"; do
    if [[ -d "$dir" ]]; then
      local count
      count=$(find "$dir" -type f 2>/dev/null | wc -l)
      log_info "  $dir/: $count files"
    else
      log_warning "  $dir/: not found"
    fi
  done
}

# =========================================================================== #
# ------------------------------- Help System ------------------------------- #
# =========================================================================== #

show_help() {
  cat <<EOF
${C_BOLD}Enhanced Competitive Programming Helper Script v${SCRIPT_VERSION}${C_RESET}
Streamlines contest workflow by wrapping Makefile commands with enhanced validation.

${C_CYAN}USAGE:${C_RESET}
  $SCRIPT_NAME <command> [options]

${C_CYAN}COMMANDS:${C_RESET}
  ${C_GREEN}init${C_RESET}                        - Initialize project structure and IDE configuration
  ${C_GREEN}new <Name> [template]${C_RESET}       - Create new problem from template
                                   Templates: default, pbds, graph, string
  ${C_GREEN}run <Name> [args...]${C_RESET}        - Build and run solution with optional arguments
  ${C_GREEN}test <Name>${C_RESET}                 - Run with I/O redirection (input/ -> output/)
  ${C_GREEN}bench <Name> [tests...]${C_RESET}     - Benchmark solution with specified test types
                                   Test types: small, medium, large, stress
  ${C_GREEN}list${C_RESET}                        - Show available templates and existing problems
  ${C_GREEN}debug${C_RESET}                       - Show debug information and test detection
  ${C_GREEN}config-ide${C_RESET}                  - (Re)configure IDE for current platform
  ${C_GREEN}help${C_RESET}                        - Show this help message

${C_CYAN}EXAMPLES:${C_RESET}
  $SCRIPT_NAME init                    # Set up new project
  $SCRIPT_NAME new Problem1            # Create Problem1.cpp from default template
  $SCRIPT_NAME new GraphProblem pbds   # Create with policy-based data structures template
  $SCRIPT_NAME test Problem1           # Test with input/Problem1.in -> output/Problem1.out
  $SCRIPT_NAME bench Problem1 small    # Benchmark with small test case only
  $SCRIPT_NAME run Problem1 --verbose  # Run with command line arguments
  $SCRIPT_NAME debug                   # Show debug info and test detection

${C_CYAN}DIRECTORY STRUCTURE:${C_RESET}
  project-root/
  ├── Makefile             # Enhanced build system
  ├── $TEMPLATE_DIR/       # C++ templates
  ├── $INPUT_DIR/               # Test input files
  ├── $OUTPUT_DIR/              # Generated output files
  ├── $BENCHMARK_DIR/          # Benchmark test cases
  ├── $SCRIPTS_DIR/             # Helper scripts
  └── $IDE_CONFIG_DIR/         # IDE configuration templates

${C_CYAN}INTEGRATION:${C_RESET}
  This script wraps the enhanced Makefile with additional validation and
  user-friendly error messages. You can still use make directly for more
  control over the build process.

${C_CYAN}REQUIREMENTS:${C_RESET}
  • Enhanced Makefile in current directory
  • Template files in $TEMPLATE_DIR/
  • Helper scripts in $SCRIPTS_DIR/

For more information, see the project documentation or run individual
make targets with 'make help'.
EOF
}

# =========================================================================== #
# ---------------------------- Main Entry Point ----------------------------- #
# =========================================================================== #

main() {
  # Show help if no arguments provided
  if [[ $# -eq 0 ]]; then
    show_help
    exit 0
  fi

  local command="$1"
  shift

  # Handle help command early to avoid Makefile check
  case "$command" in
  "help" | "-h" | "--help")
    show_help
    exit 0
    ;;
  esac

  # Validate environment before proceeding
  check_makefile

  # Dispatch to appropriate command handler
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
  "list" | "ls")
    cmd_list "$@"
    ;;
  "debug")
    cmd_debug "$@"
    ;;
  "config-ide")
    log_header "Configuring IDE"
    if ! make config-ide; then
      die "IDE configuration failed"
    fi
    log_success "IDE configuration completed"
    ;;
  *)
    log_error "Unknown command: $command"
    log_info "Run '$SCRIPT_NAME help' for usage information"
    exit 1
    ;;
  esac
}

# Execute main function with all arguments
main "$@"

# =========================================================================== #
# End of script
