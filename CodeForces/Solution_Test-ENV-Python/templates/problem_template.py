#!/usr/bin/env python3

import sys


def solve(data: str) -> str:
    # TODO: Implement your solution.
    return ""


def main() -> None:
    data = sys.stdin.read()
    out = solve(data)
    if out:
        sys.stdout.write(out)
        if not out.endswith("\n"):
            sys.stdout.write("\n")


if __name__ == "__main__":
    main()

