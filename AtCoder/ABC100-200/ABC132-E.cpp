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

// ABC132 https://atcoder.jp/contests/abc132

/*
 * 幅優先探索で解く。
 * 
 * 各頂点への最短路を考えるときに、「けん / けん / ぱ」の3つを区別する必要がある。
 * そのため距離distを dist[N][3] として定義し、
 *   dist[i][j]:頂点iにおいて、j=(距離 mod 3)で行けるときの最短路
 * とする。
 * これによりdist[0][i]とdist[1][i]を別々に考えることができる。
 * 
 * 後はキューを用いた一般的な幅優先探索で解いていけばよい。
 */

using Graph = vector<vector<int>>;

int main(void)
{
	// 0-indexed
	int i;
	int N, M;
	cin >> N >> M;
	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int u, v;
		cin >> u >> v;
		u--;  // 0-indexedに変換
		v--;
		graph[u].push_back(v);
	}
	int S, T;
	cin >> S >> T;
	S--;  // 0-indexedに変換
	T--;

	queue<pair<int,ll>> que;  // {頂点v, スタート地点からの距離d}
	// dist[N][3].  dist[i][j]:頂点iにおいて、j=(距離 mod 3)で行けるときの最短路
	// 未探索の場合はINF64.
	vector<vector<ll>> dist(N, vector<ll>(3, INF64));
	dist[S][0] = 0;  // スタート地点
	que.push(make_pair(S, 0));
	while(!que.empty())
	{
		int v = que.front().first;
		ll d = que.front().second;
		que.pop();

		for(auto &e : graph[v])
		{
			// すでに探索済みならスルー
			if(dist[e][(d+1)%3] != INF64) continue;

			// 重み無しのBFSのため、後からより短い距離で到達できる、ということは無い。
			// 無条件でdistを確定させてしまって良い。
			dist[e][(d+1)%3] = d+1;
			que.push(make_pair(e, d+1));
		}
	}

	if(dist[T][0] == INF64)
	{
		// 到達できなかった場合
		cout << -1 << endl;
	}
	else
	{
		// 到達できた場合
		// 「けんけんぱ」で1回なので、3で割って出力
		cout << dist[T][0]/3 << endl;
	}

	return 0;
}
