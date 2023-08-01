#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC192 https://atcoder.jp/contests/abc192

/*
 * ダイクストラ法で解く。
 * 
 * 考え方は解説と同じ。
 *   https://atcoder.jp/contests/abc192/editorial/723
 *   https://blog.hamayanhamayan.com/entry/2021/02/20/233838
 * 
 * Kの倍数ごとに移動できるので、
 * ある頂点vと辺eについて、eの出発時刻(Kの倍数ごと)になるまで待ってから移動する。
 * これはdist[v]をKで割って切り上げし、Kをかけると期待する値となる。
 * それ以外は通常のダイクストラと同じ実装。
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	ll K;  // Kの倍数ごとに出発
	Edge(int to_, ll weight_, ll K_) : to(to_), weight(weight_), K(K_) {}
};
using Graph = vector<vector<Edge>>;
// 頂点sを起点にダイクストラ。
// distは未探索部分をINFで埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void dijkstra(Graph &G, int s, vector<ll> &dist)
{
	dist[s] = 0;
	using dist_v = pair<ll, int>;  // 距離とその頂点番号のpair
	priority_queue<dist_v, vector<dist_v>, greater<dist_v>> que;
	que.push({dist[s], s});

	while(!que.empty())
	{
		ll d = que.top().first;
		int v = que.top().second;
		que.pop();

		if(d > dist[v]) continue;  // {d, v}はゴミのためスルー

		// 頂点vから出ている各辺について探索
		for(auto &e : G[v])
		{
			ll d_next;
			// ある頂点vと辺eについて、eの出発時刻(Kの倍数ごと)になるまで待ってから移動する。
			// これはdist[v]をKで割って切り上げし、Kをかけると期待する値となる。
			d_next = ((dist[v] + (e.K-1)) / e.K) * e.K + e.weight;
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
	// 0-indexed
	int i;
	ll N, M, X, Y; cin >> N >> M >> X >> Y;
	X--; Y--;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		ll a, b, t, k; cin >> a >> b >> t >> k;
		a--; b--;
		g[a].push_back(Edge(b, t, k));
		g[b].push_back(Edge(a, t, k));
	}
	vector<ll> dist(N, INF64);
	dijkstra(g, X, dist);

	ll ans = -1;
	if(dist[Y] != INF64) ans = dist[Y];
	cout << ans << endl;

	return 0;
}
