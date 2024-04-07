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
vector<tuple<int, int, int>> ansquery;

vector<vector<ll>> a(9, vector(9, 0LL));

vector S(20, vector(3, vector(3, 0LL)));
vector<vector<vector<ll>>> s;
vector<vector<int>> add_query;
vector<tuple<int, int, int>> query;
ll sum = 0;

void eval() {
    if(sum > anssum and (int)query.size() <= 81) {
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

void push(int id, int y, int x, int mul) {
    rep(i, 3) rep(j, 3) {
        a[y + i][x + j] += s[id][i][j] * mul;
        sum += s[id][i][j] * mul;
        if(a[y + i][x + j] >= mod) a[y + i][x + j] -= mod, sum -= mod;
        if(a[y + i][x + j] < 0) a[y + i][x + j] += mod, sum += mod;
    }
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    int n, m, K;
    cin >> n >> m >> K;
    rep(i, n) rep(j, n) {
        cin >> a[i][j];
        sum += a[i][j];
    }
    rep(i, m) rep(j, 3) rep(k, 3) cin >> S[i][j][k];

    auto A = a;
    ll Sum = sum;

    vector<vector<ll>> snow(3, vector(3, 0LL));

    vector<int> squery;
    auto dfs = [&](auto&& dfs, int i, int cnt, int flag, int base) -> void {
        add_query.push_back(squery);
        s.push_back(snow);
        if(cnt == base) {
            return;
        }
        for(int j = i; j < 20; j ++) {
            rep(y, 3) rep(x, 3) {
                snow[y][x] += S[j][y][x];
                if(snow[y][x] >= mod) snow[y][x] -= mod;
            }
            squery.push_back(j);
            dfs(dfs, j + flag, cnt + 1, flag, base);
            rep(y, 3) rep(x, 3) {
                snow[y][x] -= S[j][y][x];
                if(snow[y][x] < 0) snow[y][x] += mod;
            }
            squery.pop_back();
        }
    };
    dfs(dfs, 0, 0, 0, 7);
    
    int sz = s.size();
    
    for(int i = 0; i < 9; i += 3) {
        for(int j = 0; j < 9; j += 3) {
            ll M = -1;
            rep(id, sz) {
                push(id, i, j, 1);
                M = max(M, sum);
                push(id, i, j, -1);
            }
            rep(id, sz) {
                push(id, i, j, 1);
                if(M != sum) {
                    push(id, i, j, -1);
                    continue;
                }
                foa(e, add_query[id]) {
                    query.push_back({e, i, j});
                }
                break;
            }
            eval();
        }
    }
    a = A;
    sum = Sum;
    query.clear();
    add_query.clear();
    s.clear();
    rep(i, 3) rep(j, 3) snow[i][j] = 0;

    dfs(dfs, 0, 0, 1, 8);
    sz = s.size();
    for(int i = 0; i < 9; i += 3) {
        for(int j = 0; j < 9; j += 3) {
            ll M = -1;
            rep(id, sz) {
                push(id, i, j, 1);
                M = max(M, sum);
                push(id, i, j, -1);
            }
            rep(id, sz) {
                push(id, i, j, 1);
                if(M != sum) {
                    push(id, i, j, -1);
                    continue;
                }
                foa(e, add_query[id]) {
                    query.push_back({e, i, j});
                }
                break;
            }
            eval();
        }
    }
    print();
    return 0;
}
