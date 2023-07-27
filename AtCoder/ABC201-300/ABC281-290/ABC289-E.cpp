#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC289 https://atcoder.jp/contests/abc289

/*
 * コンテスト中に解けず、解説を見て実装した。
 * ※コンテスト中はmapでデータを持っていたが、
 *   定数倍が悪くなるのか、グラフの持ち方が悪かったのか、TLEとなった。
 * 
 * BFSで解く。
 * グラフの探索を {高橋君がいる頂点, 青木君がいる頂点} の形で考える。
 * すると取りうる頂点の組み合わせは N^2 となる。
 * 
 * BFSについて、以下のように実装する。
 * ・queueを pair<int,int> の形式で持つ
 * ・初期状態からの距離distを dist[高橋君がいる頂点][青木君がいる頂点] の形式で持つ
 * ・初期値は{頂点0, 頂点N-1}, ゴールは{頂点N-1, 0}
 * ・BFSの探索部分は、高橋君が行ける頂点と、青木君が行ける頂点の組み合わせで考える
 * 
 * dist[N-1][0]が答。
 */

using Graph = vector<vector<int>>;

void solve(void)
{
	// 0-indexed
	int i;
	int N, M;
	cin >> N >> M;
	vector<int> color(N);
	for(i = 0; i < N; i++) cin >> color[i];
	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int u, v;
		cin >> u >> v;
		u--; v--;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	// {高橋君がいる頂点, 青木君がいる頂点} の形でデータを持つ
	queue<pair<int,int>> que;
	vector<vector<int>> dist(N, vector<int>(N,INF32));  // dist[Taka][Aoki]
	// 初期値は{頂点0, 頂点N-1}
	que.push({0, N-1});
	dist[0][N-1] = 0;

	// BFS
	while(!que.empty())
	{
		int v_taka, v_aoki;
		tie(v_taka, v_aoki) = que.front();
		que.pop();

		// 高橋君が行ける頂点と、青木君が行ける頂点の組み合わせ
		for(auto &e_taka : graph[v_taka])
		{
			for(auto &e_aoki : graph[v_aoki])
			{
				// {v_taka, v_aoki} -> {e_taka, e_aoki}
				if(color[e_taka] == color[e_aoki]) continue;  // 色が同じはNG
				if(dist[e_taka][e_aoki] != INF32) continue;  // 探索済み

				dist[e_taka][e_aoki] = dist[v_taka][v_aoki] + 1;
				que.push({e_taka, e_aoki});
			}
		}
	}

	// 答はdist[N-1][0]
	if(dist[N-1][0] != INF32) cout << dist[N-1][0] << endl;
	else cout << -1 << endl;
}

int main(void)
{
	int T;
	cin >> T;

	while(T > 0)
	{
		solve();
		T--;
	}

	return 0;
}
