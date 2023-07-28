#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ダイクストラ法のメモ、ライブラリ

/*
 * [関連する問題]
 * ABC305-E
 * ABC277-E ダイクストラ / 0-1BFS
 * ABC191-E
 * ABC142-E ただし解説はいずれもbitDPだった
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
	// 以下はAOJのGRL_1_Aを解く内容
	// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_A&lang=ja
	int i;
	int N, E, s; cin >> N >> E >> s;
	Graph g(N);
	for(i = 0; i < E; i++)
	{
		int s, t;
		ll d;
		cin >> s >> t >> d;  // s->t
		g[s].push_back({t, d});
	}
	vector<ll> dist(N, INF64);
	dijkstra(g, s, dist);

	for(i = 0; i < N; i++)
	{
		if(dist[i] != INF64)
		{
			cout << dist[i] << endl;
		}
		else
		{
			cout << "INF" << endl;
		}
	}

	return 0;
}
