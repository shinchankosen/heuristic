#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>
using namespace std;
#define all(v) (v).begin(),(v).end()
#define pb(a) push_back(a)
#define rep(i, n) for(int i=0;i<n;i++)
#define foa(e, v) for(auto&& e : v)
using ll = long long;

const ll mod = 998244353;

ll anssum = 0;
vector<tuple<ll, ll, ll>> ansquery;

vector<vector<ll>> a(9, vector(9, 0LL));
vector s(20, vector(3, vector(3, 0LL)));
vector<tuple<ll, ll, ll>> query;
ll sum = 0;

void eval() {
    if(sum > anssum) {
        anssum = sum;
        ansquery = query;
    }
    return;
}

void print() {
    cout << ansquery.size() << endl;
    for(auto [x, y, z] : ansquery) {
        cout << x << " " << y << " " << z << endl;
    }
}

void push(tuple<int, int, int> op, int mul) {
    auto [id, y, x] = op;
    rep(i, 3) rep(j, 3) {
        a[y + i][x + j] += s[id][i][j] * mul;
        sum += s[id][i][j] * mul;
        if(a[y + i][x + j] >= mod) a[y + i][x + j] -= mod, sum -= mod;
        if(a[y + i][x + j] < 0) a[y + i][x + j] += mod, sum += mod;
    }
}

void solve1(ll base, int Max = 7) {
    int K = 81;
    rep(i, 6) rep(j, 6) {
        ll M = a[i][j], id = -1;
        rep(k, 20) {
            ll nx = a[i][j] + s[k][0][0];
            if(nx >= mod) nx -= mod;
            if(M < nx) {
                M = nx;
                id = k;
            }
        }
        if(M >= base) {
            if(id >= 0) {
                query.push_back({id, i, j});
                push(query.back(), 1);
                K --;
            }
            continue;
        }
        ll id1 = -1, id2 = -1;
        rep(x, 20) for(int y = x; y < 20; y ++) {
            ll nx = a[i][j] + s[x][0][0] + s[y][0][0];
            nx %= mod;
            if(M < nx) {
                M = nx;
                id1 = x;
                id2 = y;
            }
        }
        if(id1 == -1) {
            if(id >= 0) {
                query.push_back({id, i, j});
                push(query.back(), 1);
                K --;
            }
            continue;
        }
        query.push_back({id1, i, j});
        push(query.back(), 1);
        query.push_back({id2, i, j});
        push(query.back(), 1);
        K -= 2;
    }
    rep(i, 6) { 
        {
            ll S = 0;
            rep(j, 3) S += a[i][6 + j];
            if(S >= base * 3) continue;
        }
        ll M = 0;
        auto dfs = [&](auto&& dfs, int id, int cnt) -> void {
            ll S = 0;
            rep(j, 3) S += a[i][6 + j];
            M = max(M, S);
            if(cnt < 3) {
                for(int x = id; x < 20; x ++) {
                    query.push_back({x, i, 6});
                    push(query.back(), 1);
                    dfs(dfs, x, cnt + 1);
                    push(query.back(), -1);
                    query.pop_back();
                }
            }
        };
        dfs(dfs, 0, 0);
        auto dfs2 = [&](auto&& dfs2, int id, int cnt) -> bool {
            ll S = 0;
            rep(j, 3) S += a[i][6 + j];
            if(S == M) {
                K -= cnt;
                return true;
            }
            if(cnt < 3) {
                for(int x = id; x < 20; x ++) {
                    query.push_back({x, i, 6});
                    push(query.back(), 1);
                    if(dfs2(dfs2, x, cnt + 1)) return true;
                    push(query.back(), -1);
                    query.pop_back();
                }
            }
            return false;
        };
        dfs2(dfs2, 0, 0);
    }
    rep(j, 6) {
        {
            ll S = 0;
            rep(i, 3) S += a[6 + i][j];
            if(S >= base * 3) continue;
        }
        ll M = 0;
        int cmax = min(3, K);
        auto dfs = [&](auto&& dfs, int id, int cnt) -> void {
            ll S = 0;
            rep(i, 3) S += a[6 + i][j];
            M = max(M, S);
            if(cnt < cmax) {
                for(int x = id; x < 20; x ++) {
                    query.push_back({x, 6, j});
                    push(query.back(), 1);
                    dfs(dfs, x, cnt + 1);
                    push(query.back(), -1);
                    query.pop_back();
                }
            }
        };
        dfs(dfs, 0, 0);
        auto dfs2 = [&](auto&& dfs2, int id, int cnt) -> bool {
            ll S = 0;
            rep(i, 3) S += a[6 + i][j];
            if(S == M) {
                K -= cnt;
                return true;
            }
            if(cnt < cmax) {
                for(int x = id; x < 20; x ++) {
                    query.push_back({x, 6, j});
                    push(query.back(), 1);
                    if(dfs2(dfs2, x, cnt + 1)) return true;
                    push(query.back(), -1);
                    query.pop_back();
                }
            }
            return false;
        };
        dfs2(dfs2, 0, 0);
    }
    {
        ll M = 0;
        int cmax = min(Max, K);
        auto dfs = [&](auto&& dfs, int id, int cnt) -> void {
            eval();
            M = max(M, sum);
            if(cnt < cmax) {
                for(int x = id; x < 20; x ++) {
                    query.push_back({x, 6, 6});
                    push(query.back(), 1);
                    dfs(dfs, x, cnt + 1);
                    push(query.back(), -1);
                    query.pop_back();
                }
            }
        };
        dfs(dfs, 0, 0);
        auto dfs2 = [&](auto&& dfs2, int id, int cnt) -> bool {
            if(sum == M) return true;
            if(cnt < cmax) {
                for(int x = id; x < 20; x ++) {
                    query.push_back({x, 6, 6});
                    push(query.back(), 1);
                    if(dfs2(dfs2, x, cnt + 1)) return true;
                    push(query.back(), -1);
                    query.pop_back();
                }
            }
            return false;
        };
        dfs2(dfs2, 0, 0);
    }
    eval();
    while(!query.empty()) {
        push(query.back(), -1);
        query.pop_back();
    }
    return;
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    auto t_start = chrono::system_clock::now();

    int n, m, K;
    cin >> n >> m >> K;
    rep(i, n) rep(j, n) {
        cin >> a[i][j];
        sum += a[i][j];
    }
    rep(i, m) rep(j, 3) rep(k, 3) cin >> s[i][j][k];

    ll M = 0;
    int id = -1;
    for(ll i = 850000000; i <= 970000000; i += 20000000) {
        auto t_now = chrono::system_clock::now();
        auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
        if (dt > 1300) break;
        solve1(i);
        if(anssum > M) {
            M = anssum;
            id = i;
        }
    }
    for(ll i = 960000000; i >= 860000000; i -= 20000000) {
        auto t_now = chrono::system_clock::now();
        auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
        if (dt > 1600) break;
        solve1(i);
        if(anssum > M) {
            M = anssum;
            id = i;
        }
    }

    for(ll i = 840000000; i >= 780000000; i -= 20000000) {
        auto t_now = chrono::system_clock::now();
        auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
        if (dt > 1700) break;
        solve1(i);
        if(anssum > M) {
            M = anssum;
            id = i;
        }
    }

    for(ll i = 875000000; i <= 960000000; i += 10000000) {
        auto t_now = chrono::system_clock::now();
        auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
        if (dt > 1800) break;
        solve1(i);
        if(anssum > M) {
            M = anssum;
            id = i;
        }
    }
    // cout << id << endl;

    print();
    return 0;
}
