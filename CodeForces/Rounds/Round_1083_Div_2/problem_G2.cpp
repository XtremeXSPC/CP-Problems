#include <bits/stdc++.h>
using namespace std;

static constexpr int MAXM = (1 << 19) - 1;

static int mu_arr[MAXM + 1];
static vector<int> primes;

struct Interval { int l, r; };

static inline void sieve_mobius() {
    int N = MAXM;
    vector<int> lp(N + 1, 0);
    mu_arr[1] = 1;
    for (int i = 2; i <= N; ++i) mu_arr[i] = 0;

    for (int i = 2; i <= N; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            primes.push_back(i);
            mu_arr[i] = -1;
        }
        for (int p : primes) {
            long long x = 1LL * p * i;
            if (x > N) break;
            lp[x] = p;
            if (p == lp[i]) {
                mu_arr[x] = 0;
                break;
            } else {
                mu_arr[x] = -mu_arr[i];
            }
        }
    }
}

static inline void gen_intervals_for_v(
    int v, int m, int maxBit,
    vector<Interval> &pool,
    int &start, int &cnt,
    long long &Wv
) {
    start = (int)pool.size();
    Wv = 0;

    struct Node { int bit, prefix; unsigned char less1, less2; };
    Node st[64];
    int top = 0;
    st[top++] = {maxBit, 0, 0, 0};

    auto emit = [&](int L, int R) {
        if (L < 0) L = 0;
        if (R > m) R = m;
        if (L > R) return;

        if ((int)pool.size() > start && L <= pool.back().r + 1) {
            if (R > pool.back().r) {
                Wv += (long long)(R - pool.back().r);
                pool.back().r = R;
            }
        } else {
            pool.push_back({L, R});
            Wv += (long long)(R - L + 1);
        }
    };

    while (top) {
        Node cur = st[--top];
        int bit = cur.bit;
        int prefix = cur.prefix;
        bool less1 = cur.less1;
        bool less2 = cur.less2;

        if (less1 && less2) {
            int addMask = (1 << (bit + 1)) - 1;
            emit(prefix, prefix + addMask);
            continue;
        }
        if (bit < 0) {
            emit(prefix, prefix);
            continue;
        }

        int mb = (m >> bit) & 1;
        int vb = (v >> bit) & 1;

        for (int c = 1; c >= 0; --c) {
            if (!less1 && c > mb) continue;
            int xb = c ^ vb;
            if (!less2 && xb > mb) continue;
            unsigned char nless1 = (unsigned char)(less1 || (c < mb));
            unsigned char nless2 = (unsigned char)(less2 || (xb < mb));
            st[top++] = {bit - 1, prefix | (c << bit), nless1, nless2};
        }
    }

    cnt = (int)pool.size() - start;
}

static inline vector<int> divisors_leq_M(long long n, int M) {
    vector<pair<int,int>> fac;
    long long tmp = n;
    for (int p : primes) {
        if (1LL * p * p > tmp) break;
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            fac.push_back({p, e});
        }
    }
    if (tmp > 1) fac.push_back({(int)tmp, 1});

    vector<long long> divs;
    divs.push_back(1);
    for (auto [p, e] : fac) {
        int sz = (int)divs.size();
        long long pe = 1;
        for (int i = 1; i <= e; ++i) {
            pe *= p;
            for (int j = 0; j < sz; ++j) divs.push_back(divs[j] * pe);
        }
    }

    vector<int> res;
    res.reserve(divs.size());
    for (long long d : divs) if (1 <= d && d <= M) res.push_back((int)d);
    sort(res.begin(), res.end());
    res.erase(unique(res.begin(), res.end()), res.end());
    return res;
}

static inline void print_int128(__int128 x) {
    if (x == 0) { cout << 0; return; }
    if (x < 0) { cout << '-'; x = -x; }
    string s;
    while (x > 0) {
        int digit = (int)(x % 10);
        s.push_back(char('0' + digit));
        x /= 10;
    }
    reverse(s.begin(), s.end());
    cout << s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    sieve_mobius();

    int T;
    cin >> T;

    static int startIdx[MAXM + 1];
    static int cntIdx[MAXM + 1];
    static long long W[MAXM + 1];

    while (T--) {
        long long n;
        int m;
        cin >> n >> m;

        int hb = 31 - __builtin_clz((unsigned)m);
        int M = (1 << (hb + 1)) - 1;

        vector<Interval> pool;
        pool.reserve((size_t)M * 6);

        for (int v = 1; v <= M; ++v) {
            gen_intervals_for_v(v, m, hb, pool, startIdx[v], cntIdx[v], W[v]);
        }

        vector<int> divs = divisors_leq_M(n, M);

        vector<int> H(M + 1, 0);
        for (int d : divs) {
            for (int x = d, k = 1; x <= M; x += d, ++k) {
                H[x] += mu_arr[k];
            }
        }

        __int128 ans = 0;

        for (int d : divs) ans += 2 * (__int128)W[d];

        vector<int> delta(m + 2, 0);
        vector<int> mark(m + 2, 0);
        vector<int> used;
        used.reserve(m + 2);

        int ver = 1;
        auto add_event = [&](int pos, int val) {
            if (mark[pos] != ver) {
                mark[pos] = ver;
                delta[pos] = 0;
                used.push_back(pos);
            }
            delta[pos] += val;
        };

        for (int x = 1; x <= M; ++x) {
            int hx = H[x];
            if (hx == 0) continue;

            ++ver;
            used.clear();

            for (int v = x; v <= M; v += x) {
                int s = startIdx[v], e = s + cntIdx[v];
                for (int idx = s; idx < e; ++idx) {
                    int L = pool[idx].l;
                    int R = pool[idx].r;
                    add_event(L, +1);
                    add_event(R + 1, -1);
                }
            }

            if (used.empty()) continue;
            sort(used.begin(), used.end());

            long long active = 0;
            int last = 0;
            __int128 Vx = 0;

            for (int pos : used) {
                if (pos > last) {
                    __int128 len = pos - last;
                    Vx += len * (__int128)active * (__int128)active;
                }
                active += delta[pos];
                last = pos;
            }
            if (last <= m + 1) {
                __int128 len = (m + 1) - last;
                Vx += len * (__int128)active * (__int128)active;
            }

            ans += (__int128)hx * Vx;
        }

        print_int128(ans);
        cout << '\n';
    }

    return 0;
}
