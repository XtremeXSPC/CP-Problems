# Enhanced Makefile for Competitive Programming C++ projects
# Optimized for GCC with bits/stdc++.h and pb_ds support

# =========================================================================== #
# ------------------------ System Pre-configuration  ------------------------ #
# =========================================================================== #
# Use Homebrew's bash if available to avoid macOS SIP issues with tools like
# 'bear', which rely on DYLD_INSERT_LIBRARIES.
HOMEBREW_BASH := $(shell which bash | grep 'homebrew')
ifeq ($(strip $(HOMEBREW_BASH)),)
    SHELL := /bin/bash
else
    SHELL := $(firstword $(HOMEBREW_BASH))
endif

.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DEFAULT_GOAL := all

# =========================================================================== #
# -------------------------- ANSI Color Codes ------------------------------- #
# =========================================================================== #
# Check if terminal supports colors and define color variables.
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
# ----------------------- User-configurable Variables ----------------------- #
# =========================================================================== #

TARGET_NAME    ?= Tester
SRCDIR         ?= .
BINDIR         ?= bin
OBJDIR_BASE    ?= obj

# Project directories
INPUT_DIR      ?= input
OUTPUT_DIR     ?= output
IDE_CONFIG_DIR ?= .ide-config
SCRIPTS_DIR    ?= scripts

# Build profile for competitive programming: debug (default), release, or sanitize
BUILD_TYPE     ?= debug

# IDE configuration mode - when set to 'yes', produces clean compile commands
# without toolchain-specific flags that can confuse clangd
IDE_CONFIG     ?= no

# Competitive programming settings
ENABLE_FAST_IO ?= 1
ENABLE_DEBUG   ?= 1

# =========================================================================== #
# -------------- Platform Detection and Path Configuration ------------------ #
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
	TARGET_EXEC := $(TARGET_NAME).exe
  # Cygwin prefers Unix-style slashes
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
OBJDIR := $(OBJDIR_BASE)/$(BUILD_TYPE)

# =========================================================================== #
# ------------- Competitive Programming Compiler Configuration -------------- #
# =========================================================================== #

# ------------- Enhanced CP header detection (ignores comments) ------------- #
# Use the get_gcc_includes script to detect competitive programming headers
NEEDS_GCC_RESULT := $(shell ./$(SCRIPTS_DIR)/get_gcc_includes.sh --check-competitive $(SRCDIR) 2>/dev/null || echo "no")
NEEDS_GCC := $(NEEDS_GCC_RESULT)

# Debug output for competitive header detection
ifeq ($(NEEDS_GCC),yes)
    $(info $(C_CYAN)Competitive programming headers detected - GCC required$(C_RESET))
else
    $(info $(C_GREEN)No competitive programming headers found - Clang can be used$(C_RESET))
endif

# ------------------------ Compiler Detection Logic ------------------------- #
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
        # Platform-Aware Heuristic
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
    endif
endif

# -------------------- Enhanced Compiler Selection Logic -------------------- #
EFFECTIVE_COMPILER := 
EFFECTIVE_CC := 
USE_PCH_FALLBACK := no
SANITIZER_AVAILABLE := no

# Determine effective compiler based on build type and GCC availability
ifeq ($(BUILD_TYPE),sanitize)
    ifeq ($(NEEDS_GCC),yes)
        ifneq ($(GCC_BINARY),)
            # Test if GCC sanitizers are available
            GCC_SANITIZER_TEST := $(shell echo 'int main(){}' | $(GCC_BINARY) -x c++ -fsanitize=address,undefined - -o /tmp/sanitizer_test 2>/dev/null && echo "yes" || echo "no")
            ifeq ($(GCC_SANITIZER_TEST),yes)
                SANITIZER_AVAILABLE := yes
                EFFECTIVE_COMPILER := gcc
                EFFECTIVE_CC := $(subst g++,gcc,$(GCC_BINARY))
                $(info $(C_CYAN)Using GCC with sanitizers for competitive programming$(C_RESET))
            else
                SANITIZER_AVAILABLE := yes
                EFFECTIVE_COMPILER := clang
                EFFECTIVE_CC := clang
                USE_PCH_FALLBACK := yes
                $(info $(C_YELLOW)GCC sanitizers unavailable, using Clang with PCH fallback$(C_RESET))
            endif
        else
            SANITIZER_AVAILABLE := yes
            EFFECTIVE_COMPILER := clang
            EFFECTIVE_CC := clang
            USE_PCH_FALLBACK := yes
            $(info $(C_YELLOW)No GCC found, using Clang with PCH fallback for sanitizers$(C_RESET))
        endif
    else
        SANITIZER_AVAILABLE := yes
        EFFECTIVE_COMPILER := clang
        EFFECTIVE_CC := clang
        $(info $(C_CYAN)Using Clang with sanitizers for standard C++$(C_RESET))
    endif
else
    # Non-sanitize modes - use original logic
    ifeq ($(NEEDS_GCC),yes)
        ifneq ($(GCC_BINARY),)
            EFFECTIVE_COMPILER := gcc
            EFFECTIVE_CC := $(subst g++,gcc,$(GCC_BINARY))
            $(info $(C_CYAN)Using GCC for competitive programming$(C_RESET))
        else
            $(error $(C_RED)Competitive programming headers detected but compatible GCC not found!$(C_RESET))
        endif
    else
        EFFECTIVE_COMPILER := clang
        EFFECTIVE_CC := clang
        $(info $(C_CYAN)Using Clang for standard C++$(C_RESET))
    endif
endif

# Set final compiler variables based on effective compiler
ifeq ($(EFFECTIVE_COMPILER),gcc)
    CC := $(EFFECTIVE_CC)
    CXX := $(GCC_BINARY)
else
    CC := clang
    CXX := clang++
endif

# =========================================================================== #
# ----------------- Enhanced Toolchain Configuration System ----------------- #
# =========================================================================== #

# Initialize toolchain-specific variables
SYSTEM_INCLUDES := -I/usr/local/include
CXXFLAGS_TOOLCHAIN := 
STDLIB_FLAG := 
PCH_INCLUDE :=

# Configure toolchain based on effective compiler and mode
ifeq ($(EFFECTIVE_COMPILER),gcc)
    ifeq ($(USE_PCH_FALLBACK),no)
        # Standard GCC configuration for competitive programming
        ifneq ($(GCC_BINARY),)
            GCC_ISYSTEM_FLAGS := $(shell ./$(SCRIPTS_DIR)/get_gcc_includes.sh $(CXX))
        else
            GCC_ISYSTEM_FLAGS :=
        endif

        # For IDE configuration, we skip the toolchain-specific flags
        ifeq ($(IDE_CONFIG),yes)
            CXXFLAGS_TOOLCHAIN :=
            SYSTEM_INCLUDES := -I/usr/local/include
            STDLIB_FLAG :=
        else
            # Use -nostdinc++ to prevent default search paths
            ifneq ($(GCC_ISYSTEM_FLAGS),)
                CXXFLAGS_TOOLCHAIN := -nostdinc++
                SYSTEM_INCLUDES := $(GCC_ISYSTEM_FLAGS)
            else
                CXXFLAGS_TOOLCHAIN :=
                SYSTEM_INCLUDES := -I/usr/local/include
            endif

            # Platform-specific standard library flag for macOS
            ifeq ($(detected_os),MACOS)
                STDLIB_FLAG := -stdlib=libstdc++
            endif
        endif
    else
        # PCH fallback mode
        CXXFLAGS_TOOLCHAIN := -DUSE_PCH_FALLBACK=1 -DUSE_CLANG_SANITIZE=1
        PCH_INCLUDE := 
        SYSTEM_INCLUDES := -I/usr/local/include
        STDLIB_FLAG := 
        $(info $(C_YELLOW)Using PCH.h fallback for competitive programming headers$(C_RESET))
    endif
else
    # Clang configuration
    CXXFLAGS_TOOLCHAIN := 
    SYSTEM_INCLUDES := -I/usr/local/include
    STDLIB_FLAG := 
    
    ifeq ($(USE_PCH_FALLBACK),yes)
        CXXFLAGS_TOOLCHAIN := -DUSE_PCH_FALLBACK=1 -DUSE_CLANG_SANITIZE=1
        PCH_INCLUDE := 
        $(info $(C_YELLOW)PCH fallback mode enabled for Clang$(C_RESET))
    endif
endif

# Define project-specific include directories
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

# Include paths
CPPFLAGS := $(sort $(PROJECT_INCLUDES)) $(SYSTEM_INCLUDES)

# ------------------- Enhanced Build Profile Configuration ------------------- #
# Initialize LDFLAGS with standard library flag if needed
LDFLAGS := $(STDLIB_FLAG)

ifeq ($(BUILD_TYPE), release)
    # Release: Maximum optimization for contest submission
    ifeq ($(detected_os),MACOS)
        ARCH_FLAGS := -mcpu=native
    else
        ARCH_FLAGS := -march=native -mtune=native
    endif
    
    PROFILE_FLAGS := -O2 -DNDEBUG $(ARCH_FLAGS)
    WARNING_FLAGS := -w
    $(info $(C_GREEN)Build mode: Release (optimized for submission)$(C_RESET))
else ifeq ($(BUILD_TYPE), sanitize)
    # Enhanced sanitize mode
    ifeq ($(SANITIZER_AVAILABLE),no)
        $(error $(C_RED)Sanitizers not available with current configuration$(C_RESET))
    endif
		
    # Base sanitizer flags
    SANITIZER_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer
    PROFILE_FLAGS := -g -O1 $(SANITIZER_FLAGS)
    WARNING_FLAGS := -Wall -Wextra
    $(info $(C_YELLOW)Build mode: Sanitize (debug with memory checks)$(C_RESET))
    
    # Enhanced sanitizer library configuration
    ifeq ($(EFFECTIVE_COMPILER),gcc)
        # GCC sanitizer library setup
        ifeq ($(detected_os),MACOS)
            # macOS GCC sanitizer library detection
            LIBASAN_PATH := $(shell $(CXX) -print-file-name=libasan.dylib 2>/dev/null)
            ifneq ($(LIBASAN_PATH),libasan.dylib)
                SANITIZER_LIB_DIR := $(dir $(LIBASAN_PATH))
                LDFLAGS += -L$(SANITIZER_LIB_DIR) -Wl,-rpath,$(SANITIZER_LIB_DIR)
                $(info $(C_CYAN)Found GCC libasan at: $(LIBASAN_PATH)$(C_RESET))
            else
                # Fallback search in GCC installation
                GCC_BASE := $(shell dirname $(GCC_BINARY))/..
                LIBASAN_SEARCH := $(shell find $(GCC_BASE) -name "libasan.*" -type f 2>/dev/null | head -1)
                ifneq ($(LIBASAN_SEARCH),)
                    SANITIZER_LIB_DIR := $(dir $(LIBASAN_SEARCH))
                    LDFLAGS += -L$(SANITIZER_LIB_DIR) -Wl,-rpath,$(SANITIZER_LIB_DIR)
                    $(info $(C_CYAN)Found GCC libasan at: $(LIBASAN_SEARCH)$(C_RESET))
                else
                    $(info $(C_YELLOW)Warning: GCC libasan not found, linking may fail$(C_RESET))
                endif
            endif
        else
            # Linux GCC - standard library paths should work
            LIBASAN_PATH := $(shell $(CXX) -print-file-name=libasan.so 2>/dev/null)
            ifneq ($(LIBASAN_PATH),libasan.so)
                SANITIZER_LIB_DIR := $(dir $(LIBASAN_PATH))
                LDFLAGS += -L$(SANITIZER_LIB_DIR)
                $(info $(C_CYAN)Found GCC libasan at: $(LIBASAN_PATH)$(C_RESET))
            endif
        endif
    else
        $(info $(C_CYAN)Using Clang sanitizers with system libraries$(C_RESET))
    endif
else
    # Debug: Full debug info, no optimization
    BUILD_TYPE    := debug
    PROFILE_FLAGS := -g -O0
    WARNING_FLAGS := -Wall -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare
    $(info $(C_BLUE)Build mode: Debug (development and testing)$(C_RESET))
endif

# Final compiler flags
CXXFLAGS := $(COMMON_FLAGS) $(WARNING_FLAGS) $(PROFILE_FLAGS) $(COMPETITIVE_DEFINES) $(CXXFLAGS_TOOLCHAIN)

# ---------------------- Enhanced Linker Configuration ---------------------- #
# Add GCC library path if using GCC and not in IDE config mode
ifeq ($(EFFECTIVE_COMPILER),gcc)
    ifneq ($(IDE_CONFIG),yes)
        ifneq ($(USE_PCH_FALLBACK),yes)
            # Standard GCC linking
            GCC_LIB_BASE := $(shell dirname $(GCC_BINARY))/../lib
            LDFLAGS += -L$(GCC_LIB_BASE)
            # On macOS, add rpath for runtime library discovery
            ifeq ($(detected_os),MACOS)
                LDFLAGS += -Wl,-rpath,$(GCC_LIB_BASE)
            endif
        endif
    endif
endif

# Standard math library
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
# ---------------------- Enhanced Linker Configuration ---------------------- #
# =========================================================================== #
ifeq ($(strip $(CPP_SRCS)),)
	LD = @echo "$(C_RED)No C++ sources found for competitive programming!$(C_RESET)"; false
else
	LD := $(CXX)
	LDFLAGS_FINAL := $(LDFLAGS) $(PROFILE_FLAGS) $(LIBS)
endif

# =========================================================================== #
# ---------------------- Competitive Programming Rules ---------------------- #
# =========================================================================== #
.PHONY: all

# Default target
all: $(TARGET_PATH)

# Directory creation with sentinel
$(DIR_SENTINEL):
	@echo "$(C_CYAN)Creating directories $(OBJDIR) and $(BINDIR)...$(C_RESET)"
	@$(MKDIR_P) $(OBJDIR) $(BINDIR)
	@touch $@

# Enhanced linking rule
$(TARGET_PATH): $(OBJS)
	@echo "$(C_BLUE)# ------ Linking Competitive Programming Executable ------ #$(C_RESET)"
	@echo "$(C_BLUE)Compiler:$(C_RESET) $(CXX)"
	@echo "$(C_BLUE)Mode:$(C_RESET) $(BUILD_TYPE)"
	@echo "$(C_BLUE)Output:$(C_RESET) $@"
	@if [ "$(BUILD_TYPE)" = "sanitize" ]; then \
		echo "$(C_YELLOW)Sanitizer linking - this may take extra time$(C_RESET)"; \
	fi
	$(LD) $(OBJS) $(LDFLAGS_FINAL) -o $@
	@echo "$(C_GREEN)✓ Build successful$(C_RESET)"

# Enhanced compilation rules
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DIR_SENTINEL)
	@echo "$(C_CYAN)Compiling C++:$(C_RESET) $< -> $@"
	@if [ "$(USE_PCH_FALLBACK)" = "yes" ]; then \
		echo "$(C_YELLOW)  Using PCH.h fallback for competitive programming headers$(C_RESET)"; \
	fi
	@if [ "$(BUILD_TYPE)" = "sanitize" ]; then \
		echo "$(C_YELLOW)  Compiling with sanitizers enabled$(C_RESET)"; \
	fi
	@echo "$(C_CYAN)  Compiler:$(C_RESET) $(CXX)"
	@echo "$(C_CYAN)  Includes:$(C_RESET)"
	@echo "$(CPPFLAGS)" | tr ' ' '\n' | sed 's/^/    /'
	@echo "$(C_CYAN)  Options:$(C_RESET)"
	@echo "$(CXXFLAGS)" | tr ' ' '\n' | sed 's/^/    /'
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(DIR_SENTINEL)
	@echo "$(C_CYAN)Compiling C++:$(C_RESET) $< -> $@"
	@if [ "$(USE_PCH_FALLBACK)" = "yes" ]; then \
		echo "$(C_YELLOW)  Using PCH.h fallback for competitive programming headers$(C_RESET)"; \
	fi
	@echo "$(C_CYAN)  Compiler:$(C_RESET) $(CXX)"
	@echo "$(C_CYAN)  Includes:$(C_RESET)"
	@echo "$(CPPFLAGS)" | tr ' ' '\n' | sed 's/^/    /'
	@echo "$(C_CYAN)  Options:$(C_RESET)"
	@echo "$(CXXFLAGS)" | tr ' ' '\n' | sed 's/^/    /'
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@
	
$(OBJDIR)/%.o: $(SRCDIR)/%.cxx $(DIR_SENTINEL)
	@echo "$(C_CYAN)Compiling C++:$(C_RESET) $< -> $@"
	@if [ "$(USE_PCH_FALLBACK)" = "yes" ]; then \
		echo "$(C_YELLOW)  Using PCH.h fallback for competitive programming headers$(C_RESET)"; \
	fi
	@echo "$(C_CYAN)  Compiler:$(C_RESET) $(CXX)"
	@echo "$(C_CYAN)  Includes:$(C_RESET)"
	@echo "$(CPPFLAGS)" | tr ' ' '\n' | sed 's/^/    /'
	@echo "$(C_CYAN)  Options:$(C_RESET)"
	@echo "$(CXXFLAGS)" | tr ' ' '\n' | sed 's/^/    /'
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEPS)

# =========================================================================== #
# ----------------- Competitive Programming Specific Targets ---------------- #
# =========================================================================== #
.PHONY: contest run test

contest: clean
	@$(MAKE) BUILD_TYPE=release all

# Enhanced run target
INPUT_FILE ?= $(INPUT_DIR)/$(TARGET_NAME).in
OUTPUT_FILE ?= $(OUTPUT_DIR)/$(TARGET_NAME).out
ARGS ?=

run: all
	@echo "$(C_BLUE)Running $(TARGET_PATH)...$(C_RESET)"
	@if [ "$(BUILD_TYPE)" = "sanitize" ]; then \
		echo "$(C_YELLOW)Running with sanitizers - expect slower performance$(C_RESET)"; \
	fi
	@if [ ! -f "$(INPUT_FILE)" ]; then \
		echo "$(C_YELLOW)Input file '$(INPUT_FILE)' not found, running without redirection.$(C_RESET)"; \
		./$(TARGET_PATH) $(ARGS); \
	else \
		echo "$(C_CYAN)Input from: $(INPUT_FILE)$(C_RESET)"; \
		./$(TARGET_PATH) $(ARGS) < $(INPUT_FILE); \
	fi

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
.PHONY: init-ide-config config-ide

init-ide-config:
	@echo "$(C_CYAN)Initializing IDE configuration directory...$(C_RESET)"
	@$(MKDIR_P) $(IDE_CONFIG_DIR)
	@echo "$(C_GREEN)✓ IDE config directory ensured: $(IDE_CONFIG_DIR)/$(C_RESET)"
	@echo "$(C_YELLOW)Place your IDE templates (e.g., 'clangd-macos.yml') in this directory.$(C_RESET)"
	@if [ ! -f "$(IDE_CONFIG_DIR)/clangd-macos.yml" ]; then \
		echo "$(C_YELLOW)Warning: Template 'clangd-macos.yml' not found. 'config-ide' may fail on macOS.$(C_RESET)"; \
	fi

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

BENCH_TARGET ?= $(TARGET_NAME)
BENCH_TESTS  ?= small medium large
SOLUTIONS    ?=

create-bench-tests:
	@if [ ! -f "$(SCRIPTS_DIR)/benchmark.sh" ]; then echo "$(C_RED)benchmark.sh not found in '$(SCRIPTS_DIR)/'!$(C_RESET)"; exit 1; fi
	@./$(SCRIPTS_DIR)/benchmark.sh create-tests
	
benchmark:
	@if [ ! -f "$(SCRIPTS_DIR)/benchmark.sh" ]; then echo "$(C_RED)benchmark.sh not found in '$(SCRIPTS_DIR)/'!$(C_RESET)"; exit 1; fi
	@echo "$(C_CYAN)Running benchmark for target $(BENCH_TARGET) with tests: $(BENCH_TESTS)...$(C_RESET)"
	@./$(SCRIPTS_DIR)/benchmark.sh bench $(BENCH_TARGET).cpp $(BENCH_TESTS)

compare:
	@if [ ! -f "$(SCRIPTS_DIR)/benchmark.sh" ]; then echo "$(C_RED)benchmark.sh not found in '$(SCRIPTS_DIR)/'!$(C_RESET)"; exit 1; fi
	@if [ -z "$(SOLUTIONS)" ]; then echo "$(C_RED)Please specify solutions to compare, e.g., make compare SOLUTIONS=\"A.cpp B.cpp\"$(C_RESET)"; exit 1; fi
	@echo "$(C_CYAN)Comparing solutions: $(SOLUTIONS)...$(C_RESET)"
	@./$(SCRIPTS_DIR)/benchmark.sh compare $(SOLUTIONS)

# =========================================================================== #
# ----------------------- Utility & Development Targets --------------------- #
# =========================================================================== #
.PHONY: test-colors clean distclean compile_commands debug-info help

test-colors:
	@echo "Testing color configuration..."
	@echo "TTY test: $(shell test -t 1 && echo 'true' || echo 'false')"
	@echo "N_COLORS: '$(N_COLORS)'"
	@echo "TERM: '$(TERM)'"
	@echo "Shell: '$(SHELL)'"
	@echo "Colors test:"
	@echo "$(C_RED)Red text$(C_RESET)"
	@echo "$(C_GREEN)Green text$(C_RESET)"
	@echo "$(C_YELLOW)Yellow text$(C_RESET)"
	@echo "$(C_BLUE)Blue text$(C_RESET)"
	@echo "$(C_CYAN)Cyan text$(C_RESET)"
	@echo "$(C_BOLD)Bold text$(C_RESET)"

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
	-$(RM) *~ *.bak .clangd compile_commands.json
	@echo "$(C_GREEN)Distclean complete.$(C_RESET)"

compile_commands:
	@echo "$(C_CYAN)Generating compile_commands.json for IDE support...$(C_RESET)"
	@compiledb -n make clean all BUILD_TYPE=$(BUILD_TYPE)

# Enhanced debug-info target
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
	@echo "$(C_CYAN)Enhanced Compiler Configuration:$(C_RESET)"
	@echo "  EFFECTIVE_COMPILER: $(EFFECTIVE_COMPILER)"
	@echo "  CC: $(CC), CXX: $(CXX), LD: $(LD)"
	@echo "  GCC_PREFIX: $(GCC_PREFIX)"
	@echo "  GCC_BINARY: $(GCC_BINARY)"
	@echo "  USE_PCH_FALLBACK: $(USE_PCH_FALLBACK)"
	@echo "  SANITIZER_AVAILABLE: $(SANITIZER_AVAILABLE)"
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
	@echo "$(C_BOLD)/====------------------------------------------------------------====/$(C_RESET)"

help:
	@echo "$(C_BOLD)Enhanced Competitive Programming Makefile$(C_RESET)"
	@echo "This Makefile automatically handles GCC/Clang selection and sanitizer fallbacks."
	@echo "Usage: make $(C_YELLOW)[TARGET] [VARIABLE=VALUE]$(C_RESET)"
	@echo ""
	@echo "$(C_CYAN)Core Build Targets:$(C_RESET)"
	@echo "  $(C_GREEN)all$(C_RESET)           - Build with specified BUILD_TYPE (default: debug)"
	@echo "  $(C_GREEN)contest$(C_RESET)       - Quick release build for contests"
	@echo "  $(C_GREEN)run$(C_RESET)           - Build and run (uses 'input/\$(TARGET_NAME).in' by default)"
	@echo "  $(C_GREEN)test$(C_RESET)          - Run with I/O redirection ('input/' -> 'output/')"
	@echo ""
	@echo "$(C_CYAN)Build Types:$(C_RESET)"
	@echo "  $(C_YELLOW)BUILD_TYPE=debug$(C_RESET)     - Debug build (default, -g -O0)"
	@echo "  $(C_YELLOW)BUILD_TYPE=release$(C_RESET)   - Optimized for contest submission (-O2)"
	@echo "  $(C_YELLOW)BUILD_TYPE=sanitize$(C_RESET)  - Debug with memory sanitizers (enhanced)"
	@echo ""
	@echo "$(C_CYAN)IDE & Tooling:$(C_RESET)"
	@echo "  $(C_GREEN)config-ide$(C_RESET)           - Configure IDE (clangd) for the current platform"
	@echo "  $(C_GREEN)init-ide-config$(C_RESET)      - Create/verify the IDE config directory and templates"
	@echo "  $(C_GREEN)compile_commands$(C_RESET)     - Generate compile_commands.json for IDEs"
	@echo ""
	@echo "$(C_CYAN)Benchmarking Targets:$(C_RESET)"
	@echo "  $(C_GREEN)create-bench-tests$(C_RESET) - Create standard benchmark input files"
	@echo "  $(C_GREEN)benchmark$(C_RESET)          - Benchmark a target (e.g., 'make benchmark BENCH_TARGET=A')"
	@echo "  $(C_GREEN)compare$(C_RESET)            - Compare solutions  (e.g., 'make compare SOLUTIONS=\"A.cpp B.cpp\"')"
	@echo ""
	@echo "$(C_CYAN)Cleaning Targets:$(C_RESET)"
	@echo "  $(C_GREEN)clean$(C_RESET)         - Remove build artifacts"
	@echo "  $(C_GREEN)distclean$(C_RESET)     - Remove everything including output and IDE config"
	@echo ""
	@echo "$(C_CYAN)Debug & Info:$(C_RESET)"
	@echo "  $(C_GREEN)debug-info$(C_RESET)    - Show detailed build configuration"
	@echo "  $(C_GREEN)test-colors$(C_RESET)   - Test color output"
	@echo "  $(C_GREEN)help$(C_RESET)          - Show this message"

.SUFFIXES:

# ============================= End of Makefile ============================= #
