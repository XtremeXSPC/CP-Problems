# Generic Makefile for Competitive Programming C++ projects.
# Optimized for GCC with bits/stdc++.h and pb_ds support.

# =========================================================================== #
# ------------------------ System Pre-configuration  ------------------------ #
# =========================================================================== #
# Use Homebrew's bash if available to avoid macOS SIP issues with tools like 'bear',
# which rely on DYLD_INSERT_LIBRARIES. SIP strips this variable from system shells.
HOMEBREW_BASH := $(shell which bash | grep 'homebrew')

ifeq ($(strip $(HOMEBREW_BASH)),)
    SHELL := /bin/bash
else
    # Use the first bash found in a homebrew path
    SHELL := $(firstword $(HOMEBREW_BASH))
endif

# Continue with the rest of the Makefile...
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c

# =========================================================================== #
# -------------------------- ANSI Color Codes ------------------------------- #
# =========================================================================== #
# Check if terminal supports colors and define color variables.
# These will be empty if colors are not supported, allowing for graceful degradation.
ifeq ($(shell test -t 1 -o -t 2 && echo true), true)
    N_COLORS := $(shell tput colors 2>/dev/null || echo 0)
    ifeq ($(shell test $(N_COLORS) -ge 8 && echo true), true)
        C_RESET   := $(shell tput sgr0 2>/dev/null)
        C_BOLD    := $(shell tput bold 2>/dev/null)
        C_RED     := $(shell tput setaf 1 2>/dev/null)
        C_GREEN   := $(shell tput setaf 2 2>/dev/null)
        C_YELLOW  := $(shell tput setaf 3 2>/dev/null)
        C_BLUE    := $(shell tput setaf 4 2>/dev/null)
        C_CYAN    := $(shell tput setaf 6 2>/dev/null)
    endif
endif

# =========================================================================== #
# ----------------------- User-configurable variables ----------------------- #
# =========================================================================== #
TARGET_NAME    ?= Tester
SRCDIR         ?= .
BINDIR         ?= bin
OBJDIR_BASE    ?= obj

# Standardized I/O and config directories
INPUT_DIR      ?= input
OUTPUT_DIR     ?= output
IDE_CONFIG_DIR ?= .ide-config

# Define the location for helper scripts
SCRIPTS_DIR    ?= scripts

# Build profile for competitive programming: debug (default) or release.
# 'sanitize' profile is also available for debugging memory issues.
BUILD_TYPE     ?= debug

# IDE configuration mode - when set to 'yes', produces clean compile commands
# without toolchain-specific flags that can confuse clangd
IDE_CONFIG     ?= no

# Competitive programming specific settings
ENABLE_FAST_IO ?= 1
ENABLE_DEBUG   ?= 1

# =========================================================================== #
# -------------- Automatic OS configuration and path settings --------------- #
# =========================================================================== #
# Initial default definitions to avoid empty values if OS logic fails
PathSep        := /
TARGET_EXEC    := $(TARGET_NAME)
RM             := rm -f
RMDIR_CMD      := rm -rf
MKDIR_P        := mkdir -p

# Try to determine the operating system
UNAME_S        := $(shell uname -s)
detected_os    := UNKNOWN_UNIX

ifeq ($(UNAME_S),Darwin)
	detected_os := MACOS
  # Default Unix settings are already correct for macOS
else ifeq ($(UNAME_S),Linux)
	detected_os := LINUX
  # Default Unix settings are already correct for Linux
else ifneq (,$(findstring MINGW,$(UNAME_S))) # MinGW (Git Bash, MSYS2)
	detected_os := WINDOWS_MINGW
	TARGET_EXEC := $(TARGET_NAME).exe
  # MinGW prefers Unix-style slashes
	PathSep     := /
	RM          := rm -f
	RMDIR_CMD   := rm -rf
	MKDIR_P     := mkdir -p
else ifneq (,$(findstring CYGWIN,$(UNAME_S))) # Cygwin
	detected_os := WINDOWS_CYGWIN
  # Cygwin can handle with or without .exe
	TARGET_EXEC := $(TARGET_NAME).exe
	PathSep     := /
	RM          := rm -f
	RMDIR_CMD   := rm -rf
	MKDIR_P     := mkdir -p
else ifeq ($(OS),Windows_NT) # Environment variable from cmd.exe or PowerShell
	detected_os := WINDOWS_NATIVE_OS_VAR
	TARGET_EXEC := $(TARGET_NAME).exe
	PathSep     := \
	RM          := del /Q /F
	RMDIR_CMD   := rd /S /Q
  # MKDIR_P handled in create_dirs rule
else # Fallback to `ver` check only if others fail
	WINDOWS_VER_CHECK := $(shell ver 2>/dev/null) 
	ifneq (,$(findstring Windows,$(WINDOWS_VER_CHECK)))
    # MKDIR_P handled in create_dirs rule
		detected_os := WINDOWS_NATIVE_VER_CMD
		TARGET_EXEC := $(TARGET_NAME).exe
		PathSep     := \
		RM          := del /Q /F
		RMDIR_CMD   := rd /S /Q
	else
    # Final default if everything fails
		detected_os ?= UNKNOWN_UNIX
	endif
endif

TARGET_DIR_WITH_SEP := $(BINDIR)$(PathSep)
TARGET_PATH         := $(TARGET_DIR_WITH_SEP)$(TARGET_EXEC)

# =========================================================================== #
# ------------- Competitive Programming Compiler Configuration -------------- #
# =========================================================================== #

# ------------------------ GCC Detection for bits/stdc++.h ------------------ #
# Find the include path and prefix of the latest installed GCC version via Homebrew
GCC_PREFIX      := $(shell brew --prefix gcc 2>/dev/null || echo "")

# Find GCC binary - Prioritize Homebrew installation over system GCC
GCC_BINARY :=
ifneq ($(GCC_PREFIX),)
	GCC_BINARY := $(shell find $(GCC_PREFIX)/bin -name "g++-*" -type f 2>/dev/null | sort -V | tail -n 1)
endif

# Only use system GCC as last resort and warn about potential issues
ifeq ($(GCC_BINARY),)
    SYSTEM_GCC := $(shell which g++ 2>/dev/null)
    ifneq ($(SYSTEM_GCC),)
        # ----- START: Platform-Aware Heuristic ----- #
        # This check is platform-specific. On macOS, /usr/bin/g++ is a
        # symlink to clang that must be skipped. On Linux, it is the real GCC.
        ifeq ($(detected_os),MACOS)
            ifeq ($(SYSTEM_GCC),/usr/bin/g++)
                # This is Xcode's g++ symlink to clang, skip it
                GCC_BINARY :=
            else
                GCC_BINARY := $(SYSTEM_GCC)
            endif
        else
            # On non-macOS systems (like Linux), trust the system g++
            GCC_BINARY := $(SYSTEM_GCC)
        endif
        # ------ END: Platform-Aware Heuristic ------ #
    endif
endif

# Check if competitive programming headers are needed by scanning source files
NEEDS_PBDS      := $(shell grep -l "ext/pb_ds" $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.cc $(SRCDIR)/*.cxx) 2>/dev/null)
NEEDS_BITS_STDC := $(shell grep -l "bits/stdc++" $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.cc $(SRCDIR)/*.cxx) 2>/dev/null)
NEEDS_GCC       := $(if $(strip $(NEEDS_BITS_STDC)$(NEEDS_PBDS)),yes,no)

# ----------- Compiler Configuration for Competitive Programming ------------- #
ifeq ($(NEEDS_GCC),no)
    # Standard configuration - use Clang for better error messages and tooling
    CC             := clang
    CXX            := clang++
    SYSTEM_INCLUDES:= -I/usr/local/include
    $(info $(C_CYAN)Using Clang - No competitive programming headers detected$(C_RESET))
else
    # Competitive programming configuration - requires GCC
	ifneq ($(GCC_BINARY),)
        CC  := $(subst g++,gcc,$(GCC_BINARY))
        CXX := $(GCC_BINARY)
        $(info $(C_CYAN)Using GCC for competitive programming:$(C_RESET) $(CXX))
        $(info $(C_CYAN)Detected headers:$(C_RESET) $(if $(NEEDS_BITS_STDC),bits/stdc++.h) $(if $(NEEDS_PBDS),pb_ds))
        
        # ----- START: Toolchain Replication Logic ---------- #
        # Call the external helper script to get the include flags.
        # The script takes the compiler path and returns a space-separated list of flags.
        ifneq ($(GCC_BINARY),)
            GCC_ISYSTEM_FLAGS := $(shell ./assets/get_gcc_includes.sh $(CXX))
        else
            GCC_ISYSTEM_FLAGS :=
        endif

        # For IDE configuration, we skip the toolchain-specific flags to avoid
        # confusing clangd. The .clangd file will handle toolchain integration.
        ifeq ($(IDE_CONFIG),yes)
            # Clean configuration for IDE - let clangd handle toolchain discovery
            CXXFLAGS_TOOLCHAIN :=
            SYSTEM_INCLUDES := -I/usr/local/include
            STDLIB_FLAG :=
        else
            # Use -nostdinc++ to prevent the compiler from using its default search paths.
            # This forces it to use only the '-isystem' paths we provide.
            ifneq ($(GCC_ISYSTEM_FLAGS),)
                CXXFLAGS_TOOLCHAIN := -nostdinc++
                SYSTEM_INCLUDES := $(GCC_ISYSTEM_FLAGS)
            else
                # If we can't get GCC includes, fall back to standard includes
                CXXFLAGS_TOOLCHAIN :=
                SYSTEM_INCLUDES := -I/usr/local/include
            endif

            # ----- Platform-Specific Standard Library Flag ------ #
            # The -stdlib=libstdc++ flag is needed on macOS to steer the toolchain,
            # but it's an unrecognized (and unnecessary) option for native GCC on Linux.
            ifeq ($(detected_os),MACOS)
                ifneq ($(GCC_BINARY),)
                    STDLIB_FLAG := -stdlib=libstdc++
                endif
            endif
        endif
        # ----- END: Toolchain Replication Logic ------------- #

	else
		$(error $(C_RED)Competitive programming headers detected but compatible GCC not found!$(C_RESET))
		$(error $(C_YELLOW)Install GCC via: brew install gcc$(C_RESET))
	endif
endif

# Define project-specific include directories EXPLICITLY.
PROJECT_INCLUDES := -I$(SRCDIR) -I$(SRCDIR)/include

# ----------------- Competitive Programming Compiler Flags ------------------ #
# Base flags optimized for competitive programming
COMMON_FLAGS := -std=c++23

# Competitive programming specific defines
COMPETITIVE_DEFINES := -DLOCAL=1

ifeq ($(ENABLE_FAST_IO),1)
    COMPETITIVE_DEFINES += -DFAST_IO=1
endif

ifeq ($(ENABLE_DEBUG),1)
    COMPETITIVE_DEFINES += -DDEBUG=1
endif

# Include paths - combine and deduplicate
CPPFLAGS := $(sort $(PROJECT_INCLUDES)) $(SYSTEM_INCLUDES)

# -------------- Build Profile Configuration for Competitive Programming ---- #
# Initialize LDFLAGS with standard library flag if needed
LDFLAGS := $(STDLIB_FLAG)

ifeq ($(BUILD_TYPE), release)
    # Release: Maximum optimization for contest submission
    # -O2 is often preferred over -O3 for competitive programming (more predictable)
    
    # Platform-specific optimization flags
    ifeq ($(detected_os),MACOS)
        # On macOS ARM, use mcpu instead of march for better GCC compatibility
        ARCH_FLAGS := -mcpu=native
    else
        # On Linux, march=native usually works fine
        ARCH_FLAGS := -march=native -mtune=native
    endif
    
    PROFILE_FLAGS := -O2 -DNDEBUG $(ARCH_FLAGS)
    # Disable warnings in release mode to reduce noise
    WARNING_FLAGS := -w
    $(info $(C_GREEN)Build mode: Release (optimized for submission)$(C_RESET))
else ifeq ($(BUILD_TYPE), sanitize)
    # Sanitize: Debug with memory sanitizers for finding bugs
    PROFILE_FLAGS := -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer
    WARNING_FLAGS := -Wall -Wextra
    $(info $(C_YELLOW)Build mode: Sanitize (debug with memory checks)$(C_RESET))
    
    # Configure sanitizer library path based on compiler and platform
    ifeq ($(NEEDS_GCC),yes)
        ifeq ($(detected_os),MACOS)
            # On macOS with Homebrew GCC, try multiple strategies to find libasan
            
            # Strategy 1: Use GCC's own library search
            LIBASAN_PATH := $(shell $(CXX) -print-file-name=libasan.dylib 2>/dev/null)
            ifneq ($(LIBASAN_PATH),libasan.dylib)
                # Found it! Use the directory
                SANITIZER_LIB_DIR := $(dir $(LIBASAN_PATH))
                SANITIZER_LDFLAGS := -L$(SANITIZER_LIB_DIR) -Wl,-rpath,$(SANITIZER_LIB_DIR)
                $(info $(C_CYAN)Found libasan at: $(LIBASAN_PATH)$(C_RESET))
            else
                # Strategy 2: Search in common Homebrew GCC locations
                GCC_BASE := $(shell dirname $(GCC_BINARY))/..
                POSSIBLE_PATHS := $(GCC_BASE)/lib $(GCC_BASE)/lib/gcc/*/15 $(GCC_BASE)/lib/gcc/current/gcc/*/15
                
                LIBASAN_FOUND := $(shell for path in $(POSSIBLE_PATHS); do \
                    if [ -f "$path/libasan.dylib" ]; then \
                        echo "$path"; \
                        break; \
                    fi; \
                done)
                
                ifneq ($(LIBASAN_FOUND),)
                    SANITIZER_LDFLAGS := -L$(LIBASAN_FOUND) -Wl,-rpath,$(LIBASAN_FOUND)
                    $(info $(C_CYAN)Found libasan in: $(LIBASAN_FOUND)$(C_RESET))
                else
                    # Strategy 3: Fallback - use static linking or disable specific sanitizers
                    $(info $(C_YELLOW)Warning: libasan not found, trying alternative sanitizer config$(C_RESET))
                    # Try without address sanitizer, keep undefined behavior sanitizer
                    PROFILE_FLAGS := -g -O1 -fsanitize=undefined -fno-omit-frame-pointer
                    SANITIZER_LDFLAGS :=
                endif
            endif
        else
            # Linux: Standard approach
            LIBASAN_PATH := $(shell $(CXX) -print-file-name=libasan.so 2>/dev/null)
            ifneq ($(LIBASAN_PATH),libasan.so)
                SANITIZER_LIB_DIR := $(dir $(LIBASAN_PATH))
                SANITIZER_LDFLAGS := -L$(SANITIZER_LIB_DIR)
            endif
        endif
    else
        # For Clang: Use system paths, sanitizers should be available by default
        SANITIZER_LDFLAGS :=
    endif
else
    # Debug: Full debug info, no optimization - best for development
    BUILD_TYPE    := debug
    PROFILE_FLAGS := -g -O0
    # More permissive warnings for competitive programming
    WARNING_FLAGS := -Wall -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare
    $(info $(C_BLUE)Build mode: Debug (development and testing)$(C_RESET))
endif

# Define final object directory based on build type
OBJDIR := $(OBJDIR_BASE)/$(BUILD_TYPE)

# Final compiler flags for competitive programming
# Add CXXFLAGS_TOOLCHAIN and STDLIB_FLAG which contains toolchain-specific flags
CXXFLAGS := $(COMMON_FLAGS) $(WARNING_FLAGS) $(PROFILE_FLAGS) $(COMPETITIVE_DEFINES) $(CXXFLAGS_TOOLCHAIN) $(STDLIB_FLAG)

# -------------- Linker Configuration for Competitive Programming ------------ #
# Add GCC library path if using GCC and not in IDE config mode
ifeq ($(strip $(NEEDS_GCC)$(GCC_BINARY)),yesyes)
    ifneq ($(IDE_CONFIG),yes)
        # Get the lib directory relative to the GCC binary
        GCC_LIB_BASE := $(shell dirname $(GCC_BINARY))/../lib
        LDFLAGS += -L$(GCC_LIB_BASE)
        
        # On macOS, also add rpath for runtime library discovery
        ifeq ($(detected_os),MACOS)
            LDFLAGS += -Wl,-rpath,$(GCC_LIB_BASE)
        endif
    endif
endif

# Add sanitizer-specific linker flags if in sanitize mode
ifeq ($(BUILD_TYPE), sanitize)
    LDFLAGS += $(SANITIZER_LDFLAGS)
endif

# Standard math library (commonly needed in competitive programming)
LIBS := -lm

# =========================================================================== #
# ------------------ Source Detection for Competitive Programming ----------- # 
# =========================================================================== #
CPP_SRCS       := $(wildcard $(SRCDIR)/$(TARGET_NAME).cpp \
                             $(SRCDIR)/$(TARGET_NAME).cc \
														 $(SRCDIR)/$(TARGET_NAME).cxx)
CPP_OBJS       := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(filter %.cpp,$(CPP_SRCS))) \
                  $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(filter %.cc,$(CPP_SRCS))) \
                  $(patsubst $(SRCDIR)/%.cxx,$(OBJDIR)/%.o,$(filter %.cxx,$(CPP_SRCS)))
OBJS           := $(CPP_OBJS)
DEPS           := $(OBJS:.o=.d)

# Sentinel file to track directory creation
DIR_SENTINEL := $(OBJDIR)/.dirs_created

# =========================================================================== #
# -------------------- Linker Configuration for C++ Only -------------------- #
# =========================================================================== #
ifeq ($(strip $(CPP_SRCS)),)
	LD = @echo "$(C_RED)No C++ sources found for competitive programming!$(C_RESET)"; false
else
	# Use appropriate compiler for linking in sanitize mode
	ifeq ($(SANITIZE_MODE_COMPILER),yes)
		LD := $(SANITIZER_COMPILER)
		# For Clang in sanitize mode, use simpler flags
		LDFLAGS_FINAL := $(PROFILE_FLAGS) $(LIBS)
	else
		LD := $(CXX)
		LDFLAGS_FINAL := $(LDFLAGS) $(CXXFLAGS)
	endif
endif
LDFLAGS_FINAL += $(LIBS)

# =========================================================================== #
# ------------------------- Competitive Programming Rules ------------------- #
# =========================================================================== #
.PHONY: all clean distclean run test compile_commands debug-info help contest config-ide

# Default target
all: $(TARGET_PATH)

# Directory creation with sentinel
$(DIR_SENTINEL):
	@echo "$(C_CYAN)Creating directories $(OBJDIR) and $(BINDIR)...$(C_RESET)"
	@$(MKDIR_P) $(OBJDIR) $(BINDIR)
	@touch $@

# Linking rule
$(TARGET_PATH): $(OBJS)
	@echo "$(C_BLUE)# ------ Linking Competitive Programming Executable ------ #$(C_RESET)"
	@echo "$(C_BLUE)Output:$(C_RESET) $@"
	$(LD) $(OBJS) $(LDFLAGS_FINAL) -o $@

# Compilation rules with improved logging
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DIR_SENTINEL)
	@echo "$(C_CYAN)Compiling C++:$(C_RESET) $< -> $@"
	@# Use appropriate compiler and flags based on mode
	@if [ "$(SANITIZE_MODE_COMPILER)" = "yes" ]; then \
		echo "$(C_YELLOW)  Using Clang for sanitize mode compilation$(C_RESET)"; \
		$(SANITIZER_COMPILER) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@; \
	else \
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@; \
	fi

$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(DIR_SENTINEL)
	@echo "$(C_CYAN)Compiling C++:$(C_RESET) $< -> $@"
	@# Use appropriate compiler and flags based on mode
	@if [ "$(SANITIZE_MODE_COMPILER)" = "yes" ]; then \
		echo "$(C_YELLOW)  Using Clang for sanitize mode compilation$(C_RESET)"; \
		$(SANITIZER_COMPILER) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@; \
	else \
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@; \
	fi

$(OBJDIR)/%.o: $(SRCDIR)/%.cxx $(DIR_SENTINEL)
	@echo "$(C_CYAN)Compiling C++:$(C_RESET) $< -> $@"
	@# Use appropriate compiler and flags based on mode
	@if [ "$(SANITIZE_MODE_COMPILER)" = "yes" ]; then \
		echo "$(C_YELLOW)  Using Clang for sanitize mode compilation$(C_RESET)"; \
		$(SANITIZER_COMPILER) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@; \
	else \
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@; \
	fi

# Include dependency files
-include $(DEPS)

# =========================================================================== #
# ----------------- Competitive Programming Specific Targets ---------------- #
# =========================================================================== #

# Quick build for contest environment (release mode)
contest: clean
	@$(MAKE) BUILD_TYPE=release all

# Run with standardized input file location
INPUT_FILE ?= $(INPUT_DIR)/$(TARGET_NAME).in
OUTPUT_FILE ?= $(OUTPUT_DIR)/$(TARGET_NAME).out
ARGS ?=

run: all
	@echo "$(C_BLUE)Running $(TARGET_PATH)...$(C_RESET)"
	@if [ ! -f "$(INPUT_FILE)" ]; then \
		echo "$(C_YELLOW)Input file '$(INPUT_FILE)' not found, running without redirection.$(C_RESET)"; \
		./$(TARGET_PATH) $(ARGS); \
	else \
		echo "$(C_CYAN)Input from: $(INPUT_FILE)$(C_RESET)"; \
		./$(TARGET_PATH) $(ARGS) < $(INPUT_FILE); \
	fi

# Test with standardized input/output file locations
test: all
	@echo "$(C_BLUE)Running $(TARGET_PATH) with I/O redirection...$(C_RESET)"
	@if [ ! -f "$(INPUT_FILE)" ]; then \
		echo "$(C_RED)Error: Input file '$(INPUT_FILE)' not found for testing.$(C_RESET)"; \
		exit 1; \
	fi
	@$(MKDIR_P) $(OUTPUT_DIR)
	./$(TARGET_PATH) $(ARGS) < $(INPUT_FILE) > $(OUTPUT_FILE)
	@echo "$(C_GREEN)Output written to $(OUTPUT_FILE)$(C_RESET)"

# =========================================================================== #
# ----------------- Platform-Aware IDE Configuration System ----------------- #
# =========================================================================== #

.PHONY: init-ide-config
init-ide-config:
	@echo "$(C_CYAN)Initializing IDE configuration directory...$(C_RESET)"
	@$(MKDIR_P) $(IDE_CONFIG_DIR)
	@echo "$(C_GREEN)✓ IDE config directory ensured: $(IDE_CONFIG_DIR)/$(C_RESET)"
	@echo "$(C_YELLOW)Place your IDE templates (e.g., 'clangd-macos.yml') in this directory.$(C_RESET)"
	@if [ ! -f "$(IDE_CONFIG_DIR)/clangd-macos.yml" ]; then \
		echo "$(C_YELLOW)Warning: Template 'clangd-macos.yml' not found. 'config-ide' may fail on macOS.$(C_RESET)"; \
	fi

.PHONY: config-ide
config-ide:
	@echo "$(C_BOLD)----- Configuring IDE Integration (Clangd) -----$(C_RESET)"
	@echo "$(C_CYAN)Detected OS: $(detected_os)$(C_RESET)"
	
	@echo "$(C_CYAN)Step 1: Managing platform-specific .clangd configuration...$(C_RESET)"
	@if [ "$(NEEDS_GCC)" = "yes" ]; then \
		if [ "$(detected_os)" = "MACOS" ]; then \
			if [ -z "$(GCC_BINARY)" ]; then \
				echo "$(C_RED)Error: Cannot configure IDE because compatible GCC was not found.$(C_RESET)"; \
				exit 1; \
			fi; \
			echo "$(C_YELLOW)macOS + Competitive Headers detected. Deploying .clangd config...$(C_RESET)"; \
			CLANGD_TEMPLATE="$(IDE_CONFIG_DIR)/clangd-macos.yml"; \
			if [ -f "$$CLANGD_TEMPLATE" ]; then \
				cp "$$CLANGD_TEMPLATE" .clangd; \
				echo "$(C_GREEN)✓ .clangd configured for GCC toolchain using template: $$CLANGD_TEMPLATE$(C_RESET)"; \
			else \
				echo "$(C_RED)Error: .clangd template not found at '$$CLANGD_TEMPLATE'$(C_RESET)"; \
				echo "$(C_YELLOW)Please run 'make init-ide-config' and ensure the template exists.$(C_RESET)"; \
				exit 1; \
			fi; \
		else \
			echo "$(C_CYAN)Linux/Unix detected. Native GCC support, removing .clangd...$(C_RESET)"; \
			$(RM) .clangd 2>/dev/null || true; \
			echo "$(C_GREEN)✓ Clean environment configured for Linux$(C_RESET)"; \
		fi; \
	else \
		echo "$(C_CYAN)Standard C++ (Clang) setup detected. Ensuring clean environment...$(C_RESET)"; \
		$(RM) .clangd 2>/dev/null || true; \
		echo "$(C_GREEN)✓ Clean Clang environment configured$(C_RESET)"; \
	fi

	@echo "$(C_CYAN)Step 2: Generating compile_commands.json...$(C_RESET)"
	@if ! command -v compiledb > /dev/null; then \
		echo "$(C_RED)Error: 'compiledb' is not installed. Cannot generate compile_commands.json.$(C_RESET)"; \
		echo "$(C_YELLOW)Please install it with: pip install compiledb$(C_RESET)"; \
		exit 1; \
	fi
	@echo "$(C_YELLOW)Note: Using clean configuration without toolchain-specific flags for clangd.$(C_RESET)"
	@compiledb -n make clean all IDE_CONFIG=yes BUILD_TYPE=$(BUILD_TYPE)
	
	@echo "$(C_BOLD)$(C_GREEN)✓ IDE Configuration Complete!$(C_RESET)"
	@echo "$(C_YELLOW)Please reload your editor to apply the changes.$(C_RESET)"

# =========================================================================== #
# ---------------------- Benchmark Integration Targets ---------------------- #
# =========================================================================== #
.PHONY: benchmark create-bench-tests compare

# Variables for benchmark targets
BENCH_TARGET ?= $(TARGET_NAME)
BENCH_TESTS  ?= small medium large
SOLUTIONS    ?=

# Create standard benchmark test cases (small, medium, large, stress)
create-bench-tests:
	@if [ ! -f "$(SCRIPTS_DIR)/benchmark.sh" ]; then echo "$(C_RED)benchmark.sh not found in '$(SCRIPTS_DIR)/'!$(C_RESET)"; exit 1; fi
	@./$(SCRIPTS_DIR)/benchmark.sh create-tests
	
# Run benchmark for a specific target
benchmark:
	@if [ ! -f "$(SCRIPTS_DIR)/benchmark.sh" ]; then echo "$(C_RED)benchmark.sh not found in '$(SCRIPTS_DIR)/'!$(C_RESET)"; exit 1; fi
	@echo "$(C_CYAN)Running benchmark for target $(BENCH_TARGET) with tests: $(BENCH_TESTS)...$(C_RESET)"
	@./$(SCRIPTS_DIR)/benchmark.sh bench $(BENCH_TARGET).cpp $(BENCH_TESTS)

# Compare multiple solutions
compare:
	@if [ ! -f "$(SCRIPTS_DIR)/benchmark.sh" ]; then echo "$(C_RED)benchmark.sh not found in '$(SCRIPTS_DIR)/'!$(C_RESET)"; exit 1; fi
	@if [ -z "$(SOLUTIONS)" ]; then echo "$(C_RED)Please specify solutions to compare, e.g., make compare SOLUTIONS=\"A.cpp B.cpp\"$(C_RESET)"; exit 1; fi
	@echo "$(C_CYAN)Comparing solutions: $(SOLUTIONS)...$(C_RESET)"
	@./$(SCRIPTS_DIR)/benchmark.sh compare $(SOLUTIONS)

# =========================================================================== #
# ----------------------- Utility & Development Targets --------------------- #
# =========================================================================== #

clean:
	@echo "$(C_YELLOW)Cleaning build artifacts...$(C_RESET)"
	-$(RM) $(OBJDIR)$(PathSep)*.o
	-$(RM) $(OBJDIR)$(PathSep)*.d
	-$(RM) $(BINDIR)$(PathSep)*
	-$(RM) $(DIR_SENTINEL)
	@echo "$(C_GREEN)Clean complete.$(C_RESET)"

distclean: clean
	@echo "$(C_YELLOW)Cleaning all directories and temporary files...$(C_RESET)"
	-$(RMDIR_CMD) $(OBJDIR) $(BINDIR) $(OUTPUT_DIR)
	# Remove competitive programming artifacts and IDE configuration
	-$(RM) *~ *.bak .clangd compile_commands.json
	@echo "$(C_GREEN)Distclean complete.$(C_RESET)"

# Compilation Database for clangd (essential for competitive programming IDEs)
compile_commands:
	@echo "$(C_CYAN)Generating compile_commands.json for IDE support...$(C_RESET)"
	@compiledb -n make clean all BUILD_TYPE=$(BUILD_TYPE)

debug-info:
	@echo "$(C_BOLD)/====------------ Competitive Programming Debug Info ------------====/$(C_RESET)"
	@echo "$(C_CYAN)Build Configuration:$(C_RESET)"
	@echo "  UNAME_S: $(UNAME_S)"
	@echo "  Detected OS: $(detected_os)"
	@echo "  PathSep: $(PathSep)"
	@echo "  BUILD_TYPE: $(BUILD_TYPE)"
	@echo "  IDE_CONFIG: $(IDE_CONFIG)"
	@echo "  TARGET_EXEC: $(TARGET_EXEC)"
	@echo "  TARGET_NAME: $(TARGET_NAME)"
	@echo "  TARGET_PATH: $(TARGET_PATH)"
	@echo "  SRCDIR: $(SRCDIR), OBJDIR: $(OBJDIR), BINDIR: $(BINDIR)"
	@echo ""
	@echo "$(C_CYAN)Sources & Objects:$(C_RESET)"
	@echo "  CPP_SRCS: $(CPP_SRCS)"
	@echo "  OBJS: $(OBJS)"
	@echo "  DEPS: $(DEPS)"
	@echo ""
	@echo "$(C_CYAN)Competitive Programming Detection:$(C_RESET)"
	@echo "  NEEDS_GCC: $(NEEDS_GCC)"
	@echo "  NEEDS_BITS_STDC: $(if $(NEEDS_BITS_STDC),YES ($(NEEDS_BITS_STDC)),NO)"
	@echo "  NEEDS_PBDS: $(if $(NEEDS_PBDS),YES ($(NEEDS_PBDS)),NO)"
	@echo ""
	@echo "$(C_CYAN)Compiler & Linker Configuration:$(C_RESET)"
	@echo "  CC: $(CC), CXX: $(CXX), LD: $(LD)"
	@echo "  GCC_PREFIX: $(GCC_PREFIX)"
	@echo "  GCC_BINARY: $(GCC_BINARY)"
	@echo ""
	@echo "$(C_CYAN)IDE Integration:$(C_RESET)"
	@echo "  IDE_CONFIG_DIR: $(IDE_CONFIG_DIR)"
	@echo "  .clangd needed: $(if $(filter $(NEEDS_GCC)-$(detected_os),yes-MACOS),YES,NO)"
	@echo "  compile_commands.json: $(if $(wildcard compile_commands.json),EXISTS,MISSING)"
	@echo ""
	@echo "$(C_CYAN)Build Flags:$(C_RESET)"
	@echo "  CPPFLAGS:"
	@echo "$(CPPFLAGS)" | tr ' ' '\n' | sed 's/^-/    -/' | head -20
	@echo "  CXXFLAGS:"
	@echo "$(CXXFLAGS)" | tr ' ' '\n' | sed 's/^-/    -/' | head -20
	@echo "  LDFLAGS_FINAL:"
	@echo "$(LDFLAGS_FINAL)" | tr ' ' '\n' | sed 's/^-/    -/' | head -20
	@echo ""
	@echo "$(C_CYAN)Sanitizer Configuration (if sanitize mode):$(C_RESET)"
	@if [ "$(BUILD_TYPE)" = "sanitize" ]; then \
		echo "  SANITIZER_LDFLAGS: $(SANITIZER_LDFLAGS)"; \
		if [ "$(NEEDS_GCC)" = "yes" ]; then \
			echo "  Libasan detection results:"; \
			LIBASAN_CHECK=$($(CXX) -print-file-name=libasan.dylib 2>/dev/null); \
			if [ "$LIBASAN_CHECK" != "libasan.dylib" ]; then \
				echo "    ✓ Found: $LIBASAN_CHECK"; \
			else \
				echo "    ✗ Not found via print-file-name"; \
				echo "    Searching manually in GCC directories..."; \
				find $(shell dirname $(GCC_BINARY))/.. -name "*asan*" -type f 2>/dev/null | head -5 || echo "    No libasan found anywhere"; \
			fi; \
		fi; \
	else \
		echo "  (Not in sanitize mode)"; \
	fi
	@echo ""
	@echo "$(C_CYAN)OS Commands:$(C_RESET)"
	@echo "  RM: $(RM)"
	@echo "  RMDIR_CMD: $(RMDIR_CMD)"
	@echo "  MKDIR_P: $(MKDIR_P)"
	@echo "$(C_BOLD)/====------------------------------------------------------------====/$(C_RESET)"

help:
	@echo "$(C_BOLD)Competitive Programming Makefile$(C_RESET)"
	@echo "This Makefile is best used with the 'contest-helper.sh' script."
	@echo "Usage: make $(C_YELLOW)[TARGET] [VARIABLE=VALUE]$(C_RESET)"
	@echo ""
	@echo "$(C_CYAN)Core Build Targets:$(C_RESET)"
	@echo "  $(C_GREEN)all$(C_RESET)         - Build with specified BUILD_TYPE (default: debug)"
	@echo "  $(C_GREEN)contest$(C_RESET)     - Quick release build for contests"
	@echo "  $(C_GREEN)run$(C_RESET)         - Build and run (uses 'input/\$(TARGET_NAME).in' by default)"
	@echo "  $(C_GREEN)test$(C_RESET)        - Run with I/O redirection ('input/' -> 'output/')"
	@echo ""
	@echo "$(C_CYAN)IDE & Tooling:$(C_RESET)"
	@echo "  $(C_GREEN)config-ide$(C_RESET)         - Configure IDE (clangd) for the current platform"
	@echo "  $(C_GREEN)init-ide-config$(C_RESET)    - Create/verify the IDE config directory and templates"
	@echo ""
	@echo "$(C_CYAN)Benchmarking Targets:$(C_RESET)"
	@echo "  $(C_GREEN)create-bench-tests$(C_RESET) - Create standard benchmark input files"
	@echo "  $(C_GREEN)benchmark$(C_RESET)          - Benchmark a target (e.g., 'make benchmark BENCH_TARGET=A')"
	@echo "  $(C_GREEN)compare$(C_RESET)            - Compare solutions  (e.g., 'make compare SOLUTIONS=\"A.cpp B.cpp\"')"
	@echo ""
	@echo "$(C_CYAN)Cleaning Targets:$(C_RESET)"
	@echo "  $(C_GREEN)clean$(C_RESET)       - Remove build artifacts"
	@echo "  $(C_GREEN)distclean$(C_RESET)   - Remove everything including output and IDE config"
	@echo ""
	@echo "$(C_CYAN)Debug & Info:$(C_RESET)"
	@echo "  $(C_GREEN)debug-info$(C_RESET)  - Show detailed build configuration"
	@echo "  $(C_GREEN)help$(C_RESET)        - Show this message"

.SUFFIXES:

# =========================================================================== #