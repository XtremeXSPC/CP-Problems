# Enhanced Competitive Programming Build System

A sophisticated and robust build system designed specifically for competitive programming in C++. This system automatically handles toolchain selection (GCC/Clang), competitive programming headers (`bits/stdc++.h`, `ext/pb_ds`), IDE integration, and performance benchmarking.

## Features

**Automatic Toolchain Detection**: Intelligently selects between GCC and Clang based on code requirements  
**Multi-Platform Support**: Works seamlessly on macOS, Linux, and Windows  
**Competitive Programming Optimized**: Built-in support for `bits/stdc++.h` and policy-based data structures  
**IDE Integration**: Automatic configuration for clangd/LSP with `compile_commands.json`  
**Advanced Sanitizers**: Memory debugging with automatic fallback handling  
**Performance Benchmarking**: Comprehensive benchmarking system for solution optimization  
**Rich Terminal Output**: Color-coded build output with detailed progress information  

## Quick Start

### 1. Project Setup

```bash
# Initialize project structure
./scripts/contest-helper.sh init

# Create a new problem
./scripts/contest-helper.sh new Problem_A

# Edit Problem_A.cpp and add test input to input/Problem_A.in
# Then test your solution
./scripts/contest-helper.sh test Problem_A
```

### 2. Basic Workflow

```bash
# Build and run with input redirection
make run TARGET_NAME=Problem_A

# Build optimized version for contest submission
make contest TARGET_NAME=Problem_A

# Debug with sanitizers
make BUILD_TYPE=sanitize run TARGET_NAME=Problem_A
```

## System Requirements

### Essential Dependencies

- **GCC** (recommended) or **Clang++**
- **Make** (GNU Make 4.0+)
- **Bash** (4.0+)

### Platform-Specific Setup

#### macOS

```bash
# Install dependencies via Homebrew
brew install gcc make bash

# For IDE integration (optional)
pip install compiledb
```

#### Linux (Ubuntu/Debian)

```bash
# Install dependencies
sudo apt update
sudo apt install build-essential gcc g++ clang make

# For IDE integration (optional)
pip install compiledb
```

#### Windows (WSL/MinGW)

```bash
# WSL (recommended)
sudo apt install build-essential gcc g++ make

# Or MinGW/MSYS2
pacman -S gcc make bash
```

## Project Structure

```dir
competitive-programming/
├── Makefile                 # Enhanced build system
├── scripts/
│   ├── contest-helper.sh    # Main workflow script
│   ├── get_gcc_includes.sh  # Compiler detection utility
│   └── benchmark.sh         # Performance benchmarking
├── templates/cpp/
│   ├── default.cpp          # Standard template
│   ├── pbds.cpp             # Policy-based data structures
│   └── graph.cpp            # Graph algorithms template
├── input/                   # Test input files
├── output/                  # Generated output files
├── benchmarks/              # Benchmark test cases
├── .ide-config/             # IDE configuration templates
└── bin/                     # Compiled executables
```

## Usage Guide

### Contest Helper Script

The `contest-helper.sh` script provides a user-friendly interface for common tasks:

#### Create New Problems

```bash
# Create with default template
./scripts/contest-helper.sh new Problem_B

# Create with specific template
./scripts/contest-helper.sh new GraphProblem pbds
./scripts/contest-helper.sh new StringProblem default
```

#### Build and Test

```bash
# Test with I/O redirection
./scripts/contest-helper.sh test Problem_A

# Run interactively
./scripts/contest-helper.sh run Problem_A

# Run with arguments
./scripts/contest-helper.sh run Problem_A --verbose
```

#### Project Management

```bash
# List available templates and existing problems
./scripts/contest-helper.sh list

# Show debug information
./scripts/contest-helper.sh debug

# Configure IDE integration
./scripts/contest-helper.sh config-ide
```

### Makefile Targets

#### Core Build Targets

```bash
# Build with current configuration
make all TARGET_NAME=Problem_A

# Quick optimized build for submission
make contest TARGET_NAME=Problem_A

# Build and run with input file
make run TARGET_NAME=Problem_A

# Test with I/O redirection
make test TARGET_NAME=Problem_A
```

#### Build Types

```bash
# Debug build (default) - full debugging info
make BUILD_TYPE=debug TARGET_NAME=Problem_A

# Release build - maximum optimization
make BUILD_TYPE=release TARGET_NAME=Problem_A

# Sanitizer build - memory debugging
make BUILD_TYPE=sanitize TARGET_NAME=Problem_A
```

#### IDE and Development

```bash
# Configure IDE integration (clangd)
make config-ide

# Generate compile_commands.json only
make compile_commands

# Show detailed configuration info
make debug-info

# Test color output
make test-colors
```

#### Benchmarking

```bash
# Create standard benchmark test cases
make create-bench-tests

# Benchmark a solution
make benchmark BENCH_TARGET=Problem_A

# Compare multiple solutions
make compare SOLUTIONS="SolutionA.cpp SolutionB.cpp"
```

#### Maintenance

```bash
# Clean build artifacts
make clean

# Deep clean (removes everything)
make distclean
```

## Advanced Features

### Automatic Toolchain Selection

The system automatically detects and configures the appropriate compiler:

1. **Competitive Headers Detected** (`#include <bits/stdc++.h>`)
   - **Primary**: Uses GCC with proper include paths
   - **Fallback**: Uses Clang with PCH fallback mode

2. **Standard C++ Only**
   - Uses Clang for optimal performance and diagnostics

3. **Sanitizer Mode**
   - Automatically tests compiler capabilities
   - Falls back gracefully if sanitizers unavailable

### IDE Integration

#### Automatic Configuration

```bash
# Configure for your platform
make config-ide
```

This automatically:

- Detects your OS and compiler setup
- Configures `.clangd` for macOS with competitive headers
- Generates `compile_commands.json` for LSP support
- Sets up optimal IntelliSense/autocompletion

#### Manual IDE Setup

For **VS Code**:

1. Install the `clangd` extension
2. Run `make config-ide`
3. Reload VS Code

For **Vim/Neovim**:

1. Install your preferred LSP plugin
2. Run `make config-ide`
3. Point LSP to the generated `compile_commands.json`

### Performance Benchmarking

#### Quick Benchmarking

```bash
# Benchmark with default test sizes
./scripts/benchmark.sh bench Problem_A.cpp

# Benchmark with specific test types
./scripts/benchmark.sh bench Problem_A.cpp small large
```

#### Comprehensive Analysis

```bash
# Compare multiple solutions
./scripts/benchmark.sh compare SolutionA.cpp SolutionB.cpp SolutionC.cpp

# Create custom test cases
./scripts/benchmark.sh create-tests
```

Results are saved in `benchmark-results/` with both JSON data and human-readable reports.

### Templates

#### Available Templates

- **default.cpp**: Standard competitive programming template
- **pbds.cpp**: Policy-based data structures (GNU PBDS)
- **graph.cpp**: Graph algorithms with common utilities
- **string.cpp**: String processing algorithms

#### Creating Custom Templates

1. Add your template to `templates/cpp/mytemplate.cpp`
2. Use placeholders: `__FILE_NAME__`, `__PROBLEM_NAME__`, `__DATE__`, `__TIME__`
3. Use with: `./scripts/contest-helper.sh new Problem mytemplate`

## Troubleshooting

### Common Issues

#### "Competitive programming headers detected but compatible GCC not found!"

```bash
# On macOS - install GCC via Homebrew
brew install gcc

# On Linux - install build-essential
sudo apt install build-essential

# Verify installation
./scripts/contest-helper.sh debug
```

#### Colors not working

```bash
# Test color support
make test-colors

# Check terminal compatibility
echo $TERM
tput colors
```

#### IDE integration not working

```bash
# Reconfigure IDE
make config-ide

# Check if compiledb is installed
pip install compiledb

# Verify files were created
ls -la .clangd compile_commands.json
```

#### Sanitizers failing

```bash
# Check sanitizer support
make BUILD_TYPE=sanitize debug-info

# Try with different compiler
CC=clang CXX=clang++ make BUILD_TYPE=sanitize
```

### Debug Information

Run comprehensive diagnostics:

```bash
# Script-level debugging
./scripts/contest-helper.sh debug

# Build system debugging
make debug-info

# Enable verbose script output
DEBUG=true ./scripts/contest-helper.sh debug
```

## Environment Variables

### Build Configuration

- `BUILD_TYPE`: `debug` (default), `release`, `sanitize`
- `TARGET_NAME`: Problem name (default: `Tester`)
- `IDE_CONFIG`: Set to `yes` for IDE-friendly compilation

### Competitive Programming

- `ENABLE_FAST_IO`: Enable fast I/O optimizations (default: `1`)
- `ENABLE_DEBUG`: Enable debug mode defines (default: `1`)

### Benchmarking

- `BENCHMARK_ITERATIONS`: Number of runs per test (default: `10`)
- `BENCHMARK_TIMEOUT`: Timeout per run in seconds (default: `5`)

### Example Usage

```bash
# Build release version with custom target
BUILD_TYPE=release TARGET_NAME=Solution make all

# Benchmark with more iterations
BENCHMARK_ITERATIONS=50 make benchmark BENCH_TARGET=Solution

# Debug script execution
DEBUG=true ./scripts/contest-helper.sh debug
```

## Best Practices

### Contest Workflow

1. **Setup**: `./scripts/contest-helper.sh init`
2. **Create**: `./scripts/contest-helper.sh new Problem_A`
3. **Develop**: Edit `Problem_A.cpp`, add test input to `input/Problem_A.in`
4. **Test**: `./scripts/contest-helper.sh test Problem_A`
5. **Debug**: Use `BUILD_TYPE=sanitize` if needed
6. **Optimize**: Benchmark different approaches
7. **Submit**: `make contest TARGET_NAME=Problem_A` for final optimized build

### Code Organization

- Keep each problem in a separate `.cpp` file
- Use descriptive names: `graph_shortest_path.cpp` vs `A.cpp`
- Place test inputs in `input/` with matching names
- Use templates for common patterns

### Performance Optimization

- Use `make contest` for submission builds
- Profile with sanitizers during development
- Benchmark critical algorithms
- Consider both time and memory constraints

## Contributing

This build system is designed to be extensible:

1. **Adding Templates**: Place new templates in `templates/cpp/`
2. **Custom Benchmarks**: Extend `benchmark.sh` for specific test generation
3. **IDE Support**: Add configuration templates in `.ide-config/`
4. **Platform Support**: Enhance platform detection in the Makefile

## License

This project is designed for competitive programming and educational use. Feel free to adapt and modify for your specific needs.

---

For issues or questions, check the debug output first:

```bash
./scripts/contest-helper.sh debug
make debug-info
```
