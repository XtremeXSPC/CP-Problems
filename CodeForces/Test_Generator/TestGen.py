#!/usr/bin/env python3

# =========================================================================== #
"""
TestGen - Advanced Test Case Generator for Competitive Programming
A modular, extensible framework for generating diverse test cases
"""
# =========================================================================== #

import random
import json
import argparse
import sys
import os
from abc import ABC, abstractmethod
from typing import List, Dict, Any, Optional, Tuple
from dataclasses import dataclass, field
from enum import Enum
import string
import math
from collections import defaultdict

# =========================================================================== #
# ------------------------- Core Classes and Types -------------------------- #
# =========================================================================== #

class TestSize(Enum):
    """Standard test sizes for competitive programming"""
    TINY = "tiny"         # ~10 elements
    SMALL = "small"       # ~100 elements  
    MEDIUM = "medium"     # ~10^4 elements
    LARGE = "large"       # ~10^5 elements
    STRESS = "stress"     # ~10^6 elements
    CUSTOM = "custom"     # User-defined size

@dataclass
class TestConfig:
    """Configuration for test generation"""
    size: TestSize
    custom_size: Optional[int] = None
    seed: Optional[int] = None
    constraints: Dict[str, Any] = field(default_factory=dict)
    
    def __post_init__(self):
        if self.seed is not None:
            random.seed(self.seed)

    def get_n(self) -> int:
        """Get the actual size value"""
        if self.size == TestSize.CUSTOM:
            return self.custom_size or 100
        
        size_map = {
            TestSize.TINY: 10,
            TestSize.SMALL: 100,
            TestSize.MEDIUM: 10000,
            TestSize.LARGE: 100000,
            TestSize.STRESS: 1000000
        }
        return size_map.get(self.size, 100)

# =========================================================================== #
# -------------------------- Base Generator Class --------------------------- #
# =========================================================================== #

class TestGenerator(ABC):
    """Abstract base class for all test generators"""
    
    def __init__(self, config: TestConfig):
        self.config = config
        self.n = config.get_n()
    
    @abstractmethod
    def generate(self) -> str:
        """Generate test case and return as string"""
        pass
    
    @property
    @abstractmethod
    def name(self) -> str:
        """Return generator name"""
        pass
    
    @property
    @abstractmethod
    def description(self) -> str:
        """Return generator description"""
        pass

# =========================================================================== #
# -------------------------- Array-based Generators ------------------------- #
# =========================================================================== #

class ArrayGenerator(TestGenerator):
    """Generate array of integers"""
    
    @property
    def name(self) -> str:
        return "array"
    @property
    def description(self) -> str:
        return "Generate array of random integers"
    
    def generate(self) -> str:
        min_val = self.config.constraints.get('min_val', 1)
        max_val = self.config.constraints.get('max_val', 10**9)
        
        result = [str(self.n)]
        for _ in range(self.n):
            result.append(str(random.randint(min_val, max_val)))
        
        return '\n'.join(result)

class SortedArrayGenerator(TestGenerator):
    """Generate sorted array"""
    
    @property
    def name(self) -> str:
        return "sorted_array"
    @property
    def description(self) -> str:
        return "Generate sorted array (ascending or descending)"
    
    def generate(self) -> str:
        min_val = self.config.constraints.get('min_val', 1)
        max_val = self.config.constraints.get('max_val', 10**9)
        descending = self.config.constraints.get('descending', False)
        
        arr = sorted([random.randint(min_val, max_val) for _ in range(self.n)], 
                    reverse=descending)
        
        result = [str(self.n)]
        result.extend(map(str, arr))
        return '\n'.join(result)

class PermutationGenerator(TestGenerator):
    """Generate permutation of 1..n"""
    
    @property
    def name(self) -> str:
        return "permutation"
    @property
    def description(self) -> str:
        return "Generate random permutation of numbers 1 to n"
    
    def generate(self) -> str:
        perm = list(range(1, self.n + 1))
        random.shuffle(perm)
        
        result = [str(self.n)]
        result.extend(map(str, perm))
        return '\n'.join(result)

class MatrixGenerator(TestGenerator):
    """Generate 2D matrix"""
    
    @property
    def name(self) -> str:
        return "matrix"
    @property
    def description(self) -> str:
        return "Generate n×m matrix of integers"
    
    def generate(self) -> str:
        rows = self.n
        cols = self.config.constraints.get('cols', self.n)
        min_val = self.config.constraints.get('min_val', 0)
        max_val = self.config.constraints.get('max_val', 100)
        
        result = [f"{rows} {cols}"]
        for _ in range(rows):
            row = [str(random.randint(min_val, max_val)) for _ in range(cols)]
            result.append(' '.join(row))
        
        return '\n'.join(result)

# =========================================================================== #
# ---------------------------- String Generators ---------------------------- #
# =========================================================================== #

class StringGenerator(TestGenerator):
    """Generate random string"""
    
    @property
    def name(self) -> str:
        return "string"
    @property
    def description(self) -> str:
        return "Generate random string with specified alphabet"
    
    def generate(self) -> str:
        alphabet = self.config.constraints.get('alphabet', string.ascii_lowercase)
        length = self.config.constraints.get('length', self.n)
        
        s = ''.join(random.choice(alphabet) for _ in range(length))
        return s

class PalindromeGenerator(TestGenerator):
    """Generate palindrome string"""
    
    @property
    def name(self) -> str:
        return "palindrome"
    @property
    def description(self) -> str:
        return "Generate palindrome string"
    
    def generate(self) -> str:
        alphabet = self.config.constraints.get('alphabet', string.ascii_lowercase)
        length = self.config.constraints.get('length', self.n)
        
        half = length // 2
        first_half = ''.join(random.choice(alphabet) for _ in range(half))
        
        if length % 2 == 0:
            return first_half + first_half[::-1]
        else:
            return first_half + random.choice(alphabet) + first_half[::-1]

class BinaryStringGenerator(TestGenerator):
    """Generate binary string"""
    
    @property
    def name(self) -> str:
        return "binary_string"
    @property
    def description(self) -> str:
        return "Generate string of 0s and 1s"
    
    def generate(self) -> str:
        prob_one = self.config.constraints.get('prob_one', 0.5)
        length = self.config.constraints.get('length', self.n)
        
        s = ''.join('1' if random.random() < prob_one else '0' for _ in range(length))
        return s

# =========================================================================== #
# ---------------------------- Graph Generators ----------------------------- #
# =========================================================================== #

class GraphGenerator(TestGenerator):
    """Base class for graph generators"""
    
    def edges_to_string(self, edges: List[Tuple], weighted: bool = False) -> str:
        """Convert edge list to string format"""
        n = self.n
        m = len(edges)
        
        result = [f"{n} {m}"]
        for edge in edges:
            if weighted:
                u, v, w = edge
                result.append(f"{u} {v} {w}")
            else:
                u, v = edge
                result.append(f"{u} {v}")
        
        return '\n'.join(result)

class RandomGraphGenerator(GraphGenerator):
    """Generate random graph with specified edge probability"""
    
    @property
    def name(self) -> str:
        return "random_graph"
    @property
    def description(self) -> str:
        return "Generate random graph with edge probability"
    
    def generate(self) -> str:
        edge_prob = self.config.constraints.get('edge_prob', 0.3)
        directed = self.config.constraints.get('directed', False)
        weighted = self.config.constraints.get('weighted', False)
        min_weight = self.config.constraints.get('min_weight', 1)
        max_weight = self.config.constraints.get('max_weight', 100)
        
        edges = []
        for i in range(1, self.n + 1):
            start = i + 1 if not directed else 1
            for j in range(start, self.n + 1):
                if i != j and random.random() < edge_prob:
                    if weighted:
                        w = random.randint(min_weight, max_weight)
                        edges.append((i, j, w))
                    else:
                        edges.append((i, j))
        
        return self.edges_to_string(edges, weighted)

class TreeGenerator(GraphGenerator):
    """Generate random tree"""
    
    @property
    def name(self) -> str:
        return "tree"
    @property
    def description(self) -> str:
        return "Generate random tree (connected acyclic graph)"
    
    def generate(self) -> str:
        weighted = self.config.constraints.get('weighted', False)
        min_weight = self.config.constraints.get('min_weight', 1)
        max_weight = self.config.constraints.get('max_weight', 100)
        
        edges = []
        
        # Use Prüfer sequence or simpler approach
        for i in range(2, self.n + 1):
            parent = random.randint(1, i - 1)
            if weighted:
                w = random.randint(min_weight, max_weight)
                edges.append((parent, i, w))
            else:
                edges.append((parent, i))
        
        # Shuffle node labels for randomness
        permutation = list(range(1, self.n + 1))
        random.shuffle(permutation)
        
        # Remap edges
        remapped_edges = []
        for edge in edges:
            if weighted:
                u, v, w = edge
                remapped_edges.append((permutation[u-1], permutation[v-1], w))
            else:
                u, v = edge
                remapped_edges.append((permutation[u-1], permutation[v-1]))
        
        return self.edges_to_string(remapped_edges, weighted)

class DAGGenerator(GraphGenerator):
    """Generate Directed Acyclic Graph"""
    
    @property
    def name(self) -> str:
        return "dag"
    @property
    def description(self) -> str:
        return "Generate random DAG"
    
    def generate(self) -> str:
        edge_prob = self.config.constraints.get('edge_prob', 0.3)
        weighted = self.config.constraints.get('weighted', False)
        min_weight = self.config.constraints.get('min_weight', 1)
        max_weight = self.config.constraints.get('max_weight', 100)
        
        # Generate topological order
        nodes = list(range(1, self.n + 1))
        random.shuffle(nodes)
        
        edges = []
        for i in range(self.n):
            for j in range(i + 1, self.n):
                if random.random() < edge_prob:
                    if weighted:
                        w = random.randint(min_weight, max_weight)
                        edges.append((nodes[i], nodes[j], w))
                    else:
                        edges.append((nodes[i], nodes[j]))
        
        return self.edges_to_string(edges, weighted)

class BipartiteGraphGenerator(GraphGenerator):
    """Generate bipartite graph"""
    
    @property
    def name(self) -> str:
        return "bipartite"
    @property
    def description(self) -> str:
        return "Generate random bipartite graph"
    
    def generate(self) -> str:
        left_size = self.config.constraints.get('left_size', self.n // 2)
        right_size = self.n - left_size
        edge_prob = self.config.constraints.get('edge_prob', 0.3)
        weighted = self.config.constraints.get('weighted', False)
        min_weight = self.config.constraints.get('min_weight', 1)
        max_weight = self.config.constraints.get('max_weight', 100)
        
        edges = []
        for i in range(1, left_size + 1):
            for j in range(left_size + 1, self.n + 1):
                if random.random() < edge_prob:
                    if weighted:
                        w = random.randint(min_weight, max_weight)
                        edges.append((i, j, w))
                    else:
                        edges.append((i, j))
        
        return self.edges_to_string(edges, weighted)

# =========================================================================== #
# ------------------------- Query-based Generators -------------------------- #
# =========================================================================== #

class RangeQueryGenerator(TestGenerator):
    """Generate array with range queries"""
    
    @property
    def name(self) -> str:
        return "range_query"
    @property
    def description(self) -> str:
        return "Generate array followed by range queries"
    
    def generate(self) -> str:
        array_size = self.n
        num_queries = self.config.constraints.get('num_queries', self.n)
        min_val = self.config.constraints.get('min_val', 1)
        max_val = self.config.constraints.get('max_val', 10**9)
        
        result = [f"{array_size} {num_queries}"]
        
        # Generate array
        arr = [str(random.randint(min_val, max_val)) for _ in range(array_size)]
        result.append(' '.join(arr))
        
        # Generate queries
        for _ in range(num_queries):
            l = random.randint(1, array_size)
            r = random.randint(l, array_size)
            result.append(f"{l} {r}")
        
        return '\n'.join(result)

class UpdateQueryGenerator(TestGenerator):
    """Generate array with update and query operations"""
    
    @property
    def name(self) -> str:
        return "update_query"
    @property
    def description(self) -> str:
        return "Generate mixed update and query operations"
    
    def generate(self) -> str:
        array_size = self.n
        num_ops = self.config.constraints.get('num_ops', self.n)
        update_prob = self.config.constraints.get('update_prob', 0.5)
        min_val = self.config.constraints.get('min_val', 1)
        max_val = self.config.constraints.get('max_val', 10**9)
        
        result = [f"{array_size} {num_ops}"]
        
        # Generate initial array
        arr = [str(random.randint(min_val, max_val)) for _ in range(array_size)]
        result.append(' '.join(arr))
        
        # Generate operations
        for _ in range(num_ops):
            if random.random() < update_prob:
                # Update operation
                pos = random.randint(1, array_size)
                val = random.randint(min_val, max_val)
                result.append(f"U {pos} {val}")
            else:
                # Query operation
                l = random.randint(1, array_size)
                r = random.randint(l, array_size)
                result.append(f"Q {l} {r}")
        
        return '\n'.join(result)

# =========================================================================== #
# --------------------------- Geometry Generators --------------------------- #
# =========================================================================== #

class PointsGenerator(TestGenerator):
    """Generate 2D points"""
    
    @property
    def name(self) -> str:
        return "points"
    @property
    def description(self) -> str:
        return "Generate random 2D points"
    
    def generate(self) -> str:
        min_coord = self.config.constraints.get('min_coord', -10**9)
        max_coord = self.config.constraints.get('max_coord', 10**9)
        unique = self.config.constraints.get('unique', False)
        
        result = [str(self.n)]
        
        if unique:
            points = set()
            while len(points) < self.n:
                x = random.randint(min_coord, max_coord)
                y = random.randint(min_coord, max_coord)
                points.add((x, y))
            
            for x, y in points:
                result.append(f"{x} {y}")
        else:
            for _ in range(self.n):
                x = random.randint(min_coord, max_coord)
                y = random.randint(min_coord, max_coord)
                result.append(f"{x} {y}")
        
        return '\n'.join(result)

class ConvexPolygonGenerator(TestGenerator):
    """Generate points forming a convex polygon"""
    
    @property
    def name(self) -> str:
        return "convex_polygon"
    @property
    def description(self) -> str:
        return "Generate points forming a convex polygon"
    
    def generate(self) -> str:
        # Generate points on a circle and perturb slightly
        points = []
        for i in range(self.n):
            angle = 2 * math.pi * i / self.n
            r = 10000 + random.randint(-100, 100)
            x = int(r * math.cos(angle))
            y = int(r * math.sin(angle))
            points.append((x, y))
        
        result = [str(self.n)]
        for x, y in points:
            result.append(f"{x} {y}")
        
        return '\n'.join(result)

# =========================================================================== #
# ----------------------- Special Pattern Generators ------------------------ #
# =========================================================================== #

class GridPathGenerator(TestGenerator):
    """Generate grid with path finding scenario"""
    
    @property
    def name(self) -> str:
        return "grid_path"
    @property
    def description(self) -> str:
        return "Generate n×m grid for pathfinding"
    
    def generate(self) -> str:
        rows = int(math.sqrt(self.n))
        cols = rows
        obstacle_prob = self.config.constraints.get('obstacle_prob', 0.2)
        
        # Ensure start and end are not blocked
        grid = []
        for i in range(rows):
            row = []
            for j in range(cols):
                if (i == 0 and j == 0) or (i == rows-1 and j == cols-1):
                    row.append('.')
                elif random.random() < obstacle_prob:
                    row.append('#')
                else:
                    row.append('.')
            grid.append(''.join(row))
        
        result = [f"{rows} {cols}"]
        result.extend(grid)
        return '\n'.join(result)

class IntervalGenerator(TestGenerator):
    """Generate intervals for scheduling problems"""
    
    @property
    def name(self) -> str:
        return "intervals"
    @property
    def description(self) -> str:
        return "Generate intervals with start and end times"
    
    def generate(self) -> str:
        max_time = self.config.constraints.get('max_time', 10**6)
        
        intervals = []
        for _ in range(self.n):
            start = random.randint(0, max_time - 1)
            end = random.randint(start + 1, max_time)
            intervals.append((start, end))
        
        result = [str(self.n)]
        for start, end in intervals:
            result.append(f"{start} {end}")
        
        return '\n'.join(result)

# =========================================================================== #
# --------------------------- Generator Registry ---------------------------- #
# =========================================================================== #

class GeneratorRegistry:
    """Registry of all available generators"""
    
    def __init__(self):
        self.generators: Dict[str, type] = {}
        self._register_builtin_generators()
    
    def _register_builtin_generators(self):
        """Register all built-in generators"""
        generator_classes = [
            # Arrays
            ArrayGenerator, SortedArrayGenerator, PermutationGenerator, MatrixGenerator,
            # Strings
            StringGenerator, PalindromeGenerator, BinaryStringGenerator,
            # Graphs
            RandomGraphGenerator, TreeGenerator, DAGGenerator, BipartiteGraphGenerator,
            # Queries
            RangeQueryGenerator, UpdateQueryGenerator,
            # Geometry
            PointsGenerator, ConvexPolygonGenerator,
            # Special
            GridPathGenerator, IntervalGenerator
        ]
        
        for gen_class in generator_classes:
            self.register(gen_class.name, gen_class)
    
    def register(self, name: str, generator_class: type):
        """Register a new generator"""
        self.generators[name] = generator_class
    
    def get(self, name: str) -> Optional[type]:
        """Get generator class by name"""
        return self.generators.get(name)
    
    def list_generators(self) -> List[Tuple[str, str]]:
        """List all available generators with descriptions"""
        result = []
        for name, gen_class in sorted(self.generators.items()):
            result.append((name, gen_class.description))
        return result

# =========================================================================== #
# ------------------------------ CLI Interface ------------------------------ #
# =========================================================================== #

def main():
    parser = argparse.ArgumentParser(
        description='Advanced Test Case Generator for Competitive Programming',
        formatter_class=argparse.RawDescriptionHelpFormatter
    )
    
    subparsers = parser.add_subparsers(dest='command', help='Commands')
    
    # List command
    list_parser = subparsers.add_parser('list', help='List available generators')
    
    # Generate command
    gen_parser = subparsers.add_parser('generate', help='Generate test case')
    gen_parser.add_argument('generator', help='Generator type')
    gen_parser.add_argument('--size', choices=['tiny', 'small', 'medium', 'large', 'stress', 'custom'],
                           default='small', help='Test size')
    gen_parser.add_argument('--custom-size', type=int, help='Custom size value')
    gen_parser.add_argument('--seed', type=int, help='Random seed for reproducibility')
    gen_parser.add_argument('--output', '-o', help='Output file')
    gen_parser.add_argument('--constraints', '-c', type=json.loads, 
                           help='JSON string with constraints')
    
    # Batch command
    batch_parser = subparsers.add_parser('batch', help='Generate multiple test cases')
    batch_parser.add_argument('generator', help='Generator type')
    batch_parser.add_argument('--prefix', default='test', help='Output file prefix')
    batch_parser.add_argument('--count', type=int, default=10, help='Number of test cases')
    batch_parser.add_argument('--constraints', '-c', type=json.loads,
                             help='JSON string with constraints')
    
    args = parser.parse_args()
    
    registry = GeneratorRegistry()
    
    if args.command == 'list':
        print("Available Test Generators:")
        print("-" * 60)
        for name, desc in registry.list_generators():
            print(f"  {name:<20} - {desc}")
    
    elif args.command == 'generate':
        gen_class = registry.get(args.generator)
        if not gen_class:
            print(f"Error: Unknown generator '{args.generator}'")
            print("Run 'testgen list' to see available generators")
            sys.exit(1)
        
        size_enum = TestSize[args.size.upper()]
        config = TestConfig(
            size=size_enum,
            custom_size=args.custom_size,
            seed=args.seed,
            constraints=args.constraints or {}
        )
        
        generator = gen_class(config)
        output = generator.generate()
        
        if args.output:
            with open(args.output, 'w') as f:
                f.write(output)
            print(f"Test case written to {args.output}")
        else:
            print(output)
    
    elif args.command == 'batch':
        gen_class = registry.get(args.generator)
        if not gen_class:
            print(f"Error: Unknown generator '{args.generator}'")
            sys.exit(1)
        
        for i in range(args.count):
            sizes = [TestSize.TINY, TestSize.SMALL, TestSize.MEDIUM, TestSize.LARGE]
            size = random.choice(sizes)
            
            config = TestConfig(
                size=size,
                seed=random.randint(1, 10**9),
                constraints=args.constraints or {}
            )
            
            generator = gen_class(config)
            output = generator.generate()
            
            filename = f"{args.prefix}_{i:03d}.in"
            with open(filename, 'w') as f:
                f.write(output)
            
            print(f"Generated {filename} (size: {size.value})")
    
    else:
        parser.print_help()

if __name__ == '__main__':
    main()

# =========================================================================== #
# ----------------------------- End of TestGen.py --------------------------- #