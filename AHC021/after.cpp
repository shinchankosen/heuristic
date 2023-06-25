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
};

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

void solve00() {
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

void solve1(int seed) { // 回転数重視
    vector<tuple<int, int, int, int>> action;
    random_devicer rnd(seed);
    auto b = a;
    int lim = ans.size() - 1;
    bool flag = rnd.next(0, 1);
    auto ret = (flag ? max_id(b) : min_id(b));
    rep(i, lim) {
        auto [x1, y1, x2, y2] = ret;
        if(x1 == -1) break;
        action.push_back(ret);
        swap(b[x1][y1], b[x2][y2]);
        while(1) {
            swap(x1, x2);
            swap(y1, y2);
            if(flag) { // max
                if(x1 == 29) break;
                if(b[x1 + 1][y1] > b[x1][y1]) {
                    if(b[x1 + 1][y1 + 1] > b[x1][y1]) break;
                    else x2 = x1 + 1, y2 = y1 + 1;
                } else {
                    if(b[x1 + 1][y1 + 1] > b[x1][y1]) x2 = x1 + 1, y2 = y1;
                    else {
                        if(b[x1 + 1][y1] > b[x1 + 1][y1 + 1]) x2 = x1 + 1, y2 = y1 + 1;
                        else x2 = x1 + 1, y2 = y1;
                    }
                }
            } else { // min
                if(y1 == 0 or b[x1 - 1][y1 - 1] < b[x1][y1]) {
                    if(y1 == x1 or b[x1 - 1][y1] < b[x1][y1]) break;
                    else x2 = x1 - 1, y2 = y1;
                } else {
                    if(y1 == x1 or b[x1 - 1][y1] < b[x1][y1]) x2 = x1 - 1, y2 = y1 - 1;
                    else {
                        if(b[x1 - 1][y1 - 1] < b[x1 - 1][y1]) x2 = x1 - 1, y2 = y1;
                        else x2 = x1 - 1, y2 = y1 - 1;
                    }
                }
            }
            action.push_back({x1, y1, x2, y2});
            swap(b[x1][y1], b[x2][y2]);
            i ++;
        }
        flag = rnd.next(0, 1);
        if(flag) ret = max_id(b);
        else ret = min_id(b);
    }
    if(get<0>(ret) == -1) ans = action;
    return;
}

void solve2(int seed) { // 1回操作するごとに最大値または最小値の場所の取得
    vector<tuple<int, int, int, int>> action;
    random_devicer rnd(seed);
    auto b = a;
    int lim = ans.size() - 1;
    
    auto ret = (rnd.next(0, 1) ? max_id(b) : min_id(b));
    rep(i, lim) {
        auto [x1, y1, x2, y2] = ret;
        if(x1 == -1) break;
        action.push_back(ret);
        swap(b[x1][y1], b[x2][y2]);
        if(rnd.next(0, 1)) ret = max_id(b);
        else ret = min_id(b);
    }
    if(get<0>(ret) == -1) ans = action;
    return;
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    a.resize(30, vector<int> (30));
    rep(i, 30) rep(j, i + 1) cin >> a[i][j];

    auto t_start = chrono::system_clock::now();

    solve00(); // ランダム性のないおまじない
    solve01();
    solve02();

    int seed = 0;

    while(1) {
        auto t_now = chrono::system_clock::now();
        auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
        if (dt > 1000) break;
        solve1(seed ++); // 回転数重視
    }

    seed = 0;
    while(1) {
        auto t_now = chrono::system_clock::now();
        auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
        if (dt > 1985) break;
        solve2(seed ++); // コンテスト中のやつ、毎回取得
    }

    cout << ans.size() << endl;
    for(auto [x, y, z, t] : ans) {
        cout << x << " " << y << " " << z << " " << t << endl;
    }
    
    return 0;
}
