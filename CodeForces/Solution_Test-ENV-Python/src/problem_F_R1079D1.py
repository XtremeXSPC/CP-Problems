#!/usr/bin/env python3

import math
import sys


def encode_edge(u, v):
    if u > v:
        u, v = v, u
    return (u << 32) | v


def minimal_odd_k_for_m(need):
    d = math.isqrt(1 + 8 * need)
    while (d + 1) * (d + 1) <= 1 + 8 * need:
        d += 1
    while d * d > 1 + 8 * need:
        d -= 1

    k = (1 + d) // 2
    while k * (k - 1) // 2 < need:
        k += 1
    if k % 2 == 0:
        k += 1
    return k


def build_circulant(k, deg_even):
    half = deg_even // 2
    edges = []
    for d in range(1, half + 1):
        for i in range(1, k + 1):
            j = i + d
            if j > k:
                j -= k
            if i < j:
                edges.append((i, j))
            else:
                edges.append((j, i))
    return edges


def construct_on_k(k, need):
    q = (2 * need) // k
    r = (2 * need) % k

    if q % 2 == 0:
        edges = build_circulant(k, q)
        shift = k // 2
        for i in range(1, r // 2 + 1):
            edges.append((i, i + shift))
        return edges

    base = build_circulant(k, q + 1)
    limit = k - r
    removed = set()
    for i in range(1, limit, 2):
        removed.add(encode_edge(i, i + 1))

    edges = []
    for u, v in base:
        if encode_edge(u, v) not in removed:
            edges.append((u, v))
    return edges


def construct_odd_total(need):
    k = minimal_odd_k_for_m(need)
    return construct_on_k(k, need)


def edges_for_m12():
    edges = []
    for i in range(1, 7):
        for j in range(i + 1, 7):
            if i <= 3 and j <= 3:
                continue
            edges.append((i, j))
    return edges


def complement_edges(n_all, small):
    small_set = set()
    for u, v in small:
        small_set.add(encode_edge(u, v))

    edges = []
    for u in range(1, n_all + 1):
        for v in range(u + 1, n_all + 1):
            if u == n_all or v == n_all:
                edges.append((u, v))
            elif encode_edge(u, v) not in small_set:
                edges.append((u, v))
    return edges


def construct_dense_comp12(n_all):
    missing = set()
    for i in range(1, 7):
        for j in range(i + 1, 7):
            if i <= 3 and j <= 3:
                continue
            missing.add(encode_edge(i, j))

    edges = []
    for u in range(1, n_all + 1):
        for v in range(u + 1, n_all + 1):
            if encode_edge(u, v) not in missing:
                edges.append((u, v))
    return edges


def fallback_construct(n, m):
    bad_small = {1, 2, 4, 6}
    bad_comp = {0, 1, 2, 4, 6}

    if m in bad_small:
        return None

    if m == 12:
        return edges_for_m12()

    if n % 2 == 1:
        return construct_odd_total(m)

    threshold = (n - 1) * (n - 2) // 2
    if m <= threshold:
        return construct_odd_total(m)

    m_comp = n * (n - 1) // 2 - m
    if m_comp == 12:
        return construct_dense_comp12(n)
    if m_comp in bad_comp:
        return None

    edges_comp = construct_odd_total(m_comp)
    return complement_edges(n, edges_comp)


def solve():
    input_data = sys.stdin.read().split()
    if not input_data:
        return

    T = int(input_data[0])
    out = []

    idx = 1
    for _ in range(T):
        n = int(input_data[idx])
        m = int(input_data[idx + 1])
        idx += 2

        M = n * (n - 1) // 2

        # Filter mathematically impossible cases.
        if m in (1, 2, 4, 6):
            out.append("No\n")
            continue

        if n % 2 == 0:
            threshold = (n - 1) * (n - 2) // 2
            if m > threshold and m in (M, M - 1, M - 2, M - 4, M - 6):
                out.append("No\n")
                continue

        # Primary path.
        fallback_edges = fallback_construct(n, m)
        if fallback_edges is None:
            out.append("No\n")
        else:
            out.append("Yes\n")
            for src, dst in fallback_edges:
                out.append(f"{src} {dst}\n")

    sys.stdout.write("".join(out))


if __name__ == "__main__":
    solve()
