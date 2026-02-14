#!/usr/bin/env python3

"""
Version Management System for Competitive Programming Template.
Manages versions, tracks changes, and provides rollback capabilities.
"""

import json
import hashlib
import shutil
from pathlib import Path
from datetime import datetime
import sys
from typing import Dict, List, Optional
import subprocess
import difflib


# ---------------------------- VERSION MANAGEMENT ---------------------------- #
class VersionManager:
    def __init__(self, templates_dir: Path):
        self.templates_dir = templates_dir
        self.versions_dir = templates_dir.parent / ".template_versions"
        self.metadata_file = self.versions_dir / "versions.json"
        self.current_version_file = self.versions_dir / "current.txt"

        # Initialize versioning directory.
        self.versions_dir.mkdir(exist_ok=True)

        # Load or initialize metadata.
        self.metadata = self.load_metadata()

    def load_metadata(self) -> Dict:
        """Load version metadata from JSON file."""
        if self.metadata_file.exists():
            try:
                with open(self.metadata_file, "r", encoding="utf-8") as f:
                    return json.load(f)
            except (json.JSONDecodeError, OSError) as e:
                backup = self.metadata_file.with_suffix(
                    f".corrupted.{datetime.now().strftime('%Y%m%d_%H%M%S')}.json"
                )
                shutil.copy2(self.metadata_file, backup)
                print(
                    f"Warning: invalid metadata file. Backup saved to: {backup}\n"
                    f"         Reinitializing metadata ({e})"
                )
        return {"versions": [], "current": None, "created": datetime.now().isoformat()}

    def save_metadata(self):
        """Save version metadata to JSON file."""
        with open(self.metadata_file, "w") as f:
            json.dump(self.metadata, f, indent=2)

    def calculate_checksum(self, directory: Path) -> str:
        """Calculate SHA256 checksum of all files in directory."""
        hasher = hashlib.sha256()

        for file_path in sorted(directory.rglob("*.hpp")):
            if file_path.is_file():
                with open(file_path, "rb") as f:
                    hasher.update(f.read())

        return hasher.hexdigest()

    def create_version(self, message: str, tag: Optional[str] = None) -> str:
        """Create a new version snapshot."""
        # Generate version ID.
        timestamp = datetime.now()
        version_id = timestamp.strftime("%Y%m%d_%H%M%S")

        # Calculate checksum.
        checksum = self.calculate_checksum(self.templates_dir)

        # Check if content has changed.
        if self.metadata["versions"]:
            last_version = self.metadata["versions"][-1]
            if last_version["checksum"] == checksum:
                print("No changes detected since last version.")
                return last_version["id"]

        # Create version directory.
        version_dir = self.versions_dir / version_id
        version_dir.mkdir(exist_ok=True)

        # Copy all template files.
        for file_path in self.templates_dir.glob("*.hpp"):
            shutil.copy2(file_path, version_dir / file_path.name)

        # Create version entry.
        version_entry = {
            "id": version_id,
            "timestamp": timestamp.isoformat(),
            "message": message,
            "tag": tag,
            "checksum": checksum,
            "files": [f.name for f in self.templates_dir.glob("*.hpp")],
        }

        # Add performance metrics if available.
        metrics = self.collect_performance_metrics()
        if metrics:
            version_entry["metrics"] = metrics

        # Update metadata.
        self.metadata["versions"].append(version_entry)
        self.metadata["current"] = version_id
        self.save_metadata()

        # Update current version file.
        with open(self.current_version_file, "w") as f:
            f.write(version_id)

        print(f"Created version: {version_id}")
        if tag:
            print(f"Tagged as: {tag}")

        return version_id

    def collect_performance_metrics(self) -> Optional[Dict]:
        """Collect compilation and performance metrics."""
        test_file = self.templates_dir.parent / "test_compile.cpp"
        try:
            metrics = {}
            config_compiler = None
            config_path = self.templates_dir.parent / ".template_config.json"
            if config_path.exists():
                try:
                    with open(config_path, "r", encoding="utf-8") as f:
                        config_compiler = json.load(f).get("compiler")
                except (json.JSONDecodeError, OSError):
                    config_compiler = None

            compiler = (
                shutil.which(config_compiler) if config_compiler else None
            ) or shutil.which("g++-15") or shutil.which("g++") or shutil.which("c++") or "c++"

            # Test compilation time for basic template.
            with open(test_file, "w", encoding="utf-8") as f:
                f.write(
                    """
#define NEED_CORE
#define NEED_IO
#include "templates/Base.hpp"
int main() { return 0; }
"""
                )

            import time

            start = time.time()
            result = subprocess.run(
                [
                    compiler,
                    "-std=c++23",
                    "-DUSE_CLANG_SANITIZE",
                    "-fsyntax-only",
                    f"-I{self.templates_dir.parent}",
                    str(test_file),
                ],
                capture_output=True,
                timeout=10,
            )
            compile_time = time.time() - start

            metrics["compile_time_basic"] = compile_time
            metrics["compile_success"] = result.returncode == 0
            metrics["compiler"] = compiler

            # Count lines of code.
            total_lines = 0
            for file_path in self.templates_dir.glob("*.hpp"):
                with open(file_path, "r") as f:
                    total_lines += len(f.readlines())

            metrics["total_lines"] = total_lines
            metrics["file_count"] = len(list(self.templates_dir.glob("*.hpp")))

            return metrics

        except Exception as e:
            print(f"Warning: Could not collect metrics: {e}")
            return None
        finally:
            test_file.unlink(missing_ok=True)

    def list_versions(self, detailed: bool = False):
        """List all available versions."""
        if not self.metadata["versions"]:
            print("No versions found.")
            return

        print("Available versions:")
        print("-" * 60)

        for version in self.metadata["versions"]:
            current_marker = (
                " [CURRENT]" if version["id"] == self.metadata["current"] else ""
            )
            tag_info = f" (tag: {version['tag']})" if version["tag"] else ""

            print(f"{version['id']}{current_marker}{tag_info}")
            print(f"  Date: {version['timestamp']}")
            print(f"  Message: {version['message']}")

            if detailed and "metrics" in version:
                metrics = version["metrics"]
                compile_time = metrics.get("compile_time_basic")
                compile_time_str = (
                    f"{compile_time:.3f}s"
                    if isinstance(compile_time, (int, float))
                    else "N/A"
                )
                print(f"  Metrics:")
                print(f"    - Files: {metrics.get('file_count', 'N/A')}")
                print(f"    - Lines: {metrics.get('total_lines', 'N/A')}")
                print(f"    - Compile time: {compile_time_str}")

            print()

    def checkout_version(self, version_id: str) -> bool:
        """Switch to a specific version."""
        # Find version.
        version = None
        for v in self.metadata["versions"]:
            if v["id"] == version_id or v.get("tag") == version_id:
                version = v
                break

        if not version:
            print(f"Version not found: {version_id}")
            return False

        version_dir = self.versions_dir / version["id"]
        if not version_dir.exists():
            print(f"Version directory not found: {version_dir}")
            return False

        # Backup current state.
        backup_dir = self.versions_dir / "backup_current"
        if backup_dir.exists():
            shutil.rmtree(backup_dir)
        backup_dir.mkdir()

        for file_path in self.templates_dir.glob("*.hpp"):
            shutil.copy2(file_path, backup_dir / file_path.name)

        # Remove stale files not present in target version.
        target_files = {f.name for f in version_dir.glob("*.hpp")}
        for file_path in self.templates_dir.glob("*.hpp"):
            if file_path.name not in target_files:
                file_path.unlink()

        # Restore version.
        for file_path in version_dir.glob("*.hpp"):
            shutil.copy2(file_path, self.templates_dir / file_path.name)

        # Update current version.
        self.metadata["current"] = version["id"]
        self.save_metadata()

        with open(self.current_version_file, "w") as f:
            f.write(version["id"])

        print(f"Switched to version: {version['id']}")
        return True

    def diff_versions(self, version1_id: str, version2_id: Optional[str] = None):
        """Show differences between two versions."""
        # If version2 not specified, compare with current.
        if not version2_id:
            version2_id = self.metadata["current"]

        # Find versions.
        v1_dir = self.versions_dir / version1_id
        v2_dir = (
            self.versions_dir / version2_id
            if version2_id and version2_id != "current"
            else self.templates_dir
        )

        if not v1_dir.exists():
            print(f"Version not found: {version1_id}")
            return

        print(f"Comparing {version1_id} with {version2_id}")
        print("=" * 60)

        # Compare each file.
        all_files = set()
        if v1_dir.exists():
            all_files.update(f.name for f in v1_dir.glob("*.hpp"))
        if v2_dir.exists():
            all_files.update(f.name for f in v2_dir.glob("*.hpp"))

        for filename in sorted(all_files):
            file1 = v1_dir / filename
            file2 = v2_dir / filename

            if not file1.exists():
                print(f"\n+ {filename} (added in {version2_id})")
            elif not file2.exists():
                print(f"\n- {filename} (removed in {version2_id})")
            else:
                with open(file1, "r") as f1, open(file2, "r") as f2:
                    lines1 = f1.readlines()
                    lines2 = f2.readlines()

                    diff = list(
                        difflib.unified_diff(
                            lines1,
                            lines2,
                            fromfile=f"{version1_id}/{filename}",
                            tofile=f"{version2_id}/{filename}",
                            lineterm="",
                        )
                    )

                    if diff:
                        print(f"\n~ {filename} (modified)")
                        for line in diff[:50]:  # Show first 50 lines of diff.
                            print(line)
                        if len(diff) > 50:
                            print(f"... and {len(diff) - 50} more lines")

    def export_version(self, version_id: str, output_path: Path):
        """Export a specific version as a tar archive."""
        import tarfile

        version_dir = self.versions_dir / version_id
        if not version_dir.exists():
            print(f"Version not found: {version_id}")
            return

        with tarfile.open(output_path, "w:gz") as tar:
            tar.add(version_dir, arcname=f"template_{version_id}")

        print(f"Exported version {version_id} to {output_path}")

    def auto_version(self):
        """Automatically create a version if changes detected."""
        current_checksum = self.calculate_checksum(self.templates_dir)

        if self.metadata["versions"]:
            last_checksum = self.metadata["versions"][-1]["checksum"]
            if current_checksum != last_checksum:
                self.create_version("Auto-save: Changes detected")
                return True
        else:
            self.create_version("Initial version")
            return True

        return False


# -------------------------------- MAIN LOGIC -------------------------------- #


def main():
    import argparse

    parser = argparse.ArgumentParser(description="Template Version Manager")
    parser.add_argument("--dir", type=Path, help="Templates directory")

    subparsers = parser.add_subparsers(dest="command", help="Commands")

    # Create version.
    create_parser = subparsers.add_parser("create", help="Create new version")
    create_parser.add_argument("message", help="Version message")
    create_parser.add_argument("--tag", help="Optional tag")

    # List versions.
    list_parser = subparsers.add_parser("list", help="List versions")
    list_parser.add_argument(
        "--detailed", action="store_true", help="Show detailed info"
    )

    # Checkout version.
    checkout_parser = subparsers.add_parser("checkout", help="Switch to version")
    checkout_parser.add_argument("version", help="Version ID or tag")

    # Diff versions.
    diff_parser = subparsers.add_parser("diff", help="Compare versions")
    diff_parser.add_argument("version1", help="First version")
    diff_parser.add_argument(
        "version2", nargs="?", help="Second version (default: current)"
    )

    # Export version.
    export_parser = subparsers.add_parser("export", help="Export version")
    export_parser.add_argument("version", help="Version ID")
    export_parser.add_argument("output", type=Path, help="Output file")

    # Auto version.
    auto_parser = subparsers.add_parser("auto", help="Auto-create version if changed")

    args = parser.parse_args()

    # Find templates directory.
    if args.dir:
        templates_dir = args.dir
    else:
        templates_dir = Path.cwd() / "templates"

    if not templates_dir.exists():
        print(f"Templates directory not found: {templates_dir}")
        return 1

    vm = VersionManager(templates_dir)

    if args.command == "create":
        vm.create_version(args.message, args.tag)
    elif args.command == "list":
        vm.list_versions(args.detailed)
    elif args.command == "checkout":
        vm.checkout_version(args.version)
    elif args.command == "diff":
        vm.diff_versions(args.version1, args.version2)
    elif args.command == "export":
        vm.export_version(args.version, args.output)
    elif args.command == "auto":
        if vm.auto_version():
            print("Created automatic version")
        else:
            print("No changes detected")
    else:
        parser.print_help()

    return 0


if __name__ == "__main__":
    sys.exit(main())
