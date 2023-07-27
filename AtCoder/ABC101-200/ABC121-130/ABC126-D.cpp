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

// ABC126 https://atcoder.jp/contests/abc126

/*
 * ダイクストラ法で解く。
 * ※ライブラリ化していたからダイクストラ法で解いただけで、DFSで解くのが一般的かと考える。
 *   DFSでACしたコード：https://atcoder.jp/contests/abc126/submissions/43912611
 * 
 * 頂点0を起点にダイクストラを実施し、各頂点について
 *   距離が偶数：色0
 *   距離が奇数：色1
 * で塗っていけばよい。
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
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
			if(dist[e.to] > dist[v] + e.weight)
			{
				dist[e.to] = dist[v] + e.weight;
				que.push({dist[e.to], e.to});
			}
		}
	}
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int u, v, w; cin >> u >> v >> w;
		u--; v--;
		g[u].push_back(Edge(v, w));
		g[v].push_back(Edge(u, w));
	}

	vector<ll> dist(N, INF64);
	dijkstra(g, 0, dist);

	for(i = 0; i < N; i++)
	{
		cout << dist[i]%2 << endl;
	}

	return 0;
}
