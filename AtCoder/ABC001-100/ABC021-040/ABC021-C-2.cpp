#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC021 https://atcoder.jp/contests/abc021

/*
 * 公式解説の手法を実装してみた。
 * 最短路DAGを求めてトポロジカルソートしてDPする。
 * 
 * まず通常のBFSを行い、距離が短い⇒遠い方へ辺を張った別のグラフを作成すると、これはDAGになる。
 * このグラフに対してトポロジカルソートを行い、先頭からDPしていく。
 * 
 * ABC021-C.cppよりも実装が面倒になった…感はあるが、
 * DAGに対してトポロジカルソートしてDP, は練習になったと思う。
 */

#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;

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

// サイクルの無い有向グラフGについて、トポロジカルソートを行う。
// ソートした結果の頂点をresultに、最長経路(=辺の本数)をdepthに、それぞれ格納する。
// resultとdepthはそれぞれ空要素で渡してよい。
// ※トポロジカルソートできない場合(サイクルがある場合)、G.size()とresult.size()が不一致かどうかで判断可能。
// 参考：
//   https://algo-logic.info/topological-sort/
//   https://qiita.com/Morifolium/items/6c8f0a188af2f9620db2
void bfs_topological_sort(Graph &G, vector<int> &result, vector<int> &depth)
{
	int i;
	int n = G.size();

	vector<int> indegree(n);  // 入次数
	for(i = 0; i < n; i++)
	{
		for(auto &e : G[i]) indegree[e]++;  // i->eへの辺
	}
	queue<int> que;
	for(i = 0; i < n; i++)
	{
		if(indegree[i] == 0) que.push(i);
	}

	depth.resize(n, 0);
	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		result.push_back(v);
		for(auto &e : G[v])  // 頂点vを消すイメージで、vから出た先の頂点の入次数を1減らす
		{
			indegree[e]--;
			if(indegree[e] == 0)
			{
				que.push(e);
				depth[e] = depth[v]+1;
			}
		}
	}
}

int main(void)
{
	// 0-indexed
	int i;
	int N; cin >> N;
	int a, b; cin >> a >> b;
	a--; b--;
	Graph g1(N);
	int M; cin >> M;
	vector<int> x(M), y(M);
	for(i = 0; i < M; i++)
	{
		cin >> x[i] >> y[i];
		x[i]--; y[i]--;
		g1[x[i]].push_back(y[i]);
		g1[y[i]].push_back(x[i]);
	}
	vector<int> dist_b(N, INF32);
	bfs_graph(g1, a, dist_b);

	Graph g2(N);  // DAG
	for(i = 0; i < M; i++)
	{
		int xx = x[i];
		int yy = y[i];
		if(dist_b[xx] < dist_b[yy]) g2[xx].push_back(yy);
		if(dist_b[xx] > dist_b[yy]) g2[yy].push_back(xx);
		// dist[x] = dist[y] は辺を張らない
	}
	vector<int> res, dep;
	bfs_topological_sort(g2, res, dep);

	vector<mint> cnt(N);
	cnt[a] = 1;
	for(i = 0; i < (int)res.size(); i++)  // トポロジカルソートして先頭からDP
	{
		// v = res[i]
		// v->e
		for(auto &e : g2[res[i]])
		{
			cnt[e] += cnt[res[i]];
		}
	}
	cout << cnt[b].val() << endl;

	return 0;
}
