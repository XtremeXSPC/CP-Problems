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
    $(info Using Homebrew's bash to ensure compatibility with other tools: $(SHELL))
endif

# Continue with the rest of the Makefile...
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c

# =========================================================================== #
# ----------------------- User-configurable variables ----------------------- #
# =========================================================================== #
TARGET_NAME    ?= Tester
SRCDIR         ?= .
OBJDIR         ?= obj
BINDIR         ?= bin

# Build profile for competitive programming: debug (default) or release.
# 'sanitize' profile is also available for debugging memory issues.
BUILD_TYPE     ?= debug

# Competitive programming specific settings
ENABLE_FAST_IO ?= 1
ENABLE_DEBUG   ?= 1

# =========================================================================== #
# ------------- Competitive Programming Compiler Configuration -------------- #
# =========================================================================== #

# ------------------------ GCC Detection for bits/stdc++.h ------------------ #
# Find the include path and prefix of the latest installed GCC version via Homebrew
GCC_PREFIX      := $(shell brew --prefix gcc 2>/dev/null)
GCC_INCLUDE_DIR := $(shell \
	if [ -n "$(GCC_PREFIX)" ] && [ -d "$(GCC_PREFIX)/include/c++" ]; then \
		find $(GCC_PREFIX)/include/c++/ -maxdepth 1 -type d | \
		sort -V | tail -n 1; \
	fi \
)

# Find GCC binary - Prioritize Homebrew installation over system GCC
GCC_BINARY := $(shell \
	if [ -n "$(GCC_PREFIX)" ]; then \
		find $(GCC_PREFIX)/bin -name "g++-*" -type f | sort -V | tail -n 1; \
	fi \
)

# Only use system GCC as last resort and warn about potential issues
ifeq ($(GCC_BINARY),)
    SYSTEM_GCC := $(shell which g++ 2>/dev/null)
    ifneq ($(SYSTEM_GCC),)
        ifeq ($(SYSTEM_GCC),/usr/bin/g++)
            # This is likely Xcode's g++ symlink to clang, skip it
            GCC_BINARY :=
        else
            GCC_BINARY := $(SYSTEM_GCC)
        endif
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
    $(info Using Clang - No competitive programming headers detected)
else
    # Competitive programming configuration - requires GCC
	ifneq ($(GCC_BINARY),)
        CC  := $(subst g++,gcc,$(GCC_BINARY))
        CXX := $(GCC_BINARY)
        SYSTEM_INCLUDES := -I/usr/local/include \
                           -I/opt/homebrew/include/c++/15 \
                           -I$(GCC_INCLUDE_DIR) \
                           -I$(GCC_INCLUDE_DIR)/aarch64-apple-darwin23 \
                           -I$(GCC_INCLUDE_DIR)/backward
        $(info Using GCC for competitive programming: $(CXX))
        $(info Detected headers: $(if $(NEEDS_BITS_STDC),bits/stdc++.h) $(if $(NEEDS_PBDS),pb_ds))
	else
		$(error Competitive programming headers detected but compatible GCC not found!)
		$(error Install GCC via: brew install gcc)
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
CPPFLAGS := $(sort $(CPPFLAGS) $(SYSTEM_INCLUDES) $(PROJECT_INCLUDES))

# -------------- Build Profile Configuration for Competitive Programming ---- #
ifeq ($(BUILD_TYPE), release)
    # Release: Maximum optimization for contest submission
    # -O2 is often preferred over -O3 for competitive programming (more predictable)
    PROFILE_FLAGS := -O2 -DNDEBUG -march=native -mtune=native
    # Disable warnings in release mode to reduce noise
    WARNING_FLAGS := -w
    $(info Build mode: Release (optimized for submission))
else ifeq ($(BUILD_TYPE), sanitize)
    # Sanitize: Debug with memory sanitizers for finding bugs
    PROFILE_FLAGS := -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer
    WARNING_FLAGS := -Wall -Wextra
    $(info Build mode: Sanitize (debug with memory checks))
else
    # Debug: Full debug info, no optimization - best for development
    BUILD_TYPE    := debug
    PROFILE_FLAGS := -g -O0
    # More permissive warnings for competitive programming
    WARNING_FLAGS := -Wall -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare
    $(info Build mode: Debug (development and testing))
endif

# Final compiler flags for competitive programming
CXXFLAGS := $(COMMON_FLAGS) $(WARNING_FLAGS) $(PROFILE_FLAGS) $(COMPETITIVE_DEFINES)

# Linker flags - minimal for competitive programming
LDFLAGS ?=
ifeq ($(strip $(NEEDS_GCC)$(GCC_BINARY)),yesyes)
    # Add GCC library path if using GCC
    LDFLAGS += -L$(dir $(GCC_BINARY))../lib
endif

# Standard math library (commonly needed in competitive programming)
LIBS := -lm

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

# If SRCDIR, OBJDIR, BINDIR are relative, it is necessary to ensure they are interpreted correctly.
# For now, we assume they are simple directory names like '.', 'obj', 'bin'

TARGET_DIR_WITH_SEP := $(BINDIR)$(PathSep)
TARGET_PATH         := $(TARGET_DIR_WITH_SEP)$(TARGET_EXEC)

# =========================================================================== #
# ------------------ Source Detection for Competitive Programming ----------- # 
# =========================================================================== #
# Competitive programming typically uses only C++ files
CPP_SRCS       := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.cc $(SRCDIR)/*.cxx)

# Usually competitive programming has single source files, but support multiple
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
	LD = @echo "No C++ sources found for competitive programming!"; false
else
	LD := $(CXX)
	LDFLAGS_FINAL := $(LDFLAGS) $(CXXFLAGS)
endif
LDFLAGS_FINAL += $(LIBS)

# =========================================================================== #
# ------------------------- Competitive Programming Rules ------------------- #
# =========================================================================== #
.PHONY: all clean distclean run test compile_commands debug-info help contest

# Default target
all: $(TARGET_PATH)

# Directory creation with sentinel
$(DIR_SENTINEL):
	@echo "Creating directories $(OBJDIR) and $(BINDIR)..."
ifeq ($(detected_os),WINDOWS_NATIVE)
	@if not exist $(subst /,\,$(OBJDIR)) mkdir $(subst /,\,$(OBJDIR))
	@if not exist $(subst /,\,$(BINDIR)) mkdir $(subst /,\,$(BINDIR))
else
	@$(MKDIR_P) $(OBJDIR)
	@$(MKDIR_P) $(BINDIR)
endif
	@touch $@

# Linking rule
$(TARGET_PATH): $(OBJS)
	@echo "# ------ Linking Competitive Programming Executable ------ #"
	@echo "Output: $@"
	$(LD) $(OBJS) $(LDFLAGS_FINAL) -o $@

# Compilation rules
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DIR_SENTINEL)
	@echo "Compiling C++: $< to $@"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(DIR_SENTINEL)
	@echo "Compiling C++: $< to $@"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cxx $(DIR_SENTINEL)
	@echo "Compiling C++: $< to $@"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEPS)

# =========================================================================== #
# --------------- Competitive Programming Specific Targets ------------------ #
# =========================================================================== #

# Quick build for contest environment (release mode)
contest: clean
	@$(MAKE) BUILD_TYPE=release all

# Run with input redirection (common in competitive programming)
INPUT_FILE ?= input.txt
OUTPUT_FILE ?= output.txt
ARGS ?=

run: all
	@echo "Running $(TARGET_PATH)..."
ifeq ($(findstring WINDOWS,$(detected_os)),WINDOWS)
	$(if $(wildcard $(INPUT_FILE)), \
		$(subst /,\,$(TARGET_PATH)) $(ARGS) < $(INPUT_FILE), \
		$(subst /,\,$(TARGET_PATH)) $(ARGS))
else
	$(if $(wildcard $(INPUT_FILE)), \
		./$(TARGET_PATH) $(ARGS) < $(INPUT_FILE), \
		./$(TARGET_PATH) $(ARGS))
endif

# Run with both input and output redirection
test: all
	@echo "Running $(TARGET_PATH) with I/O redirection..."
ifeq ($(findstring WINDOWS,$(detected_os)),WINDOWS)
	$(if $(wildcard $(INPUT_FILE)), \
		$(subst /,\,$(TARGET_PATH)) $(ARGS) < $(INPUT_FILE) > $(OUTPUT_FILE), \
		$(subst /,\,$(TARGET_PATH)) $(ARGS) > $(OUTPUT_FILE))
else
	$(if $(wildcard $(INPUT_FILE)), \
		./$(TARGET_PATH) $(ARGS) < $(INPUT_FILE) > $(OUTPUT_FILE), \
		./$(TARGET_PATH) $(ARGS) > $(OUTPUT_FILE))
endif
	@echo "Output written to $(OUTPUT_FILE)"

# =========================================================================== #
# ----------------------- Utility & Development Targets -------------------- #
# =========================================================================== #

clean:
	@echo "Cleaning build artifacts..."
	-$(RM) $(OBJDIR)$(PathSep)*.o
	-$(RM) $(OBJDIR)$(PathSep)*.d
	-$(RM) $(TARGET_PATH)
	-$(RM) $(DIR_SENTINEL)
	@echo "Clean complete."

distclean: clean
	@echo "Cleaning all directories and temporary files..."
ifeq ($(findstring WINDOWS,$(detected_os)),WINDOWS)
	@if exist $(subst /,\,$(OBJDIR)) $(RMDIR_CMD) $(subst /,\,$(OBJDIR))
	@if exist $(subst /,\,$(BINDIR)) $(RMDIR_CMD) $(subst /,\,$(BINDIR))
else
	-$(RMDIR_CMD) $(OBJDIR)
	-$(RMDIR_CMD) $(BINDIR)
endif
	# Remove competitive programming artifacts
	-$(RM) $(OUTPUT_FILE) *~ *.bak
	@echo "Distclean complete."

# Compilation Database for clangd (essential for competitive programming IDEs)
.PHONY: compile_commands
compile_commands:
	@echo "Generating compile_commands.json for IDE support..."
	@compiledb -n make clean all BUILD_TYPE=$(BUILD_TYPE)

# Debug information - enhanced for competitive programming
debug-info:
	@echo "#====------------ Competitive Programming Debug Info ------------====#"
	@echo "Build Configuration:"
	@echo "  UNAME_S: $(UNAME_S)"
	@echo "  Detected OS: $(detected_os)"
	@echo "  PathSep: $(PathSep)"
	@echo "  BUILD_TYPE: $(BUILD_TYPE)"
	@echo "  TARGET_EXEC: $(TARGET_EXEC)"
	@echo "  TARGET_NAME: $(TARGET_NAME)"
	@echo "  TARGET_PATH: $(TARGET_PATH)"
	@echo "  SRCDIR: $(SRCDIR), OBJDIR: $(OBJDIR), BINDIR: $(BINDIR)"
	@echo ""
	@echo "Sources & Objects:"
	@echo "  C_SRCS: $(C_SRCS)"
	@echo "  CPP_SRCS: $(CPP_SRCS)"
	@echo "  OBJS: $(OBJS)"
	@echo "  DEPS: $(DEPS)"
	@echo ""
	@echo "Competitive Programming Detection:"
	@echo "  NEEDS_GCC: $(NEEDS_GCC)"
	@echo "  NEEDS_BITS_STDC: $(if $(NEEDS_BITS_STDC),YES ($(NEEDS_BITS_STDC)),NO)"
	@echo "  NEEDS_PBDS: $(if $(NEEDS_PBDS),YES ($(NEEDS_PBDS)),NO)"
	@echo ""
	@echo "Compiler & Linker Configuration:"
	@echo "CC: $(CC), CXX: $(CXX), LD: $(LD)"
	@echo "  GCC_PREFIX: $(GCC_PREFIX)"
	@echo "  GCC_BINARY: $(GCC_BINARY)"
	@echo "  GCC_INCLUDE_DIR: $(GCC_INCLUDE_DIR)"
	@echo ""
	@echo "Build Flags:"
	@echo "  CFLAGS: $(CFLAGS)"
	@echo "  CPPFLAGS: $(CPPFLAGS)"
	@echo "  CXXFLAGS: $(CXXFLAGS)"
	@echo "  LDFLAGS_FINAL: $(LDFLAGS_FINAL)"
	@echo ""
	@echo "Sources:"
	@echo "  CPP_SRCS: $(CPP_SRCS)"
	@echo "  OBJS: $(OBJS)"
	@echo ""
	@echo "OS Commands"
	@echo "  RM: $(RM)"
	@echo "  RMDIR_CMD: $(RMDIR_CMD)"
	@echo "  MKDIR_P (for Unix-like): $(MKDIR_P)"
	@echo "#====------------------------------------------------------------====#"

help:
	@echo "Competitive Programming Makefile"
	@echo "Usage: make [TARGET] [VARIABLE=VALUE]"
	@echo ""
	@echo "Build Targets:"
	@echo "  all               - Build with specified BUILD_TYPE (default: debug)"
	@echo "  contest           - Quick release build for contests"
	@echo "  run               - Build and run (uses input.txt if available)"
	@echo "  test              - Run with I/O redirection (input.txt -> output.txt)"
	@echo "  compile_commands  - Generate compilation database for IDE"
	@echo "  clean             - Remove build artifacts"
	@echo "  distclean         - Remove everything including output files"
	@echo "  debug-info        - Show detailed build configuration"
	@echo "  help              - Show this message"
	@echo ""
	@echo "Configuration Variables:"
	@echo "  BUILD_TYPE=[debug|release|sanitize]  - Build optimization level"
	@echo "  TARGET_NAME=<name>                   - Executable name (default: main)"
	@echo "  INPUT_FILE=<file>                    - Input file for 'run'/'test'"
	@echo "  OUTPUT_FILE=<file>                   - Output file for 'test'"
	@echo "  ENABLE_FAST_IO=[0|1]                 - Enable fast I/O optimizations"
	@echo "  ENABLE_DEBUG=[0|1]                   - Enable debug output macros"
	@echo "  ARGS=\"...\"                           - Arguments for run/test"
	@echo ""
	@echo "Examples:"
	@echo "  make contest                         - Quick optimized build"
	@echo "  make run INPUT_FILE=sample.in        - Run with custom input"
	@echo "  make test INPUT_FILE=in.txt OUTPUT_FILE=out.txt"

.SUFFIXES:

# =========================================================================== #
