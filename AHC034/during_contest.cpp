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

void chmin(ll &a, ll b) {
    a = min(a, b);
}
void chmax(ll &a, ll b) {
    a = max(a, b);
}

int dx[4] = {-1, 0, 0, 1};
int dy[4] = {0, -1, 1, 0};
string S = "LUDR";
int n;
vector<vector<int>> A(20, vector(20, 0));
vector<pair<int, int>> ans_vec;
ll ans_score = (1LL << 30);

void solve0(int h = n, int w = n) {
    vector<pair<int, int>> ans;
    ll score = 0, weight = 0;
    auto a = A;
    for(int i = 0; i < h; i ++) {
        if(i & 1) {
            for(int j = w - 1; j > 0; j --) {
                if(a[i][j] > 0) {
                    score += a[i][j];
                    weight += a[i][j];
                    ans.push_back({1, a[i][j]});
                    a[i][j] = 0;
                }
                ans.push_back({0, 0});
                score += weight + 100;
            }
            {
                int j = 0;
                if(a[i][j] > 0) {
                    score += a[i][j];
                    weight += a[i][j];
                    ans.push_back({1, a[i][j]});
                    a[i][j] = 0;
                }
                if(i != h - 1) {
                    ans.push_back({0, 2});
                    score += weight + 100;
                }
            }
        } else {
            for(int j = 0; j < w - 1; j ++) {
                if(a[i][j] > 0) {
                    score += a[i][j];
                    weight += a[i][j];
                    ans.push_back({1, a[i][j]});
                    a[i][j] = 0;
                }
                ans.push_back({0, 3});
                score += weight + 100;
            }
            {
                int j = w - 1;
                if(a[i][j] > 0) {
                    score += a[i][j];
                    weight += a[i][j];
                    ans.push_back({1, a[i][j]});
                    a[i][j] = 0;
                }
                ans.push_back({0, 2});
                score += weight + 100;
            }
        }
    }
    for(int i = h - 1; i >= 0; i --) {
        if(i & 1) {
            for(int j = 0; j < w - 1; j ++) {
                if(a[i][j] < 0) {
                    score -= a[i][j];
                    weight += a[i][j];
                    ans.push_back({-1, -a[i][j]});
                    a[i][j] = 0;
                }
                ans.push_back({0, 3});
                score += weight + 100;
            }
            {
                int j = w - 1;
                if(a[i][j] < 0) {
                    score -= a[i][j];
                    weight += a[i][j];
                    ans.push_back({-1, -a[i][j]});
                    a[i][j] = 0;
                }
                ans.push_back({0, 1});
                score += weight + 100;
            }
        } else {
            for(int j = w - 1; j > 0; j --) {
                if(a[i][j] < 0) {
                    score -= a[i][j];
                    weight += a[i][j];
                    ans.push_back({-1, -a[i][j]});
                    a[i][j] = 0;
                }
                ans.push_back({0, 0});
                score += weight + 100;
            }
            {
                int j = 0;
                if(a[i][j] < 0) {
                    score -= a[i][j];
                    weight += a[i][j];
                    ans.push_back({-1, -a[i][j]});
                    a[i][j] = 0;
                }
                if(i) {
                    ans.push_back({0, 1});
                    score += weight + 100;
                }
            }
        }
    }
    if(score < ans_score) {
        ans_score = score;
        ans_vec = ans;
    }
}

bool solve1(int h = n, int w = n, vector<vector<int>> a = A) {
    vector<pair<int, int>> ans;
    ll score = 0, weight = 0;
    for(int i = 0; i < h; i ++) {
        if(i & 1) {
            for(int j = w - 1; j > 0; j --) {
                if(a[i][j] > 0) {
                    score += a[i][j];
                    weight += a[i][j];
                    ans.push_back({1, a[i][j]});
                    a[i][j] = 0;
                } else if(a[i][j] < 0 and i >= 10) {
                    if(weight < -a[i][j]) return 0;
                    weight += a[i][j];
                    score -= a[i][j];
                    ans.push_back({-1, -a[i][j]});
                    a[i][j] = 0;
                }
                ans.push_back({0, 0});
                score += weight + 100;
            }
            {
                int j = 0;
                if(a[i][j] > 0) {
                    score += a[i][j];
                    weight += a[i][j];
                    ans.push_back({1, a[i][j]});
                    a[i][j] = 0;
                } else if(a[i][j] < 0 and i >= 10) {
                    if(weight < -a[i][j]) return 0;
                    weight += a[i][j];
                    score -= a[i][j];
                    ans.push_back({-1, -a[i][j]});
                    a[i][j] = 0;
                }
                if(i != h - 1) {
                    ans.push_back({0, 2});
                    score += weight + 100;
                }
            }
        } else {
            for(int j = 0; j < w - 1; j ++) {
                if(a[i][j] > 0) {
                    score += a[i][j];
                    weight += a[i][j];
                    ans.push_back({1, a[i][j]});
                    a[i][j] = 0;
                } else if(a[i][j] < 0 and i >= 10) {
                    if(weight < -a[i][j]) return 0;
                    weight += a[i][j];
                    score -= a[i][j];
                    ans.push_back({-1, -a[i][j]});
                    a[i][j] = 0;
                }
                ans.push_back({0, 3});
                score += weight + 100;
            }
            {
                int j = w - 1;
                if(a[i][j] > 0) {
                    score += a[i][j];
                    weight += a[i][j];
                    ans.push_back({1, a[i][j]});
                    a[i][j] = 0;
                } else if(a[i][j] < 0 and i >= 10) {
                    if(weight < -a[i][j]) return 0;
                    weight += a[i][j];
                    score -= a[i][j];
                    ans.push_back({-1, -a[i][j]});
                    a[i][j] = 0;
                }
                ans.push_back({0, 2});
                score += weight + 100;
            }
        }
    }
    rep(i, 10) {
        ans.push_back({0, 1});
        score += weight + 100;
    }
    for(int i = 9; i >= 0; i --) { //////
        if(i & 1) {
            for(int j = 0; j < w - 1; j ++) {
                if(a[i][j] < 0) {
                    score -= a[i][j];
                    weight += a[i][j];
                    ans.push_back({-1, -a[i][j]});
                    a[i][j] = 0;
                }
                ans.push_back({0, 3});
                score += weight + 100;
            }
            {
                int j = w - 1;
                if(a[i][j] < 0) {
                    score -= a[i][j];
                    weight += a[i][j];
                    ans.push_back({-1, -a[i][j]});
                    a[i][j] = 0;
                }
                ans.push_back({0, 1});
                score += weight + 100;
            }
        } else {
            for(int j = w - 1; j > 0; j --) {
                if(a[i][j] < 0) {
                    score -= a[i][j];
                    weight += a[i][j];
                    ans.push_back({-1, -a[i][j]});
                    a[i][j] = 0;
                }
                ans.push_back({0, 0});
                score += weight + 100;
            }
            {
                int j = 0;
                if(a[i][j] < 0) {
                    score -= a[i][j];
                    weight += a[i][j];
                    ans.push_back({-1, -a[i][j]});
                    a[i][j] = 0;
                }
                if(i) {
                    ans.push_back({0, 1});
                    score += weight + 100;
                }
            }
        }
    }
    if(score < ans_score) {
        ans_score = score;
        ans_vec = ans;
        return true;
    }
    return false;
}

void solve2(int h = n, int w = n) {
    return;
}

void solve3(int h = n, int w = n) {
    auto a = A;
    for(int i = 0; i < h; i ++) {
        for(int j = 0; j < w; j ++) {
            a[i][j] = A[j][i];
        }
    }
    bool ret = solve1(h, w, a);
    if(ret) {
        for(auto& [x, y] : ans_vec) {
            if(!x) y ^= 1;
        }
    }
}

pair<vector<pair<int, int>>, ll> solve_par(int h, int w, vector<vector<int>> aa, ll weight_st) {
    ll Score = (1LL << 30);
    vector<pair<int, int>> Ans;
    
    for(int B = 0; B <= h; B += 2) {
        auto a = aa;
        bool ng = false;
        ll score = 0, weight = weight_st;
        vector<pair<int, int>> ans;
        int cnt = 0;
        for(int i = 0; i < h; i ++) {
            if(ng) break;
            if(i & 1) {
                for(int j = w - 1; j > 0; j --) {
                    cnt ++;
                    if(a[i][j] > 0) {
                        score += a[i][j];
                        weight += a[i][j];
                        ans.push_back({1, a[i][j]});
                        a[i][j] = 0;
                    } else if(a[i][j] < 0 and i >= B) {
                        if(weight < -a[i][j]) {
                            ng = true;
                            break;
                        }
                        weight += a[i][j];
                        score -= a[i][j];
                        ans.push_back({-1, -a[i][j]});
                        a[i][j] = 0;
                    }
                    ans.push_back({0, 0});
                    score += weight + 100;
                }
                {
                    int j = 0;
                    if(a[i][j] > 0) {
                        score += a[i][j];
                        weight += a[i][j];
                        ans.push_back({1, a[i][j]});
                        a[i][j] = 0;
                    }else if(a[i][j] < 0 and i >= B) {
                        if(weight < -a[i][j]) {
                            ng = true;
                            break;
                        }
                        weight += a[i][j];
                        score -= a[i][j];
                        ans.push_back({-1, -a[i][j]});
                        a[i][j] = 0;
                    }
                    if(i != h - 1) {
                        cnt ++;
                        ans.push_back({0, 2});
                        score += weight + 100;
                    }
                }
            } else {
                for(int j = 0; j < w - 1; j ++) {
                    cnt ++;
                    if(a[i][j] > 0) {
                        score += a[i][j];
                        weight += a[i][j];
                        ans.push_back({1, a[i][j]});
                        a[i][j] = 0;
                    } else if(a[i][j] < 0 and i >= B) {
                        if(weight < -a[i][j]) {
                            ng = true;
                            break;
                        }
                        weight += a[i][j];
                        score -= a[i][j];
                        ans.push_back({-1, -a[i][j]});
                        a[i][j] = 0;
                    }
                    ans.push_back({0, 3});
                    score += weight + 100;
                }
                {
                    cnt ++;
                    int j = w - 1;
                    if(a[i][j] > 0) {
                        score += a[i][j];
                        weight += a[i][j];
                        ans.push_back({1, a[i][j]});
                        a[i][j] = 0;
                    } else if(a[i][j] < 0 and i >= B) {
                        if(weight < -a[i][j]) {
                            ng = true;
                            break;
                        }
                        weight += a[i][j];
                        score -= a[i][j];
                        ans.push_back({-1, -a[i][j]});
                        a[i][j] = 0;
                    }
                    ans.push_back({0, 2});
                    score += weight + 100;
                }
            }
        }
        if(ng) continue;

        for(int i = h - 1; i >= max(B, 1); i --) {
            cnt ++;
            ans.push_back({0, 1});
            score += weight + 100;
        }
        for(int i = B - 1; i >= 0; i --) { //////
            if(i & 1) {
                for(int j = 0; j < w - 1; j ++) {
                    cnt ++;
                    if(a[i][j] < 0) {
                        score -= a[i][j];
                        weight += a[i][j];
                        ans.push_back({-1, -a[i][j]});
                        a[i][j] = 0;
                    }
                    ans.push_back({0, 3});
                    score += weight + 100;
                }
                {
                    int j = w - 1;
                    if(a[i][j] < 0) {
                        score -= a[i][j];
                        weight += a[i][j];
                        ans.push_back({-1, -a[i][j]});
                        a[i][j] = 0;
                    }
                    cnt ++;
                    ans.push_back({0, 1});
                    score += weight + 100;
                }
            } else {
                for(int j = w - 1; j > 0; j --) {
                    cnt ++;
                    if(a[i][j] < 0) {
                        score -= a[i][j];
                        weight += a[i][j];
                        ans.push_back({-1, -a[i][j]});
                        a[i][j] = 0;
                    }
                    ans.push_back({0, 0});
                    score += weight + 100;
                }
                {
                    int j = 0;
                    if(a[i][j] < 0) {
                        score -= a[i][j];
                        weight += a[i][j];
                        ans.push_back({-1, -a[i][j]});
                        a[i][j] = 0;
                    }
                    if(i) {
                        cnt ++;
                        ans.push_back({0, 1});
                        score += weight + 100;
                    }
                }
            }
        }
        if(Score > score) {
            Score = score;
            Ans = ans;
        }
    }
    
    return make_pair(Ans, Score);
}
bool solve4(int h = n, int w = n, vector<vector<int>> a = A) {
    
    
    vector suma(2, vector(4, 0));
    h = 10, w = 5;
    rep(i, n) rep(j, n) {
        suma[i / h][j / w] += a[i][j]; 
    } 
    vector<int> ord(8, 0);
    iota(all(ord), 0);
    bool flag = 0;

    
    do {
        {
            bool ng = 0;
            ll tmp = 0;
            foa(e, ord) {
                int y = e >> 2;
                int x = e & 3;
                tmp += suma[y][x];
                if(tmp < 0) ng = 1;
            }
            if(ng) continue;
        }
        vector<pair<int, int>> ans;
        ll score = 0, weight = 0;
        int nowy = 0, nowx = 0;

        // string S = "LUDR";
        foa(idx, ord) {
            {
                int ny = idx >> 2;
                int nx = idx & 3;
                ny *= h;
                nx *= w;
                score += (weight + 100) * (abs(nowx - nx) + abs(nowy - ny));
                while(nowy > ny) {
                    nowy --;
                    ans.push_back({0, 1});
                }
                while(nowy < ny) {
                    nowy ++;
                    ans.push_back({0, 2});
                }
                while(nowx > nx) {
                    nowx --;
                    ans.push_back({0, 0});
                }
                while(nowx < nx) {
                    nowx ++;
                    ans.push_back({0, 3});
                }
            }
            { // 部分問題
                vector<vector<int>> b(h);
                rep(i, h) {
                    rep(j, w) {
                        b[i].pb(a[nowy + i][nowx + j]);
                    }
                }

                auto [vec, Score] = solve_par(h, w, b, weight);
                foa(e, vec) ans.push_back(e);
                score += Score;
                weight += suma[nowy / h][nowx / w];
            }
        }
        if(score < ans_score) {
            ans_score = score;
            ans_vec = ans;
            flag = true;
        }
    } while(next_permutation(all(ord)));

    return flag;
}

void solve5(int h = n, int w = n) {
    auto a = A;
    for(int i = 0; i < h; i ++) {
        for(int j = 0; j < w; j ++) {
            a[i][j] = A[j][i];
        }
    }
    bool ret = solve4(h, w, a);
    if(ret) {
        for(auto& [x, y] : ans_vec) {
            if(!x) y ^= 1;
        }
    }
}

bool solve6(int h = n, int w = n, vector<vector<int>> a = A) {
    
    int H = n / h;
    int W = n / w;
    vector suma(H, vector(W, 0));
    rep(i, n) rep(j, n) {
        suma[i / h][j / w] += a[i][j]; 
    } 
    vector<int> ord(H * W, 0);
    iota(all(ord), 0);
    bool flag = 0;

    
    do {
        {
            bool ng = 0;
            ll tmp = 0;
            foa(e, ord) {
                int y = e / W;
                int x = e % W;
                tmp += suma[y][x];
                if(tmp < 0) ng = 1;
            }
            if(ng) continue;
        }
        vector<pair<int, int>> ans;
        ll score = 0, weight = 0;
        int nowy = 0, nowx = 0;

        // string S = "LUDR";
        foa(idx, ord) {
            {
                int ny = idx / W;
                int nx = idx % W;
                ny *= h;
                nx *= w;
                score += (weight + 100) * (abs(nowx - nx) + abs(nowy - ny));
                while(nowy > ny) {
                    nowy --;
                    ans.push_back({0, 1});
                }
                while(nowy < ny) {
                    nowy ++;
                    ans.push_back({0, 2});
                }
                while(nowx > nx) {
                    nowx --;
                    ans.push_back({0, 0});
                }
                while(nowx < nx) {
                    nowx ++;
                    ans.push_back({0, 3});
                }
            }
            { // 部分問題
                vector<vector<int>> b(h);
                rep(i, h) {
                    rep(j, w) {
                        b[i].pb(a[nowy + i][nowx + j]);
                    }
                }

                auto [vec, Score] = solve_par(h, w, b, weight);
                foa(e, vec) ans.push_back(e);
                score += Score;
                weight += suma[nowy / h][nowx / w];
            }
        }
        if(score < ans_score) {
            ans_score = score;
            ans_vec = ans;
            flag = true;
        }
    } while(next_permutation(all(ord)));

    return flag;
}

void solve_final() {
    auto tran = A;
    for(int i = 0; i < n; i ++) {
        for(int j = 0; j < n; j ++) {
            tran[i][j] = A[j][i];
        }
    }
    bool ret = 0;
    solve6(20, 20, A);
    ret = solve6(20, 20, tran);
    if(ret) {
        for(auto& [x, y] : ans_vec) {
            if(!x) y ^= 1;
        }
    }
    solve6(10, 20, A);
    ret = solve6(10, 20, tran);
    if(ret) {
        for(auto& [x, y] : ans_vec) {
            if(!x) y ^= 1;
        }
    }
    solve6(20, 10, A);
    ret = solve6(20, 10, tran);
    if(ret) {
        for(auto& [x, y] : ans_vec) {
            if(!x) y ^= 1;
        }
    }

    solve6(10, 10, A);
    ret = solve6(10, 10, tran);
    if(ret) {
        for(auto& [x, y] : ans_vec) {
            if(!x) y ^= 1;
        }
    }
}

void solve_rnd(int seed = 0) {
    random_devicer rnd(seed);
}

// string S = "LUDR";

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    auto t_start = chrono::system_clock::now();
    int seed = 0;

    cin >> n;
    rep(i, n) rep(j, n) cin >> A[i][j];


    solve0();
    solve1();
    solve2(); // 10*10の　総和が最大のところから
    solve3(); // 転置
    solve4(); // 8個に分ける 8!通りやる
    solve5(); // 転置

    solve_final();
    
    while(0) {
        auto t_now = chrono::system_clock::now();
        auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
        if (dt > 1900) break;
        solve_rnd(seed);
    }

    // return 0;

    for(auto [x, y] : ans_vec) {
        if(x == 1) {
            cout << "+" << y << endl;
        } else if(x == -1) {
            cout << "-" << y << endl;
        } else {
            cout << S[y] << endl;
        }
    }
    return 0;
}
