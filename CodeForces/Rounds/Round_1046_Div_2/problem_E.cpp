//===----------------------------------------------------------------------===//
/**
 * @file: problem_E.cpp
 * @brief Codeforces Round 1046 (Div. 2) - Problem E
 * @author: Costantino Lombardi
 *
 * @status: PASSED
 */
//===----------------------------------------------------------------------===//
/* Included library */

// clang-format off
// Compiler optimizations:
#if defined(__GNUC__) && !defined(__clang__)
  #pragma GCC optimize("Ofast,unroll-loops,fast-math,O3")
  // Apple Silicon optimizations:
  #ifdef __aarch64__
    #pragma GCC target("+simd")
  #endif
#endif

#ifdef __clang__
  #pragma clang optimize on
#endif

// Sanitaze macro:
#ifdef USE_CLANG_SANITIZE
  #include "PCH.h"
#else
  #include <bits/stdc++.h>
#endif

// Debug macro:
#ifdef LOCAL
  #include "debug.h"
#else
  #define debug(...) 42
#endif
// clang-format on

//===----------------------------------------------------------------------===//
/* Type Aliases and Constants */

// Type aliases
using ll     = long long;
using ull    = unsigned long long;
using ld     = long double;
using pii    = std::pair<int, int>;
using pll    = std::pair<long long, long long>;
using vi     = std::vector<int>;
using vui    = std::vector<unsigned int>;
using vll    = std::vector<long long>;
using vvi    = std::vector<std::vector<int>>;
using vvll   = std::vector<std::vector<long long>>;
using vs     = std::vector<std::string>;
using vpii   = std::vector<std::pair<int, int>>;
using vpll   = std::vector<std::pair<long long, long long>>;
using map_ll = std::map<long long, long long>;

// Mathematical constants
constexpr long double PI   = 3.141592653589793238462643383279502884L;
constexpr long double E    = 2.718281828459045235360287471352662498L;
constexpr long double EPS  = 1e-9L;
constexpr int         INF  = 0x3f3f3f3f;
constexpr long long   LINF = 0x3f3f3f3f3f3f3f3fLL;
constexpr int         LIM  = 1000000 + 5;
constexpr int         MOD  = 1000000007;
constexpr int         MOD2 = 998244353;

using namespace std;

//===----------------------------------------------------------------------===//
/* Data Types and Function Definitions */

// Graph utilities namespace.
namespace GraphComponents {
  // Function to compute (base^exponent) % modulus using binary exponentiation.
  template <typename T>
  T modularExponentiation(T base, ll exponent, T modulus) {
    T result = 1 % modulus;
    base %= modulus;
    while (exponent) {
      if (exponent & 1)
        result = static_cast<T>((1LL * result * base) % modulus);
      base = static_cast<T>((1LL * base * base) % modulus);
      exponent >>= 1;
    }
    return result;
  }

  // Disjoint Set Union (Union-Find) data structure.
  class SetUnion {
    mutable vi representative;
    vi         depth;

  public:
    explicit SetUnion(int size) : representative(size), depth(size, 1) { std::iota(representative.begin(), representative.end(), 0); }

    int getRepresentative(int element) const {
      if (representative[element] != element) {
        representative[element] = getRepresentative(representative[element]);
      }
      return representative[element];
    }

    bool joinSets(int first, int second) {
      first  = getRepresentative(first);
      second = getRepresentative(second);
      if (first == second)
        return false;

      if (depth[first] > depth[second]) {
        representative[second] = first;
        depth[first] += depth[second];
      } else {
        representative[first] = second;
        depth[second] += depth[first];
      }
      return true;
    }

    bool areConnected(int first, int second) const { return getRepresentative(first) == getRepresentative(second); }
  };

  // Class to analyze the network and find biconnected components.
  struct NetworkAnalyzer {
    int                 nodeCount;
    vector<vector<pii>> connections;
    vi                  visitTime, minReachTime;
    int                 currentTime{0};
    vector<pii>         edgeBuffer;
    vector<vpii>        biconnectedGroups;

    explicit NetworkAnalyzer(int nodes) : nodeCount(nodes), connections(nodes + 1), visitTime(nodes + 1, -1), minReachTime(nodes + 1, -1) {}

    // Insert a bidirectional connection between two nodes.
    void insertConnection(int source, int target, int index) {
      connections[source].emplace_back(target, index);
      connections[target].emplace_back(source, index);
    }

    // Depth-First Search to explore and identify biconnected components.
    void exploreBiconnected(int node, int parentEdge) {
      visitTime[node] = minReachTime[node] = currentTime++;
      // Explore all adjacent nodes.
      for (const auto& [neighbor, edgeId] : connections[node]) {
        if (visitTime[neighbor] == -1) {
          edgeBuffer.emplace_back(node, neighbor);
          exploreBiconnected(neighbor, edgeId);
          minReachTime[node] = min(minReachTime[node], minReachTime[neighbor]);
          // Check if the current edge is a bridge.
          if (minReachTime[neighbor] >= visitTime[node]) {
            vpii currentGroup;
            pii  lastEdge;
            do {
              if (edgeBuffer.empty())
                break;
              lastEdge = edgeBuffer.back();
              edgeBuffer.pop_back();
              currentGroup.push_back(lastEdge);
            } while (!edgeBuffer.empty() && !(lastEdge.first == node && lastEdge.second == neighbor));

            if (!currentGroup.empty()) {
              biconnectedGroups.push_back(currentGroup);
            }
          }
        } else if (edgeId != parentEdge && visitTime[neighbor] < visitTime[node]) {
          edgeBuffer.emplace_back(node, neighbor);
          minReachTime[node] = min(minReachTime[node], visitTime[neighbor]);
        }
      }
    }
    // Extract all biconnected components from the network.
    vector<vpii> extractBiconnectedGroups() {
      currentTime = 0;
      edgeBuffer.clear();
      biconnectedGroups.clear();
      exploreBiconnected(1, -1);
      return biconnectedGroups;
    }
  };

  // Function to verify if a subgraph is bipartite using BFS.
  bool verifyBipartiteness(const vi& nodeList, const vpii& edgeList) {
    if (nodeList.empty())
      return true;

    int                     count = nodeList.size();
    unordered_map<int, int> indexMapping;
    for (int i = 0; i < count; ++i) {
      indexMapping[nodeList[i]] = i;
    }

    vvi localGraph(count);
    for (const auto& [u, v] : edgeList) {
      if (indexMapping.find(u) == indexMapping.end() || indexMapping.find(v) == indexMapping.end())
        continue;
      int idx1 = indexMapping[u];
      int idx2 = indexMapping[v];
      localGraph[idx1].push_back(idx2);
      localGraph[idx2].push_back(idx1);
    }

    vi colorAssignment(count, -1);
    for (int start = 0; start < count; ++start) {
      if (colorAssignment[start] != -1)
        continue;

      queue<int> bfsQueue;
      colorAssignment[start] = 0;
      bfsQueue.push(start);

      while (!bfsQueue.empty()) {
        int current = bfsQueue.front();
        bfsQueue.pop();

        for (int adjacent : localGraph[current]) {
          if (colorAssignment[adjacent] == -1) {
            colorAssignment[adjacent] = 1 - colorAssignment[current];
            bfsQueue.push(adjacent);
          } else if (colorAssignment[adjacent] == colorAssignment[current]) {
            return false;
          }
        }
      }
    }
    return true;
  }
} // namespace GraphComponents

// Function to solve a single test case.
void solve() {
  int numNodes, numEdges;
  ll  valueLimit;
  cin >> numNodes >> numEdges >> valueLimit;

  // Read assigned values for each node (-1 means unassigned).
  vi assignedValues(numNodes + 1);
  for (int i = 1; i <= numNodes; ++i) {
    cin >> assignedValues[i];
  }

  GraphComponents::NetworkAnalyzer network(numNodes);

  // Read edges and build the graph.
  for (int i = 0; i < numEdges; ++i) {
    int nodeA, nodeB;
    cin >> nodeA >> nodeB;
    network.insertConnection(nodeA, nodeB, i);
  }

  // Extract all biconnected components from the graph.
  auto biconnectedSets = network.extractBiconnectedGroups();

  // Initialize union-find structure and tracking arrays.
  GraphComponents::SetUnion unionStructure(numNodes + 1);
  vector<bool>              mustBeZero(numNodes + 1, false);
  vector<bool>              belongsToGroup(numNodes + 1, false);

  // Process each biconnected component.
  for (const auto& componentEdges : biconnectedSets) {
    // Skip trivial components with only one edge.
    if (componentEdges.size() <= 1)
      continue;

    // Collect all nodes involved in this component.
    set<int> involvedNodes;
    for (const auto& [src, dst] : componentEdges) {
      involvedNodes.insert(src);
      involvedNodes.insert(dst);
    }

    vi nodeArray(involvedNodes.begin(), involvedNodes.end());

    // Check if the component is bipartite.
    bool isBipartite = GraphComponents::verifyBipartiteness(nodeArray, componentEdges);

    if (!isBipartite) {
      // Non-bipartite components require all nodes to have value 0.
      for (int node : nodeArray) {
        mustBeZero[node] = true;
      }
    } else {
      // Bipartite components: all nodes must have the same value.
      // Union all nodes in the component.
      for (size_t idx = 1; idx < nodeArray.size(); ++idx) {
        unionStructure.joinSets(nodeArray[0], nodeArray[idx]);
      }
      // Mark all nodes as belonging to a group.
      for (int node : nodeArray) {
        belongsToGroup[node] = true;
      }
    }
  }

  // Check for immediate contradictions with must-be-zero nodes.
  for (int i = 1; i <= numNodes; ++i) {
    if (mustBeZero[i] && assignedValues[i] != -1 && assignedValues[i] != 0) {
      cout << "0\n";
      return;
    }
  }

  // Group nodes by their union-find representative.
  map<int, vector<int>> nodeGroups;
  for (int i = 1; i <= numNodes; ++i) {
    if (belongsToGroup[i]) {
      nodeGroups[unionStructure.getRepresentative(i)].push_back(i);
    }
  }

  ll independentVariables = 0;

  // Process each group of nodes that must have the same value.
  for (const auto& [groupId, groupMembers] : nodeGroups) {
    optional<ll> determinedValue;

    // Check if any node in the group must be zero.
    for (int member : groupMembers) {
      if (mustBeZero[member]) {
        if (!determinedValue.has_value()) {
          determinedValue = 0;
        } else if (determinedValue.value() != 0) {
          // Contradiction: group must be zero but already has different value.
          cout << "0\n";
          return;
        }
      }
    }

    // Check assigned values in the group.
    for (int member : groupMembers) {
      if (assignedValues[member] != -1) {
        if (!determinedValue.has_value()) {
          determinedValue = assignedValues[member];
        } else if (determinedValue.value() != assignedValues[member]) {
          // Contradiction: different assigned values in same group.
          cout << "0\n";
          return;
        }
      }
    }

    // If no value is determined, this group contributes one free variable.
    if (!determinedValue.has_value()) {
      independentVariables++;
    }
  }

  // Handle nodes that don't belong to any biconnected component.
  for (int i = 1; i <= numNodes; ++i) {
    if (belongsToGroup[i])
      continue;

    if (mustBeZero[i]) {
      // Isolated node that must be zero.
      if (assignedValues[i] != -1 && assignedValues[i] != 0) {
        cout << "0\n";
        return;
      }
    } else if (assignedValues[i] == -1) {
      // Unassigned isolated node contributes one free variable.
      independentVariables++;
    }
  }

  // Calculate result: valueLimit^independentVariables mod MOD2.
  ll result = GraphComponents::modularExponentiation<ll>(valueLimit, independentVariables, MOD2);
  cout << result << "\n";
}

//===----------------------------------------------------------------------===//
/* Main function */

int main() {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  cin >> t;
  while (t--) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
