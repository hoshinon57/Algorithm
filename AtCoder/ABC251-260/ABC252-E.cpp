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

// ABC252 https://atcoder.jp/contests/abc252

/*
 * ダイクストラ法で解く。
 * 
 * 都市1をスタートとし、各頂点への最短路をダイクストラ法で求める。
 * その際、スタートから見た経路で各頂点に入る辺が、保守する道路となる。
 * 
 * これは各頂点ごとにただ1つのみ決まるので(＝つまり保守する道路はN-1本になる)、
 * 頂点数の配列 in_edge[] を用意してダイクストラ法の処理時に設定し、最後に出力すればよい。
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	int no;  // 辺の番号
	Edge(int to_, ll weight_, int no_) : to(to_), weight(weight_), no(no_) {}
};

using Graph = vector<vector<Edge>>;

int main(void)
{
	// 0-indexed
	int i;
	int N, M;
	cin >> N >> M;
	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int a, b, c;
		cin >> a >> b >> c;
		a--;  // 0-indexedに変換
		b--;
		graph[a].push_back(Edge(b, c, i));
		graph[b].push_back(Edge(a, c, i));
	}
	// ここまで入力

	vector<ll> dist(N, INF64);  // 始点から各頂点への距離
	dist[0] = 0;  // 始点

	// {dist[v], v} のペアを要素とした優先度付きキュー
	// dist[v]が小さい方が優先度が高い
	priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>> > que;
	que.push(make_pair(dist[0], 0));

	vector<int> in_edge(N);  // in_edge[i]:始点から見た経路で、頂点iへ入る辺の番号

	while(!que.empty())
	{
		ll d = que.top().first;
		int v = que.top().second;
		que.pop();

		if(d > dist[v]) continue;  // より短い距離で更新済みのため、スルー
		
		// 頂点vから出ている各辺について探索
		for(auto &e : graph[v])  // e:vと繋がる頂点、その辺の重み、その辺の番号 を持つ
		{
			// 距離を更新できるかチェック
			if(dist[v] + e.weight < dist[e.to])
			{
				// 距離を更新してキューに積む
				dist[e.to] = dist[v] + e.weight;
				que.push(make_pair(dist[e.to], e.to));
				in_edge[e.to] = e.no;  // e.toに入る辺の番号
			}
		}
	}

	for(i = 1; i < N; i++)  // 始点i=0は除く
	{
		cout << in_edge[i]+1 << endl;  // 1-indexedに変換するため、+1
	}

	return 0;
}
