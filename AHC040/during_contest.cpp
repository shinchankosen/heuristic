#include <bits/stdc++.h>
using namespace std;
#define all(v) (v).begin(),(v).end()
#define pb(a) push_back(a)
#define rep(i, n) for(int i=0;i<n;i++)
#define foa(e, v) for(auto& e : v)
using ll = long long;
const int inf = 1000000000;
const ll INF = inf;

using P = pair<ll, ll>;
using T = tuple<int, int, char, int>;
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


const bool VIS_FLAG = 0;////////////
ll debug_ans = INF;
int max_state = -1, state = -1;
vector<T> past_ans;

pair<ll, ll> eval(vector<ll> w, vector<ll> h, vector<T> &ans) {
    int n = w.size();
    vector<ll> x(n, -1), y(n, -1);
    for(auto [i, turn, c, p] : ans) {
        if(turn) swap(w[i], h[i]);
        if(c == 'L') {
            if(p == -1) {
                y[i] = 0;
            } else {
                y[i] = h[p] + y[p];
            }
            x[i] = 0LL;
            rep(j, i) {
                if(min(y[i] + h[i], y[j] + h[j]) - max(y[i], y[j]) > 0LL) {
                    x[i] = max(x[i], x[j] + w[j]);
                }
            }
        } else {
            if(p == -1) {
                x[i] = 0;
            } else {
                x[i] = w[p] + x[p];
            }
            y[i] = 0LL;
            rep(j, i) {
                if(min(x[i] + w[i], x[j] + w[j]) - max(x[i], x[j]) > 0LL) {
                    y[i] = max(y[i], y[j] + h[j]);
                }
            }
        }
    }

    ll Maxx = 0, Maxy = 0;
    rep(i, n) {
        Maxx = max(Maxx, x[i] + w[i]);
        Maxy = max(Maxy, y[i] + h[i]);
    }
    return {Maxx, Maxy};
}


vector<ll> w_true, h_true, w_diff, h_diff;
int vis_phase = 0;
pair<ll, ll> In_ans() {
    if(VIS_FLAG) {
        auto [a, b] = eval(w_true, h_true, past_ans);
        a += w_diff[vis_phase];
        b += h_diff[vis_phase];
        vis_phase ++;
        if(debug_ans > a + b) {
            max_state = state;
            debug_ans = a + b;
        }
        return {a, b};
    }
    ll a, b;
    cin >> a >> b;
    if(debug_ans > a + b) {
        max_state = state;
        debug_ans = a + b;
    }
    return {a, b};
}



void chmin(pair<ll, ll> &a, pair<ll, ll> b) {a = min(a, b);}

int tim = -1;
vector<ll> vv = {2000, 1000, 500, 300, 200}, vb = {5000, 3500, 2000, 1500, 1200};

void solver1(vector<ll> w, vector<ll> h, int t, int lim = 10000) {

    int n = w.size();
    random_devicer rnd(t);
    ll le1 = 0, ri1 = 0;
    ll le2 = 0, ri2 = 0;
    rep(i, n) {
        le1 = max(le1, min(w[i], h[i]));
        ri1 = max(ri1, max(w[i], h[i]));
        le2 = ri1;
        ri2 += max(w[i], h[i]);
    }
    ri2 /= int(sqrt(n));
    ri2 *= 3;
    if(ri2 < le2) ri2 = le2;
    
    ll bai = rnd.next(7000, 9000);
    rep(__, tim) {
        ll ans = INF, yokoT = INF, tateT = INF;
        rep(_, lim) {
            ll yoko = rnd.next(le1, ri1);
            ll tate = rnd.next(le2, ri2);
            ll num = 0;
            ll sum = tate;
            auto process = [&](ll H) -> void {
                if(sum + H > tate) {
                    sum = H;
                    num ++;
                } else {
                    sum += H;
                }
            };
            rep(i, n) {
                if(w[i] <= yoko) {
                    if(h[i] <= yoko and h[i] > w[i]) {
                        process(w[i]);
                    } else {
                        process(h[i]);
                    }
                } else {
                    assert(h[i] <= yoko);
                    process(w[i]);
                }
            }
            if(num * yoko * bai / 10000LL + tate < ans) {
                ans = num * yoko * bai / 10000LL + tate;
                yokoT = yoko;
                tateT = tate;
            }
        }
        cout << n << endl;
        ll sum = tateT, num = 0;
        past_ans.clear();
        auto Out = [&](int i, bool turn, bool over) -> void {
            past_ans.push_back({i, (turn ? 1 : 0), 'L', (over ? -1 : i - 1)});
            cout << i << " " << (turn ? 1 : 0) << " L " << (over ? -1 : i - 1) << endl;
        };
        rep(i, n) {
            if(w[i] <= yokoT) {
                if(h[i] <= yokoT and h[i] > w[i]) {
                    if(sum + w[i] > tateT) {
                        Out(i, 1, 1);
                        sum = w[i];
                        num ++;
                    } else {
                        Out(i, 1, 0);
                        sum += w[i];
                    }
                } else {
                    if(sum + h[i] > tateT) {
                        Out(i, 0, 1);
                        sum = h[i];
                        num ++;
                    } else {
                        Out(i, 0, 0);
                        sum += h[i];
                    }
                }
            } else {
                assert(h[i] <= yokoT);
                if(sum + w[i] > tateT) {
                    Out(i, 1, 1);
                    sum = w[i];
                    num ++;
                } else {
                    Out(i, 1, 0);
                    sum += w[i];
                }
            }
        }
        auto [A, B] = In_ans();
        if(A > yokoT * bai * num) {
            bai += vv[__];
        } else {
            bai -= vv[__];
        }
    }

}


void solver(vector<ll> w, vector<ll> h, ll sigma, int seed, int lim) {
    sigma *= 2;
    int n = w.size();
    random_devicer rnd(seed);
    
    ll le1 = 0, ri1 = 0;
    ll le2 = 0, ri2 = 0;
    rep(i, n) {
        le1 = max(le1, min(w[i], h[i]));
        ri1 = max(ri1, max(w[i], h[i]));
        le2 = ri1;
        ri2 += max(w[i], h[i]);
    }
    ri2 /= int(sqrt(n));
    ri2 *= 5;
    if(ri2 < le2) ri2 = le2;
    ri1 *= 3;
    

    ll bai = 8500 + rnd.next(-1500, 1500);
    rep(__, tim) {
        
        ll ans = INF, yokoT = INF, tateT = INF; 
        rep(_, lim) {
            ll yoko = rnd.next(le1, ri1);
            ll tate = rnd.next(le2, ri2);
            // turn, 前のやつの下 or 横, Uもあるな...
            // dp?
            // 復元方法
            vector<pair<ll, ll>> dp(n + 1, {INF, INF});
            dp[0] = {0, tate};

            auto dfs = [&](auto &&dfs, int i, int j, vector<ll> &y, ll Max, ll sum) -> void {
                auto [A, B] = dp[i];
                if(sum > yoko) return;
                if(Max + B > tate) {
                    if(i == 0) chmin(dp[j], {A + 1, Max});
                    else {
                        bool ng = 0;
                        foa(e, y) {
                            if(e != Max and Max - sigma < e) {
                                ng = 1;
                                break;
                            }
                        }
                        if(!ng) chmin(dp[j], {A + 1, Max});
                    }
                } else {
                    if(A == 1) chmin(dp[j], {A, B + Max});
                    else {
                        bool ng = 0;
                        foa(e, y) {
                            if(e != Max and Max - sigma < e) {
                                ng = 1;
                                break;
                            }
                        }
                        if(!ng) chmin(dp[j], {A, B + Max});
                    }
                }
                if(i + 4 <= j) return;
                if(j == n) return;
                {
                    y.pb(w[j]);
                    dfs(dfs, i, j + 1, y, max(Max, w[j]), sum + h[j]);
                    y.pop_back();
                }
                {
                    y.pb(h[j]);
                    dfs(dfs, i, j + 1, y, max(Max, h[j]), sum + w[j]);
                    y.pop_back();
                }
                return ;
            };
            vector<ll> y;
            for(int i = 0; i < n; i ++) {
                y.clear();
                dfs(dfs, i, i, y, 0, 0);
            }
            auto [A, B] = dp[n];
            if(A * yoko * bai / 10000LL + tate < ans) {
                ans = A * yoko * bai / 10000LL + tate;
                yokoT = yoko;
                tateT = tate;
            }
        }
        
        { // output
            ll yoko = yokoT, tate = tateT;
            vector<pair<ll, ll>> dp(n + 1, {INF, INF});
            dp[0] = {0, tate};
            vector<int> p(n + 1, -1);

            vector<int> max_id(n + 1, -1);
            auto dfs = [&](auto &&dfs, int i, int j, vector<ll> &y, ll Max, ll sum) -> void {
                auto [A, B] = dp[i];
                if(sum > yoko) return;
                if(Max + B > tate) {
                    P val = {A + 1, Max};
                    if(i == 0) {
                        if(dp[j] > val) {
                            dp[j] = val;
                            p[j] = i;
                        }
                    }
                    else {
                        bool ng = 0;
                        int ii = -1;
                        rep(id, j - i) {
                            ll e = y[id];
                            if(e != Max and Max - sigma < e) {
                                ng = 1;
                                break;
                            }
                            if(e == Max) ii = id; 
                        }
                        if(!ng and dp[j] > val) {
                            dp[j] = val;
                            p[j] = i;
                            max_id[j] = ii + i;
                        }
                    }
                } else {
                    P val = {A, B + Max};
                    if(A == 1) {
                        if(dp[j] > val) {
                            dp[j] = val;
                            p[j] = i;
                        }
                    }
                    else {
                        bool ng = 0;
                        int ii = -1;
                        rep(id, j - i) {
                            ll e = y[id];
                            if(e != Max and Max - sigma < e) {
                                ng = 1;
                                break;
                            }
                            if(e == Max) ii = id; 
                        }
                        if(!ng and dp[j] > val) {
                            dp[j] = val;
                            p[j] = i;
                            max_id[j] = ii + i;
                        }
                    }
                }
                if(i + 4 <= j) return;
                if(j == n) return;
                {
                    y.pb(w[j]);
                    dfs(dfs, i, j + 1, y, max(Max, w[j]), sum + h[j]);
                    y.pop_back();
                }
                {
                    y.pb(h[j]);
                    dfs(dfs, i, j + 1, y, max(Max, h[j]), sum + w[j]);
                    y.pop_back();
                }
                return ;
            };
            vector<ll> y;
            for(int i = 0; i < n; i ++) {
                y.clear();
                dfs(dfs, i, i, y, 0, 0);
            }
            y.clear();
            vector<int> nx(n, -1);
            {
                int now = n;
                while(now != 0) {
                    nx[p[now]] = now;
                    now = p[now];
                }
            }
            vector<T> op;
            auto Out = [&]() -> void {
                past_ans.clear();
                cout << n << endl;
                past_ans = op;
                assert(n == (int)op.size());
                for(auto [i, turn, c, p] : op) {
                    cout << i << " " << turn << " " << c << " " << p << endl;
                }
            };
            vector<int> vec;
            auto dfs1 = [&](auto &&dfs1, int i, int j, ll Max, ll sum) -> bool {
                auto [A, B] = dp[i];
                if(sum > yoko) return false;
                if(nx[i] == j) {
                    if(Max + B > tate) {
                        P val = {A + 1, Max};
                        if(i == 0) {
                            if(dp[j] == val) {
                                rep(k, j - i) {
                                    op.push_back({i + k, vec[k], 'L', -1});
                                }
                                return true;
                            }
                        }
                        else {
                            bool ng = 0;
                            foa(e, y) {
                                if(e != Max and Max - sigma < e) {
                                    ng = 1;
                                    break;
                                }
                            }
                            if(!ng and dp[j] == val) {
                                rep(k, j - i) {
                                    op.push_back({i + k, vec[k], 'L', -1});
                                }
                                return true;
                            }
                        }
                    } else {
                        P val = {A, B + Max};
                        if(A == 1) {
                            if(dp[j] == val) {
                                rep(k, j - i) {
                                    if(k == 0) op.push_back({i + k, vec[k], 'U', -1});
                                    else op.push_back({i + k, vec[k], 'U', i + k - 1});
                                }
                                return true;
                            }
                        }
                        else {
                            bool ng = 0;
                            foa(e, y) {
                                if(e != Max and Max - sigma < e) {
                                    ng = 1;
                                    break;
                                }
                            }
                            if(!ng and dp[j] == val) {
                                rep(k, j - i) {
                                    op.push_back({i + k, vec[k], 'L', max_id[i]}); ////
                                }
                                return true;
                            } 
                        }
                    }
                }
                    
                if(nx[i] == j) return false;
                if(i + 4 <= j) return false;
                if(j == n) return false;

                bool ret = 0;
                {
                    vec.push_back(1);
                    y.pb(w[j]);
                    ret |= dfs1(dfs1, i, j + 1, max(Max, w[j]), sum + h[j]);
                    y.pop_back();
                    if(ret) return true;
                    vec.pop_back();
                }
                {
                    vec.push_back(0);
                    y.pb(h[j]);
                    ret |= dfs1(dfs1, i, j + 1, max(Max, h[j]), sum + w[j]);
                    y.pop_back();
                    if(ret) return true;
                    vec.pop_back();
                }
                return false;
            };
            for(int i = 0; i < n; i ++) {
                y.clear();
                vec.clear();
                if(nx[i] != -1) dfs1(dfs1, i, i, 0, 0);
            }
            Out();
        }
        auto [A, B] = In_ans();
        if(A > B * 2) {
            sigma += vb[__] * 2;
            bai += vv[__];
        } else if(A > ans - tateT) {
            bai += vv[__];
            sigma += vb[__];
        } else {
            bai -= vv[__];
            sigma -= vb[__];
        }
    }
}

void solve_bin1(vector<ll> w, vector<ll> h, int t) {

    int n = w.size();
    random_devicer rnd(t);
    ll le1 = 0, ri1 = 0;
    ll le2 = 0, ri2 = 0;
    rep(i, n) {
        le1 = max(le1, min(w[i], h[i]));
        ri1 = max(ri1, max(w[i], h[i]));
        le2 = ri1;
        ri2 += max(w[i], h[i]);
    }
    ri2 /= int(sqrt(n));
    ri2 *= 3;
    if(ri2 < le2) ri2 = le2;
    
    ll bai = 8000;
    int lim = 10000;

    rep(__, 3) {
        ll ans = INF, yokoT = INF;
        ll tateT = (le2 + ri2) / 2;
        rep(_, lim) {
            ll yoko = rnd.next(le1, ri1);
            ll tate = tateT;
            ll num = 0;
            ll sum = tate;
            auto process = [&](ll H) -> void {
                if(sum + H > tate) {
                    sum = H;
                    num ++;
                } else {
                    sum += H;
                }
            };
            rep(i, n) {
                if(w[i] <= yoko) {
                    if(h[i] <= yoko and h[i] > w[i]) {
                        process(w[i]);
                    } else {
                        process(h[i]);
                    }
                } else {
                    assert(h[i] <= yoko);
                    process(w[i]);
                }
            }
            if(num * yoko * bai / 10000LL + tate < ans) {
                ans = num * yoko * bai / 10000LL + tate;
                yokoT = yoko;
                tateT = tate;
            }
        }
        cout << n << endl;
        ll sum = tateT, num = 0;
        past_ans.clear();
        auto Out = [&](int i, bool turn, bool over) -> void {
            past_ans.push_back({i, (turn ? 1 : 0), 'L', (over ? -1 : i - 1)});
            cout << i << " " << (turn ? 1 : 0) << " L " << (over ? -1 : i - 1) << endl;
        };
        rep(i, n) {
            if(w[i] <= yokoT) {
                if(h[i] <= yokoT and h[i] > w[i]) {
                    if(sum + w[i] > tateT) {
                        Out(i, 1, 1);
                        sum = w[i];
                        num ++;
                    } else {
                        Out(i, 1, 0);
                        sum += w[i];
                    }
                } else {
                    if(sum + h[i] > tateT) {
                        Out(i, 0, 1);
                        sum = h[i];
                        num ++;
                    } else {
                        Out(i, 0, 0);
                        sum += h[i];
                    }
                }
            } else {
                assert(h[i] <= yokoT);
                if(sum + w[i] > tateT) {
                    Out(i, 1, 1);
                    sum = w[i];
                    num ++;
                } else {
                    Out(i, 1, 0);
                    sum += w[i];
                }
            }
        }
        auto [A, B] = In_ans();
        if(A > B) {
            le2 = tateT;
        } else {
            ri2 = tateT;
        }
    }

}

void solver2(vector<ll> w, vector<ll> h, ll sigma, int seed, int lim) {
    sigma *= 2;
    int n = w.size();
    random_devicer rnd(seed);
    
    ll le1 = 0, ri1 = 0;
    ll le2 = 0, ri2 = 0;
    rep(i, n) {
        le1 = max(le1, min(w[i], h[i]));
        ri1 = max(ri1, max(w[i], h[i]));
        le2 = ri1;
        ri2 += max(w[i], h[i]);
    }
    ri2 /= int(sqrt(n));
    ri2 *= 3 / 2;
    if(ri2 < le2) ri2 = le2;
    ri1 *= 3;
    

    ll bai = 8500 + rnd.next(-1000, 1000);
    rep(__, tim) {
        ll ans = INF, yokoT = INF, tateT = (ri2 + le2) / 2; 
        rep(_, lim) {
            ll yoko = rnd.next(le1, ri1);
            ll tate = tateT;
            // turn, 前のやつの下 or 横, Uもあるな...
            // dp?
            // 復元方法
            vector<pair<ll, ll>> dp(n + 1, {INF, INF});
            dp[0] = {0, tate};

            auto dfs = [&](auto &&dfs, int i, int j, vector<ll> &y, ll Max, ll sum) -> void {
                auto [A, B] = dp[i];
                if(sum > yoko) return;
                if(Max + B > tate) {
                    if(i == 0) chmin(dp[j], {A + 1, Max});
                    else {
                        bool ng = 0;
                        foa(e, y) {
                            if(e != Max and Max - sigma < e) {
                                ng = 1;
                                break;
                            }
                        }
                        if(!ng) chmin(dp[j], {A + 1, Max});
                    }
                } else {
                    if(A == 1) chmin(dp[j], {A, B + Max});
                    else {
                        bool ng = 0;
                        foa(e, y) {
                            if(e != Max and Max - sigma < e) {
                                ng = 1;
                                break;
                            }
                        }
                        if(!ng) chmin(dp[j], {A, B + Max});
                    }
                }
                if(i + 5 <= j) return;
                if(j == n) return;
                {
                    y.pb(w[j]);
                    dfs(dfs, i, j + 1, y, max(Max, w[j]), sum + h[j]);
                    y.pop_back();
                }
                {
                    y.pb(h[j]);
                    dfs(dfs, i, j + 1, y, max(Max, h[j]), sum + w[j]);
                    y.pop_back();
                }
                return ;
            };
            vector<ll> y;
            for(int i = 0; i < n; i ++) {
                y.clear();
                dfs(dfs, i, i, y, 0, 0);
            }
            auto [A, B] = dp[n];
            if(A * yoko * bai / 10000LL + tate < ans) {
                ans = A * yoko * bai / 10000LL + tate;
                yokoT = yoko;
                tateT = tate;
            }
        }
        
        { // output
            ll yoko = yokoT, tate = tateT;
            vector<pair<ll, ll>> dp(n + 1, {INF, INF});
            dp[0] = {0, tate};
            vector<int> p(n + 1, -1);

            vector<int> max_id(n + 1, -1);
            auto dfs = [&](auto &&dfs, int i, int j, vector<ll> &y, ll Max, ll sum) -> void {
                auto [A, B] = dp[i];
                if(sum > yoko) return;
                if(Max + B > tate) {
                    P val = {A + 1, Max};
                    if(i == 0) {
                        if(dp[j] > val) {
                            dp[j] = val;
                            p[j] = i;
                        }
                    }
                    else {
                        bool ng = 0;
                        int ii = -1;
                        rep(id, j - i) {
                            ll e = y[id];
                            if(e != Max and Max - sigma < e) {
                                ng = 1;
                                break;
                            }
                            if(e == Max) ii = id; 
                        }
                        if(!ng and dp[j] > val) {
                            dp[j] = val;
                            p[j] = i;
                            max_id[j] = ii + i;
                        }
                    }
                } else {
                    P val = {A, B + Max};
                    if(A == 1) {
                        if(dp[j] > val) {
                            dp[j] = val;
                            p[j] = i;
                        }
                    }
                    else {
                        bool ng = 0;
                        int ii = -1;
                        rep(id, j - i) {
                            ll e = y[id];
                            if(e != Max and Max - sigma < e) {
                                ng = 1;
                                break;
                            }
                            if(e == Max) ii = id; 
                        }
                        if(!ng and dp[j] > val) {
                            dp[j] = val;
                            p[j] = i;
                            max_id[j] = ii + i;
                        }
                    }
                }
                if(i + 5 <= j) return;
                if(j == n) return;
                {
                    y.pb(w[j]);
                    dfs(dfs, i, j + 1, y, max(Max, w[j]), sum + h[j]);
                    y.pop_back();
                }
                {
                    y.pb(h[j]);
                    dfs(dfs, i, j + 1, y, max(Max, h[j]), sum + w[j]);
                    y.pop_back();
                }
                return ;
            };
            vector<ll> y;
            for(int i = 0; i < n; i ++) {
                y.clear();
                dfs(dfs, i, i, y, 0, 0);
            }
            y.clear();
            vector<int> nx(n, -1);
            {
                int now = n;
                while(now != 0) {
                    nx[p[now]] = now;
                    now = p[now];
                }
            }
            vector<T> op;
            auto Out = [&]() -> void {
                past_ans.clear();
                cout << n << endl;
                past_ans = op;
                assert(n == (int)op.size());
                for(auto [i, turn, c, p] : op) {
                    cout << i << " " << turn << " " << c << " " << p << endl;
                }
            };
            vector<int> vec;
            auto dfs1 = [&](auto &&dfs1, int i, int j, ll Max, ll sum) -> bool {
                auto [A, B] = dp[i];
                if(sum > yoko) return false;
                if(nx[i] == j) {
                    if(Max + B > tate) {
                        P val = {A + 1, Max};
                        if(i == 0) {
                            if(dp[j] == val) {
                                rep(k, j - i) {
                                    op.push_back({i + k, vec[k], 'L', -1});
                                }
                                return true;
                            }
                        }
                        else {
                            bool ng = 0;
                            foa(e, y) {
                                if(e != Max and Max - sigma < e) {
                                    ng = 1;
                                    break;
                                }
                            }
                            if(!ng and dp[j] == val) {
                                rep(k, j - i) {
                                    op.push_back({i + k, vec[k], 'L', -1});
                                }
                                return true;
                            }
                        }
                    } else {
                        P val = {A, B + Max};
                        if(A == 1) {
                            if(dp[j] == val) {
                                rep(k, j - i) {
                                    if(k == 0) op.push_back({i + k, vec[k], 'U', -1});
                                    else op.push_back({i + k, vec[k], 'U', i + k - 1});
                                }
                                return true;
                            }
                        }
                        else {
                            bool ng = 0;
                            foa(e, y) {
                                if(e != Max and Max - sigma < e) {
                                    ng = 1;
                                    break;
                                }
                            }
                            if(!ng and dp[j] == val) {
                                rep(k, j - i) {
                                    op.push_back({i + k, vec[k], 'L', max_id[i]}); ////
                                }
                                return true;
                            } 
                        }
                    }
                }
                    
                if(nx[i] == j) return false;
                if(i + 5 <= j) return false;
                if(j == n) return false;

                bool ret = 0;
                {
                    vec.push_back(1);
                    y.pb(w[j]);
                    ret |= dfs1(dfs1, i, j + 1, max(Max, w[j]), sum + h[j]);
                    y.pop_back();
                    if(ret) return true;
                    vec.pop_back();
                }
                {
                    vec.push_back(0);
                    y.pb(h[j]);
                    ret |= dfs1(dfs1, i, j + 1, max(Max, h[j]), sum + w[j]);
                    y.pop_back();
                    if(ret) return true;
                    vec.pop_back();
                }
                return false;
            };
            for(int i = 0; i < n; i ++) {
                y.clear();
                vec.clear();
                if(nx[i] != -1) dfs1(dfs1, i, i, 0, 0);
            }
            Out();
        }
        auto [A, B] = In_ans();
        if(B * yokoT * 3/2 < A * tateT) {
            sigma = min(sigma + 10000, sigma * 2);
            bai += 1000;
        }
        if(A > ans - tateT) {
            bai += vv[__];
            sigma += vb[__];
        } else {
            bai -= vv[__];
            sigma -= vb[__];
        }
        if(A > B) le2 = tateT;
        else ri2 = tateT;
    }
}

void last_solver(vector<ll> w, vector<ll> h, int seed, int lim, int th) {
    int n = w.size();
    random_devicer rnd(seed);
    
    ll le1 = 0, ri1 = 0;
    ll le2 = 0, ri2 = 0;
    rep(i, n) {
        le1 = max(le1, min(w[i], h[i]));
        ri1 = max(ri1, max(w[i], h[i]));
        le2 = ri1;
        ri2 += max(w[i], h[i]);
    }
    ri2 /= int(sqrt(n));
    ri2 *= 3 / 2;
    if(ri2 < le2) ri2 = le2;
    ri1 *= 2;
    
    ll rii = ri1 * 4;
    ll bai = 8500 + rnd.next(-1000, 1000);

    ll base = 3000;
    rep(__, tim) {
        ll ans = INF, yokoT1 = INF, tateT = INF, yokoT2 = INF;
        rep(_, lim) {
            ll yoko = rnd.next(le1, rii), yoko2 = rnd.next(le1, ri1);
            ll tate = rnd.next(le2, ri2);
            vector<ll> dp(n + 1, INF);
            dp[0] = 0;
            auto dfs = [&](auto &&dfs, int i, int j, ll Max, ll sum) -> void {
                if(sum > yoko) return;
                if(Max + dp[i] > tate) return;
                if(dp[j] > dp[i] + Max) dp[j] = dp[i] + Max;
                if(i + th <= j) return;
                if(j == n) return;
                dfs(dfs, i, j + 1, max(Max, w[j]), sum + h[j]);
                dfs(dfs, i, j + 1, max(Max, h[j]), sum + w[j]);
                return ;
            };
            int id = -1;
            for(int i = 0; i < n; i ++) {
                if(dp[i] == INF) break;
                id = i;
                dfs(dfs, i, i, 0, 0); 
            }
            ll num = 0;
            ll sum = tate;
            auto process = [&](ll H) -> void {
                if(sum + H > tate) {
                    sum = H;
                    num ++;
                } else {
                    sum += H;
                }
            };
            for(int i = id; i < n; i ++) {
                if(w[i] <= yoko2) {
                    if(h[i] <= yoko2 and h[i] > w[i]) {
                        process(w[i]);
                    } else {
                        process(h[i]);
                    } 
                } else {
                    process(w[i]);
                }
            }

            if((num * yoko2 + yoko) * bai / 10000LL + tate < ans) {
                ans = (num * yoko2 + yoko) * bai / 10000LL + tate;
                yokoT2 = yoko2;
                yokoT1 = yoko;
                tateT = tate;
            }
        }
        {
            ll yoko = yokoT1, yoko2 = yokoT2;
            ll tate = tateT;
            vector<ll> dp(n + 1, INF);
            vector<int> p(n + 1, -1);
            dp[0] = 0;
            auto dfs = [&](auto &&dfs, int i, int j, ll Max, ll sum) -> void {
                if(sum > yoko) return;
                if(Max + dp[i] > tate) return;
                if(dp[j] > dp[i] + Max) {
                    dp[j] = dp[i] + Max;
                    p[j] = i;
                }
                if(i + th <= j) return;
                if(j == n) return;
                dfs(dfs, i, j + 1, max(Max, w[j]), sum + h[j]);
                dfs(dfs, i, j + 1, max(Max, h[j]), sum + w[j]);
                return ;
            };
            int id = -1;
            for(int i = 0; i < n; i ++) {
                if(dp[i] == INF) break;
                id = i;
                dfs(dfs, i, i, 0, 0); 
            }

            vector<int> nx(n + 1, -1);
            {
                int now = id;
                while(now != 0) {
                    nx[p[now]] = now;
                    now = p[now];
                }
            }
            vector<int> turn;
            auto dfs1 = [&](auto &&dfs1, int i, int j, ll Max, ll sum) -> bool {
                if(sum > yoko) return false;
                if(Max + dp[i] > tate) return false;
                if(nx[i] == j and dp[j] == dp[i] + Max) {
                    return true;
                }
                if(nx[i] == j) return false;
                if(i + th <= j) return false;
                if(j == n) return false;

                turn.pb(1);
                bool ret = 0;
                ret |= dfs1(dfs1, i, j + 1, max(Max, w[j]), sum + h[j]);
                if(ret) return true;
                turn.pop_back();

                turn.pb(0);
                ret |= dfs1(dfs1, i, j + 1, max(Max, h[j]), sum + w[j]);
                if(ret) return true;
                turn.pop_back();

                return false;
            };
            vector<T> op;
            for(int i = 0; i < id; i ++) if(nx[i] != -1) {
                turn.clear();
                dfs1(dfs1, i, i, 0, 0);
                int z = nx[i] - i;
                assert((int)turn.size() == z);
                rep(k, z) {
                    if(k == 0) {
                        op.push_back({i + k, turn[k], 'U', -1});
                    } else {
                        op.push_back({i + k, turn[k], 'U', i + k - 1});
                    }
                }
            }

            ll num = 0;
            ll sum = tate;
            for(int i = id; i < n; i ++) {
                if(w[i] <= yoko2) {
                    if(h[i] <= yoko2 and h[i] > w[i]) {
                        if(sum + w[i] > tate) {
                            op.push_back({i, 1, 'L', -1});
                            sum = w[i];
                            num ++;
                        } else {
                            op.push_back({i, 1, 'L', i - 1});
                            sum += w[i];
                        }
                    } else {
                        if(sum + h[i] > tate) {
                            op.push_back({i, 0, 'L', -1});
                            sum = h[i];
                            num ++;
                        } else {
                            op.push_back({i, 0, 'L', i - 1});
                            sum += h[i];
                        }
                    }
                } else {
                    assert(h[i] <= yoko2);
                    if(sum + w[i] > tate) {
                        op.push_back({i, 1, 'L', -1});
                        sum = w[i];
                        num ++;
                    } else {
                        op.push_back({i, 1, 'L', i - 1});
                        sum += w[i];
                    }
                }
            }
            auto Out = [&]() -> void {
                past_ans.clear();
                cout << n << endl;
                past_ans = op;
                assert(n == (int)op.size());
                for(auto [i, turn, c, p] : op) {
                    cout << i << " " << turn << " " << c << " " << p << endl;
                }
            };
            Out();
            auto [A, B] = In_ans();
            if(A > B) {
                bai += base / (__ + 1);
            } else {
                bai -= base / (__ + 1);
            }
        }
    }
}


void solve1(vector<ll> w, vector<ll> h, int n, int sigma, int t, int lim) {
    rep(i, n) {
        w[i] = clamp(w[i], 1LL, INF);
        h[i] = clamp(h[i], 1LL, INF);
    }
    solver2(w, h, sigma, t + 42, lim);
    return;
}


// 同じようなやつを集める

void solve(vector<ll> w, vector<ll> h, int n, int mu, int sigma, int t, int lim) {
    random_devicer rnd(t); 
    rep(i, n) {
        w[i] += rnd.next(mu, mu + sigma);
        h[i] += rnd.next(mu, mu + sigma);
        w[i] = clamp(w[i], 1LL, INF);
        h[i] = clamp(h[i], 1LL, INF);
    }
    solver(w, h, sigma, t + 41, lim);
    return;
}


void solvefast(vector<ll> w, vector<ll> h, int n, int mu, int sigma, int t, int lim) {
    random_devicer rnd(t); 
    rep(i, n) {
        w[i] += rnd.next(mu - sigma, mu + sigma);
        h[i] += rnd.next(mu - sigma, mu + sigma);
        w[i] = clamp(w[i], 1LL, INF);
        h[i] = clamp(h[i], 1LL, INF);
    }
    solver1(w, h, t + 2, lim);
    return;
}

void last_solve(vector<ll> w, vector<ll> h, int n, int mu, int sigma, int t, int lim, int th = 9) {
    random_devicer rnd(t); 
    rep(i, n) {
        w[i] += rnd.next(mu, mu + sigma);
        h[i] += rnd.next(mu, mu + sigma);
        w[i] = clamp(w[i], 1LL, INF);
        h[i] = clamp(h[i], 1LL, INF);
    }
    last_solver(w, h, t + 4, lim, th);
    return;
}


void bokuga_kangaeta_saikyono_algorithm(vector<ll> w, vector<ll> h, int sigma, int seed, int lim, int diff_th) {
    int n = w.size();
    rep(i, n) {
        w[i] = clamp(w[i], 1LL, INF);
        h[i] = clamp(h[i], 1LL, INF);
    }

    random_devicer rnd(seed);

    ll ley = 0, riy = 0, let = 0, rit = 0;
    rep(i, n) {
        ley = max(ley, min(w[i], h[i]));
        let = max({let, w[i], h[i]});
    }
    {
        ll sum = 0;
        rep(i, n) sum += w[i] + h[i];
        sum /= (int) sqrt(n);
        sum *= 2;
        riy = rit = sum;
    }

    auto Out = [&]() -> void {
        // return;
        cout << n << endl;
        for(auto [i, turn, c, p] : past_ans) {
            cout << i << " " << turn << " " << c << " " << p << endl;
        }
    };

    rep(times, tim) {
        ll ans = INF, yoko_opt = INF, tate_opt = INF;
        rep(_, lim) {
            ll yoko = rnd.next(ley, riy);
            ll tate = rnd.next(let, rit);
            vector<ll> dp1(n + 1, INF);
            dp1[0] = 0;
            for(int i = 0; i < n; i ++) {
                ll sum_min = 0;
                vector<pair<ll, ll>> v;
                for(int j = i + 1; j <= n; j ++) {
                    if(j - i > diff_th) break;
                    sum_min += min(w[j - 1], h[j - 1]);
                    if(sum_min > yoko) break;
                    v.push_back({max(w[j - 1], h[j - 1]), min(w[j - 1], h[j - 1])});
                    {
                        int id = v.size() - 1;
                        while(id > 0 and v[id] > v[id - 1]) {
                            swap(v[id], v[id - 1]);
                            id --;
                        }
                    }
                    auto V = v;
                    ll sum = sum_min;
                    for(int k = v.size() - 1; k >= 0; k --) {
                        if(sum + V[k].first - V[k].second > yoko) break;
                        sum += V[k].first - V[k].second;
                        swap(V[k].first, V[k].second);
                    }
                    ll Max = 0;
                    for(auto [x, y] : V) Max = max(Max, x);
                    dp1[j] = min(dp1[j], dp1[i] + Max);
                }
            }
            
            vector<ll> dp2(n + 1, INF);
            int idx = -1;
            rep(i, n + 1) if(dp1[i] <= tate) idx = i;
            dp2[idx] = yoko;
            
            for(int i = idx; i < n; i ++) {
                ll sum_min = 0;
                vector<pair<ll, ll>> v;
                for(int j = i + 1; j <= n; j ++) {
                    if(j - i > diff_th) break;
                    sum_min += min(w[j - 1], h[j - 1]);
                    if(sum_min > tate) break;
                    v.push_back({max(w[j - 1], h[j - 1]), min(w[j - 1], h[j - 1])});
                    {
                        int id = v.size() - 1;
                        while(id > 0 and v[id] > v[id - 1]) {
                            swap(v[id], v[id - 1]);
                            id --;
                        }
                    }
                    auto V = v;
                    ll sum = sum_min;
                    for(int k = v.size() - 1; k >= 0; k --) {
                        if(sum + V[k].first - V[k].second > tate) break;
                        sum += V[k].first - V[k].second;
                        swap(V[k].first, V[k].second);
                    }
                    ll Max = 0;
                    for(auto [x, y] : V) Max = max(Max, x);
                    dp2[j] = min(dp2[j], dp2[i] + Max);
                }
            }
            if(ans > dp2[n] + tate) {
                ans = dp2[n] + tate;
                yoko_opt = yoko;
                tate_opt = tate;
            }
        }{
            ll yoko = yoko_opt;
            ll tate = tate_opt;
            vector<int> p(n + 1, -1);
            
            vector<ll> dp1(n + 1, INF);
            dp1[0] = 0;
            for(int i = 0; i < n; i ++) {
                ll sum_min = 0;
                vector<pair<ll, ll>> v;
                for(int j = i + 1; j <= n; j ++) {
                    if(j - i > diff_th) break;
                    sum_min += min(w[j - 1], h[j - 1]);
                    if(sum_min > yoko) break;
                    v.push_back({max(w[j - 1], h[j - 1]), min(w[j - 1], h[j - 1])});
                    {
                        int id = v.size() - 1;
                        while(id > 0 and v[id] > v[id - 1]) {
                            swap(v[id], v[id - 1]);
                            id --;
                        }
                    }
                    auto V = v;
                    ll sum = sum_min;
                    for(int k = v.size() - 1; k >= 0; k --) {
                        if(sum + V[k].first - V[k].second > yoko) break;
                        sum += V[k].first - V[k].second;
                        swap(V[k].first, V[k].second);
                    }
                    ll Max = 0;
                    for(auto [x, y] : V) Max = max(Max, x);
                    if(dp1[j] > dp1[i] + Max) {
                        dp1[j] = dp1[i] + Max; 
                        p[j] = i;
                    }
                }
            }
            
            vector<ll> dp2(n + 1, INF);
            int idx = -1;
            rep(i, n + 1) if(dp1[i] <= tate) idx = i;
            dp2[idx] = yoko;
            
            for(int i = idx; i < n; i ++) {
                ll sum_min = 0;
                vector<pair<ll, ll>> v;
                for(int j = i + 1; j <= n; j ++) {
                    if(j - i > diff_th) break;
                    sum_min += min(w[j - 1], h[j - 1]);
                    if(sum_min > tate) break;
                    v.push_back({max(w[j - 1], h[j - 1]), min(w[j - 1], h[j - 1])});
                    {
                        int id = v.size() - 1;
                        while(id > 0 and v[id] > v[id - 1]) {
                            swap(v[id], v[id - 1]);
                            id --;
                        }
                    }
                    auto V = v;
                    ll sum = sum_min;
                    for(int k = v.size() - 1; k >= 0; k --) {
                        if(sum + V[k].first - V[k].second > tate) break;
                        sum += V[k].first - V[k].second;
                        swap(V[k].first, V[k].second);
                    }
                    ll Max = 0;
                    for(auto [x, y] : V) Max = max(Max, x);
                    if(dp2[j] > dp2[i] + Max) {
                        dp2[j] = dp2[i] + Max; 
                        p[j] = i;
                    }
                }
            }
            
            vector<pair<int, int>> vec;
            {
                int now = n;
                while(now) {
                    vec.push_back({p[now], now});
                    now = p[now];
                }
            }
            reverse(all(vec));
            past_ans.clear();

            auto op = [&](ll from, ll to, ll wid, char c) -> void {
                vector<int> turn(n, 0);
                vector<pair<ll, int>> v;
                ll sum = 0;
                for(int i = from; i < to; i ++) {
                    if(w[i] < h[i]) {
                        sum += w[i];
                        v.push_back({h[i], i});
                        if(c == 'U') turn[i] = 1;
                        else turn[i] = -1;
                    } else {
                        sum += h[i];
                        v.push_back({w[i], i});
                        if(c == 'U') turn[i] = -1;
                        else turn[i] = 1;
                    }
                }
                sort(all(v));
                reverse(all(v));
                for(auto [val, id] : v) {
                    if(sum + val - min(w[id], h[id]) > wid) {
                        break;
                    } 
                    sum += val - min(w[id], h[id]);
                    val = min(w[id], h[id]);
                    turn[id] = -turn[id];
                }
                for(int i = from; i < to; i ++) {
                    past_ans.push_back({i, (turn[i] == 1 ? 0 : 1), c, (i == from ? -1 : i - 1)});
                }
                return;
            };
            for(auto [i, j] : vec) {
                if(i >= idx) {
                    op(i, j, tate, 'L');
                } else {
                    op(i, j, yoko, 'U');
                }
            }
            Out();
            In_ans();
        }
    }
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    auto t_start = chrono::system_clock::now();

    // 提出時はVIS_FLAG = 0にする

    int n, t, sigma; cin >> n >> t >> sigma;
    vb[0] = sigma * 2;
    vb[1] = sigma;
    vb[2] = sigma * 2 / 3;
    vb[3] = sigma / 2;
    vector<ll> w(n), h(n);
    rep(i, n) cin >> w[i] >> h[i];

    if(VIS_FLAG) {
        w_true.resize(n);
        h_true.resize(n);
        w_diff.resize(t);
        h_diff.resize(t);
        rep(i, n) cin >> w_true[i] >> h_true[i];
        rep(i, t) cin >> w_diff[i] >> h_diff[i];
    }

    random_devicer rnd(n);
    // while(t) {
    //     tim = min(10, t);
    //     last_solve(w, h, n, sigma, sigma, t, 1500);
    //     t -= tim;
    // }

    int base = int(sqrt(n)) * 3/2;
    int lim = 1000000000 / (t * base * base * n);
    while(t) { 
        auto t_now = chrono::system_clock::now();
        auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
        if(rnd.next(0, 5)) {
            tim = 1;
            if(dt <= 1000) {
                bokuga_kangaeta_saikyono_algorithm(w, h, sigma, t, lim, base);
            } else if(dt <= 2500) {
                bokuga_kangaeta_saikyono_algorithm(w, h, sigma, t, lim/2, base);
            } else if(dt <= 2600) {
                bokuga_kangaeta_saikyono_algorithm(w, h, sigma, t, 100, base);
            } else {
                bokuga_kangaeta_saikyono_algorithm(w, h, sigma, t, 10, base);
            }
            t --;
            continue;
        }
            
        if(t >= 5) tim = 5;
        else tim = t;
        if(dt <= 1500) {
            state = -1;
            tim = min(5, t);
            last_solve(w, h, n, sigma, sigma, t, 2000, 6);
            t -= tim;
        }
        else if(dt <= 2000) {
            state = 0;
            tim = min(3, t);
            last_solve(w, h, n, sigma, sigma, t, 2000, 4);
            t -= tim;
        }else if(dt <= 2750) {
            if(t >= 3) tim = 3;
            else tim = t;
            if(t & 1) {
                state = 3;
                solvefast(w, h, n, sigma + rnd.next(0, sigma * 2), sigma, t, 2000);
            } else {
                state = 4;
                solve(w, h, n, rnd.next(0, sigma), sigma, t, 200);
            }
            t = max(t - 3, 0);
        } else if(dt <= 2800) {
            if(t >= 3) tim = 3;
            else tim = t;
            state = 5;
            solvefast(w, h, n, sigma + rnd.next(0, sigma * 2), sigma, t, 300);
            t = max(t - 3, 0);
        }
        else {
            state = 6;
            solvefast(w, h, n, sigma + rnd.next(0, sigma * 2), sigma, t, 30);
            t = max(t - 5, 0);
        }   
    }

    // cout<<  "opt " << state << " " << debug_ans << endl;

    // vis_flag
    return 0;
}
