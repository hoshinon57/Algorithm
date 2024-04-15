#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1e9+7;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * dp[v][col]を以下のように定義する。
 *   頂点vを根とした部分木を考えたときに、頂点vをcolで塗る通り数
 *   col=0:黒, 1:白
 * 頂点0を根として計算していったときに、dp[0][0]+dp[0][1]が答。
 * 
 * dp値の計算は、頂点0からDFSしていけばよい。
 * 頂点vの子をeについて、
 *   dp[v][0] = dp[e][1] の積
 *   dp[v][1] = dp[e][0]+dp[e][1] の積
 * となる。
 */

using Graph = vector<vector<int>>;
vector<vector<ll>> dp;

void dfs(Graph &g, int v, int p = -1)
{
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dfs(g, e, v);
		dp[v][0] *= dp[e][1];
		dp[v][0] %= MOD;
		dp[v][1] *= (dp[e][0] + dp[e][1]);
		dp[v][1] %= MOD;
	}
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int x, y; cin >> x >> y;
		x--; y--;
		g[x].push_back(y);
		g[y].push_back(x);
	}
	dp.resize(N, vector<ll>(2, 1));  // 値は1で初期化

	dfs(g, 0);
	ll ans = (dp[0][0]+dp[0][1])%MOD;
	cout << ans << endl;

	return 0;
}
