#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>
using namespace std;
#define all(v) (v).begin(),(v).end()
#define pb(a) push_back(a)
#define rep(i, n) for(int i=0;i<n;i++)
#define foa(e, v) for(auto& e : v)
class random_devicer {
	int seed;
	std::mt19937 engine;

public:
	random_devicer(const int seed = 0): seed(seed), engine(seed) {}

	// [lower, upper] 中の整数から一様ランダムに選ぶ。
	int next(int lower, int upper) {
		std::uniform_int_distribution<> dist(lower, upper);
		return dist(engine);
	}
};

void chmin(int &a, int b) {
    a = min(a, b);
}
void chmax(int &a, int b) {
    a = max(a, b);
}

int ans = 10000;
const int inf = 10000000;
vector<pair<int, int>> ans_vec;
vector<vector<int>> a;
int n, m;

void solve0() {
    vector<int> base(m, 0);
    vector<int> down(n + 1, 0), up(n + 1, 0);

    vector<pair<int, int>> work;

    int Move_cnt = 0;

    auto to_zero = [&](int val, int to) -> void {
        down[up[val]] = -to - 1;
        base[to] = up[val];
        up[val] = 0;
    };

    auto Move = [&](int val, int to) -> int {
        int now = up[val], cnt = 0;
        while(now != 0) {
            cnt ++;
            now = up[now];
        }
        if(!cnt) return 0;
        work.push_back({up[val], to + 1});
        if(base[to] == 0) to_zero(val, to);
        else {
            int now = base[to];
            while(up[now] != 0) {
                now = up[now];
            }
            up[now] = up[val];
            down[up[val]] = now;
            up[val] = 0;
        }
        return cnt + 1;
    };

    for(int i = 0; i < m; i ++) {
        base[i] = a[i][0];
        down[a[i][0]] = - i - 1;
        for(int j = 1; j < 20; j ++) {
            down[a[i][j]] = a[i][j - 1];
            up[a[i][j - 1]] = a[i][j];
        }
    }
    for(int i = 1; i <= n; i ++) {
        vector<int> M(m, inf);
        rep(j, m) {
            int now = base[j];
            while(now != 0) {
                chmin(M[j], now);
                now = up[now];
            }
        }
        int idm = 0;
        rep(j, m) {
            if(M[j] > M[idm]) idm = j;
        }
        Move_cnt += Move(i, idm); // iより上をidmへ。
        if(down[i] < 0) {
            base[-down[i] - 1] = 0;
            work.push_back({i, 0});
        } else {
            up[down[i]] = 0;
            work.push_back({i, 0});
        }
    }
    if(ans > Move_cnt) {
        ans = Move_cnt;
        ans_vec = work;
    }
}

void solve1() {
    vector<int> base(m, 0);
    vector<int> down(n + 1, 0), up(n + 1, 0);

    vector<pair<int, int>> work;

    int Move_cnt = 0;

    auto to_zero = [&](int val, int to) -> void {
        down[up[val]] = -to - 1;
        base[to] = up[val];
        up[val] = 0;
    };

    auto Move = [&](int val, int to) -> int {
        int now = up[val], cnt = 0;
        while(now != 0) {
            cnt ++;
            now = up[now];
        }
        if(!cnt) return 0;
        work.push_back({up[val], to + 1});
        if(base[to] == 0) to_zero(val, to);
        else {
            int now = base[to];
            while(up[now] != 0) {
                now = up[now];
            }
            up[now] = up[val];
            down[up[val]] = now;
            up[val] = 0;
        }
        return cnt + 1;
    };

    for(int i = 0; i < m; i ++) {
        base[i] = a[i][0];
        down[a[i][0]] = - i - 1;
        for(int j = 1; j < 20; j ++) {
            down[a[i][j]] = a[i][j - 1];
            up[a[i][j - 1]] = a[i][j];
        }
    }
    for(int i = 1; i <= n; i ++) {
        vector<int> M(m, inf);
        rep(j, m) {
            int now = base[j];
            while(now != 0) {
                chmin(M[j], now);
                now = up[now];
            }
        }
        int idm = 0;
        rep(j, m) {
            if(M[j] > M[idm]) idm = j;
        }

        vector<int> v;
        {
            int now = i;
            while(now != 0) {
                if(now <= i + 8) v.pb(now);
                now = up[now];
            }
        }
        reverse(all(v));
        foa(I, v) {
            Move_cnt += Move(I, idm); // Iより上をidmへ。
            if(I == i) {
                if(down[i] < 0) {
                    base[-down[i] - 1] = 0;
                    work.push_back({i, 0});
                } else {
                    up[down[i]] = 0;
                    work.push_back({i, 0});
                }
                continue;
            } 
            int id = -1, Min = inf * 10;
            rep(j, m) if(M[j] > I and M[j] < Min) {
                Min = M[j];
                id = j;
            }
            Move_cnt += Move(down[I], id);
            M[id] = I;
        }
    }
    if(ans > Move_cnt) {
        ans = Move_cnt;
        ans_vec = work;
    }
}

void solve2(int th) {
    vector<int> base(m, 0);
    vector<int> down(n + 1, 0), up(n + 1, 0);

    vector<pair<int, int>> work;

    int Move_cnt = 0;

    auto to_zero = [&](int val, int to) -> void {
        down[up[val]] = -to - 1;
        base[to] = up[val];
        up[val] = 0;
    };

    auto Move = [&](int val, int to) -> int {
        int now = up[val], cnt = 0;
        while(now != 0) {
            cnt ++;
            now = up[now];
        }
        if(!cnt) return 0;
        work.push_back({up[val], to + 1});
        if(base[to] == 0) to_zero(val, to);
        else {
            int now = base[to];
            while(up[now] != 0) {
                now = up[now];
            }
            up[now] = up[val];
            down[up[val]] = now;
            up[val] = 0;
        }
        return cnt + 1;
    };

    for(int i = 0; i < m; i ++) {
        base[i] = a[i][0];
        down[a[i][0]] = - i - 1;
        for(int j = 1; j < 20; j ++) {
            down[a[i][j]] = a[i][j - 1];
            up[a[i][j - 1]] = a[i][j];
        }
    }
    for(int i = 1; i <= n; i ++) {
        vector<int> M(m, inf);
        rep(j, m) {
            int now = base[j];
            while(now != 0) {
                chmin(M[j], now);
                now = up[now];
            }
        }
        int idm = 0;
        rep(j, m) {
            if(M[j] > M[idm]) idm = j;
        }

        vector<int> v;
        {
            int now = i;
            while(now != 0) {
                if(now <= i + th) v.pb(now);
                now = up[now];
            }
        }
        reverse(all(v));
        foa(I, v) {
            Move_cnt += Move(I, idm); // Iより上をidmへ。
            if(I == i) {
                if(down[i] < 0) {
                    base[-down[i] - 1] = 0;
                    work.push_back({i, 0});
                } else {
                    up[down[i]] = 0;
                    work.push_back({i, 0});
                }
                continue;
            } 
            int id = -1, Min = inf * 10;
            rep(j, m) if(M[j] > I and M[j] < Min) {
                Min = M[j];
                id = j;
            }
            if(id == -1) return;
            Move_cnt += Move(down[I], id);
            M[id] = I;
        }
    }
    if(ans > Move_cnt) {
        ans = Move_cnt;
        ans_vec = work;
    }
}

void solve3(int seed, int th) {
    random_devicer rnd(seed);

    vector<int> base(m, 0);
    vector<int> down(n + 1, 0), up(n + 1, 0);

    vector<pair<int, int>> work;

    int Move_cnt = 0;

    auto to_zero = [&](int val, int to) -> void {
        down[up[val]] = -to - 1;
        base[to] = up[val];
        up[val] = 0;
    };

    auto Move = [&](int val, int to) -> int {
        int now = up[val], cnt = 0;
        while(now != 0) {
            cnt ++;
            now = up[now];
        }
        if(!cnt) return 0;
        work.push_back({up[val], to + 1});
        if(base[to] == 0) to_zero(val, to);
        else {
            int now = base[to];
            while(up[now] != 0) {
                now = up[now];
            }
            up[now] = up[val];
            down[up[val]] = now;
            up[val] = 0;
        }
        return cnt + 1;
    };

    for(int i = 0; i < m; i ++) {
        base[i] = a[i][0];
        down[a[i][0]] = - i - 1;
        for(int j = 1; j < 20; j ++) {
            down[a[i][j]] = a[i][j - 1];
            up[a[i][j - 1]] = a[i][j];
        }
    }
    for(int i = 1; i <= n; i ++) {
        vector<int> M(m, inf);
        rep(j, m) {
            int now = base[j];
            while(now != 0) {
                chmin(M[j], now);
                now = up[now];
            }
        }
        int idm = 0;
        rep(j, m) {
            if(M[j] > M[idm]) idm = j;
        }

        vector<int> v;
        {
            int now = i;
            while(now != 0) {
                if(now <= i + 8) v.pb(now);
                else if(now <= i + th and rnd.next(0, i + th + 1 - now) != 0) v.pb(now);
                now = up[now];
            }
        }
        reverse(all(v));
        foa(I, v) {
            Move_cnt += Move(I, idm); // Iより上をidmへ。
            if(I == i) {
                if(down[i] < 0) {
                    base[-down[i] - 1] = 0;
                    work.push_back({i, 0});
                } else {
                    up[down[i]] = 0;
                    work.push_back({i, 0});
                }
                continue;
            } 
            int id = -1, Min = inf * 10;
            rep(j, m) if(M[j] > I and M[j] < Min) {
                Min = M[j];
                id = j;
            }
            if(id == -1) return;
            Move_cnt += Move(down[I], id);
            M[id] = I;
        }
    }
    if(ans > Move_cnt) {
        ans = Move_cnt;
        ans_vec = work;
    }
}

void solve() {
    const int INF = 1e+9;
    int N, M;
    vector<vector<int>> b;
    vector<int> pos;
    vector<bool> is_top;
    int nv = 0;
    vector<pair<int, int>> Wo;
    int w_ans = 0;


    auto vec = a;
    
    auto op = [&](int val, int i) -> void {
        val ++;
        int ret = 0;
        int idi = -1, idj = -1;
        rep(j, 10) {
            int s = vec[j].size();
            rep(k, s) {
                if(vec[j][k] == val) {
                    idi = j;
                    idj = k;
                    break;
                }
            }
        } 
        vector<int> tmp;
        int s = vec[idi].size();
        for(int j = idj; j < s; j ++) tmp.pb(vec[idi][j]);
        
        w_ans += tmp.size() + 1;
        for(int j = s - 1; j >= idj; j --) vec[idi].pop_back();
        foa(e, tmp) vec[i].pb(e);
    };

    N=n;
    M=m;
    b.resize(M);
    pos.assign(N, -1);
    is_top.assign(N, false);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N/M; j++) {
            int x= a[i][j];
            x--;
            b[i].push_back(x);
            pos[x] = i;
        }
        is_top[b[i].back()] = true;
    }
    
    auto op1 = [&](int v, int i) -> void {
        if (pos[v] == i) return;
        if (b[i].size() > 0) is_top[b[i].back()] = false;
        int p = pos[v];
        vector<int> V;
        while (V.empty() || V.back() != v) {
            V.push_back(b[p].back());
            b[p].pop_back();
        }
        if (b[p].size() > 0) is_top[b[p].back()] = true;
        while (!V.empty()) {
            int w = V.back();
            V.pop_back();
            pos[w] = i;
            b[i].push_back(w);
        }
        is_top[b[i].back()] = true;
        Wo.push_back({v+1, i+1});
        op(v, i);
    };
    auto op2 = [&]() -> void {
        is_top[nv] = false;
        b[pos[nv]].pop_back();
        if (b[pos[nv]].size() > 0) is_top[b[pos[nv]].back()] = true;
        pos[nv] = -1;
        Wo.push_back({nv+1,0});
        nv++;
    };


    for (int i = 0; i < M; i++) {
        vector<pair<int,int>> Q;
        for (int j = 0; j < M; j++) if (j != i) Q.push_back(make_pair(-1, j));
        sort(Q.begin(), Q.end());
        vector<int> W;
        for (int v : b[i]) W.push_back(v);

        while (b[i].size() > 0) {
            vector<int> V;
            int k = b[i].size()-1;
            V.push_back(b[i][k]);
            while (k > 0 && b[i][k-1] < b[i][k]) {
                k--;
                V.push_back(b[i][k]);
            }
            for (int k = (int)Q.size()-1; k >= 0; k--) {
                int l = Q[k].first, j = Q[k].second;
                if (l < V.back() || k == 0) {
                    Q[k].first = V[0];
                    op1(V.back(), j);
                    break;
                }
            }
            sort(Q.begin(), Q.end());
        }

        sort(W.begin(), W.end());
        for (auto it = W.rbegin(); it != W.rend(); it++) {
            if (pos[*it] != i) op1(*it, i);
        }
    }

    while (nv < N) {
        if (!is_top[nv]) {
            int p = pos[nv];
            int k = b[p].size() - 1;
            while (b[p][k-1] != nv) k--;
            for (int l = 0; l < M; l++) if (l != p && (b[l].back() > b[p][k] || l == M-1 || (p == M-1 && l == M-2))) {
                op1(b[p][k], l);
                break;
            }
        }
        op2();
    }

    {
        if(w_ans < ans) {
            ans = w_ans;
            ans_vec = Wo;
        }
    }
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    auto t_start = chrono::system_clock::now();
    int seed = 0;

    cin >> n >> m;
    a.resize(10, vector<int> (20, 0));
    rep(i, 10) rep(j, 20) cin >> a[i][j];
    
    solve0();
    solve1();
    for(int th = 9; th <= 30; th ++) {
        solve2(th);
    }
    solve();


    for(int th = 9; th <= 30; th ++) {
        while(1) {
            auto t_now = chrono::system_clock::now();
            auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
            if (dt > (th - 7) * 80) break;
            solve3(seed ++, th);
        }
    }
    while(1) {
        auto t_now = chrono::system_clock::now();
        auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
        if (dt > 1995) break;
        for(int th = 20; th <= 24; th ++) {
            solve3(seed ++, th);
        }
    }
    for(auto [x, y] : ans_vec) {
        cout << x << " " << y << endl;
    }
    return 0;
}
