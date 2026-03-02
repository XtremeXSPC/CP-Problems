# I/O Profile Policy

This project supports explicit I/O profiles that map to `NEED_*` dependencies
in `templates/Base.hpp`.

## Profiles

- `CP_IO_PROFILE_SIMPLE`
  - Enables: `NEED_IO`
  - Use when standard stream I/O is enough (most simple problems).

- `CP_IO_PROFILE_FAST_MINIMAL`
  - Enables: `NEED_FAST_IO`
  - Use when you want fast buffered I/O with a lean feature set.

- `CP_IO_PROFILE_FAST_EXTENDED`
  - Enables: `NEED_FAST_IO`, `NEED_MOD_INT`, `NEED_TYPE_SAFETY`
  - Also enables Fast I/O extensions:
    - `CP_FAST_IO_ENABLE_MODINT=1`
    - `CP_FAST_IO_ENABLE_STRONG_TYPE=1`
  - Use for advanced rounds where both speed and typed abstractions are needed.

Only one profile macro can be defined in the same source file.

## Backward Compatibility

Direct `NEED_IO` / `NEED_FAST_IO` usage remains supported.
Profiles are convenience shortcuts, not a breaking replacement.

## Fast_IO Extensions

Fast I/O extension toggles remain available independently:

- `CP_FAST_IO_ENABLE_MODINT` (default `1` when `NEED_MOD_INT` is enabled)
- `CP_FAST_IO_ENABLE_STRONG_TYPE` (default `0`)

This allows keeping submissions lean when extended type support is unnecessary.
