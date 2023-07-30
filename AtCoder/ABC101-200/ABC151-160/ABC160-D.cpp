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

// ABC160 https://atcoder.jp/contests/abc160

/*
 * 幅優先探索で解く。
 * 
 * 各頂点kからBFSを行い、kから各頂点への距離をカウントしていく。
 * BFS1回あたりO(N)であり、全体でO(N^2). 制約より間に合う。
 * 
 * i<jとなる頂点の組のみカウントしたいので、2で割って出力。
 */

using Graph = vector<vector<int>>;
// 頂点sを起点にBFS.
// distは未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph &G, int s, vector<int> &dist)
{
	queue<int> que;
	dist[s] = 0;
	que.push(s);

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		for(auto &e : G[v])
		{
			if(dist[e] != INF32) continue;
			dist[e] = dist[v] + 1;
			que.push(e);
		}
	}
	return;
}

int main(void)
{
	int i;
	int N, X, Y; cin >> N >> X >> Y;
	X--; Y--;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		g[i].push_back(i+1);
		g[i+1].push_back(i);
	}
	g[X].push_back(Y);
	g[Y].push_back(X);

	vector<int> ans(N);  // ans[i]:最短距離がiとなるような頂点の組の数
	for(i = 0; i < N; i++)  // 頂点iを起点にBFS
	{
		vector<int> dist(N, INF32);
		bfs_graph(g, i, dist);
		for(auto &e : dist)
		{
			ans[e]++;
		}
	}

	for(i = 1; i <= N-1; i++)
	{
		cout << ans[i]/2 << endl;  // i<jとなる頂点の組のみカウントしたいので、/2する
	}

	return 0;
}
