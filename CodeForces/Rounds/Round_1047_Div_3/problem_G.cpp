//===----------------------------------------------------------------------===//
/**
 * @file: problem_G.cpp
 * @brief Codeforces Round 1047 Div. 3 - Problem G 
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

// Type aliases:
using ll       = long long;
using ull      = unsigned long long;
using ld       = long double;
using P_ii     = std::pair<int, int>;
using P_ll     = std::pair<long long, long long>;
using V_b      = std::vector<bool>;
using V_i      = std::vector<int>;
using V_ui     = std::vector<unsigned int>;
using V_ll     = std::vector<long long>;
using VV_i     = std::vector<std::vector<int>>;
using VV_ll    = std::vector<std::vector<long long>>;
using V_s      = std::vector<std::string>;
using VV_s     = std::vector<std::vector<std::string>>;
using VP_ii    = std::vector<std::pair<int, int>>;
using VVP_ii   = std::vector<std::vector<std::pair<int, int>>>;
using VP_ll    = std::vector<std::pair<long long, long long>>;
using VT_iii   = std::vector<std::tuple<int, int, int>>;
using Map_ll   = std::map<long long, long long>;
using VUMap_il = std::vector<std::unordered_map<int, ll>>;

// Mathematical constants:
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

// Graph structure to represent the game.
struct GameGraph {
    int nodeCount;
    VV_i adjacencyList;
    VV_i reverseAdjList;
    V_i outDegree;
    V_b isRedNode;
    V_b isTerminalNode;
    
    // Game state arrays.
    V_b canWinBasic;
    V_b canWinAdvanced;
    
    // Counters for decision making.
    V_i terminalChildCount;  
    V_i winningChildCount;
    V_i losingChildCount;
    V_b hasRedDescendant;

    GameGraph(int n) : nodeCount(n) {
        adjacencyList.resize(n + 1);
        reverseAdjList.resize(n + 1);
        outDegree.assign(n + 1, 0);
        isRedNode.assign(n + 1, false);
        isTerminalNode.assign(n + 1, false);
        canWinBasic.assign(n + 1, true);
        canWinAdvanced.assign(n + 1, true);
        terminalChildCount.assign(n + 1, 0);
        winningChildCount.assign(n + 1, 0);
        losingChildCount.assign(n + 1, 0);
        hasRedDescendant.assign(n + 1, false);
    }
    
    // Add directed edge to the graph.
    void addEdge(int from, int to) {
        adjacencyList[from].push_back(to);
        reverseAdjList[to].push_back(from);
        outDegree[from]++;
    }
    
    // Initialize game states.
    void initialize() {
        // Mark terminal nodes.
        for (int node = 1; node <= nodeCount; ++node) {
            isTerminalNode[node] = (outDegree[node] == 0);
        }
        
        // Count initial child states.
        for (int node = 1; node <= nodeCount; ++node) {
            for (int child : adjacencyList[node]) {
                if (isTerminalNode[child]) {
                    terminalChildCount[node]++;
                } else {
                    winningChildCount[node]++;
                }
            }
        }
    }
    
    // Propagate basic win state changes.
    template<typename Queue>
    void enqueueBasicUpdate(int node, Queue& queue, V_b& inQueue) {
        if (!canWinBasic[node] || inQueue[node]) return;
        canWinBasic[node] = false;
        inQueue[node] = true;
        queue.push_back(node);
    }
    
    // Propagate advanced win state changes.
    template<typename Queue>
    void enqueueAdvancedUpdate(int node, Queue& queue, V_b& inQueue) {
        if (!canWinAdvanced[node] || inQueue[node]) return;
        canWinAdvanced[node] = false;
        inQueue[node] = true;
        queue.push_back(node);
    }
    
    // Mark a node as red and update game states.
    void markNodeRed(int targetNode) {
        if (isRedNode[targetNode]) return;
        
        bool previousBasicWin = canWinBasic[targetNode];
        bool previousAdvancedWin = canWinAdvanced[targetNode];
        
        isRedNode[targetNode] = true;
        canWinBasic[targetNode] = false;
        canWinAdvanced[targetNode] = false;
        
        deque<int> basicQueue, advancedQueue;
        V_b inBasicQueue(nodeCount + 1, false);
        V_b inAdvancedQueue(nodeCount + 1, false);
        
        // Update parent nodes.
        for (int parent : reverseAdjList[targetNode]) {
            if (isTerminalNode[targetNode]) {
                terminalChildCount[parent]--;
            } else {
                if (previousAdvancedWin) winningChildCount[parent]--;
                if (!previousBasicWin) losingChildCount[parent]--;
            }
            
            if (!hasRedDescendant[parent]) {
                hasRedDescendant[parent] = true;
                if (canWinAdvanced[parent]) {
                    enqueueAdvancedUpdate(parent, advancedQueue, inAdvancedQueue);
                }
            }
            
            bool newBasicWin = (terminalChildCount[parent] > 0) || 
                               (winningChildCount[parent] > 0);
            if (canWinBasic[parent] && !newBasicWin) {
                enqueueBasicUpdate(parent, basicQueue, inBasicQueue);
            }
        }
        
        // Propagate changes.
        while (!advancedQueue.empty() || !basicQueue.empty()) {
            // Process advanced queue.
            while (!advancedQueue.empty()) {
                int current = advancedQueue.front();
                advancedQueue.pop_front();
                inAdvancedQueue[current] = false;
                
                for (int parent : reverseAdjList[current]) {
                    if (isRedNode[current] || isTerminalNode[current]) continue;
                    
                    if (winningChildCount[parent] > 0) {
                        winningChildCount[parent]--;
                    }
                    
                    bool newBasicWin = (terminalChildCount[parent] > 0) || 
                                       (winningChildCount[parent] > 0);
                    if (canWinBasic[parent] && !newBasicWin) {
                        enqueueBasicUpdate(parent, basicQueue, inBasicQueue);
                    }
                }
            }
            
            // Process basic queue.
            while (!basicQueue.empty()) {
                int current = basicQueue.front();
                basicQueue.pop_front();
                inBasicQueue[current] = false;
                
                for (int parent : reverseAdjList[current]) {
                    if (isRedNode[current] || isTerminalNode[current]) continue;
                    
                    losingChildCount[parent]++;
                    
                    if (!hasRedDescendant[parent] && canWinAdvanced[parent] && 
                        losingChildCount[parent] > 0) {
                        enqueueAdvancedUpdate(parent, advancedQueue, inAdvancedQueue);
                    }
                }
            }
        }
    }
    
    bool canCryWin(int startNode) const {
        return canWinBasic[startNode];
    }
};

// Function to solve each test case.
void solve() {
    int numNodes, numEdges, numQueries;
    cin >> numNodes >> numEdges >> numQueries;
    
    GameGraph game(numNodes);
    
    // Read edges.
    for (int i = 0; i < numEdges; ++i) {
        int from, to;
        cin >> from >> to;
        game.addEdge(from, to);
    }
    
    game.initialize();
    
    // Process queries.
    for (int queryIdx = 0; queryIdx < numQueries; ++queryIdx) {
        int queryType, nodeId;
        cin >> queryType >> nodeId;
        
        if (queryType == 1) {
            // Mark node as red.
            game.markNodeRed(nodeId);
        } else {
            // Check if Cry wins.
            cout << (game.canCryWin(nodeId) ? "YES" : "NO") << '\n';
        }
    }
}

//===----------------------------------------------------------------------===//
/* Main function */

auto main() -> int {
  // Fast I/O
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int T = 1;
  cin >> T;
  for ([[maybe_unused]] auto _ : views::iota(0, T)) {
    solve();
  }

  return 0;
}

//===----------------------------------------------------------------------===//
