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

// ABC168 https://atcoder.jp/contests/abc168

/*
 * 幅優先探索で解く。
 * 
 * まず、部屋1から各部屋への距離をBFSで求める。
 * 部屋xの距離をdとすると、部屋xに繋がる部屋を調べていったときに、距離がd-1であるものが必ず存在する。
 * それが道しるべとなる。
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
	// 0-indexed
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	vector<int> dist(N, INF32);
	bfs_graph(g, N,0, dist);

	cout << "Yes" << endl;  // Yes/NoはYes確定
	for(i = 1; i < N; i++)
	{
		for(auto &e : g[i])
		{
			if(dist[e] == dist[i] - 1)
			{
				cout << e+1 << endl;
				break;
			}
		}
	}

	return 0;
}
