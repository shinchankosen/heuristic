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

void solve4(int seed, int th) {
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
                else if(now <= i + th and rnd.next(0, 1) != 0) v.pb(now);
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

void solve5(int seed) {
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
                else if(now <= i + 15 and rnd.next(0, 1) != 0) v.pb(now);
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

    for(int th = 9; th <= 30; th ++) {
        while(1) {
            auto t_now = chrono::system_clock::now();
            auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
            if (dt > (th - 7) * 85) break;
            solve3(seed ++, th);
        }
    }
    for(auto [x, y] : ans_vec) {
        cout << x << " " << y << endl;
    }
    return 0;
}
