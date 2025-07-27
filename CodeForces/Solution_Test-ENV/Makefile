# Generic Makefile for C/C++ projects

# =========================================================================== #
# ----------------------- User-configurable variables ----------------------- #
# =========================================================================== #
TARGET_NAME    ?= Tester
SRCDIR         ?= .
OBJDIR         ?= obj
BINDIR         ?= bin

# Build profile can be: debug, release, sanitize. 'debug' is the default.
BUILD_TYPE     ?= debug

CC             ?= clang
CXX            ?= clang++

# =========================================================================== #
# Build Profile Configuration

# Common flags
COMMON_FLAGS   := -Wall -Wextra -pedantic
CPPFLAGS       ?= -I$(SRCDIR) $(foreach dir,$(shell find $(SRCDIR) -type d),-I$(dir))

# Profile-specific flags
ifeq ($(BUILD_TYPE), release)
	# Release: High optimization, no debug info, NDEBUG defined.
	PROFILE_FLAGS := -O3 -DNDEBUG
else ifeq ($(BUILD_TYPE), sanitize)
	# Sanitize: Debug info + AddressSanitizer and UndefinedBehaviorSanitizer.
	PROFILE_FLAGS := -g3 -O1 -fsanitize=address,undefined
else
	# Debug: Full debug info, no optimization. Default case.
	BUILD_TYPE    := debug
	PROFILE_FLAGS := -g3 -O0
endif

# Base flags (to be combined with profile flags)
BASE_CFLAGS    := -DLOCAL -std=c23
BASE_CXXFLAGS  := -DLOCAL -std=c++23

# Final flags
CFLAGS         := $(BASE_CFLAGS) $(COMMON_FLAGS) $(PROFILE_FLAGS)
CXXFLAGS       := $(BASE_CXXFLAGS) $(COMMON_FLAGS) $(PROFILE_FLAGS)
LDFLAGS        ?=
LIBS           ?= -lm

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

ifeq ($(UNAME_S),Darwin)
	detected_os := MACOS
	# Default Unix settings are already correct for macOS
else ifeq ($(UNAME_S),Linux)
	detected_os := LINUX
	# Default Unix settings are already correct for Linux
else ifneq (,$(findstring MINGW,$(UNAME_S))) # MinGW (Git Bash, MSYS2)
	detected_os := WINDOWS_MINGW
	TARGET_EXEC := $(TARGET_NAME).exe
	PathSep     := / # MinGW prefers Unix-style slashes
	RM          := rm -f
	RMDIR_CMD   := rm -rf
	MKDIR_P     := mkdir -p
else ifneq (,$(findstring CYGWIN,$(UNAME_S))) # Cygwin
	detected_os := WINDOWS_CYGWIN
	TARGET_EXEC := $(TARGET_NAME).exe # Cygwin can handle with or without .exe
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
	WINDOWS_VER_CHECK := $(shell ver 2>/dev/null) # Suppress error if 'ver' doesn't exist
	ifneq (,$(findstring Windows,$(WINDOWS_VER_CHECK)))
		detected_os := WINDOWS_NATIVE_VER_CMD
		TARGET_EXEC := $(TARGET_NAME).exe
		PathSep     := \
		RM          := del /Q /F
		RMDIR_CMD   := rd /S /Q
		# MKDIR_P handled in create_dirs rule
	else
		detected_os ?= UNKNOWN_UNIX # Final default if everything fails
	endif
endif

# If SRCDIR, OBJDIR, BINDIR are relative, it is necessary to ensure they are interpreted correctly.
# For now, we assume they are simple directory names like '.', 'obj', 'bin'

TARGET_DIR_WITH_SEP := $(BINDIR)$(PathSep)
TARGET_PATH         := $(TARGET_DIR_WITH_SEP)$(TARGET_EXEC)

# =========================================================================== #
# ----------------------- Source and Object Detection ----------------------- # 
# =========================================================================== #
C_SRCS         := $(wildcard $(SRCDIR)/*.c)
CPP_SRCS       := $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*.cc $(SRCDIR)/*.cxx)

C_OBJS         := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(C_SRCS))
CPP_OBJS       := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(filter %.cpp,$(CPP_SRCS))) \
			      $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(filter %.cc,$(CPP_SRCS))) \
			      $(patsubst $(SRCDIR)/%.cxx,$(OBJDIR)/%.o,$(filter %.cxx,$(CPP_SRCS)))

OBJS           := $(C_OBJS) $(CPP_OBJS)
DEPS           := $(OBJS:.o=.d)

# =========================================================================== #
# ---------------------- Linker Choice and Final Flags ---------------------- #
# =========================================================================== #
ifeq ($(strip $(CPP_SRCS))$(strip $(C_SRCS)),) # If no C or C++ sources
	LD = @echo "No C or C++ sources found, linker not set."; false
else ifeq ($(strip $(CPP_SRCS)),) # Only C sources
	LD := $(CC)
	LDFLAGS_FINAL := $(LDFLAGS) $(CFLAGS)
else # There are C++ sources (there might also be C)
	LD := $(CXX)
	LDFLAGS_FINAL := $(LDFLAGS) $(CXXFLAGS)
endif
LDFLAGS_FINAL += $(LIBS)

# =========================================================================== #
# ------------------------------- Build Rules ------------------------------- #
# =========================================================================== #
.PHONY: all clean distclean run debug-info help create_dirs

all: $(TARGET_PATH)

create_dirs:
	@echo "Creating directories $(OBJDIR) and $(BINDIR) if they do not exist..."
ifeq ($(findstring WINDOWS,$(detected_os)),WINDOWS)
	cmd /C "if not exist $(subst /,\,$(OBJDIR)) mkdir $(subst /,\,$(OBJDIR))"
	cmd /C "if not exist $(subst /,\,$(BINDIR)) mkdir $(subst /,\,$(BINDIR))"
else
# Non Windows (MACOS, LINUX, MINGW, CYGWIN, UNKNOWN_UNIX)
	$(MKDIR_P) $(OBJDIR)
	$(MKDIR_P) $(BINDIR)
endif

$(TARGET_PATH): create_dirs $(OBJS)
	@echo "# ------ Linking ------ #"
	@echo "Output file (-o switch target): $@"
	$(LD) $(OBJS) $(LDFLAGS_FINAL) -o $@

# Compilation rules
$(OBJDIR)/%.o: $(SRCDIR)/%.c create_dirs
	@echo "Compiling C: $< to $@"
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp create_dirs
	@echo "Compiling C++: $< to $@"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cc create_dirs
	@echo "Compiling C++: $< to $@"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cxx create_dirs
	@echo "Compiling C++: $< to $@"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

# =========================================================================== #
# ----------------------- Utility & Analysis Targets ------------------------ #
# =========================================================================== #
clean:
	@echo "Cleaning build artifacts..."
	-$(RM) $(OBJDIR)$(PathSep)*.o
	-$(RM) $(OBJDIR)$(PathSep)*.d
	-$(RM) $(TARGET_PATH)
	@echo "Clean complete."

distclean: clean
	@echo "Cleaning all directories..."
ifeq ($(findstring WINDOWS,$(detected_os)),WINDOWS)
	if exist $(subst /,\,$(OBJDIR)) $(RMDIR_CMD) $(subst /,\,$(OBJDIR))
	if exist $(subst /,\,$(BINDIR)) $(RMDIR_CMD) $(subst /,\,$(BINDIR))
else
	-$(RMDIR_CMD) $(OBJDIR)
	-$(RMDIR_CMD) $(BINDIR)
endif
	-$(RM) *~ *.bak # Remove common backup files
	@echo "Distclean complete."

ARGS ?=
run: $(TARGET_PATH)
	@echo "Running $(TARGET_PATH) $(ARGS)..."
ifeq ($(findstring WINDOWS,$(detected_os)),WINDOWS)
	$(subst /,\,$(TARGET_PATH)) $(ARGS)
else
	./$(TARGET_PATH) $(ARGS)
endif

# Static Analysis with clang-tidy
analyze:
	@echo "Running static analysis with clang-tidy..."
	@clang-tidy --quiet $(C_SRCS) $(CPP_SRCS) -- $(CPPFLAGS) $(CXXFLAGS)

# Compilation Database for clangd
compile_commands:
	@echo "Generating compile_commands.json..."
	@bear -- make --always-make BUILD_TYPE=$(BUILD_TYPE) all

# Help and Debug
debug-info:
	@echo "#====------ Makefile Debug Information ------====#"
	@echo "UNAME_S: $(UNAME_S)"
	@echo "Detected OS: $(detected_os)"
	@echo "PathSep: $(PathSep)"
	@echo "TARGET_EXEC: $(TARGET_EXEC)"
	@echo "TARGET_PATH: $(TARGET_PATH)"
	@echo "SRCDIR: $(SRCDIR), OBJDIR: $(OBJDIR), BINDIR: $(BINDIR)"
	@echo "#====---------- Sources & Objects -----------====#"
	@echo "C_SRCS: $(C_SRCS)"
	@echo "CPP_SRCS: $(CPP_SRCS)"
	@echo "OBJS: $(OBJS)"
	@echo "DEPS: $(DEPS)"
	@echo "#====---------- Compilers & Linker ----------====#"
	@echo "CC: $(CC), CXX: $(CXX), LD: $(LD)"
	@echo "CPPFLAGS: $(CPPFLAGS)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "LDFLAGS_FINAL: $(LDFLAGS_FINAL)"
	@echo "#====------------- OS Commands --------------====#"
	@echo "RM: $(RM)"
	@echo "RMDIR_CMD: $(RMDIR_CMD)"
	@echo "MKDIR_P (for Unix-like): $(MKDIR_P)"
	@echo "#====----------------------------------------====#"

help:
	@echo "Usage: make [TARGET] [VARIABLE=VALUE]"
	@echo "Targets:"
	@echo "  all               - Build with specified BUILD_TYPE (default: debug)"
	@echo "  run               - Build and run the project"
	@echo "  analyze           - Run static analysis with clang-tidy"
	@echo "  compile_commands  - Generate compilation database for clangd"
	@echo "  clean             - Remove object files and executable"
	@echo "  distclean         - Remove everything including directories"
	@echo "  debug-info        - Show debug information"
	@echo "  help              - Show this message"
	@echo "Variables:"
	@echo "  BUILD_TYPE=[debug|release|sanitize]"

.SUFFIXES:

# =========================================================================== #