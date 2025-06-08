#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC409 https://atcoder.jp/contests/abc409

/*
 * ある部分木について考えたとき、
 * 電子が辺をどちら方向に進んでもエネルギーは変わらないことから、
 * 電荷は根に集めてよさそう、となる。
 * 
 * なのでdpで各頂点の{電荷, 使用エネルギー}として値を持たせて
 * 根から木DPして解ける。
 * dp[0].secondが答。
**/

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> x(N); for(i = 0; i < N; i++) {cin >> x[i];}
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		ll u, v, w; cin >> u >> v >> w;
		u--;
		v--;
		g[u].push_back({(int)v, w});
		g[v].push_back({(int)u, w});
	}
	vector<pair<ll,ll>> dp(N);  // {電荷, 使用エネルギー}
	for(i = 0; i < N; i++) dp[i] = {x[i], 0};

	auto dfs = [&](auto &self, int v, int p = -1) -> void
	{
		for(auto &e : g[v])  // e:辺
		{
			if(e.to == p) continue;
			self(self, e.to, v);
			dp[v].first += dp[e.to].first;
			dp[v].second += dp[e.to].second + abs(dp[e.to].first) * e.weight;
		}
	};

	dfs(dfs, 0);
	cout << dp[0].second << endl;

	return 0;
}
