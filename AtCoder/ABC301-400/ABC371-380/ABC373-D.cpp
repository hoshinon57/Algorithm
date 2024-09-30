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

// ABC373 https://atcoder.jp/contests/abc373

// 制約より矛盾はないことが言えるので、未探索の頂点からDFSしていけば解ける。
// コンテスト時、dfs()にてcostをint型にしており1WA.

struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;

vector<ll> ans;
void dfs(Graph &g, int v, ll cost, vector<bool> &seen)
{
	seen[v] = true;
	ans[v] = cost;

	for(auto &e : g[v])
	{
		if(seen[e.to]) continue;
		dfs(g, e.to, cost+e.weight, seen);
	}
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		ll w; cin >> w;
		g[u].push_back({v, w});
		g[v].push_back({u, -w});
	}
	vector<bool> seen(N, false);
	ans.resize(N);
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;
		dfs(g, i, 0, seen);
	}
	
	int sz_ = (int)ans.size();
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}
