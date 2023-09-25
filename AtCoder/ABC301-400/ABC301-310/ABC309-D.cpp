#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC309 https://atcoder.jp/contests/abc309

// 幅優先探索で解く。
// ほぼコンテスト時のコードそのまま。
//   頂点1側の連結成分のうち、頂点1から最も遠い頂点の距離d1
//   頂点N1+N2側の連結成分のうち、頂点N1+N2から最も遠い頂点の距離d2
// とすると、d1+d2+1が答。

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
	ll i;
	ll N1, N2, M; cin >> N1 >> N2 >> M;
	Graph g(N1+N2);
	for(i = 0; i < M; i++)
	{
		ll a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	vector<int> dist1(N1+N2, INF32);
	bfs_graph(g, N1+N2, 0, dist1);
	int max1 = -1;
	for(i = 0; i < N1+N2; i++)
	{
		if(dist1[i] == INF32) continue;
		max1 = max(max1, dist1[i]);
	}

	vector<int> dist2(N1+N2, INF32);
	bfs_graph(g, N1+N2, N1+N2-1, dist2);
	int max2 = -1;
	for(i = 0; i < N1+N2; i++)
	{
		if(dist2[i] == INF32) continue;
		max2 = max(max2, dist2[i]);
	}

	cout << max1+max2+1 << endl;

	return 0;
}
