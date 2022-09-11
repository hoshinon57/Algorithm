#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// code14.4の写経

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};

// 重み付きグラフ
// 各頂点が持つのは、頂点ではなく辺
using Graph = vector<vector<Edge>>;

int main(void)
{
	int i;
	int N, M, s;  // 頂点の数、辺の数、始点
	cin >> N >> M >> s;
	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		ll w;
		cin >> a >> b >> w;
		graph[a].push_back(Edge(b, w));  // 辺を持つ
	}

	// [memo]
	// ダイクストラ部分を関数化するならば、ここからwhile()の終端までが良さそう。
	//   vector<ll> dist(N);
	//   Dijkstra(dist, s);
	// ダイクストラで何を求めるか、によって引数は変える。

	// ヒープを用いたダイクストラ法で解く
	vector<ll> dist(N, INF64);  // 始点から各頂点への距離
	dist[s] = 0;  // 始点は0に

	// {dist[], v}のペアを要素とした優先度付きキュー
	// dist[]が小さい方が優先度が高い
	priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> que;
	que.push(make_pair(dist[s], s));

	while(!que.empty())
	{
		// v_min:「使用済み」ではない頂点のうち、始点からの距離が最小の頂点
		// d:始点から頂点v_minへの距離
		int v_min = que.top().second;
		ll d = que.top().first;
		que.pop();

		if(d > dist[v_min]) continue;  // {d, v_min}はゴミであり、スルー

		// 頂点v_minから出ている各辺について探索
		for(auto &e : graph[v_min])
		{
			// v_min->e.to への更新チェック
			if(dist[e.to] > dist[v_min] + e.weight)
			{
				dist[e.to] = dist[v_min] + e.weight;
				que.push(make_pair(dist[e.to], e.to));  // ヒープに挿入
			}
		}
	}

	for(i = 0; i < N; i++)
	{
		cout << i << ":";
		if(dist[i] == INF64)
		{
			cout << "INF" << endl;
		}
		else
		{
			cout << dist[i] << endl;
		}
	}

	return 0;
}
