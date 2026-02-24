#!/usr/bin/env python3

import sys


def solve():
    input_data = sys.stdin.read().split()
    if not input_data:
        return

    t = int(input_data[0])
    out = []
    idx = 1

    for _ in range(t):
        n = int(input_data[idx])
        idx += 1

        a = [int(x) for x in input_data[idx : idx + n]]
        idx += n

        S = int(n**0.5)
        ans = 0

        # Pairs where the left element a[i] <= S
        for j in range(n):
            aj = a[j]
            for x in range(1, S + 1):
                i = j - aj * x
                if i < 0:
                    break
                if a[i] == x:
                    ans += 1

        # Pairs where the left element a[i] > S and the right element a[j] <= S
        for i in range(n):
            ai = a[i]
            if ai > S:
                for x in range(1, S + 1):
                    j = i + ai * x
                    if j >= n:
                        break
                    if a[j] == x:
                        ans += 1

        out.append(str(ans))

    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    solve()
