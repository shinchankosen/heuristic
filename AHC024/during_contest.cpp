#include <bits/stdc++.h>

#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

using namespace std;
#define all(v) (v).begin(),(v).end()
#define pb(a) push_back(a)
#define rep(i, n) for(int i=0;i<n;i++)
#define foa(e, v) for(auto& e : v)
using ll = long long;
const ll MOD7 = 1000000007, MOD998 = 998244353, INF = (1LL << 60);
#define dout(a) cout<<fixed<<setprecision(10)<<a<<endl;

struct UnionFind {
    vector<int> par;
    UnionFind(int n) :par(n, -1) { }
    void init(int n) { par.assign(n, -1); }
    int root(int x) {
        if (par[x] < 0) return x;
        else return par[x] = root(par[x]);
    }
    bool connect(int x, int y) {
        x = root(x); y = root(y);
        if (x == y) return false;
        if (par[x] > par[y]) swap(x, y);
        par[x] += par[y];
        par[y] = x;
        return true;
    }
    int size(int x) {
        return -par[root(x)];
    }
};

int dy[4] = {-1, 0, 0, 1};
int dx[4] = {0, -1, 1, 0};

vector<vector<int>> ans, a, linked;
int best = 0;
int n, m, N;
void chmin(int &a, int b) { a = min(a, b); }

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    auto t_start = chrono::system_clock::now();

    cin >> n >> m;
    N = n + 2;
    a.resize(n + 2, vector<int> (n + 2, 0));
    for(int i = 1; i <= n; i ++) {
        for(int j = 1; j <= n; j ++) {
            cin >> a[i][j];
        }
    }
    ans = a;

    linked.resize(m + 1, vector<int> (m + 1, 0));
    for(int i = 0; i <= m; i ++) linked[i][i] = 1;

    for(int i = 0; i <= n + 1; i ++) {
        for(int j = 0; j <= n; j ++) {
            linked[a[i][j]][a[i][j + 1]] = linked[a[i][j + 1]][a[i][j]] = 1;
        }
    }

    for(int i = 0; i <= n; i ++) {
        for(int j = 0; j <= n + 1; j ++) {
            linked[a[i + 1][j]][a[i][j]] = linked[a[i][j]][a[i + 1][j]] = 1;
        }
    }

    {
        auto grid = a;

        vector<int> ord(m + 1);
        iota(all(ord), 0);
        vector<int> dis(m + 1, 1000000);
        rep(i, N) rep(j, N) {
            int Y = min(abs(i - 26), abs(25 - i));
            int X = min(abs(j - 26), abs(25 - j));
            chmin(dis[a[i][j]], X + Y);
        }
        sort(all(ord), [&](int i, int j){
            if(i == 0) return false;
            if(j == 0) return true;
            return dis[i] < dis[j];
        });
        assert(ord.back() == 0);

        vector<int> DD = {m, 0, m - 1, m - 10, m - 2, m - 3, m - 4, m - 5, m - 90, 1,  m - 20, m - 40, m - 50, m - 70};

        foa(_, DD) {
            {
                auto t_now = chrono::system_clock::now();
                auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
                if (dt > 1850) {
                    break;
                }
            }
            
            auto grid = a;
            vector<int> D;
            for(int i = _; i <= m; i ++) {
                D.pb(ord[i]);
            }
        
            foa(del, D) {
                queue<pair<int, int>> que;
                for(int i = 1; i <= n; i ++) {
                    for(int j = 1; j <= n; j ++) {
                        bool F = false;
                        rep(k, 4) {
                            int y = i + dy[k];
                            int x = j + dx[k];
                            if(grid[y][x] == del) {
                                F = true;
                            }
                        }
                        if(F) que.push({i, j});
                    }
                }

                auto ok = [&](int y, int x) -> bool {

                    int no = y * N + x;
                    
                    vector L(m + 1, vector(m + 1, 0));
                    rep(i, m + 1) L[i][i] = 1;
                    rep(i, N) rep(j, N - 1) {
                        if(i * N + j == no or i * N + j + 1 == no) continue;
                        L[grid[i][j]][grid[i][j + 1]] = L[grid[i][j + 1]][grid[i][j]] = 1;
                    }
                    rep(i, N - 1) rep(j, N) {
                        if(i * N + j == no or (i + 1) * N + j == no) continue;
                        L[grid[i + 1][j]][grid[i][j]] = L[grid[i][j]][grid[i + 1][j]] = 1;
                    }

                    if(linked != L) {
                        return false;
                    }

                    UnionFind uf(N * N);
                    rep(i, N) rep(j, N - 1) {
                        if(i * N + j == no or i * N + j + 1 == no) continue;
                        if(grid[i][j] == grid[i][j + 1]) {
                            uf.connect(i * N + j, i * N + j + 1);
                        }
                    }
                    rep(i, N - 1) rep(j, N) {
                        if(i * N + j == no or (i + 1) * N + j == no) continue;
                        if(grid[i][j] == grid[i + 1][j]) {
                            uf.connect(i * N + j, (i + 1) * N + j);
                        }
                    }

                    int p = -1;

                    rep(k, 4) {
                        int i = y + dy[k];
                        int j = x + dx[k];
                        int col = grid[i][j];
                        if(col == del) {
                            continue;
                        } else if(col == grid[y][x]) {
                            if(p == -1) {
                                p = i * N + j;
                                continue;
                            }
                            if(uf.root(p) != uf.root(i * N + j)) {
                                return false;
                            }
                        } else if(!linked[col][del]) {
                            return false;
                        }
                    }
                    return p != -1;
                };
                while(!que.empty()) {
                    {
                        auto t_now = chrono::system_clock::now();
                        auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
                        if (dt > 1920) {
                            break;
                        }
                    }
                    auto [i, j] = que.front();
                    que.pop();
                    if(grid[i][j] == del) continue;
                    if(ok(i, j)) {
                        rep(k, 4) {
                            int y = i + dy[k];
                            int x = j + dx[k];
                            if(grid[y][x] != del and grid[y][x]) que.push({y, x});
                        }
                        grid[i][j] = del;
                    }
                }
            }
                
            int cnt = 0;
            for(int i = 1; i <= n; i ++) for(int j = 1; j <= n; j ++) if(grid[i][j] == 0) cnt ++;
            if(cnt >= best) {
                best = cnt;
                ans = grid;
            }
        }
    }
    for(int i = 1; i <= n; i ++) {
        for(int j = 1; j <= n; j ++) {
            cout << ans[i][j] << " ";
        }
        cout << endl;
    }


    return 0;
}
