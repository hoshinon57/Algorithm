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
 * まず、この実装ではACとならない。コンテスト後の追加テストケースにてTLEとなる。
 * 幅優先探索で解く。
 * 
 * スイッチを押す前の初期状態、押した後の状態、それぞれについてグラフを構築する。
 * スイッチがある頂点は、スイッチを押す前後の2パターンの頂点を行き来できる。
 * 
 * グラフg[j][i]を以下のように定義する。
 *   [j]:スイッチ反転後なら0, 最初の状態なら1
 *   [i]:頂点i 0～N-1
 *   にて繋がっている頂点の一覧
 * またdist[0 or 1][i]を以下のように定義する。
 *   始点から、スイッチが反転(0)/正転(1)のときの頂点iまでの最短距離
 * 
 * 始点をスイッチが正転(1)のときの頂点0としたときに、
 * dist[0][N-1]またはdist[1][N-1]のうち小さい方が答となる。
 * これを幅優先探索で解く。
 * 
 * 幅優先探索において、スイッチがある頂点なら反対側へコスト0で移動できる。
 * これ以外は普通の幅優先探索と同じ実装。
 * 
 * 以上の方法は、グラフの辺の重みが1と0の複数通りになるため、
 * 単なるBFSでも正しい答は出せるが、TLEになるケースが存在する。
 * ダイクストラや0-1BFSで解く必要がある。
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

	queue<pair<ll,ll>> que;  // {スイッチが反転(0)/正転(1), 頂点番号}
	que.push({1, 0});

	while(!que.empty())
	{
		ll s = que.front().first;  // スイッチ状態
		ll v = que.front().second;  // 頂点番号
		que.pop();

		if(swit[v])  // 頂点vにスイッチがあれば、反対側に行く
		{
			if(dist[1-s][v] > dist[s][v])
			{
				dist[1-s][v] = dist[s][v];  // 距離は増えない
				que.push({1-s, v});
			}
		}

		for(auto &e : g[s][v])  // e:接続されている頂点
		{
			if(dist[s][e] > dist[s][v] + 1)
			{
				dist[s][e] = dist[s][v] + 1;
				que.push({s, e});
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
