#!/usr/bin/env python3

import sys
from array import array


def fast_ints():
    data = sys.stdin.buffer.read()
    n = len(data)
    i = 0
    while i < n:
        while i < n and data[i] <= 32:
            i += 1
        if i >= n:
            break
        sign = 1
        if data[i] == 45:  # '-'
            sign = -1
            i += 1
        num = 0
        while i < n:
            c = data[i]
            if 48 <= c <= 57:
                num = num * 10 + (c - 48)
                i += 1
            else:
                break
        yield sign * num


def solve():
    iterator = fast_ints()
    t = next(iterator, None)
    if t is None:
        return

    out = []

    for _ in range(t):
        n = next(iterator)
        q = next(iterator)

        stride = n + 1
        total_size = stride * stride

        # 1D packed arrays for 2D prefix sums (64-bit signed).
        pref_M = array("q", [0]) * total_size
        pref_R = array("q", [0]) * total_size
        pref_C = array("q", [0]) * total_size

        # Build 2D prefix sums.
        for r in range(1, n + 1):
            row_offset = r * stride
            prev_row_offset = (r - 1) * stride
            for c in range(1, n + 1):
                val = next(iterator)

                idx = row_offset + c
                left = idx - 1
                up = prev_row_offset + c
                up_left = up - 1

                # Inclusion-exclusion for 2D prefix sums.
                pref_M[idx] = val + pref_M[left] + pref_M[up] - pref_M[up_left]
                pref_R[idx] = val * r + pref_R[left] + pref_R[up] - pref_R[up_left]
                pref_C[idx] = val * c + pref_C[left] + pref_C[up] - pref_C[up_left]

        # Process queries in O(1) time each.
        for _ in range(q):
            x1 = next(iterator)
            y1 = next(iterator)
            x2 = next(iterator)
            y2 = next(iterator)

            # Calculate 1D indices for the 4 corners of the submatrix.
            idx_br = x2 * stride + y2
            idx_bl = x2 * stride + (y1 - 1)
            idx_tr = (x1 - 1) * stride + y2
            idx_tl = (x1 - 1) * stride + (y1 - 1)

            # Extract submatrix sums using inclusion-exclusion.
            sum_M = pref_M[idx_br] - pref_M[idx_bl] - pref_M[idx_tr] + pref_M[idx_tl]
            sum_R = pref_R[idx_br] - pref_R[idx_bl] - pref_R[idx_tr] + pref_R[idx_tl]
            sum_C = pref_C[idx_br] - pref_C[idx_bl] - pref_C[idx_tr] + pref_C[idx_tl]

            # Apply the algebraic expansion formula.
            C = y2 - y1 + 1
            K = 1 - x1 * C - y1

            ans = C * sum_R + sum_C + K * sum_M
            out.append(str(ans))

    # Fast bulk output
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    solve()
