#include <bits/stdc++.h>
using namespace std;
#define all(v) (v).begin(),(v).end()
#define pb(a) push_back(a)
#define rep(i, n) for(int i=0;i<n;i++)
#define foa(e, v) for(auto&& e : v)
using ll = long long;
const ll MOD7 = 1000000007, MOD998 = 998244353, INF = (1LL << 60);
#define dout(a) cout<<fixed<<setprecision(10)<<a<<endl;

int n, m, h;
vector<vector<int>> v;
vector<int> a;
vector<int> x, y;

int eval(vector<int> &ans) {
    vector<vector<int>> g(n);
    vector<int> vec;
    rep(i, n) {
        if(ans[i] == -1) {
            vec.pb(i);
        } else {
            g[ans[i]].pb(i);
        }
    }
    int sum = 0;
    auto dfs = [&](auto dfs, int now, int height) -> void {
        sum += (height + 1) * a[now];
        foa(e, g[now]) {
            dfs(dfs, e, height + 1);
        }  
    };
    foa(e, vec) dfs(dfs, e, 0);
    return sum;
}
vector<int> solve1(auto fc) {
    vector<int> ans(n, -2), ansh(n, -1);

    auto can = [&](int id, set<int> st, int H = 10) -> bool {
        vector<pair<int, int>> vec;
        foa(e, v[id]) {
            if(ans[e] == id) {
                vec.push_back({a[e], e});
            }
        }
        sort(all(vec));
        bool ok = 0;
        int cnt = 0;
        auto dfs = [&](auto dfs, int now, int height) -> bool {
            if(height == 0) {
                ans[now] = -1;
                ansh[now] = 0;
                return true;
            }
            if(++ cnt >= 5000) return false;
            foa(e, v[now]) {
                if(ansh[e] + 1 == height and !st.count(e)) {
                    ans[now] = e;
                    ansh[now] = ansh[e] + 1;
                    return true;
                } else if(ans[e] == -2) {
                    ans[now] = e;
                    st.insert(e);
                    if(dfs(dfs, e, height - 1)) {
                        ansh[now] = ansh[e] + 1;
                        return true;
                    }
                    st.erase(e);
                } 
            }
            ans[now] = -2;
            ansh[now] = -1;
            return false;
        };
        for(auto [A, i] : vec) {
            ans[id] = i;
            if(dfs(dfs, i, H - 2)) {
                ansh[id] = ansh[i] + 1;
                ok = true;
                break;
            }
            ansh[i] = -1;
            ans[i] = id;
        }
        return ok;
    };
    while(1) {
        {
            bool con = 0;
            rep(i, n) if(ans[i] == -2) con = 1;
            if(!con) break;
        }
        vector<pair<int, int>> maxs;
        rep(i, n) {
            if(ans[i] != -2) continue;
            int sum = 0;
            foa(e, v[i]) {
                if(ans[e] != -2) continue;
                sum += a[e];
            }
            if(sum > 0) maxs.push_back({sum, i});
        }
        if(maxs.empty()) {
            rep(i, n) if(ans[i] == -2) ans[i] = -1;///
            break;
        }
        sort(all(maxs), fc);
        bool ok = 0;
        for(auto [M, j] : maxs) {
            vector<int> vec;
            vec.pb(j);
            ans[j] = -1;
            foa(e, v[j]) {
                if(ans[e] != -2) continue;
                vec.pb(e);
                ans[e] = j;
            }
            set<int> st;
            foa(e, vec) st.insert(e);
            auto ret = can(j, st);
            if(ret) {
                foa(e, vec) {
                    if(ans[e] == j) ansh[e] = ansh[j] + 1;
                }
                ok = 1;
                break;
            } 
            foa(e, vec) ans[e] = -2;
        }
        if(ok) continue;
        for(int i = 9; i >= 2; i --) {
            for(auto [M, j] : maxs) {
                vector<int> vec;
                vec.pb(j);
                ans[j] = -1;
                foa(e, v[j]) {
                    if(ans[e] != -2) continue;
                    vec.pb(e);
                    ans[e] = j;
                }
                set<int> st;
                foa(e, vec) st.insert(e);
                auto ret = can(j, st, i);
                if(ret) {
                    foa(e, vec) {
                        if(ans[e] == j) ansh[e] = ansh[j] + 1;
                    }
                    ok = 1;
                    break;
                } 
                foa(e, vec) ans[e] = -2;
            }
            if(ok) break;
        }
        if(ok) continue;
        rep(i, n) if(ans[i] == -2) ans[i] = -1;
        break;
    }
    auto dfs = [&](auto dfs, int now, int par) -> void {
        ans[now] = par;
        foa(e, v[now]) {
            if(ans[e] != -2) continue;
            dfs(dfs, e, now);
        }
    };
    dfs(dfs, 0, -1);
    return ans;
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    cin >> n >> m >> h;
    v.resize(n);
    a.resize(n);
    x.resize(n);
    y.resize(n);

    rep(i, n) cin >> a[i];
    rep(i, m) {
        int a, b;
        cin >> a >> b;
        v[a].pb(b);
        v[b].pb(a);
    }
    rep(i, n) cin >> x[i] >> y[i];

    auto ans1 = solve1([&](pair<int, int> A, pair<int, int> B) {
        return A > B;
    });
    auto val1 = eval(ans1);

    int ans = val1;
    vector<int> best_vec = ans1;

    auto ans2 = solve1([&](pair<int, int> A, pair<int, int> B) {
        return x[A.second] < x[B.second];
    });
    auto val2 = eval(ans2);

    if(ans < val2) {
        best_vec = ans2;
    }

    auto ans3 = solve1([&](pair<int, int> A, pair<int, int> B) {
        return y[A.second] < y[B.second];
    });
    auto val3 = eval(ans3);

    if(ans < val3) {
        best_vec = ans3;
    }

    auto ans4 = solve1([&](pair<int, int> A, pair<int, int> B) {
        return x[A.second] > x[B.second];
    });
    auto val4 = eval(ans4);

    if(ans < val4) {
        best_vec = ans4;
    }

    auto ans5 = solve1([&](pair<int, int> A, pair<int, int> B) {
        return y[A.second] > y[B.second];
    });
    auto val5 = eval(ans5);

    if(ans < val5) {
        best_vec = ans5;
    }
    rep(i, n) cout << best_vec[i] << " ";
    cout << '\n';
    return 0;
}
