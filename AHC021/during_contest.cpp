#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for(int i=0;i<n;i++)

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
	// [lower, upper] 中の実数から一様ランダムに選ぶ。
	double next_double(double lower, double upper) {
		std::uniform_real_distribution<> dist(lower, upper);
		return dist(engine);
	}
};


const int n = 30;
vector<vector<int>> a;
vector<tuple<int, int, int, int>> ans(10000, {0, 0, 1, 0});


vector<tuple<int, int, int, int>> tentou(vector<vector<int>> &b) {
    vector<tuple<int, int, int, int>> ret;
    rep(i, 29) {
        rep(j, i + 1) {
            if(b[i][j] > b[i + 1][j]) ret.push_back({i, j, i + 1, j});
            if(b[i][j] > b[i + 1][j + 1]) ret.push_back({i, j, i + 1, j + 1});
        }
    }
    return ret;
}

vector<int> edge_candidates_X = {25, 20, 14, 11};
vector<int> edge_candidates_val = {200, 100, 50, 30};
vector<int> edge_len = {4, 3, 2, 1};
vector<tuple<int, int, int, int>> edges(vector<vector<int>> &b) {
    vector<tuple<int, int, int, int>> ret;
    int can = edge_candidates_X.size();
    rep(_, can) {
        for(int i = edge_candidates_X[_]; i < 30; i ++) {
            for(int j = 0; j < edge_len[_]; j ++) {
                if(b[i][j] < edge_candidates_val[_] and b[i][j + 1] > edge_candidates_val[_]) ret.push_back({i, j, i, j + 1});
                if(b[i][i - j] < edge_candidates_val[_] and b[i][i - j - 1] > edge_candidates_val[_]) ret.push_back({i, i - j, i, i - j - 1});
            }
        }
    }
    return ret;
}

tuple<int, int, int, int> max_id(vector<vector<int>> &b) {
    int MAX = -1;
    tuple<int, int, int, int> ret = {-1, -1, -1, -1};
    rep(i, 29) {
        rep(j, i + 1) {
            if(b[i][j] > MAX and (b[i][j] > b[i + 1][j] || b[i][j] > b[i + 1][j + 1])) {
                if(b[i + 1][j] < b[i + 1][j + 1]) {
                    ret = {i, j, i + 1, j};
                } else {
                    ret = {i, j, i + 1, j + 1};
                }
                MAX = b[i][j];
            }
        }
    }
    return ret;
}

tuple<int, int, int, int> min_id(vector<vector<int>> &b) {
    int MIN = 10000;
    tuple<int, int, int, int> ret = {-1, -1, -1, -1};
    for(int i = 29; i > 0; i --) {
        rep(j, i + 1) {
            if(b[i][j] < MIN) {
                if(j == 0) {
                    if(b[i][j] < b[i - 1][j]) {
                        MIN = b[i][j];
                        ret = {i, j, i - 1, j};
                    }
                } else if(j == i) {
                    if(b[i][j] < b[i - 1][j - 1]) {
                        MIN = b[i][j];
                        ret = {i, j, i - 1, j - 1};
                    }
                } else {
                    int M = -1;
                    if(b[i][j] < b[i - 1][j]) {
                        MIN = b[i][j];
                        M = b[i - 1][j];
                        ret = {i, j, i - 1, j};
                    }
                    if(b[i][j] < b[i - 1][j - 1] and M <= b[i - 1][j - 1]) {
                        MIN = b[i][j];
                        ret = {i, j, i - 1, j - 1};
                    }
                }
            }
        }
    }
    return ret;
}


void solve0() {
    vector<tuple<int, int, int, int>> action;
    auto b = a;
    auto ret = tentou(b);
    int lim = ans.size() - 1;
    rep(i, lim) {
        if(ret.empty()) break;
        auto [x1, y1, x2, y2] = ret.back();
        action.push_back(ret.back());
        swap(b[x1][y1], b[x2][y2]);
        ret = tentou(b);
    }

    if(ret.empty()) {
        ans = action;
    }
    return;
}

void solve1(int seed) {
    vector<tuple<int, int, int, int>> action;
    random_devicer rnd(seed);
    auto b = a;
    auto ret = tentou(b);
    int lim = ans.size() - 1;
    rep(i, lim) {
        if(ret.empty()) break;
        int nx = rnd.next(0, (int) ret.size() - 1);
        auto [x1, y1, x2, y2] = ret[nx];
        action.push_back(ret[nx]);
        swap(b[x1][y1], b[x2][y2]);
        ret = tentou(b);
    }

    if(ret.empty()) {
        ans = action;
    }
    return;
}

void solve2(int seed) {
    vector<tuple<int, int, int, int>> action;
    random_devicer rnd(seed);
    auto b = a;
    auto ret = tentou(b);
    int lim = ans.size() - 1;
    rep(i, lim) {
        if(ret.empty()) break;
        if(i % 100 == 99) {
            rep(j, 20) {
                auto edge = edges(b); // 端で、100以下かつ下にあるやつ
                if(edge.empty()) break;
                int nx = rnd.next(0, (int) edge.size() - 1);
                auto [x1, y1, x2, y2] = edge[nx];
                action.push_back(edge[nx]);
                swap(b[x1][y1], b[x2][y2]);
                i ++;
            }
            ret = tentou(b);
            if(ret.empty()) break;
        }
        int nx = rnd.next(0, (int) ret.size() - 1);
        auto [x1, y1, x2, y2] = ret[nx];
        action.push_back(ret[nx]);
        swap(b[x1][y1], b[x2][y2]);
        ret = tentou(b);
    }

    if(ret.empty()) {
        ans = action;
    }
    return;
}

void solve01() {
    vector<tuple<int, int, int, int>> action;
    auto b = a;

    int lim = ans.size() - 1;
    auto ret = max_id(b);
    rep(i, lim) {
        auto [x1, y1, x2, y2] = ret;
        if(get<0>(ret) == -1) break;
        action.push_back(ret);
        swap(b[x1][y1], b[x2][y2]);
        ret = max_id(b);
    }
    
    if(get<0>(ret) == -1) {
        ans = action;
    }
    return;
}

void solve02() {
    vector<tuple<int, int, int, int>> action;
    auto b = a;

    int lim = ans.size() - 1;
    auto ret = min_id(b);
    rep(i, lim) {
        auto [x1, y1, x2, y2] = ret;
        if(get<0>(ret) == -1) break;
        action.push_back(ret);
        swap(b[x1][y1], b[x2][y2]);
        ret = min_id(b);
    }
    
    if(get<0>(ret) == -1) {
        ans = action;
    }
    return;
}

void solve3(int seed) {
    vector<tuple<int, int, int, int>> action;
    random_devicer rnd(seed);
    auto b = a;
    int lim = ans.size() - 1;
    
    auto ret = max_id(b);
    rep(i, lim) {
        auto [x1, y1, x2, y2] = ret;
        if(get<0>(ret) == -1) break;
        action.push_back(ret);
        swap(b[x1][y1], b[x2][y2]);
        if(rnd.next(0, 1)) ret = max_id(b);
        else ret = min_id(b);
    }
    if(get<0>(ret) == -1) {
        ans = action;
    }
    return;
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    a.resize(n, vector<int> (n));
    rep(i, n) {
        rep(j, i + 1) cin >> a[i][j];
    } 

    auto t_start = chrono::system_clock::now();

    int se = 0;
    solve0();
    solve01();
    solve02();
    int cnt = 0;
    
    while(1) {
        auto t_now = chrono::system_clock::now();
        auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
        if (dt > 500) break;
        solve1(se ++);
    }

    // se = 0;

    // while(1) {
    //     auto t_now = chrono::system_clock::now();
    //     auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
    //     if (dt > 1000) break;
    //     solve2(se ++);
    // }

    se = 0;
    while(1) {
        auto t_now = chrono::system_clock::now();
        auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
        if (dt > 1980) break;
        solve3(se ++);
    }

    cout << ans.size() << endl;
    for(auto [x, y, z, t] : ans) {
        cout << x << " " << y << " " << z << " " << t << endl;
    }
    
    return 0;
}
