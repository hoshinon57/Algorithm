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

// ABC277 https://atcoder.jp/contests/abc277

/*
 * ABC277-E-1-BFS(NG).cppのTLE解消版。
 * グラフの辺の重みが0/1の2通りであることから、0-1BFSにて実装する。
 * 
 * 変数名の説明などはABC277-E-1-BFS(NG).cppを参照のこと。
 * 変化点は少なく、以下の程度。
 * ・queueをdequeに変える
 * ・キューに積む際、辺の重みが1ならpush_back(), 0ならpush_front()に置き換え
 */
using Graph = vector<vector<vector<int>>>;

int main(void)
{
	// 0-indexed
	ll i;
	ll N, M, K;
	cin >> N >> M >> K;
	// g[j][i]:
	//  [j]:スイッチ反転後なら0, 最初の状態なら1
	//  [i]:頂点i 0～N-1
	//  にて繋がっている頂点
	Graph g(2, vector<vector<int>>(N));
	for(i = 0; i < M; i++)
	{
		ll u, v, a;
		cin >> u >> v >> a;
		u--; v--;
		g[a][u].push_back(v);
		g[a][v].push_back(u);
	}
	vector<bool> swit(N, false);  // swit[i]:頂点iにスイッチがあればtrue
	for(i = 0; i < K; i++)
	{
		ll k;
		cin >> k;
		k--;
		swit[k] = true;
	}
	// dist[0,1][i]:
	//   始点から、スイッチが反転(0)/正転(1)のときの頂点iまでの最短距離
	//   始点はスイッチが正転(1)のときの頂点0とする
	vector<vector<ll>> dist(2, vector<ll>(N, INF64));
	dist[1][0] = 0;

	deque<pair<ll,ll>> que;  // {スイッチが反転(0)/正転(1), 頂点番号}
	que.push_back({1, 0});

	while(!que.empty())
	{
		ll s = que.front().first;  // スイッチ状態
		ll v = que.front().second;  // 頂点番号
		que.pop_front();

		if(swit[v])  // 頂点vにスイッチがあれば、反対側に行く
		{
			if(dist[1-s][v] == INF64)
			{
				dist[1-s][v] = dist[s][v];  // 距離は増えない
				que.push_front({1-s, v});  // コスト0なので、先頭に追加
			}
		}

		for(auto &e : g[s][v])  // e:接続されている頂点
		{
			if(dist[s][e] == INF64)
			{
				dist[s][e] = dist[s][v] + 1;
				que.push_back({s, e});  // コスト1なので、末尾に追加
			}
		}
	}

	ll answer;
	// スイッチ状態2通りそれぞれにおける頂点Nまでの距離のうち、小さい方
	answer = min(dist[0][N-1], dist[1][N-1]);
	if(answer != INF64)
	{
		cout << answer << endl;
	}
	else
	{
		cout << -1 << endl;
	}

	return 0;
}
