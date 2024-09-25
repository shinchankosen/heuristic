#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>
using namespace std;
#define all(v) (v).begin(),(v).end()
#define pb(a) push_back(a)
#define rep(i, n) for(int i=0;i<n;i++)
#define foa(e, v) for(auto& e : v)

int n, m, tsz, la, lb, ans = 1 << 30;
vector<int> light;
vector<tuple<int, int, int>> ans_vec;
vector<int> v[600];
int x[600], y[600], t[600];

void chmin(int &a, int b) {
    a = min(a, b);
}
void chmax(int &a, int b) {
    a = max(a, b);
}

inline int dist2(int x, int y) {return x*x + y*y;}

void sample() {
    vector<int> A(la, 0);
    rep(i, n) A[i] = i;

    vector<tuple<int, int, int>> task;
    // vector<int> B(lb, -1);

    int now = 0, cnt = 0;
    rep(i, tsz) {
        vector<int> path;
        vector<bool> visited(n, 0);
        auto dfs = [&](auto &&self, int cur, int prev) -> bool {
            if(visited[cur]) return false;
            if(cur != now) path.push_back(cur);
            visited[cur] = true;
            if(cur == t[i]) return true;
            vector<int> srch = v[cur];
            sort(all(srch), [&](const int &a, const int &b) {
                return dist2(x[a] - x[t[i]], y[a] - y[t[i]]) < dist2(x[b] - x[t[i]], y[b] - y[t[i]]);
            });
            for(int e : srch) {
                if(e == prev) continue;
                if(self(self, e, cur)) return true;
            }
            path.pop_back();
            return false;
        };
        dfs(dfs, now, -1);
        now = t[i];
        for(int e : path) {
            task.push_back({1, e, 0});
            task.push_back({e, -1, -1});
            cnt ++;
        }
    }
    if(cnt < ans) {
        ans = cnt;
        ans_vec = task;
        light = A;
    }
    // cout << cnt << endl;
}
void solve0() {
    vector<int> A(la, 0);
    rep(i, n) A[i] = i;

    vector<tuple<int, int, int>> task;
    // vector<int> B(lb, -1);

    int now = 0, cnt = 0;
    rep(i, tsz) {
        vector<int> prev(n, -1), seen(n, 0);
        seen[now] = 1;
        queue<int> que;
        que.push(now);
        while(!que.empty()) {
            int cur = que.front();
            que.pop();
            foa(e, v[cur]) {
                if(!seen[e]) {
                    prev[e] = cur;
                    seen[e] = 1;
                    que.push(e);
                }
            }
        }
        vector<int> path;
        int cur = t[i];
        while(now != cur) {
            path.pb(cur);
            cur = prev[cur];
        }
        reverse(all(path));
        
        now = t[i];
        for(int e : path) {
            task.push_back({1, e, 0});
            task.push_back({e, -1, -1});
            cnt ++;
        }
    }
    if(cnt < ans) {
        ans = cnt;
        ans_vec = task;
        light = A;
    }
    // cout << cnt << endl;
}

static constexpr int half = 500;

void solve1(int flag, int th) {
    if(la == n) return;

    if(flag) {
        rep(i, n) swap(x[i], y[i]);
    }


    vector<int> A(la, 0);
    rep(i, n) A[i] = i;

    vector<vector<int>> dis(n, vector(n, 1 << 20));
    rep(i, n) foa(e, v[i]) chmin(dis[i][e], 1);
    rep(i, n) dis[i][i] = 0;
    rep(k, n) rep(i, n) rep(j, n) chmin(dis[i][j], dis[i][k] + dis[k][j]);

    
    vector<tuple<int, int, int>> task;
    vector<int> B(lb, -1);

    int amari = la - n;
    vector<int> is_highway(n, -1);
    vector<int> highway;
    int hsz = -1;
    {
        int id = -1, M = 1001;
        rep(i, n) if(half - th <= y[i] and y[i] <= half + th and M > abs(x[i] - half)) {
            M = abs(x[i] - half);
            id = i;
        }
        if(id == -1) return;
        vector<int> path1;
        vector<int> path2;
        auto dfs1 = [&](auto dfs1, int now) -> void {
            int ret = 1000, nx = -1;
            foa(e, v[now]) {
                if(x[e] > x[now]) {
                    if(abs(y[e] - half) < ret) {
                        ret = abs(y[e] - half);
                        nx = e;
                    }
                } 
            }
            if(nx == -1) return;
            path1.pb(nx);
            dfs1(dfs1, nx);
        };
        auto dfs2 = [&](auto dfs2, int now) -> void {
            int ret = 1000, nx = -1;
            foa(e, v[now]) {
                if(x[e] < x[now]) {
                    if(abs(y[e] - half) < ret) {
                        ret = abs(y[e] - half);
                        nx = e;
                    }
                } 
            }
            if(nx == -1) return;
            path2.pb(nx);
            dfs2(dfs2, nx);
        };
        path1.pb(id);
        path2.pb(id);
        dfs1(dfs1, id);
        dfs2(dfs2, id);
        // {
        //     int sz = path1.size();
        //     rep(i, sz - 1) assert(dis[path1[i]][path1[i + 1]] == 1);
        //     sz = path2.size();
        //     rep(i, sz - 1) assert(dis[path2[i]][path2[i + 1]] == 1);
        // }

        int tmp = amari + 1;
        tmp /= 2;
        while((int)path1.size() > tmp) path1.pop_back();
        tmp = amari + 1 - tmp;
        while((int)path2.size() > tmp) path2.pop_back();
        
        highway = path2;
        reverse(all(highway));
        highway.pop_back();
        foa(e, path1) highway.pb(e);
        hsz = highway.size();
        rep(i, hsz) is_highway[highway[i]] = i;
        rep(i, hsz) A[i + n] = highway[i]; 

        // rep(i, hsz - 1) assert(dis[highway[i]][highway[i + 1]] == 1);
    }

    int now = 0, cnt = 0;

    auto bfs = [&](int c, int to) -> int {
        if(c == to) return 0;
        int ret = 0;
        vector<int> prev(n, -1), seen(n, 0);
        seen[c] = 1;
        queue<int> que;
        que.push(c);
        while(!que.empty()) {
            int cur = que.front();
            que.pop();
            foa(e, v[cur]) {
                if(!seen[e]) {
                    prev[e] = cur;
                    seen[e] = 1;
                    que.push(e);
                }
            }
        }
        vector<int> path;
        while(prev[to] != -1) {
            path.pb(to);
            to = prev[to];
        }
        reverse(all(path));
        
        for(int e : path) {
            task.push_back({1, e, 0});
            task.push_back({e, -1, -1});
            ret ++;
        }
        return ret;
    };
    rep(i, tsz) {
        
        int Min = dis[t[i]][now];
        int id1 = -1, id2 = -1;
        rep(ii, hsz) rep(jj, hsz) if(ii != jj) {
            int from = highway[ii];
            int to = highway[jj];
            if(from == now) {
                if(Min > (abs(ii - jj) + lb - 1) / lb + dis[to][t[i]]) {
                    Min = (abs(ii - jj) + lb - 1) / lb + dis[to][t[i]];
                    id1 = ii;
                    id2 = jj;
                }
            } else {
                if(Min > abs(ii - jj) / lb + dis[to][t[i]] + dis[from][now]) {
                    Min = abs(ii - jj) / lb + dis[to][t[i]] + dis[from][now];
                    id1 = ii;
                    id2 = jj;
                }
            }
        }
        int ii = id1, jj = id2;

        if(id1 != -1) {
            int from = highway[ii];
            if(from != now) {
                int via = -1;
                foa(e, v[from]) {
                    if(dis[e][now] + 1 == dis[now][from]) {
                        via = e;
                        break;
                    }
                }
                cnt += bfs(now, via);
                now = via;
                if(ii < jj) {
                    int d = min(jj - ii + 1, lb);
                    task.push_back({d, ii + n, 0});
                    cnt ++;
                    rep(j, d) task.push_back({highway[ii + j], -1, -1});
                    ii += d - 1;
                } else { // ii > jj
                    int d = min(ii - jj + 1, lb);
                    task.push_back({d, ii - d + 1 + n, 0});
                    cnt ++;
                    rep(j, d) task.push_back({highway[ii - j], -1, -1});
                    ii -= d - 1;
                }
            } 
            while(ii < jj) {
                int d = min(jj - ii, lb);
                task.push_back({d, ii + 1 + n, 0});
                cnt ++;
                rep(j, d) {
                    task.push_back({highway[ii + j + 1], -1, -1});
                }
                ii += d;
            }
            while(ii > jj) {
                int d = min(ii - jj, lb);
                task.push_back({d, ii - d + n, 0});
                cnt ++;
                rep(j, d) {
                    task.push_back({highway[ii - j - 1], -1, -1});
                }
                ii -= d;
            }
            now = highway[jj];
        }
        cnt += bfs(now, t[i]);
        now = t[i];
    
    }
    if(cnt < ans) {
        ans = cnt;
        ans_vec = task;
        light = A;
    }

    if(flag) {
        rep(i, n) swap(x[i], y[i]);
    }
    // foa(e, highway) cout << x[e] << " " << y[e] << endl;
    // cout << cnt << endl;
}

void solve2(int th) {
    if(la < n + 7) return;


    vector<int> A(la, 0);
    rep(i, n) A[i] = i;

    vector<vector<int>> dis(n, vector(n, 1 << 20));
    rep(i, n) foa(e, v[i]) chmin(dis[i][e], 1);
    rep(i, n) dis[i][i] = 0;
    rep(k, n) rep(i, n) rep(j, n) chmin(dis[i][j], dis[i][k] + dis[k][j]);

    
    vector<tuple<int, int, int>> task;
    vector<int> B(lb, -1);

    int amari = la - n;
    vector<int> highwayy, highwayx;
    int hxsz = -1, hysz = -1, center = -1, ybase = -1;
    {
        int M = 1001;
        rep(i, n) if(half - th <= y[i] and y[i] <= half + th and M > abs(x[i] - half)) {
            M = abs(x[i] - half);
            center = i;
        }

        if(center == -1) return;
    }
    
    { 
        vector<int> path1, path2;
        auto dfs1 = [&](auto dfs1, int now) -> void {
            int ret = 1000, nx = -1;
            foa(e, v[now]) {
                if(x[e] > x[now]) {
                    if(abs(y[e] - half) < ret) {
                        ret = abs(y[e] - half);
                        nx = e;
                    }
                } 
            }
            if(nx == -1) return;
            path1.pb(nx);
            dfs1(dfs1, nx);
        };
        auto dfs2 = [&](auto dfs2, int now) -> void {
            int ret = 1000, nx = -1;
            foa(e, v[now]) {
                if(x[e] < x[now]) {
                    if(abs(y[e] - half) < ret) {
                        ret = abs(y[e] - half);
                        nx = e;
                    }
                } 
            }
            if(nx == -1) return;
            path2.pb(nx);
            dfs2(dfs2, nx);
        };
        path1.pb(center);
        path2.pb(center);
        dfs1(dfs1, center);
        dfs2(dfs2, center);
        int tmp = max(amari / 4, 2);
        while((int)path1.size() > tmp) path1.pop_back();
        amari -= (int)path1.size() - 1;
        tmp = max(amari / 3, 2);
        while((int)path2.size() > tmp) path2.pop_back();
        amari -= (int)path2.size();
        highwayx = path2;
        reverse(all(highwayx));
        highwayx.pop_back();
        foa(e, path1) highwayx.pb(e);
        hxsz = highwayx.size();
        rep(i, hxsz) A[i + n] = highwayx[i]; 
        ybase = n + hxsz;
    }{
        rep(i, n) swap(x[i], y[i]);
        vector<int> path1, path2;
        auto dfs1 = [&](auto dfs1, int now) -> void {
            int ret = 1000, nx = -1;
            foa(e, v[now]) {
                if(x[e] > x[now]) {
                    if(abs(y[e] - half) < ret) {
                        ret = abs(y[e] - half);
                        nx = e;
                    }
                } 
            }
            if(nx == -1) return;
            path1.pb(nx);
            dfs1(dfs1, nx);
        };
        auto dfs2 = [&](auto dfs2, int now) -> void {
            int ret = 1000, nx = -1;
            foa(e, v[now]) {
                if(x[e] < x[now]) {
                    if(abs(y[e] - half) < ret) {
                        ret = abs(y[e] - half);
                        nx = e;
                    }
                } 
            }
            if(nx == -1) return;
            path2.pb(nx);
            dfs2(dfs2, nx);
        };
        path1.pb(center);
        path2.pb(center);
        dfs1(dfs1, center);
        dfs2(dfs2, center);

        int tmp = amari + 1;
        tmp /= 2;
        while((int)path1.size() > tmp) path1.pop_back();
        tmp = amari + 1 - tmp;
        while((int)path2.size() > tmp) path2.pop_back();

        highwayy = path2;
        reverse(all(highwayy));
        highwayy.pop_back();
        foa(e, path1) highwayy.pb(e);
        hysz = highwayy.size();
        rep(i, hysz) A[i + ybase] = highwayy[i];

        rep(i, n) swap(x[i], y[i]);
    }



    int now = 0, cnt = 0;

    auto bfs = [&](int c, int to) -> int {
        if(c == to) return 0;
        int ret = 0;
        vector<int> prev(n, -1), seen(n, 0);
        seen[c] = 1;
        queue<int> que;
        que.push(c);
        while(!que.empty()) {
            int cur = que.front();
            que.pop();
            foa(e, v[cur]) {
                if(!seen[e]) {
                    prev[e] = cur;
                    seen[e] = 1;
                    que.push(e);
                }
            }
        }
        vector<int> path;
        while(prev[to] != -1) {
            path.pb(to);
            to = prev[to];
        }
        reverse(all(path));
        
        for(int e : path) {
            task.push_back({1, e, 0});
            task.push_back({e, -1, -1});
            ret ++;
        }
        return ret;
    };

    int base = n + hxsz + hysz; // 余った分で初め楽しよう


    auto amaribfs = [&](int Now, int to) -> int {
        int ret = 0;
        vector<int> prev(n, -1), seen(n, 0);
        seen[Now] = 1;
        queue<int> que;
        que.push(Now);
        while(!que.empty()) {
            int cur = que.front();
            que.pop();
            foa(e, v[cur]) {
                if(!seen[e]) {
                    prev[e] = cur;
                    seen[e] = 1;
                    que.push(e);
                }
            }
        }
        vector<int> path;
        while(prev[to] != -1) {
            path.pb(to);
            to = prev[to];
        }
        reverse(all(path));
        int sz = path.size();
        for(int i = 0; i < sz ; i += lb) {
            if(i + lb >= sz) {
                task.push_back({sz - i, base + i, 0});
                ret ++;
                for(int j = i; j < sz; j ++) task.push_back({path[j], -1, -1});
                for(int j = i; j < sz; j ++) A[base + j] = path[j];
            } else {
                task.push_back({lb, base + i, 0});
                ret ++;
                for(int j = i; j < i + lb; j ++) task.push_back({path[j], -1, -1});
                for(int j = i; j < i + lb; j ++) A[base + j] = path[j];
            }
        }
        return ret;
    };

    for(int i = 0; i < tsz; i ++) {
        
        int Min = dis[t[i]][now];
        int id1 = -1, id2 = -1;
        rep(ii, hxsz) rep(jj, hxsz) if(ii != jj) {
            int from = highwayx[ii];
            int to = highwayx[jj];
            if(from == now) {
                if(Min > (abs(ii - jj) + lb - 1) / lb + dis[to][t[i]]) {
                    Min = (abs(ii - jj) + lb - 1) / lb + dis[to][t[i]];
                    id1 = ii;
                    id2 = jj;
                }
            } else {
                if(Min > abs(ii - jj) / lb + dis[to][t[i]] + dis[from][now]) {
                    Min = abs(ii - jj) / lb + dis[to][t[i]] + dis[from][now];
                    id1 = ii;
                    id2 = jj;
                }
            }
        }
        rep(ii, hysz) rep(jj, hysz) if(ii != jj) {
            int from = highwayy[ii];
            int to = highwayy[jj];
            if(from == now) {
                if(Min > (abs(ii - jj) + lb - 1) / lb + dis[to][t[i]]) {
                    Min = (abs(ii - jj) + lb - 1) / lb + dis[to][t[i]];
                    id1 = ii + ybase;
                    id2 = jj + ybase;
                }
            } else {
                if(Min > abs(ii - jj) / lb + dis[to][t[i]] + dis[from][now]) {
                    Min = abs(ii - jj) / lb + dis[to][t[i]] + dis[from][now];
                    id1 = ii + ybase;
                    id2 = jj + ybase;
                }
            }
        }
        int ii = id1, jj = id2;

        if(id1 != -1) {
            if(ii < ybase) {
                int from = highwayx[ii];
                if(from != now) {
                    int via = -1;
                    foa(e, v[from]) {
                        if(dis[e][now] + 1 == dis[now][from]) {
                            via = e;
                            break;
                        }
                    }
                    if(dis[now][via] + base <= la) {
                        cnt += amaribfs(now, via);
                        base += dis[now][via];
                    } else cnt += bfs(now, via);
                    now = via;
                    if(ii < jj) {
                        int d = min(jj - ii + 1, lb);
                        task.push_back({d, ii + n, 0});
                        cnt ++;
                        rep(j, d) task.push_back({highwayx[ii + j], -1, -1});
                        ii += d - 1;
                    } else { // ii > jj
                        int d = min(ii - jj + 1, lb);
                        task.push_back({d, ii - d + 1 + n, 0});
                        cnt ++;
                        rep(j, d) task.push_back({highwayx[ii - j], -1, -1});
                        ii -= d - 1;
                    }
                } 
                while(ii < jj) {
                    int d = min(jj - ii, lb);
                    task.push_back({d, ii + 1 + n, 0});
                    cnt ++;
                    rep(j, d) {
                        task.push_back({highwayx[ii + j + 1], -1, -1});
                    }
                    ii += d;
                }
                while(ii > jj) {
                    int d = min(ii - jj, lb);
                    task.push_back({d, ii - d + n, 0});
                    cnt ++;
                    rep(j, d) {
                        task.push_back({highwayx[ii - j - 1], -1, -1});
                    }
                    ii -= d;
                }
                now = highwayx[jj];
            } else {
                ii -= ybase;
                jj -= ybase;
                int from = highwayy[ii];
                if(from != now) {
                    int via = -1;
                    foa(e, v[from]) {
                        if(dis[e][now] + 1 == dis[now][from]) {
                            via = e;
                            break;
                        }
                    }
                    cnt += bfs(now, via);
                    now = via;
                    if(ii < jj) {
                        int d = min(jj - ii + 1, lb);
                        task.push_back({d, ii + ybase, 0});
                        cnt ++;
                        rep(j, d) task.push_back({highwayy[ii + j], -1, -1});
                        ii += d - 1;
                    } else { // ii > jj
                        int d = min(ii - jj + 1, lb);
                        task.push_back({d, ii - d + 1 + ybase, 0});
                        cnt ++;
                        rep(j, d) task.push_back({highwayy[ii - j], -1, -1});
                        ii -= d - 1;
                    }
                } 
                while(ii < jj) {
                    int d = min(jj - ii, lb);
                    task.push_back({d, ii + 1 + ybase, 0});
                    cnt ++;
                    rep(j, d) {
                        task.push_back({highwayy[ii + j + 1], -1, -1});
                    }
                    ii += d;
                }
                while(ii > jj) {
                    int d = min(ii - jj, lb);
                    task.push_back({d, ii - d + ybase, 0});
                    cnt ++;
                    rep(j, d) {
                        task.push_back({highwayy[ii - j - 1], -1, -1});
                    }
                    ii -= d;
                }
                now = highwayy[jj];
            }
        }
        //
        cnt += bfs(now, t[i]);
        now = t[i];
    }

    if(cnt < ans) {
        ans = cnt;
        ans_vec = task;
        light = A;
    }
    // foa(e, highway) cout << x[e] << " " << y[e] << endl;
    // cout << cnt << endl;
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    cin >> n >> m >> tsz >> la >> lb;
    /*
        N - 1 <= M <= 3N - 6
        N = tsz = 600
        N <= la <= 2N
        4 <= lb <= 24
        0 <= x, y <= 1000
        平面グラフ
    */
    rep(i, m) {
        int a, b;
        cin >> a >> b;
        v[a].pb(b);
        v[b].pb(a);
    }
    rep(i, tsz) cin >> t[i];
    rep(i, n) cin >> x[i] >> y[i];
    
    sample(); // ユークリッド距離が小さい方へ
    solve0(); // ホップ数小さい方へ
    
    rep(j, 2) {
        solve1(j, 50);
        solve1(j, 80);
        solve1(j, 130);
        solve1(j, 200);
    }
    // solve1(0); ホップ数 + la - nだけ横方向幹線？
    // solve1(1); 縦

    solve2(50);  // 縦横
    solve2(100);
    solve2(150);
    solve2(200);
    
    for(int e : light) {
        cout << e << " ";
    }
    cout << endl;

    for(auto [a, b, c] : ans_vec) {
        if(b == -1) cout << "m " << a << endl;
        else cout << "s " << a << " " << b << " " << c << endl;
    }
    return 0;
}
