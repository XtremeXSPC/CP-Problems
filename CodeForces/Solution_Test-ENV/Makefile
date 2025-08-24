# Enhanced Makefile for Competitive Programming C++ projects
# Optimized for GCC with bits/stdc++.h and pb_ds support

# =========================================================================== #
# ------------------------ System Pre-configuration  ------------------------ #
# =========================================================================== #
# Use Homebrew's bash if available to avoid macOS SIP issues with tools like
# 'bear', which rely on DYLD_INSERT_LIBRARIES. SIP strips this variable from
# system shells.
HOMEBREW_BASH := $(shell which bash | grep 'homebrew')
ifeq ($(strip $(HOMEBREW_BASH)),)
  SHELL := /bin/bash
else
  # Use the first bash found in a homebrew path
  SHELL := $(firstword $(HOMEBREW_BASH))
endif

.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DEFAULT_GOAL := all

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
    C_MAGENTA := $(shell tput setaf 5 2>/dev/null)
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
TIMING_DIR     ?= timing-reports

# Build profile for 'Competitive Programming': debug (default), release, or sanitize
BUILD_TYPE     ?= debug

# IDE configuration mode - when set to 'yes', produces clean compile commands
# without toolchain-specific flags that can confuse clangd
IDE_CONFIG     ?= no

# Competitive programming settings
ENABLE_FAST_IO ?= 1
ENABLE_DEBUG   ?= 1

# Timing profiling options
ENABLE_TIMING  ?= no
TIMING_FORMAT  ?= both  # Options: json, text, both

# Force specific compiler (overrides auto-detection)
FORCE_COMPILER ?= auto  # Options: auto, gcc, clang

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
# Only check for competitive headers if not forcing a specific compiler
ifeq ($(strip $(FORCE_COMPILER)),auto)
  NEEDS_GCC_RESULT := $(shell ./$(SCRIPTS_DIR)/get_gcc_includes.sh --check-competitive $(SRCDIR) 2>/dev/null || echo "no")
  NEEDS_GCC := $(NEEDS_GCC_RESULT)
else ifeq ($(strip $(FORCE_COMPILER)),gcc)
  NEEDS_GCC := yes
  $(info $(C_MAGENTA)Forcing GCC compiler as requested.$(C_RESET))
else ifeq ($(strip $(FORCE_COMPILER)),clang)
  NEEDS_GCC := no
  $(info $(C_MAGENTA)Forcing Clang compiler as requested.$(C_RESET))
else
  $(error Invalid FORCE_COMPILER value: '$(FORCE_COMPILER)'. Use 'auto', 'gcc', or 'clang')
endif

# Debug output for competitive header detection
ifeq ($(strip $(FORCE_COMPILER)),auto)
	ifeq ($(NEEDS_GCC),yes)
    $(info $(C_CYAN)Competitive programming headers detected - GCC required$(C_RESET))
	else
    $(info $(C_GREEN)No 'Competitive Programming' headers found - Clang can be used$(C_RESET))
	endif
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
        # Platform-Aware Heuristic:
        # On macOS, /usr/bin/g++ is a symlink to clang that must be skipped.
        # On Linux, it is the real GCC.
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

# Determine effective compiler based on build type, forced compiler, and GCC availability
ifeq ($(BUILD_TYPE),sanitize)
    # Special handling for sanitize mode
    ifeq ($(FORCE_COMPILER),clang)
        # User explicitly wants Clang for sanitizers
        SANITIZER_AVAILABLE := yes
        EFFECTIVE_COMPILER := clang
        EFFECTIVE_CC := clang
        ifeq ($(NEEDS_GCC),yes)
            USE_PCH_FALLBACK := yes
            $(info $(C_YELLOW)Using Clang with PCH fallback for sanitizers (forced)$(C_RESET))
        else
            $(info $(C_CYAN)Using Clang with sanitizers (forced)$(C_RESET))
        endif
    else ifeq ($(FORCE_COMPILER),gcc)
        # User explicitly wants GCC for sanitizers
        ifneq ($(GCC_BINARY),)
            # Test if GCC sanitizers are available
            GCC_SANITIZER_TEST := $(shell echo 'int main(){}' | $(GCC_BINARY) -x c++ -fsanitize=address,undefined - -o /tmp/sanitizer_test 2>/dev/null && echo "yes" || echo "no")
            ifeq ($(GCC_SANITIZER_TEST),yes)
                SANITIZER_AVAILABLE := yes
                EFFECTIVE_COMPILER := gcc
                EFFECTIVE_CC := $(subst g++,gcc,$(GCC_BINARY))
                $(info $(C_CYAN)Using GCC with sanitizers (forced)$(C_RESET))
            else
                $(error $(C_RED)GCC sanitizers not available but GCC was forced. Cannot proceed.$(C_RESET))
            endif
        else
            $(error $(C_RED)GCC was forced but no GCC binary found!$(C_RESET))
        endif
    else
        # Auto mode for sanitizers
        ifeq ($(NEEDS_GCC),yes)
            ifneq ($(GCC_BINARY),)
                # Test if GCC sanitizers are available
                GCC_SANITIZER_TEST := $(shell echo 'int main(){}' | $(GCC_BINARY) -x c++ -fsanitize=address,undefined - -o /tmp/sanitizer_test 2>/dev/null && echo "yes" || echo "no")
                ifeq ($(GCC_SANITIZER_TEST),yes)
                    SANITIZER_AVAILABLE := yes
                    EFFECTIVE_COMPILER := gcc
                    EFFECTIVE_CC := $(subst g++,gcc,$(GCC_BINARY))
                    $(info $(C_CYAN)Using GCC with sanitizers for 'Competitive Programming'$(C_RESET))
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
    endif
else
    # Non-sanitize modes
    ifeq ($(FORCE_COMPILER),clang)
        EFFECTIVE_COMPILER := clang
        EFFECTIVE_CC := clang
        ifeq ($(NEEDS_GCC),yes)
            USE_PCH_FALLBACK := yes
            $(info $(C_CYAN)Using Clang with PCH fallback (forced)$(C_RESET))
        else
            $(info $(C_CYAN)Using Clang for standard C++ (forced)$(C_RESET))
        endif
    else ifeq ($(FORCE_COMPILER),gcc)
        ifneq ($(GCC_BINARY),)
            EFFECTIVE_COMPILER := gcc
            EFFECTIVE_CC := $(subst g++,gcc,$(GCC_BINARY))
            $(info $(C_CYAN)Using GCC (forced)$(C_RESET))
        else
            $(error $(C_RED)GCC was forced but no GCC binary found!$(C_RESET))
        endif
    else
        # Auto mode
        ifeq ($(NEEDS_GCC),yes)
            ifneq ($(GCC_BINARY),)
                EFFECTIVE_COMPILER := gcc
                EFFECTIVE_CC := $(subst g++,gcc,$(GCC_BINARY))
                $(info $(C_CYAN)Using GCC for 'Competitive Programming'$(C_RESET))
            else
                $(error $(C_RED)Competitive programming headers detected but compatible GCC not found!$(C_RESET))
            endif
        else
            EFFECTIVE_COMPILER := clang
            EFFECTIVE_CC := clang
            $(info $(C_CYAN)Using Clang for standard C++$(C_RESET))
        endif
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
        # Standard GCC configuration for 'Competitive Programming'
        ifneq ($(GCC_BINARY),)
            GCC_ISYSTEM_FLAGS := $(shell ./$(SCRIPTS_DIR)/get_gcc_includes.sh $(CXX))
        else
            GCC_ISYSTEM_FLAGS :=
        endif

        # For IDE configuration, we skip the toolchain-specific flags
        ifeq ($(IDE_CONFIG),yes)
            CXXFLAGS_TOOLCHAIN := -DCOMPILER_GCC=1
            SYSTEM_INCLUDES := -I/usr/local/include
            STDLIB_FLAG :=
        else
            # Use -nostdinc++ to prevent default search paths
            ifneq ($(GCC_ISYSTEM_FLAGS),)
                CXXFLAGS_TOOLCHAIN := -nostdinc++ -DCOMPILER_GCC=1
                SYSTEM_INCLUDES := $(GCC_ISYSTEM_FLAGS)
            else
                CXXFLAGS_TOOLCHAIN := -DCOMPILER_GCC=1
                SYSTEM_INCLUDES := -I/usr/local/include
            endif

            # Platform-specific standard library flag for macOS
            ifeq ($(detected_os),MACOS)
                STDLIB_FLAG := -stdlib=libstdc++
            endif
        endif
    else
        # PCH fallback mode: use Clang with custom PCH.h instead of bits/stdc++.h
        CXXFLAGS_TOOLCHAIN := -DUSE_PCH_FALLBACK=1 -DUSE_CLANG_SANITIZE=1 -DCOMPILER_CLANG=1
        PCH_INCLUDE := 
        SYSTEM_INCLUDES := -I/usr/local/include
        STDLIB_FLAG := 
        $(info $(C_YELLOW)Using PCH.h fallback for 'Competitive Programming' headers$(C_RESET))
    endif
else
    # Clang configuration
    CXXFLAGS_TOOLCHAIN := -DCOMPILER_CLANG=1
    SYSTEM_INCLUDES := -I/usr/local/include
    STDLIB_FLAG := 
    
    ifeq ($(USE_PCH_FALLBACK),yes)
        CXXFLAGS_TOOLCHAIN := -DUSE_PCH_FALLBACK=1 -DUSE_CLANG_SANITIZE=1 -DCOMPILER_CLANG=1
        PCH_INCLUDE := 
        $(info $(C_YELLOW)PCH fallback mode enabled for Clang$(C_RESET))
    endif
endif

# Define project-specific include directories
PROJECT_INCLUDES := -I$(SRCDIR) -I$(SRCDIR)/include

# ----------------- Competitive Programming Compiler Flags ------------------ #
# Base flags optimized for 'Competitive Programming'
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

# ----------------- Compilation Timing Flags Configuration ------------------ #
TIMING_FLAGS :=
TIMING_OUTPUT_FLAGS :=

ifeq ($(ENABLE_TIMING),yes)
    ifeq ($(EFFECTIVE_COMPILER),gcc)
        # GCC timing flags
        ifeq ($(TIMING_FORMAT),json)
            $(info $(C_YELLOW)Note: GCC doesn't support JSON timing output, using text format$(C_RESET))
            TIMING_FLAGS := -ftime-report
        else
            TIMING_FLAGS := -ftime-report
        endif
    else
        # Clang timing flags
        ifeq ($(TIMING_FORMAT),json)
            TIMING_FLAGS := -ftime-trace
            TIMING_OUTPUT_FLAGS := -ftime-trace-granularity=1
        else ifeq ($(TIMING_FORMAT),text)
            TIMING_FLAGS := -ftime-report
        else
            # both
            TIMING_FLAGS := -ftime-trace -ftime-report
            TIMING_OUTPUT_FLAGS := -ftime-trace-granularity=1
        endif
    endif
    $(info $(C_MAGENTA)Compilation timing enabled (format: $(TIMING_FORMAT))$(C_RESET))
endif

# Final compiler flags
CXXFLAGS := $(COMMON_FLAGS) $(WARNING_FLAGS) $(PROFILE_FLAGS) $(COMPETITIVE_DEFINES) $(CXXFLAGS_TOOLCHAIN) $(TIMING_FLAGS) $(TIMING_OUTPUT_FLAGS)

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
  LD = @echo "$(C_RED)No C++ sources found for 'Competitive Programming'!$(C_RESET)"; false
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
	@echo "$(C_GREEN)✔ Build successful$(C_RESET)"
	@if [ "$(ENABLE_TIMING)" = "yes" ]; then \
		$(MAKE) process-timing-reports; \
	fi

# Enhanced compilation rules with timing support
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DIR_SENTINEL)
	@echo "$(C_CYAN)Compiling C++:$(C_RESET) $< -> $@"
	@if [ "$(USE_PCH_FALLBACK)" = "yes" ]; then \
		echo "$(C_YELLOW)  Using PCH.h fallback for 'Competitive Programming' headers$(C_RESET)"; \
	fi
	@if [ "$(BUILD_TYPE)" = "sanitize" ]; then \
		echo "$(C_YELLOW)  Compiling with sanitizers enabled$(C_RESET)"; \
	fi
	@if [ "$(ENABLE_TIMING)" = "yes" ]; then \
		echo "$(C_MAGENTA)  Timing profiling enabled$(C_RESET)"; \
	fi
	@echo "$(C_CYAN)  Compiler:$(C_RESET) $(CXX)"
	@echo "$(C_CYAN)  Includes:$(C_RESET)"
	@echo "$(CPPFLAGS)" | tr ' ' '\n' | sed 's/^/    /'
	@echo "$(C_CYAN)  Options:$(C_RESET)"
	@echo "$(CXXFLAGS)" | tr ' ' '\n' | sed 's/^/    /'
	@if [ "$(ENABLE_TIMING)" = "yes" ]; then \
		START_TIME=$$(date +%s.%N 2>/dev/null || date +%s); \
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@ 2>&1 | tee $(OBJDIR)/$*.timing.log; \
		END_TIME=$$(date +%s.%N 2>/dev/null || date +%s); \
		if command -v bc >/dev/null 2>&1; then \
			ELAPSED=$$(echo "$$END_TIME - $$START_TIME" | bc); \
			ELAPSED_FORMATTED=$$(printf "%.3f" $$ELAPSED 2>/dev/null || echo $$ELAPSED); \
			echo "$(C_MAGENTA)  Compilation time: $${ELAPSED_FORMATTED}s$(C_RESET)"; \
		else \
			echo "$(C_MAGENTA)  Compilation completed$(C_RESET)"; \
		fi; \
	else \
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@; \
	fi

$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(DIR_SENTINEL)
	@echo "$(C_CYAN)Compiling C++:$(C_RESET) $< -> $@"
	@if [ "$(USE_PCH_FALLBACK)" = "yes" ]; then \
		echo "$(C_YELLOW)  Using PCH.h fallback for 'Competitive Programming' headers$(C_RESET)"; \
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
		echo "$(C_YELLOW)  Using PCH.h fallback for 'Competitive Programming' headers$(C_RESET)"; \
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
# ----------------------- Compilation Timing Targets ------------------------ #
# =========================================================================== #
.PHONY: timing timing-gcc timing-clang timing-compare process-timing-reports clean-timing

# Build with timing enabled
timing:
	@echo "$(C_MAGENTA)Building with compilation timing enabled...$(C_RESET)"
	@$(MAKE) clean
	@$(MAKE) ENABLE_TIMING=yes all

# Force build with GCC and timing
timing-gcc:
	@echo "$(C_MAGENTA)Building with GCC and timing enabled...$(C_RESET)"
	@if [ -z "$(GCC_BINARY)" ]; then \
		echo "$(C_RED)Error: GCC not found!$(C_RESET)"; \
		exit 1; \
	fi
	@$(MKDIR_P) $(TIMING_DIR)/gcc
	@echo "$(C_CYAN)Using GCC: $(GCC_BINARY)$(C_RESET)"
	@$(MAKE) clean
	@$(MAKE) FORCE_COMPILER=gcc ENABLE_TIMING=yes TIMING_FORMAT=text all
	@echo "$(C_GREEN)GCC timing report saved to $(TIMING_DIR)/gcc/$(C_RESET)"

# Force build with Clang and timing
timing-clang:
	@echo "$(C_MAGENTA)Building with Clang and timing enabled...$(C_RESET)"
	@$(MKDIR_P) $(TIMING_DIR)/clang
	@echo "$(C_CYAN)Using Clang: clang++$(C_RESET)"
	@$(MAKE) clean
	@$(MAKE) FORCE_COMPILER=clang ENABLE_TIMING=yes TIMING_FORMAT=both all
	@echo "$(C_GREEN)Clang timing report saved to $(TIMING_DIR)/clang/$(C_RESET)"

# Compare timing between GCC and Clang
timing-compare: clean-timing
	@echo "$(C_BOLD)$(C_MAGENTA)/===-------------- Compiler Performance Comparison ---------------===/$(C_RESET)"
	@$(MKDIR_P) $(TIMING_DIR)
	@echo ""
	@echo "$(C_CYAN)----- Testing with GCC -----$(C_RESET)"
	@if [ -n "$(GCC_BINARY)" ]; then \
		echo "GCC Version: $$($(GCC_BINARY) --version | head -1)" > $(TIMING_DIR)/comparison.txt; \
		START_TIME=$$(date +%s.%N 2>/dev/null || date +%s); \
		$(MAKE) timing-gcc > /dev/null 2>&1; \
		END_TIME=$$(date +%s.%N 2>/dev/null || date +%s); \
		if command -v bc >/dev/null 2>&1; then \
			GCC_TIME=$$(echo "$$END_TIME - $$START_TIME" | bc); \
			echo "GCC Total Time: $${GCC_TIME}s" >> $(TIMING_DIR)/comparison.txt; \
			echo "$(C_GREEN)GCC compilation time: $${GCC_TIME}s$(C_RESET)"; \
		else \
			echo "GCC compilation completed" >> $(TIMING_DIR)/comparison.txt; \
			echo "$(C_GREEN)GCC compilation completed$(C_RESET)"; \
		fi; \
	else \
		echo "$(C_YELLOW)GCC not available, skipping...$(C_RESET)"; \
		echo "GCC: Not available" >> $(TIMING_DIR)/comparison.txt; \
	fi
	@echo ""
	@echo "$(C_CYAN)----- Testing with Clang -----$(C_RESET)"
	@echo "Clang Version: $$(clang++ --version | head -1)" >> $(TIMING_DIR)/comparison.txt
	@START_TIME=$$(date +%s.%N 2>/dev/null || date +%s)
	@$(MAKE) timing-clang > /dev/null 2>&1
	@END_TIME=$$(date +%s.%N 2>/dev/null || date +%s)
	@if command -v bc >/dev/null 2>&1; then \
		CLANG_TIME=$$(echo "$$END_TIME - $$START_TIME" | bc); \
		echo "Clang Total Time: $${CLANG_TIME}s" >> $(TIMING_DIR)/comparison.txt; \
		echo "$(C_GREEN)Clang compilation time: $${CLANG_TIME}s$(C_RESET)"; \
	else \
		echo "Clang compilation completed" >> $(TIMING_DIR)/comparison.txt; \
		echo "$(C_GREEN)Clang compilation completed$(C_RESET)"; \
	fi
	@echo ""
	@echo "$(C_BOLD)$(C_MAGENTA)/===-------------------------- Summary ---------------------------===/$(C_RESET)"
	@cat $(TIMING_DIR)/comparison.txt
	@echo ""
	@echo "$(C_CYAN)Detailed reports saved in $(TIMING_DIR)/$(C_RESET)"
	@if [ -f "$(OBJDIR)/$(TARGET_NAME).o.json" ]; then \
		echo "$(C_CYAN)Clang trace visualization available: $(OBJDIR)/$(TARGET_NAME).o.json$(C_RESET)"; \
		echo "$(C_YELLOW)Open in chrome://tracing or similar tool$(C_RESET)"; \
	fi

# Process timing reports after compilation
process-timing-reports:
	@if [ "$(ENABLE_TIMING)" = "yes" ]; then \
		$(MKDIR_P) $(TIMING_DIR)/$(EFFECTIVE_COMPILER); \
		TIMESTAMP=$$(date +%Y%m%d_%H%M%S); \
		if [ -f "$(OBJDIR)/$(TARGET_NAME).o.json" ]; then \
			cp $(OBJDIR)/$(TARGET_NAME).o.json $(TIMING_DIR)/$(EFFECTIVE_COMPILER)/trace_$${TIMESTAMP}.json; \
			echo "$(C_MAGENTA)Timing trace saved to $(TIMING_DIR)/$(EFFECTIVE_COMPILER)/trace_$${TIMESTAMP}.json$(C_RESET)"; \
		fi; \
		if [ -f "$(OBJDIR)/$(TARGET_NAME).timing.log" ]; then \
			cp $(OBJDIR)/$(TARGET_NAME).timing.log $(TIMING_DIR)/$(EFFECTIVE_COMPILER)/report_$${TIMESTAMP}.txt; \
			echo "$(C_MAGENTA)Timing report saved to $(TIMING_DIR)/$(EFFECTIVE_COMPILER)/report_$${TIMESTAMP}.txt$(C_RESET)"; \
		fi; \
	fi

# Clean timing reports
clean-timing:
	@echo "$(C_YELLOW)Cleaning timing reports...$(C_RESET)"
	-$(RMDIR_CMD) $(TIMING_DIR)
	-$(RM) $(OBJDIR)$(PathSep)*.timing.log
	-$(RM) $(OBJDIR)$(PathSep)*.json
	@echo "$(C_GREEN)Timing reports cleaned.$(C_RESET)"

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
	@echo "$(C_GREEN)✔ IDE config directory ensured: $(IDE_CONFIG_DIR)/$(C_RESET)"
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
				echo "$(C_GREEN)✔ .clangd configured for GCC toolchain using template: $$CLANGD_TEMPLATE$(C_RESET)"; \
			else \
				echo "$(C_RED)Error: .clangd template not found at '$$CLANGD_TEMPLATE'$(C_RESET)"; \
				echo "$(C_YELLOW)Please run 'make init-ide-config' and ensure the template exists.$(C_RESET)"; \
				exit 1; \
			fi; \
		else \
			echo "$(C_CYAN)Linux/Unix detected. Native GCC support, removing .clangd...$(C_RESET)"; \
			$(RM) .clangd 2>/dev/null || true; \
			echo "$(C_GREEN)✔ Clean environment configured for Linux$(C_RESET)"; \
		fi; \
	else \
		echo "$(C_CYAN)Standard C++ (Clang) setup detected. Ensuring clean environment...$(C_RESET)"; \
		$(RM) .clangd 2>/dev/null || true; \
		echo "$(C_GREEN)✔ Clean Clang environment configured$(C_RESET)"; \
	fi

	@echo "$(C_CYAN)Step 2: Generating compile_commands.json...$(C_RESET)"
	@if ! command -v compiledb > /dev/null; then \
		echo "$(C_RED)Error: 'compiledb' is not installed. Cannot generate compile_commands.json.$(C_RESET)"; \
		echo "$(C_YELLOW)Please install it with: pip install compiledb$(C_RESET)"; \
		exit 1; \
	fi
	@echo "$(C_YELLOW)Note: Using clean configuration without toolchain-specific flags for clangd.$(C_RESET)"
	@compiledb -n make clean all IDE_CONFIG=yes BUILD_TYPE=$(BUILD_TYPE)
	
	@echo "$(C_BOLD)$(C_GREEN)✔ IDE Configuration Complete!$(C_RESET)"
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
	@echo "$(C_MAGENTA)Magenta text$(C_RESET)"
	@echo "$(C_CYAN)Cyan text$(C_RESET)"
	@echo "$(C_BOLD)Bold text$(C_RESET)"

clean:
	@echo "$(C_YELLOW)Cleaning build artifacts...$(C_RESET)"
	-$(RM) $(OBJDIR)$(PathSep)*.o
	-$(RM) $(OBJDIR)$(PathSep)*.d
	-$(RM) $(OBJDIR)$(PathSep)*.json
	-$(RM) $(OBJDIR)$(PathSep)*.timing.log
	-$(RM) $(BINDIR)$(PathSep)*
	-$(RM) $(DIR_SENTINEL)
	@echo "$(C_GREEN)Clean complete.$(C_RESET)"

distclean: clean clean-timing
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
	@echo "  FORCE_COMPILER: $(FORCE_COMPILER)"
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
	@echo "$(C_CYAN)Timing Configuration:$(C_RESET)"
	@echo "  ENABLE_TIMING: $(ENABLE_TIMING)"
	@echo "  TIMING_FORMAT: $(TIMING_FORMAT)"
	@echo "  TIMING_FLAGS: $(TIMING_FLAGS)"
	@echo "  TIMING_DIR: $(TIMING_DIR)"
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
	@echo "$(C_CYAN)Compiler Control:$(C_RESET)"
	@echo "  $(C_YELLOW)FORCE_COMPILER=auto$(C_RESET)  - Auto-detect based on headers (default)"
	@echo "  $(C_YELLOW)FORCE_COMPILER=gcc$(C_RESET)   - Force GCC usage"
	@echo "  $(C_YELLOW)FORCE_COMPILER=clang$(C_RESET) - Force Clang usage"
	@echo ""
	@echo "$(C_CYAN)Compilation Timing Targets:$(C_RESET)"
	@echo "  $(C_GREEN)timing$(C_RESET)         - Build with timing profiling enabled"
	@echo "  $(C_GREEN)timing-gcc$(C_RESET)     - Force GCC build with timing"
	@echo "  $(C_GREEN)timing-clang$(C_RESET)   - Force Clang build with timing"
	@echo "  $(C_GREEN)timing-compare$(C_RESET) - Compare compilation speed between GCC and Clang"
	@echo "  $(C_GREEN)clean-timing$(C_RESET)   - Remove all timing reports"
	@echo ""
	@echo "$(C_CYAN)Timing Options:$(C_RESET)"
	@echo "  $(C_YELLOW)ENABLE_TIMING=yes/no$(C_RESET)   - Enable compilation timing (default: no)"
	@echo "  $(C_YELLOW)TIMING_FORMAT=json/text/both$(C_RESET) - Output format (default: both)"
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
	@echo ""
	@echo "$(C_CYAN)Examples:$(C_RESET)"
	@echo "  make timing-compare                # Compare GCC vs Clang compilation speed"
	@echo "  make FORCE_COMPILER=clang all      # Force Clang for build"
	@echo "  make ENABLE_TIMING=yes all         # Build with timing profiling"

.SUFFIXES:

# ============================= End of Makefile ============================= #