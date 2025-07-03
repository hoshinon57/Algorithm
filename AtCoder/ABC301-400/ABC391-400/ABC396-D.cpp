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

// ABC396 https://atcoder.jp/contests/abc396

// 単純パス(同じ頂点を2度以上通らないパス)の列挙にはDFSが有効。

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
	ll N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		ll u, v, w; cin >> u >> v >> w;
		u--; v--;
		g[u].push_back({(int)v, w});
		g[v].push_back({(int)u, w});
	}

	ll ans = INF64;
	vector<bool> seen(N, false);
	// dfs(v, xr)
	auto dfs = [&](auto &self, int v, ll xr) -> void
	{
		if(seen[v]) return;
		if(v == N-1)
		{
			chmin(ans, xr);
			return;
		}
		seen[v] = true;
		for(auto &e : g[v])  // e:辺
		{
			self(self, e.to, xr^e.weight);
		}
		seen[v] = false;
	};
	dfs(dfs, 0, 0);
	cout << ans << endl;

	return 0;
}
