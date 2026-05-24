# Timeout Policy

## Policy

This repository keeps a portability-first timeout policy:

- use `timeout` when available — standard on Linux and via Homebrew on macOS
- fall back to `gtimeout` when available — installed by `coreutils` on macOS
- never use inline script-based fallbacks (fragile, hard to audit)

## Installation

**macOS:**

```bash
brew install coreutils
```

After installation, `gtimeout` is available at `/opt/homebrew/bin/gtimeout`.
Ensure `/opt/homebrew/bin` is in `PATH`.

**Linux:** `timeout` is part of GNU coreutils and is present by default on all
major distributions.

## Verification

Run the doctor command to confirm which timeout tool is visible in `PATH`:

```bash
python3 scripts/workflow_manager.py doctor
```

Or via the Makefile:

```bash
make doctor
```

The output includes a line like:

```text
[workflow] timeout tool: /opt/homebrew/bin/timeout
```

If neither `timeout` nor `gtimeout` is found, the doctor will report the absence
and timeout-guarded operations (e.g. module verification runtime cases) will
either skip or error.

## Impact

The timeout tool is used by:

- `scripts/module_verify.py` — enforces `timeout_seconds` on runtime cases
- any script that calls into `workflow_manager.py` with a `--timeout` argument

Without a timeout tool, `runtime` verification cases that hang will block
indefinitely. `compile`-only cases are unaffected.
