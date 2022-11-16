#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題24
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_11_B&lang=ja

/*
 * 深さ優先探索で解く。
 * 
 * vector<vector<int>> の形式でグラフを構築し、
 * graph[i]には頂点iからたどれる頂点の一覧を持つようにする。
 * これと頂点が探索済みかを示すseen[]を使って、DFSを行う。
 * 
 * タイムスタンプdは、dfs()の行きがけ時に設定する。
 * タイムスタンプfは、dfs()の帰りがけ時に設定する。
 * 
 * ＞深さ優先探索の過程において、訪問する頂点の候補が複数ある場合は頂点番号が小さいものから選択します。
 * について、入力にて「頂点番号が小さいものから並んでいる」とは明記されていなかったので、
 * 入力時にgraph[]をソートしておく。
 */

using Graph = vector<vector<int>>;
Graph graph;  // graph[i]:頂点iと繋がっている頂点の一覧
vector<int> stamp_d, stamp_f;  // stamp_d[i]:頂点iを最初に発見した時刻
vector<bool> seen;  // seen[i]:頂点iを探索済みならtrue
int time_stamp;

// 頂点vを起点に深さ優先探索
void dfs(int v)
{
	if(seen[v]) return;  // 探索済みなら戻る
	seen[v] = true;

	stamp_d[v] = time_stamp;
	time_stamp++;

	for(auto &e : graph[v])  // e:頂点vから行ける頂点
	{
		dfs(e);
	}

	stamp_f[v] = time_stamp;
	time_stamp++;

	return;
}

int main(void)
{
	// 1-indexed
	int i, j;
	int n;
	cin >> n;
	graph.resize(n+1);
	stamp_d.resize(n+1);
	stamp_f.resize(n+1);
	seen.resize(n+1, false);
	for(i = 1; i <= n; i++)
	{
		int u, k;
		cin >> u >> k;
		for(j = 0; j < k; j++)
		{
			int v;
			cin >> v;
			graph[u].push_back(v);  // u->v
		}
		sort(graph[u].begin(), graph[u].end());  // 仕様に基づき、頂点uと繋がっている頂点一覧をソートしておく
	}
	time_stamp = 1;

	// 頂点1から順に起点としてDFS
	for(i = 1; i <= n; i++)
	{
		if(seen[i]) continue;  // 探索済みならスルー
		dfs(i);
	}

	for(i = 1; i <= n; i++)
		cout << i << " " << stamp_d[i] << " " << stamp_f[i] << endl;

	return 0;
}
