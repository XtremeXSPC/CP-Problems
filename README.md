# CP-Problems

A personal competitive programming repository focused on algorithmic study,
problem solving, and the development of a reusable C++ library for contests.

The repository contains solutions to programming problems together with a
centralized collection of templates, algorithms, and data structures. Its purpose
is not only to store accepted submissions, but also to document a long-term
process of studying theoretical ideas, translating them into concise C++ code,
and refining them through practical contest use.

## Overview

Competitive programming rewards both mathematical understanding and disciplined
implementation. This repository is built around that intersection: problems are
solved as exercises in reasoning, while recurring techniques are gradually
extracted into reusable components.

The central library lives under `CodeForces/Algorithms` and is organized around
two main layers:

- `templates/`: a compact template system for contest solutions;
- `modules/`: reusable implementations of algorithms and data structures.

The templates provide the basic environment used by solutions: type aliases,
constants, small utility functions, input/output helpers, modular arithmetic,
bit operations, hashing support, and optional extensions such as PBDS. The
design is intentionally modular, so that a solution can include only the
features that are useful for a given problem.

## Algorithms and Data Structures

The reusable library includes implementations from several classical areas of
competitive programming:

- Graph algorithms: shortest paths, connectivity, flows, matchings, tree
  queries, LCA, Euler tours, heavy-light decomposition;
- Data structures: Fenwick trees, segment trees, lazy propagation structures,
  sparse tables, disjoint-set union, and advanced sequence structures;
- Number theory: sieves, primality testing, factorization, modular arithmetic,
  CRT/Garner reconstruction, NTT, and arithmetic functions;
- String algorithms: KMP, Z-algorithm, tries, suffix arrays, automata, rolling
  hashes, and palindromic structures;
- Geometry: two-dimensional and three-dimensional primitives, convex hulls,
  intersections, circles, polygons, and related utilities;
- Algebraic and bitwise tools: monoids, acted monoids, XOR interval
  decompositions, and other specialized helpers.

The guiding principle is economy: an abstraction is added only when it makes
future solutions shorter, safer, or easier to reason about. The code aims to
remain close to competitive programming practice while preserving clear
interfaces and predictable behavior.

## Study Philosophy

The repository reflects a gradual method:

1. Understand the mathematical structure of a problem;
2. Identify the algorithmic pattern behind it;
3. Implement the solution with contest-level concision;
4. Extract only genuinely reusable components;
5. Validate the component through repeated practical use.

This balance between theory and implementation is the core of the project. The
library is therefore not meant to be a general-purpose framework; it is a
carefully maintained toolkit shaped by the constraints of programming contests.

## Educational Note

Solutions are kept for personal study and reference. They are most useful after
an independent attempt at the problem, when comparing approaches, checking edge
cases, or studying how a known algorithm can be implemented under contest
constraints.
