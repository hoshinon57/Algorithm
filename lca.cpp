#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ダブリングによりLCA(Lowest Common Ancestor)を解くテンプレート

/*
 * 参考サイト
 * https://algo-logic.info/lca/
 * https://ikatakos.com/pot/programming_algorithm/graph_theory/lowest_common_ancestor
 * 
 * [関連する問題]
 * AOJ GRL_5_C https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_C&lang=ja
 */

using Graph = vector<vector<int>>;

/*
 * LCA(最小共通祖先)のライブラリ
 * 
 * 【使い方】
 * 0-indexed.
 *   n:頂点数
 *   g:Graph(=vector<vector<int>>)型で各頂点と繋がっている頂点を構築したオブジェクト
 *   root:根とする頂点番号 基本は0で良い
 * として
 * LCA lca(n, g, root);
 * で初期化する。
 * クエリとして頂点u,vが与えられたときに、
 * lca.Query(u, v);
 * でu,vのLCAを返す。
 * 
 * 【補足】
 * 頂点数などint型で管理する。
 * そもそも頂点数が10^8辺りを越えたら入力できないので、long long型にする必要はない。
 * 合わせてLOG_Kも最大30程度で良い。 10^9 < 2^30
 */
struct LCA
{
	// 0-indexed
	vector<vector<int>> parent;  // [k][i]:頂点iから2^k先の親
	vector<int> depth;  // [v]:頂点vの根からの深さ
	const int LOG_K = 30;  // 頂点数の最大に応じて変える 10^9 < 2^30なので最大30で十分

	// 頂点数n, グラフg, 根rootで初期化
	LCA(int n, const Graph &g, int root = 0) { Init(n, g, root); }

	void Init(int n, const Graph &g, int root = 0)
	{
		parent.resize(LOG_K+1, vector<int>(n));
		depth.resize(n);
		
		// 頂点0を根としたときの、各頂点の深さを事前計算
		dfs(g, root, -1, 0);

		// ダブリング事前計算
		// parent[0][*]はdfs()にて計算済み
		for(int k = 1; k <= LOG_K; k++)
		{
			for(int i = 0; i < n; i++)
			{
				if(parent[k-1][i] == -1)  // 2^(k-1)先の親が無いなら、2^k先の親も無し
				{
					parent[k][i] = -1;
				}
				else
				{
					parent[k][i] = parent[k-1][parent[k-1][i]];
				}
			}
		}
	}

	// 頂点v、親p、vの深さdを起点にDFS
	// depth[]およびparent[0][*]を求める
	void dfs(const Graph &g, int v, int p, int d)
	{
		parent[0][v] = p;  // 2^0 つまり1つ先の親
		depth[v] = d;
		for(auto &e : g[v])
		{
			if(e == p) continue;  // 逆流を防ぐ
			dfs(g, e, v, d+1);
		}
	}

	// 頂点u,vのLCAを求める(頂点番号を返す)
	int Query(int u, int v)
	{
		int k;
		// uとvとではvの方が深いようにしておく
		if(depth[u] > depth[v]) swap(u, v);
		int diff = depth[v] - depth[u];
		// vをuと同じ深さまで引っ張り上げる
		for(k = 0; k <= LOG_K; k++)
		{
			if( ((diff>>k)&1) == 1 )
			{
				v = parent[k][v];
			}
		}

		// この時点でu,vが一致していれば、それが答
		if(u == v)  return u;

		for(k = LOG_K; k >= 0; k--)  // 大きい方から
		{
			// u,vが一致しないギリギリでさかのぼっていく
			if(parent[k][u] != parent[k][v])
			{
				u = parent[k][u];
				v = parent[k][v];
			}
		}
		return parent[0][u];  // 1つ親が答
	}
};

int main(void)
{
	// 以下、AOJ GRL_5_C https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_C&lang=ja
	// を入力としたときのコード
	
	int i, k;
	int n;
	cin >> n;
	Graph g(n);
	for(i = 0; i < n; i++)
	{
		cin >> k;
		for(; k > 0; k--)
		{
			int c;
			cin >> c;
			// 入力によらず頂点0を根にできるように、双方向に辺をはる
			g[i].push_back(c);
			g[c].push_back(i);
		}
	}
	LCA lca(n, g, 0);
	int q;
	cin >> q;
	while(q > 0)
	{
		int u, v;
		cin >> u >> v;
		cout << lca.Query(u, v) << endl;
		q--;
	}

	return 0;
}
