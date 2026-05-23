#ifndef CP_TEMPLATE_PROFILE_STRICT
  #define CP_TEMPLATE_PROFILE_STRICT
#endif
#ifndef CP_USE_GLOBAL_STD_NAMESPACE
  #define CP_USE_GLOBAL_STD_NAMESPACE 1
#endif

#define NEED_MACROS
#define NEED_TIMER
#define NEED_CONTAINERS
#define CP_IO_PROFILE_SIMPLE

#include "templates/Base.hpp"

//===----------------------------------------------------------------------===//
/* Main Solver Function */

const int MAXN = 200005;

I32 CD[MAXN][20];
I32 CA[MAXN][20];
I32 DW[MAXN];
I32 UP[MAXN];
I32 PR[MAXN];
VecI32 ADJ[MAXN];

I32 head[MAXN][20];
I32 prev_sib[MAXN];
I32 next_sib[MAXN];

bool in_qd[MAXN];
bool in_qc[MAXN];

I32 max_def_up[MAXN];
I32 max_spec_up[MAXN];
void add_child_list(I32 p, I32 c, I32 val) {
    next_sib[c] = head[p][val];
    if (head[p][val] != 0) {
        prev_sib[head[p][val]] = c;
    }
    head[p][val] = c;
    prev_sib[c] = 0;
}

void remove_child_list(I32 p, I32 c, I32 val) {
    I32 nxt = next_sib[c];
    I32 prv = prev_sib[c];
    if (prv != 0) next_sib[prv] = nxt;
    else head[p][val] = nxt;
    if (nxt != 0) prev_sib[nxt] = prv;
}

I32 smax(const I32* cnt) {
    I32 fnd = 0;
    for (I32 k = 19; k >= 1; --k) {
        if (cnt[k] >= 2) {
            if (fnd == 0) return k;
            else return k;
        } else if (cnt[k] == 1) {
            if (fnd == 1) return k;
            fnd = 1;
        }
    }
    return 0;
}

void solve() {
    INT(q);
    VecI32 V(q);
    FOR(i, q) IN(V[i]);

    DW[1] = 1;
    UP[1] = 0;
    I32 mx_s = 1;

    Queue<I32> qd, qc;

    VecI32 ans;

    FOR(i, q) {
        I32 p = V[i];
        I32 u = i + 2;

        PR[u] = p;
        ADJ[p].push_back(u);

        DW[u] = 1;
        CD[p][1]++;
        CA[p][1]++;
        add_child_list(p, u, 1);

        CA[p][1]--;
        I32 nu = smax(CA[p]) + 1;
        CA[p][1]++;

        UP[u] = nu;
        CA[u][nu]++;
        if (!in_qc[u]) { in_qc[u] = true; qc.push(u); }

        if (!in_qd[p]) { in_qd[p] = true; qd.push(p); }
        if (!in_qc[p]) { in_qc[p] = true; qc.push(p); }

        while (!qd.empty() || !qc.empty()) {
            if (!qd.empty()) {
                I32 c = qd.front(); qd.pop();
                in_qd[c] = false;

                I32 nd = smax(CD[c]) + 1;
                if (nd > DW[c]) {
                    I32 od = DW[c];
                    DW[c] = nd;
                    if (c != 1) {
                        I32 p_node = PR[c];
                        CD[p_node][od]--; CA[p_node][od]--;
                        CD[p_node][nd]++; CA[p_node][nd]++;
                        
                        remove_child_list(p_node, c, od);
                        add_child_list(p_node, c, nd);

                        if (!in_qd[p_node]) { in_qd[p_node] = true; qd.push(p_node); }
                        if (!in_qc[p_node]) { in_qc[p_node] = true; qc.push(p_node); }
                    }
                }
            } else if (!qc.empty()) {
                I32 c = qc.front(); qc.pop();
                in_qc[c] = false;

                I32 v1 = 0, v2 = 0, v3 = 0;
                I32 count = 0;
                for (I32 k = 19; k >= 1 && count < 3; --k) {
                    for (I32 j = 0; j < CA[c][k] && count < 3; ++j) {
                        if (count == 0) v1 = k;
                        else if (count == 1) v2 = k;
                        else if (count == 2) v3 = k;
                        count++;
                    }
                }

                I32 d_nu = v2 + 1;
                I32 s_nu = v3 + 1;

                if (d_nu > max_def_up[c]) {
                    max_def_up[c] = d_nu;
                    max_spec_up[c] = s_nu;
                    for (I32 nx : ADJ[c]) {
                        I32 nx_nu = (DW[nx] >= v2) ? s_nu : d_nu;
                        if (nx_nu > UP[nx]) {
                            I32 ou = UP[nx];
                            UP[nx] = nx_nu;
                            CA[nx][ou]--; CA[nx][nx_nu]++;
                            if (!in_qc[nx]) { in_qc[nx] = true; qc.push(nx); }
                        }
                    }
                } else if (s_nu > max_spec_up[c]) {
                    max_spec_up[c] = s_nu;
                    if (v1 > 0) {
                        for (I32 nx = head[c][v1]; nx != 0; nx = next_sib[nx]) {
                            if (s_nu > UP[nx]) {
                                I32 ou = UP[nx];
                                UP[nx] = s_nu;
                                CA[nx][ou]--; CA[nx][s_nu]++;
                                if (!in_qc[nx]) { in_qc[nx] = true; qc.push(nx); }
                            }
                        }
                    }
                    if (v2 > 0 && v2 != v1) {
                        for (I32 nx = head[c][v2]; nx != 0; nx = next_sib[nx]) {
                            if (s_nu > UP[nx]) {
                                I32 ou = UP[nx];
                                UP[nx] = s_nu;
                                CA[nx][ou]--; CA[nx][s_nu]++;
                                if (!in_qc[nx]) { in_qc[nx] = true; qc.push(nx); }
                            }
                        }
                    }
                }

                I32 ns = smax(CA[c]) + 1;
                if (ns > mx_s) {
                    mx_s = ns;
                }
            }
        }
        ans.push_back(mx_s);
    }

    FOR(i, q) {
        cout << ans[i] << (i == q - 1 ? "" : " ");
    }
    cout << "\n";
}

//===----------------------------------------------------------------------===//
/* Main Function */

auto main() -> int {
#ifdef LOCAL
  Stopwatch timer;
#endif

  solve();

  return 0;
}
//===----------------------------------------------------------------------===//
