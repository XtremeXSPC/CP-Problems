#!/usr/bin/env python3

import sys


def solve(data: str) -> str:
    values = data.split()
    if not values:
        return ""

    nums = [int(x) for x in values]
    if len(nums) >= 2:
        return str(nums[0] + nums[1])
    return str(nums[0])


def main() -> None:
    data = sys.stdin.read()
    out = solve(data)
    if out:
        sys.stdout.write(out)
        if not out.endswith("\n"):
            sys.stdout.write("\n")


if __name__ == "__main__":
    main()

