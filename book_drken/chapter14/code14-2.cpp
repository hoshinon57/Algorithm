#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// code14.2の写経

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
	for(i = 0; i < M ; i++)
	{
		int a, b;
		ll w;
		cin >> a >> b >> w;
		graph[a].push_back(Edge(b, w));  // 辺を持つ
	}

	// ベルマン・フォード法を用いて解く
	bool exist_negative_cycle = false;  // 負閉路があればtrue
	vector<ll> dist(N, INF64);  // 始点から各頂点への距離
	dist[s] = 0;  // 始点は0に
	for(i = 0; i < N; i++)  // 最大で(頂点数-1)回の反復が必要。(頂点数)回目のループは、負閉路の存在確認用。
	{
		bool update = false;  // 更新が発生したらtrue
		for(int v = 0; v < N; v++)
		{
			if(dist[v] == INF64) continue;  // 始点からまだ到達していない頂点はスルー
			for(auto &e : graph[v])  // 頂点vから出ている各辺について探索
			{
				// v->e.to への更新チェック
				if(dist[e.to] > dist[v] + e.weight)
				{
					dist[e.to] = dist[v] + e.weight;
					update = true;
				}
			}
		}

		// 更新がなければ完了している
		if(!update) break;

		// N回目の反復で更新があるならば、負閉路を持つということ
		if(i == N-1 && update) exist_negative_cycle = true;
	}

	if(exist_negative_cycle)
	{
		cout << "exist negative cycle." << endl;
	}
	else
	{
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
	}

	return 0;
}
