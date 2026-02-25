#!/usr/bin/env python3

import sys
from collections import deque


def solve():
    input_data = sys.stdin.read().split()
    if not input_data:
        return

    t = int(input_data[0])
    idx = 1

    out = []

    for _ in range(t):
        n = int(input_data[idx])

        # Initialize graph and degree arrays.
        r = [0] * (n + 1)
        in_degree = [0] * (n + 1)

        # Parse the recipient array and populate in-degrees.
        for i in range(1, n + 1):
            v = int(input_data[idx + i])
            r[i] = v
            in_degree[v] += 1

        idx += n + 1

        # Each spider starts with exactly 1 plushie.
        sz = [1] * (n + 1)
        queue = deque()

        # Enqueue all leaf nodes (spiders that receive no plushies).
        for i in range(1, n + 1):
            if in_degree[i] == 0:
                queue.append(i)

        max_sz = 0

        # Process the directed trees feeding into the cycles.
        while queue:
            u = queue.popleft()

            # Track the maximum plushie lifetime among non-cycle nodes.
            if sz[u] > max_sz:
                max_sz = sz[u]

            v = r[u]
            sz[v] += sz[u]
            in_degree[v] -= 1

            # If all incoming branches to v are resolved, it can be processed.
            if in_degree[v] == 0:
                queue.append(v)

        # The system stabilizes 2 years after the longest tree branch exhausts its plushies.
        out.append(str(max_sz + 2))

    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    solve()
