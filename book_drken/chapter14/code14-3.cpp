#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// code14.3の写経

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

	// ダイクストラ法を用いて解く
	vector<bool> used(N, false);  // used[i]:頂点iが「使用済み(=確定済み)」ならtrue
	vector<ll> dist(N, INF64);  // 始点から各頂点への距離
	dist[s] = 0;  // 始点は0に
	for(i = 0; i < N; i++)  // 最大で(頂点数)回の反復が必要
	{
		ll dist_min = INF64;
		int v_min = -1;
		// 「使用済み」ではない頂点のうち、距離が最小のものを探す
		for(int v = 0; v < N; v++)
		{
			if(used[v]) continue;  // 使用済みならスルー
			if(dist_min > dist[v])
			{
				dist_min = dist[v];
				v_min = v;
			}
		}
		if(v_min == -1) break;  // 見つからなければ終了

		// 頂点v_minから出ている各辺について探索
		for(auto &e : graph[v_min])
		{
			// v_min->e.to への更新チェック
			if(dist[e.to] > dist[v_min] + e.weight)
			{
				dist[e.to] = dist[v_min] + e.weight;
			}
		}

		used[v_min] = true;  // 頂点v_minを使用済みにする
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
