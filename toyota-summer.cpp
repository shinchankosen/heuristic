#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>
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
vector<vector<int>> a(9, vector<int> (9, -1));
int d = 9, in_cnt;


int tentou(vector<int> &v) {
    int ret = 0, m = v.size();
    rep(i, m) {
        rep(j, i) if(v[i] < v[j]) ret ++;
    }
    return ret;
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    auto t_start = chrono::system_clock::now();
    int seed = 0;

    int n;
    cin >> d >> n;
    vector<int> y(n), x(n);
    
    rep(i, n) {
        cin >> y[i] >> x[i];
        a[y[i]][x[i]] = -2;
    }
    in_cnt = d * d - 1 - n;

    vector<int> come(d * d - 1 - n, 0);

    auto uf_test = [&](int tmp) -> bool {
        UnionFind uf(81);
        rep(i, 9) rep(j, 9) {
            if(a[i][j] != -1) continue;
            rep(k, 4) {
                int ii = dy[k] + i;
                int jj = dx[k] + j;
                if(ii < 0 or ii >= d or jj < 0 or jj >= d) {
                    continue;
                } 
                if(a[ii][jj] == -1) uf.connect(i * d + j, ii * d + jj);
            }
        }
        
        rep(i, 9) rep(j, 9) {
            if(a[i][j] == -1 and uf.root(i * d + j) != uf.root(4)) return false;
        }
        return true;
    };

    
    rep(_, in_cnt) {
        int num;
        cin >> num;
        
        come[num] ++;
        bool flag = false;
        for(int i = 0; i < d; i ++) {
            for(int j = 0; j < d; j ++) {
                if(a[i][j] != -1 or (i * d + j) == 4) continue;
                int cnt = 0;
                rep(k, 4) {
                    int ii = dy[k] + i;
                    int jj = dx[k] + j;
                    if(ii < 0 or ii >= d or jj < 0 or jj >= d) {
                        continue;
                    }
                    if(a[ii][jj] == -1) cnt ++;
                }
                if(cnt == 1) {
                    flag = true;
                    a[i][j] = num;
                    cout << i << " " << j << endl;
                    break;
                }
            }
            if(flag) break;
        }
        if(flag) continue;

        int cnt = 1;
        for(int k = 0; k < num; k ++) {
            if(!come[k]) cnt ++;
        }

        vector<int> dis(d * d, 100);
        queue<int> que;
        que.push(4);
        dis[4] = 0;

        while(!que.empty()) {
            int i = que.front() / d;
            int j = que.front() % d;
            que.pop();
            rep(k, 4) {
                int ii = dy[k] + i;
                int jj = dx[k] + j;
                if(ii < 0 or ii >= d or jj < 0 or jj >= d) {
                    continue;
                }
                if(a[ii][jj] == -1 and dis[ii * d + jj] == 100) {
                    dis[ii * d + jj] = dis[i * d + j] + 1;
                    que.push(ii * d + jj);
                }
            }
        }

        vector<pair<int, int>> V; 
        
        for(int i = 0; i < d; i ++) {
            for(int j = 0; j < d; j ++) {
                if(dis[i * d + j] == 100 or (i * d + j) == 4) continue;
                V.push_back({dis[i * d + j], i * d + j});
            }
        }
        sort(all(V));
        for(auto [D, P] : V) {
            int i = P / d;
            int j = P % d;
            a[i][j] = num;

            if(!uf_test(num)) {
                cnt --;
                a[i][j] = -1;
                continue;
            }
            a[i][j] = -1;

            cnt --;
            if(cnt <= 0) {
                cout << i << " " << j << endl;
                a[i][j] = num;
                break; 
            }
        }
    }

    // ここまで入れるとこ

    // ここから出していくとこ

    int MIN;
    vector<pair<int, int>> ans;
    {
        vector<vector<int>> state(d, vector<int> (d, 0)); // wait
        state[0][4] = 2; // done
        state[0][3] = state[0][5] = state[1][4] = 1; // ready

        vector<int> v;
        vector<pair<int, int>> ret;
        
        rep(_, in_cnt) {
            int MIN = 100, idy, idx;
            rep(i, d) rep(j, d) {
                if(state[i][j] == 1 and MIN > a[i][j]) {
                    MIN = a[i][j];
                    idy = i, idx = j;
                }
            }
            state[idy][idx] = 2;
            ret.push_back({idy, idx});
            v.pb(MIN);
            rep(k, 4) {
                int ii = dy[k] + idy;
                int jj = dx[k] + idx;
                if(ii < 0 or ii >= d or jj < 0 or jj >= d) {
                    continue;
                }
                if(a[ii][jj] >= 0 and state[ii][jj] == 0) {
                    state[ii][jj] = 1;
                }
            }
        }

        MIN = tentou(v);
        ans = ret;
    }

    for(auto [A, B] : ans) {
        cout << A << " " << B << endl;
    }

    
    return 0;
}
