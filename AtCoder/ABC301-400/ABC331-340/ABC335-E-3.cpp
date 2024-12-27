#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC335 https://atcoder.jp/contests/abc335

/*
 * ダイクストラ法で解く実装。
 *   https://atcoder.jp/contests/abc335/editorial/9054
 * 
 * ダイクストラ法は「コストが小さいものから確定していく」という気持ちなので、
 * {Aの値, 得点の-1倍} をキーとしてpriority_queueに入れるとうまくいく。
 * 
 * その他細かい変化点はあるが、
 * 自作のdijkstra()にて、distの型をll->pllにすると、修正の8割は完了となる。
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;
using pll = pair<ll,ll>;
void dijkstra(Graph &G, int s, vector<pll> &dist)
{
	dist[s].second = -1;
	using dist_v = pair<pll, int>;  // {Aの値,スコアの-1倍}とその頂点番号のpair
	priority_queue<dist_v, vector<dist_v>, greater<dist_v>> que;
	que.push({dist[s], s});

	while(!que.empty())
	{
		pll d = que.top().first;
		int v = que.top().second;
		que.pop();

		if(d > dist[v]) continue;  // {d, v}はゴミのためスルー

		// 頂点vから出ている各辺について探索
		for(auto &e : G[v])
		{
			pll d_next = {dist[e.to].first, dist[v].second - e.weight};  // 得点は-1倍にしているので、辺のコストは引く
			if(dist[e.to] > d_next)
			{
				dist[e.to] = d_next;
				que.push({dist[e.to], e.to});
			}
		}
	}
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		if(a[u] > a[v]) swap(u, v);  // a[u]<=a[v]にする
		if(a[u] != a[v])  // a[u] < a[v]
		{
			g[u].push_back({v, 1});
		}
		else
		{
			g[u].push_back({v, 0});  // Aが同じなら得点0
			g[v].push_back({u, 0});
		}
	}
	vector<pll> dist(N);
	for(i = 0; i < N; i++) dist[i] = {a[i], INF64};
	dijkstra(g, 0, dist);
	ll ans = -dist[N-1].second;
	if(ans == -INF64) ans = 0;
	cout << ans << endl;

	return 0;
}
