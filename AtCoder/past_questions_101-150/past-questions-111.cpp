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

// 「上級者が解くべき過去問精選 100 + 50 問」の問題111
// https://qiita.com/e869120/items/acba3dd8649d913102b5
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_5_C&lang=ja

/*
 * 以下2サイトを参考に、LCAのアルゴリズムを実装して解く。
 * https://algo-logic.info/lca/
 * https://ikatakos.com/pot/programming_algorithm/graph_theory/lowest_common_ancestor
 * 
 * parent[k][i]:頂点iから2^k先の親 (2^k先の親が無い場合は-1)
 * depth[v]:頂点vの根からの深さ
 * として定義する。
 * parentはダブリングの考え方によるもの。
 * 
 * 事前計算として、まずDFSで頂点0を根としたときの各頂点の深さを求める。
 * この際、parent[0][i]は1つ先の親になるので、合わせて求めておく。
 * 
 * 次にダブリングの事前計算として、parent[k][i]を計算する。
 * 
 * クエリ処理では、頂点u,vの深さをまずはそろえる。
 * 深さの差をdiffとしたときに、diffにて立っているビットに対してparentを用いることで、O(logK)で処理できる。
 * この時点でu,vが一致していれば、それが答となる。
 * そうでない場合、parent[]を用いて
 *   u' = parent[k][u]
 *   v' = parent[k][v]
 * としたときに、u'!=v'の範囲でさかのぼっていく。 (※)kが大きい方から見ていく必要がある
 * つまりu'==v'にならないギリギリまでさかのぼるイメージ。
 * すると、最後にu'の1つ親が答となる。
 */

using Graph = vector<vector<int>>;
vector<vector<int>> parent;  // [k][i]:頂点iから2^k先の親
vector<int> depth;  // [v]:頂点vの根からの深さ

// 頂点v、親p、vの深さdを起点にDFS
void dfs(Graph &g, int v, int p, int d)
{
	parent[0][v] = p;  // 1つ先の親
	depth[v] = d;
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dfs(g, e, v, d+1);
	}
};

int main(void)
{
	int i, k;
	int n;
	cin >> n;
	Graph g(n);
	for(i = 0; i < n; i++)
	{
		cin >> k;
		while(k > 0)
		{
			int c;
			cin >> c;
			// 入力によらず頂点0を根にできるように、双方向に辺をはる
			g[i].push_back(c);
			g[c].push_back(i);
			k--;
		}
	}
	const int LOG_K = 20;  // 頂点の最大は10^6なので、深さの最大も10^6 < 2^20
	parent.resize(LOG_K+1, vector<int>(n));
	depth.resize(n);

	// 頂点0を根としたときの、各頂点の深さを事前計算
	dfs(g, 0, -1, 0);

	// ダブリング事前計算
	for(k = 1; k <= LOG_K; k++)
	{
		for(i = 0; i < n; i++)
		{
			if(parent[k-1][i] == -1)
			{
				parent[k][i] = -1;
			}
			else
			{
				parent[k][i] = parent[k-1][parent[k-1][i]];
			}
		}
	}

	// クエリ処理
	int q;
	cin >> q;
	while(q > 0)
	{
		int u, v;
		cin >> u >> v;
		// uとvとではvの方が深いようにしておく
		if(depth[u] > depth[v]) swap(u, v);
		int diff = depth[v] - depth[u];
		// vをuと同じ深さまで引っ張り上げる
		for(k = 0; k <= LOG_K; k++)
		{
			if( ((diff >> k)&1) == 1)
			{
				v = parent[k][v];
			}
		}

		// この時点でu,vが一致していれば、それが答
		if(u == v)
		{
			cout << u << endl;
		}
		else
		{
			for(k = LOG_K; k >= 0; k--)  // 大きい方から
			{
				if(parent[k][u] != parent[k][v])
				{
					u = parent[k][u];
					v = parent[k][v];
				}
			}
			cout << parent[0][u] << endl;  // 1つ親が答
		}
		q--;
	}

	return 0;
}
