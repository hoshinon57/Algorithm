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

// ABC292 https://atcoder.jp/contests/abc292

/*
 * 幅優先探索で解く。
 * 
 * 操作を行った最終的なグラフがどうなるかを考えると、
 *   ある頂点xから初期状態で到達できる全頂点に対し、xから有向辺を張る
 * となる。
 * 頂点xを起点にBFSを行い、距離が2以上の頂点に対して辺を追加することとなる。
 * 
 * よって全頂点から上記のBFSを行えばよい。
 * 計算量は、BFS1回あたりO(N+M)で、全頂点からBFSを行うのでO(N(N+M))になる。
 */

using Graph = vector<vector<int>>;
// 頂点数Nのグラフについて、頂点sを起点にBFS.
// distは要素数Nで、未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph &G, int N, int s, vector<int> &dist)
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
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;  // u->v
		u--; v--;
		g[u].push_back(v);
	}

	ll ans = 0;
	// 各頂点からBFS
	for(i = 0; i < N; i++)
	{
		vector<int> dist(N, INF32);
		bfs_graph(g, N, i, dist);
		for(auto &e : dist)
		{
			// 頂点iから到達できて、距離が2以上のものをカウント
			if(e != INF32 && e >= 2) ans++;
		}
	}
	cout << ans << endl;

	return 0;
}
