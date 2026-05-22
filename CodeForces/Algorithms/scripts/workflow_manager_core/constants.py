"""Static constants used by workflow manager components."""

from pathlib import Path
import re
import sys

DEFAULT_WORKSPACE_ROOT = Path(__file__).resolve().parents[4]
# Resolution chain in utils.discover_cp_tools_script covers $CPP_TOOLS_SCRIPT
# and ~/.config/cpp-tools/competitive.sh, so no hardcoded fallback is needed.
DEFAULT_CP_TOOLS_SCRIPT: Path | None = None

_SCRIPTS_DIR = Path(__file__).resolve().parents[1]
if str(_SCRIPTS_DIR) not in sys.path:
    sys.path.insert(0, str(_SCRIPTS_DIR))
from profile_registry import load_registry  # noqa: E402

TEMPLATE_CHOICES = load_registry().all_scaffold_names()
BUILD_TYPE_CHOICES = ("Debug", "Release", "Sanitize")
COMPILER_CHOICES = ("gcc", "clang", "auto")
TOGGLE_CHOICES = ("on", "off")
PCH_CHOICES = ("on", "off", "auto")

CMAKE_CONFIG_PRESET_CHOICES = (
    "cp-debug-gcc",
    "cp-release-gcc",
    "cp-sanitize-gcc",
    "cp-debug-clang",
    "cp-release-clang",
    "cp-sanitize-clang",
)

CMAKE_BUILD_PRESET_CHOICES = (
    "cp-build-debug-gcc",
    "cp-build-release-gcc",
    "cp-build-sanitize-gcc",
    "cp-build-debug-clang",
    "cp-build-release-clang",
    "cp-build-sanitize-clang",
)

TARGET_RE = re.compile(r"^[A-Za-z]\w*$")
CONTEST_SEGMENT_RE = re.compile(r"^[A-Za-z0-9._-]+$")

ALLOWED_FUNCTIONS = {
    "cpparchive",
    "cppbatch",
    "cppbuild",
    "cppcheck",
    "cppclean",
    "cppconf",
    "cppcontest",
    "cppdeepclean",
    "cppdelete",
    "cppdiag",
    "cppforcego",
    "cppfull",
    "cppgcc",
    "cppgo",
    "cpphelp",
    "cppi",
    "cppinfo",
    "cppinit",
    "cppjudge",
    "cppnew",
    "cpprun",
    "cppstats",
    "cppstress",
    "cppsubmit",
    "cpptestsubmit",
    "cppwatch",
    "cppclang",
    "cppprof",
}

INPUT_FILE_HELP = "input filename inside input_cases/"
