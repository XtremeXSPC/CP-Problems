//===---------------------------------------------------------------------===//
/**
 * @brief Codeforces Round 1037 Div. 3
 * @author: Costantino Lombardi
 * @file: C-ver_problem_F
 *
 * @details: Solution implemented in C.
 */
//===---------------------------------------------------------------------===//
/* Included library */

#ifdef LOCAL
#include "algo/debug.h"
#else
#define debug(...) 42
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//===---------------------------------------------------------------------===//
/* Costants & Structure */

const int BLOCK_SIZE      = 450; // Threshold for decomposition
const int HASH_TABLE_SIZE = 503; // A prime size for the hash table

// ----- Definition of Data Structures ----- //

// Node for adjacency list
typedef struct AdjNode {
  int             neighbor;
  int             cost;
  struct AdjNode* next;
} AdjNode;

// Node for the hash table (used for the map 'S')
typedef struct HashNode {
  int              key_color;
  long long        value_sum;
  struct HashNode* next;
} HashNode;

// The hash table itself (an array of linked lists)
typedef struct HashTable {
  HashNode** buckets;
} HashTable;

//===---------------------------------------------------------------------===//
/* Function Definitions */

// ----- Functions for Hash Table Management ----- //

// Creates and initializes a new hash table
HashTable* create_hash_table() {
  HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
  ht->buckets   = (HashNode**)calloc(HASH_TABLE_SIZE, sizeof(HashNode*));
  return ht;
}

// Simple hash function
unsigned int hash_function(int key) {
  return (unsigned int)key % HASH_TABLE_SIZE;
}

// Gets the value associated with a key. Returns 0 if the key does not exist.
long long hash_get(HashTable* ht, int key) {
  if (!ht)
    return 0;
  unsigned int index = hash_function(key);
  for (HashNode* current = ht->buckets[index]; current; current = current->next) {
    if (current->key_color == key)
      return current->value_sum;
  }
  return 0;
}

// Updates the value of a key by adding 'delta'.
// If the key does not exist, it is created.
void hash_update_arena(HashTable* ht, int key, long long delta, HashNode* arena, int* arena_idx) {
  if (!ht)
    return;
  unsigned int index = hash_function(key);

  // Checks if the key already exists
  HashNode* current = ht->buckets[index];
  while (current) {
    if (current->key_color == key) {
      current->value_sum += delta;
      return;
    }
    current = current->next;
  }

  // If it does not exist, creates a new node and adds it to the head of the list
  HashNode* newNode  = &arena[(*arena_idx)++];
  newNode->key_color = key;
  newNode->value_sum = delta;
  newNode->next      = ht->buckets[index];
  ht->buckets[index] = newNode;
}

/* Frees all memory used by a hash table
void free_hash_table(HashTable* ht) {
  if (!ht)
    return;
  for (int i = 0; i < HASH_TABLE_SIZE; ++i) {
    HashNode* current = ht->buckets[i];
    while (current) {
      HashNode* to_free = current;
      current           = current->next;
      free(to_free);
    }
  }
  free(ht->buckets);
  free(ht);
}
*/

// ----- Utility functions for the graph ----- //

// Adds an edge to the adjacency list
void add_edge_arena(AdjNode** adj, int u, int v, int c, AdjNode* arena, int* arena_idx) {
  AdjNode* newNode  = &arena[(*arena_idx)++];
  newNode->neighbor = v;
  newNode->cost     = c;
  newNode->next     = adj[u];
  adj[u]            = newNode;
}

// Function to solve a test case
void solve() {
  int n, q;
  scanf("%d %d", &n, &q);

  int* colors = (int*)malloc((n + 1) * sizeof(int));
  for (int i = 1; i <= n; ++i) {
    scanf("%d", &colors[i]);
  }

  // Data structures for the graph and nodes
  AdjNode**   adj      = (AdjNode**)calloc(n + 1, sizeof(AdjNode*));
  int*        degree   = (int*)calloc(n + 1, sizeof(int));
  bool*       is_heavy = (bool*)calloc(n + 1, sizeof(bool));
  HashTable** S        = (HashTable**)calloc(n + 1, sizeof(HashTable*));

  // Structure to store edges for initial calculation
  typedef struct {
    int u, v, c;
  } Edge;
  Edge* edges = (Edge*)malloc((n - 1) * sizeof(Edge));

  // Memory allocation area
  AdjNode* adj_arena     = (AdjNode*)malloc(2 * (n - 1) * sizeof(AdjNode));
  int      adj_arena_idx = 0;

  // Generous estimate for hash nodes. A value between N and 2*N is often sufficient
  long long hash_nodes_needed = (n > 1) ? 2LL * n : 1;
  HashNode* hash_arena        = (HashNode*)malloc(hash_nodes_needed * sizeof(HashNode));
  int       hash_arena_idx    = 0;

  for (int i = 0; i < n - 1; ++i) {
    int u, v, c;
    scanf("%d %d %d", &u, &v, &c);
    add_edge_arena(adj, u, v, c, adj_arena, &adj_arena_idx);
    add_edge_arena(adj, v, u, c, adj_arena, &adj_arena_idx);
    degree[u]++;
    degree[v]++;
    edges[i].u = u;
    edges[i].v = v;
    edges[i].c = c;
  }

  // Identifies heavy nodes
  for (int i = 1; i <= n; ++i) {
    if (degree[i] > BLOCK_SIZE) {
      is_heavy[i] = true;
    }
  }

  long long total_cost = 0;

  // Initial calculation of total cost and maps S for heavy nodes
  for (int i = 0; i < n - 1; ++i) {
    int u = edges[i].u;
    int v = edges[i].v;
    int c = edges[i].c;

    if (colors[u] != colors[v]) {
      total_cost += c;
    }
    if (is_heavy[u]) {
      if (S[u] == NULL)
        S[u] = create_hash_table();
      hash_update_arena(S[u], colors[v], c, hash_arena, &hash_arena_idx);
    }
    if (is_heavy[v]) {
      if (S[v] == NULL)
        S[v] = create_hash_table();
      hash_update_arena(S[v], colors[u], c, hash_arena, &hash_arena_idx);
    }
  }

  // Processes the queries
  for (int k = 0; k < q; ++k) {
    int v_q, new_color;
    scanf("%d %d", &v_q, &new_color);

    int old_color = colors[v_q];
    if (old_color == new_color) {
      printf("%lld\n", total_cost);
      continue;
    }

    // Calculates the cost variation
    if (is_heavy[v_q]) {
      total_cost += hash_get(S[v_q], old_color) - hash_get(S[v_q], new_color);
    } else {
      for (AdjNode* current = adj[v_q]; current; current = current->next) {
        if (colors[current->neighbor] == old_color)
          total_cost += current->cost;
        if (colors[current->neighbor] == new_color)
          total_cost -= current->cost;
      }
    }

    // Propagates the color change to heavy neighbors
    for (AdjNode* current = adj[v_q]; current; current = current->next) {
      if (is_heavy[current->neighbor]) {
        hash_update_arena(S[current->neighbor], old_color, -current->cost, hash_arena, &hash_arena_idx);
        hash_update_arena(S[current->neighbor], new_color, current->cost, hash_arena, &hash_arena_idx);
      }
    }

    colors[v_q] = new_color;
    printf("%lld\n", total_cost);
  }

  // Memory cleanup
  free(colors);
  free(degree);
  free(is_heavy);
  free(edges);
  for (int i = 1; i <= n; ++i) {
    if (S[i] != NULL) {
      free(S[i]->buckets);
      free(S[i]);
    }
  }
  free(adj);
  free(S);

  free(adj_arena);
  free(hash_arena);
}

//===---------------------------------------------------------------------===//
/* Main function */

int main() {
  int t;
  scanf("%d", &t);
  while (t--) {
    solve();
  }
  return 0;
}

//===---------------------------------------------------------------------===//