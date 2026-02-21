#!/usr/bin/env python3

import sys
from collections import Counter


def solve():
    # Fast I/O.
    input_data = sys.stdin.read().split()
    if not input_data:
        return

    t = int(input_data[0])
    out = []
    idx = 1

    MOD = 998244353

    for _ in range(t):
        n = int(input_data[idx])
        x = int(input_data[idx + 1])
        y = int(input_data[idx + 2])
        s = input_data[idx + 3]
        idx += 4

        W0 = 0
        V = []

        A = 1
        current_sum = 0
        first_q = True

        # Parse the string into independent blocks.
        for ch in s:
            if ch == "?":
                if first_q:
                    W0 = current_sum
                    first_q = False
                else:
                    V.append(abs(current_sum))
                current_sum = 0
                A = 1

            if ch == "1":
                A = -A
            current_sum += A

        if first_q:
            W0 = current_sum
        else:
            V.append(abs(current_sum))

        # Frequency grouping.
        counts = Counter(V)
        grouped_V = []
        for v, c in counts.items():
            if v == 0:
                continue
            k = 1
            while c >= k:
                grouped_V.append(v * k)
                c -= k
                k *= 2
            if c > 0:
                grouped_V.append(v * c)

        # Ultra-fast bitset shift for subset sums.
        reachable = 1
        for w in grouped_V:
            reachable |= reachable << w

        # Extract all reachable subset sums.
        bin_str = bin(reachable)[2:][::-1]

        sum_V = sum(V)
        base_S = W0 - sum_V

        unique_f = set()

        # Evaluate the closed-form formula for each reachable S_n.
        for i, bit in enumerate(bin_str):
            if bit == "1":
                Sn = base_S + 2 * i
                val = (x * Sn * Sn + (x - y) * Sn + y * n) // 2
                unique_f.add(val)

        # Sum unique cool integers modulo 998'244'353.
        ans = sum(unique_f) % MOD
        out.append(str(ans))

    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    solve()
    solve()
