# Timeout Policy

This repository keeps a portability-first timeout policy:

- prefer `timeout` when available (common on Linux)
- fallback to `gtimeout` when available (common on macOS via `coreutils`)
- avoid inline runtime script fallbacks

Practical checks:

- `workflow_manager.py doctor` now reports which timeout tool is visible in `PATH`
- if neither is found, install `coreutils` and ensure `gtimeout` is in `PATH`

Rationale:

- keeps behavior deterministic across Linux/macOS
- removes fragile ad-hoc timeout wrappers
