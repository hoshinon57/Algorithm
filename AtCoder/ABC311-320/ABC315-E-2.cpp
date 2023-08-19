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

// ABC315 https://atcoder.jp/contests/abc315

/*
 * コンテスト中に解けず、解説を見た。大反省。
 * 
 * 入力に対して、Pi->iへと辺を張る正順のグラフと、i->Piへ張る逆順のグラフを用意する。
 * (1)正順のグラフでトポロジカルソートする
 *    この結果には不要な本が混ざっているが、読むべき本の順番は正しい
 * (2)逆順のグラフにて頂点1からBFSする
 * (3)(1)の結果のうち、(2)に含まれる頂点を、(1)の順番を変えずに抽出したものが答
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

// サイクルの無い有向グラフGについて、トポロジカルソートを行う。
// ソートした結果の頂点をresultに格納する。
// ※トポロジカルソートできない場合(サイクルがある場合)、G.size()とresult.size()が不一致かどうかで判断可能。
// 参考：
//   https://algo-logic.info/topological-sort/
//   https://qiita.com/Morifolium/items/6c8f0a188af2f9620db2
void bfs_topological_sort(Graph &G, vector<int> &result)
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

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		result.push_back(v);
		for(auto &e : G[v])  // 頂点vを消すイメージで、vから出た先の頂点の入次数を1減らす
		{
			indegree[e]--;
			if(indegree[e] == 0) que.push(e);
		}
	}
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	Graph g(N), g_rev(N);  // 正順、逆順
	for(i = 0; i < N; i++)
	{
		int c; cin >> c;
		for(j = 0; j < c; j++)
		{
			int p; cin >> p;
			p--;
			g[p].push_back(i);  // p->i
			g_rev[i].push_back(p);  // i->p
		}
	}

	vector<int> tp_result;
	bfs_topological_sort(g, tp_result);

	vector<int> dist(N, INF32);
	bfs_graph(g_rev, 0, dist);

	vector<int> ans;
	for(auto &e : tp_result)
	{
		if(dist[e] == INF32) continue;  // BFSの結果に含まれていない
		if(e == 0) continue;  // 頂点1は出力しない
		ans.push_back(e+1);
	}
	int S = (int)ans.size();
	for(i = 0; i < S; i++)
	{
		cout << ans[i];
		if(i != S-1) cout << " ";
	}
	cout << endl;

	return 0;
}
