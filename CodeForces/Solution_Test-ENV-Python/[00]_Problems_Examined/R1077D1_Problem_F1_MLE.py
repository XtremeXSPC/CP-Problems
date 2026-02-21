#!/usr/bin/env python3
"""
This solution reach MLE but the algorithm is correct. Another implementation
in C++ with the same algorithm passed, so this is likely due to Python's
overhead in "Segment Tree" operations.
"""
import sys

# Increase recursion depth for deep segment tree branches.
sys.setrecursionlimit(2000005)


def solve():
    input_data = sys.stdin.read().split()
    if not input_data:
        return

    t = int(input_data[0])
    idx = 1

    # Pre-allocate global arrays to avoid overhead across test cases.
    MAX_N = 1000005
    min_val = [0] * (4 * MAX_N)
    lazy = [0] * (4 * MAX_N)

    M = 1
    while M <= MAX_N:
        M *= 2
    dp_tree = [0] * (2 * M)

    out = []

    for _ in range(t):
        n = int(input_data[idx])
        idx += 1

        a = [0] * (n + 1)
        for i in range(1, n + 1):
            a[i] = int(input_data[idx])
            idx += 1

        b = [0] * (n + 1)
        for i in range(1, n + 1):
            b[i] = int(input_data[idx])
            idx += 1

        pos_b = [0] * (n + 1)
        for i in range(n + 1):
            pos_b[b[i]] = i

        a_prime = [0] * (n + 1)
        for i in range(n + 1):
            a_prime[i] = pos_b[a[i]]

        next_smaller = [n + 1] * (n + 1)
        stack_min = []
        for i in range(n + 1):
            while stack_min and a_prime[stack_min[-1]] > a_prime[i]:
                next_smaller[stack_min.pop()] = i
            stack_min.append(i)

        def build_val(node, L, R):
            lazy[node] = 0
            if L == R:
                min_val[node] = -L
                return
            Mid = (L + R) // 2
            build_val(2 * node, L, Mid)
            build_val(2 * node + 1, Mid + 1, R)
            min_val[node] = (
                min_val[2 * node]
                if min_val[2 * node] < min_val[2 * node + 1]
                else min_val[2 * node + 1]
            )

        build_val(1, 0, n)

        def update_val(node, L, R, ql, qr, v):
            if ql <= L and R <= qr:
                min_val[node] += v
                lazy[node] += v
                return
            Mid = (L + R) // 2
            left = 2 * node
            right = left + 1

            if lazy[node]:
                lz = lazy[node]
                min_val[left] += lz
                lazy[left] += lz
                min_val[right] += lz
                lazy[right] += lz
                lazy[node] = 0

            if ql <= Mid:
                update_val(left, L, Mid, ql, qr, v)
            if qr > Mid:
                update_val(right, Mid + 1, R, ql, qr, v)

            min_val[node] = min_val[left] if min_val[left] < min_val[right] else min_val[right]

        def query_rightmost(node, L, R, ql, qr, target):
            if min_val[node] > target:
                return -1
            if ql <= L and R <= qr:
                if min_val[node] > target:
                    return -1
                # Walk down the tree to find the rightmost element.
                while L < R:
                    Mid = (L + R) // 2
                    left = 2 * node
                    right = left + 1
                    if lazy[node]:
                        lz = lazy[node]
                        min_val[left] += lz
                        lazy[left] += lz
                        min_val[right] += lz
                        lazy[right] += lz
                        lazy[node] = 0

                    if min_val[right] <= target:
                        node = right
                        L = Mid + 1
                    else:
                        node = left
                        R = Mid
                return L if min_val[node] == target else -1

            Mid = (L + R) // 2
            left = 2 * node
            right = left + 1
            if lazy[node]:
                lz = lazy[node]
                min_val[left] += lz
                lazy[left] += lz
                min_val[right] += lz
                lazy[right] += lz
                lazy[node] = 0

            res = -1
            if qr > Mid:
                res = query_rightmost(right, Mid + 1, R, ql, qr, target)
            if res == -1 and ql <= Mid:
                res = query_rightmost(left, L, Mid, ql, qr, target)
            return res

        # Clear only the used parts of the DP tree.
        for i in range(n + 2):
            dp_tree[M + i] = 0
        for i in range(1, (M + n) // 2 + 1):
            dp_tree[i] = 0

        def update_dp(i, val):
            curr = M + i
            dp_tree[curr] = val
            curr //= 2
            while curr > 0:
                dp_tree[curr] = (
                    dp_tree[2 * curr]
                    if dp_tree[2 * curr] > dp_tree[2 * curr + 1]
                    else dp_tree[2 * curr + 1]
                )
                curr //= 2

        def query_dp(L, R):
            if L > R:
                return 0
            L += M
            R += M
            res = 0
            while L <= R:
                if L % 2 == 1:
                    if dp_tree[L] > res:
                        res = dp_tree[L]
                    L += 1
                if R % 2 == 0:
                    if dp_tree[R] > res:
                        res = dp_tree[R]
                    R -= 1
                L //= 2
                R //= 2
            return res

        st = []
        for i in range(n, -1, -1):
            new_max = a_prime[i]
            while st and a_prime[st[-1]] < new_max:
                j = st.pop()
                next_idx = st[-1] - 1 if st else n
                old_max = a_prime[j]
                update_val(1, 0, n, j, next_idx, new_max - old_max)
            st.append(i)
            update_val(1, 0, n, i, i, new_max)

            target = new_max - i
            limit = next_smaller[i] - 1

            R_i = query_rightmost(1, 0, n, i, limit, target)

            if R_i == -1:
                R_i = i

            max_child_dp = query_dp(i + 1, R_i)
            dp_i = 1 + max_child_dp
            update_dp(i, dp_i)

            if i == 0:
                out.append(str(dp_i - 1))

    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    solve()
