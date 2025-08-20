#!/usr/bin/env python3

# =========================================================================== #
"""
Custom Test Generators - Examples of extending the TestGen framework
"""
# =========================================================================== #

from TestGen import TestGenerator, TestConfig, GeneratorRegistry
import random
import math

# =========================================================================== #
# ------------------ Weighted Tree with Special Properties ------------------ #
# =========================================================================== #

class CentroidTreeGenerator(TestGenerator):
    """Generate a tree with controlled centroid decomposition depth"""
    
    @property
    def name(self) -> str:
        return "centroid_tree"
    @property
    def description(self) -> str:
        return "Generate tree optimized for centroid decomposition problems"
    
    def generate(self) -> str:
        # Build a balanced tree to ensure good centroid properties
        edges = []
        node_id = 1
        
        def build_subtree(size, parent=None):
            nonlocal node_id
            if size <= 0:
                return None
            
            root = node_id
            node_id += 1
            
            if parent is not None:
                weight = random.randint(1, 100)
                edges.append((parent, root, weight))
            
            if size > 1:
                # Split remaining nodes between left and right subtrees
                left_size = size // 2
                right_size = size - 1 - left_size
                
                build_subtree(left_size, root)
                build_subtree(right_size, root)
            
            return root
        
        build_subtree(self.n)
        
        # Shuffle node labels for randomness
        perm = list(range(1, self.n + 1))
        random.shuffle(perm)
        
        # Output format: n, then n-1 edges with weights
        result = [str(self.n)]
        for u, v, w in edges:
            result.append(f"{perm[u-1]} {perm[v-1]} {w}")
        
        return '\n'.join(result)

# =========================================================================== #
# ------------------------- Flow Network Generator -------------------------- #
# =========================================================================== #

class FlowNetworkGenerator(TestGenerator):
    """Generate flow network for max-flow problems"""
    
    @property
    def name(self) -> str:
        return "flow_network"
    @property
    def description(self) -> str:
        return "Generate layered flow network with source and sink"
    
    def generate(self) -> str:
        layers = self.config.constraints.get('layers', 4)
        nodes_per_layer = max(2, self.n // layers)
        
        # Node 1 is source, node n is sink
        edges = []
        node_count = 1
        
        # Create layers
        layer_nodes = [[1]]  # Source
        
        for i in range(layers - 1):
            layer = []
            for _ in range(nodes_per_layer):
                node_count += 1
                if node_count <= self.n - 1:  # Reserve last node for sink
                    layer.append(node_count)
            layer_nodes.append(layer)
        
        layer_nodes.append([self.n])  # Sink
        
        # Connect consecutive layers
        for i in range(len(layer_nodes) - 1):
            for u in layer_nodes[i]:
                # Connect to some nodes in next layer
                next_layer = layer_nodes[i + 1]
                connections = random.randint(1, min(3, len(next_layer)))
                targets = random.sample(next_layer, connections)
                
                for v in targets:
                    capacity = random.randint(1, 1000)
                    edges.append((u, v, capacity))
        
        # Add some backward edges for complexity (but not creating negative cycles)
        backward_edges = min(len(edges) // 4, 10)
        for _ in range(backward_edges):
            layer_idx = random.randint(2, len(layer_nodes) - 2)
            if layer_nodes[layer_idx] and layer_nodes[layer_idx - 1]:
                u = random.choice(layer_nodes[layer_idx])
                v = random.choice(layer_nodes[layer_idx - 1])
                if v != 1:  # Don't create edge to source
                    capacity = random.randint(1, 100)
                    edges.append((u, v, capacity))
        
        # Output format: n m source sink, then edges with capacities
        m = len(edges)
        result = [f"{self.n} {m} 1 {self.n}"]
        for u, v, cap in edges:
            result.append(f"{u} {v} {cap}")
        
        return '\n'.join(result)

# =========================================================================== #
# ---------------------- Segment Tree Query Generator ----------------------- #
# =========================================================================== #

class SegmentTreeQueryGenerator(TestGenerator):
    """Generate queries specifically designed for segment tree testing"""
    
    @property
    def name(self) -> str:
        return "segtree_query"
    @property
    def description(self) -> str:
        return "Generate update/query operations for segment tree problems"
    
    def generate(self) -> str:
        array_size = self.n
        num_queries = self.config.constraints.get('num_queries', self.n)
        
        # Generate initial array
        result = [f"{array_size} {num_queries}"]
        arr = [str(random.randint(1, 10**9)) for _ in range(array_size)]
        result.append(' '.join(arr))
        
        # Generate queries with patterns that stress segment trees
        for i in range(num_queries):
            query_type = random.choices(
                ['point_update', 'range_update', 'point_query', 'range_query'],
                weights=[0.2, 0.2, 0.1, 0.5]
            )[0]
            
            if query_type == 'point_update':
                pos = random.randint(1, array_size)
                val = random.randint(1, 10**9)
                result.append(f"1 {pos} {pos} {val}")
                
            elif query_type == 'range_update':
                l = random.randint(1, array_size)
                r = random.randint(l, min(l + array_size // 4, array_size))
                val = random.randint(1, 10**9)
                result.append(f"1 {l} {r} {val}")
                
            elif query_type == 'point_query':
                pos = random.randint(1, array_size)
                result.append(f"2 {pos} {pos}")
                
            else:  # range_query
                # Bias towards larger ranges to stress the tree
                if random.random() < 0.3:
                    # Large range
                    l = random.randint(1, array_size // 4)
                    r = random.randint(3 * array_size // 4, array_size)
                else:
                    # Random range
                    l = random.randint(1, array_size)
                    r = random.randint(l, array_size)
                result.append(f"2 {l} {r}")
        
        return '\n'.join(result)

# =========================================================================== #
# -------------------- Graph Coloring Problem Generator --------------------- #
# =========================================================================== #

class GraphColoringGenerator(TestGenerator):
    """Generate graphs for coloring problems"""
    
    @property
    def name(self) -> str:
        return "graph_coloring"
    @property
    def description(self) -> str:
        return "Generate graphs with known chromatic number"
    
    def generate(self) -> str:
        graph_type = self.config.constraints.get('type', 'random')
        
        if graph_type == 'bipartite':
            return self._generate_bipartite()
        elif graph_type == 'cycle':
            return self._generate_cycle()
        elif graph_type == 'complete':
            return self._generate_complete()
        else:
            return self._generate_random_colorable()
    
    def _generate_bipartite(self) -> str:
        """Generate bipartite graph (2-colorable)"""
        left = self.n // 2
        right = self.n - left
        edges = []
        
        for i in range(1, left + 1):
            # Connect to random nodes in right partition
            connections = random.randint(1, min(right, 5))
            for j in random.sample(range(left + 1, self.n + 1), connections):
                edges.append((i, j))
        
        m = len(edges)
        result = [f"{self.n} {m}"]
        for u, v in edges:
            result.append(f"{u} {v}")
        return '\n'.join(result)
    
    def _generate_cycle(self) -> str:
        """Generate cycle (2 or 3 colorable based on parity)"""
        edges = []
        for i in range(1, self.n):
            edges.append((i, i + 1))
        edges.append((self.n, 1))
        
        result = [f"{self.n} {self.n}"]
        for u, v in edges:
            result.append(f"{u} {v}")
        return '\n'.join(result)
    
    def _generate_complete(self) -> str:
        """Generate complete graph (n-colorable)"""
        edges = []
        for i in range(1, self.n + 1):
            for j in range(i + 1, self.n + 1):
                edges.append((i, j))
        
        m = len(edges)
        result = [f"{self.n} {m}"]
        for u, v in edges:
            result.append(f"{u} {v}")
        return '\n'.join(result)
    
    def _generate_random_colorable(self) -> str:
        """Generate random graph with controlled chromatic number"""
        k = self.config.constraints.get('colors', 3)
        
        # Assign colors
        colors = [random.randint(1, k) for _ in range(self.n)]
        
        # Add edges between nodes of different colors
        edges = []
        for i in range(self.n):
            for j in range(i + 1, self.n):
                if colors[i] != colors[j] and random.random() < 0.3:
                    edges.append((i + 1, j + 1))
        
        m = len(edges)
        result = [f"{self.n} {m}"]
        for u, v in edges:
            result.append(f"{u} {v}")
        return '\n'.join(result)

# =========================================================================== #
# ------------------- Dynamic Programming Path Generator -------------------- #
# =========================================================================== #

class DPPathGenerator(TestGenerator):
    """Generate grids for DP path problems"""
    
    @property
    def name(self) -> str:
        return "dp_path"
    @property
    def description(self) -> str:
        return "Generate weighted grid for dynamic programming path problems"
    
    def generate(self) -> str:
        rows = int(math.sqrt(self.n))
        cols = self.n // rows
        
        # Generate grid with special patterns
        pattern = self.config.constraints.get('pattern', 'random')
        
        grid = []
        for i in range(rows):
            row = []
            for j in range(cols):
                if pattern == 'increasing':
                    # Values increase as we go right/down
                    val = (i + j) * 10 + random.randint(1, 10)
                elif pattern == 'diagonal':
                    # Diagonal patterns
                    val = abs(i - j) * 100 + random.randint(1, 100)
                elif pattern == 'spiral':
                    # Spiral pattern from center
                    cx, cy = rows // 2, cols // 2
                    dist = abs(i - cx) + abs(j - cy)
                    val = dist * 50 + random.randint(1, 50)
                else:
                    # Random
                    val = random.randint(1, 1000)
                
                row.append(str(val))
            grid.append(' '.join(row))
        
        result = [f"{rows} {cols}"]
        result.extend(grid)
        return '\n'.join(result)

# =========================================================================== #
# -------------------------- Registration Function -------------------------- #
# =========================================================================== #

def register_custom_generators(registry: GeneratorRegistry):
    """Register all custom generators with the main registry"""
    custom_generators = [
        CentroidTreeGenerator,
        FlowNetworkGenerator,
        SegmentTreeQueryGenerator,
        GraphColoringGenerator,
        DPPathGenerator
    ]
    
    for gen_class in custom_generators:
        registry.register(gen_class.name, gen_class)
    
    print(f"Registered {len(custom_generators)} custom generators")

# =========================================================================== #
# ------------------------------ Example Usage ------------------------------ #
# =========================================================================== #

if __name__ == '__main__':
    from TestGen import TestConfig, TestSize
    
    # Example 1: Generate a flow network
    config = TestConfig(
        size=TestSize.SMALL,
        constraints={'layers': 5}
    )
    generator = FlowNetworkGenerator(config)
    print("Flow Network:")
    print(generator.generate())
    print("\n" + "="*50 + "\n")
    
    # Example 2: Generate a graph coloring instance
    config = TestConfig(
        size=TestSize.TINY,
        constraints={'type': 'bipartite'}
    )
    generator = GraphColoringGenerator(config)
    print("Bipartite Graph:")
    print(generator.generate())
    print("\n" + "="*50 + "\n")
    
    # Example 3: Generate segment tree queries
    config = TestConfig(
        size=TestSize.TINY,
        constraints={'num_queries': 15}
    )
    generator = SegmentTreeQueryGenerator(config)
    print("Segment Tree Queries:")
    print(generator.generate())
