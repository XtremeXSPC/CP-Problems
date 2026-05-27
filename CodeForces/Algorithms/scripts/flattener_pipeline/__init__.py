"""End-to-end flattening pipeline orchestrating the ``flattener_core`` primitives.

Stages are split into single-responsibility modules: ``context`` (immutable
input dataclasses), ``macros`` (registry-aware macro extraction),
``pipeline`` (template-bundle + source-rewrite orchestrator), ``validation``
(syntax-only compile check), ``submission`` (size-reducing post-pass), and
``cli`` (argparse front-end and ``main`` entry point).

Every IO-bearing concern of the flattener (environment variables, file
reads, subprocess invocation) lives in this package; the core layer stays
pure.
"""
