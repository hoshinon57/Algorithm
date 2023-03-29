#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 「分野別 初中級者が解くべき過去問精選 100問」の問題112より
// https://qiita.com/e869120/items/acba3dd8649d913102b5
// 
// https://atcoder.jp/contests/abc014/tasks/abc014_4

/*
 * LCAを用いて解く。
 * 
 * a,bのLCAをlとすると、求める値は
 *   (aの深さ) + (bの深さ) - 2*(lの深さ) + 1
 * となる。(+1はLCAの頂点部分)
 * 
 * よって各頂点の深さをDFSで、LCAをダブリングにて求めればよい。
 * これはライブラリとして整備しているので、それを使う。
 * 
 * 計算量は、
 *   前処理：DFSがO(N), ダブリングがO(NlogN), よって全体でO(NlogN)
 *   各クエリ：O(logN)
 * より、全体でO(NlogN + QlogN) かなと。
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
	// 0-indexd
	int i;
	int N;
	cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int x, y;
		cin >> x >> y;
		x--;
		y--;
		g[x].push_back(y);
		g[y].push_back(x);
	}
	LCA lca(N, g, 0);

	int Q;
	cin >> Q;
	while(Q > 0)
	{
		int a, b;
		cin >> a >> b;
		a--;
		b--;
		int l = lca.Query(a, b);
		// +1はLCAとなる頂点の分
		cout << lca.depth[a] + lca.depth[b] - 2*lca.depth[l] + 1 << endl;
		Q--;
	}

	return 0;
}
