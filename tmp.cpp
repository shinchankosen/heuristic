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

void solve0() {
}

void solve_rnd(int seed = 0) {
    random_devicer rnd(seed);
}


int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);

    auto t_start = chrono::system_clock::now();
    int seed = 0;

    solve0();
    while(0) {
        auto t_now = chrono::system_clock::now();
        auto dt = chrono::duration_cast<chrono::milliseconds>(t_now - t_start).count();
        if (dt > 1900) break;
        solve_rnd(seed);
    }
    return 0;
}
