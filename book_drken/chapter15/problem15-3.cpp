#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題15.3
// AOJ 1350
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1350
// https://onlinejudge.u-aizu.ac.jp/problems/1350

/*
 * クラスカル法と、グラフのサイクル検出を用いて解く。
 * 
 * まずはクラスカル法を用いて最小全域木(以下MST)を求めていく。
 * その際、MSTに不要な辺があれば、あえてその辺を追加してサイクルを発生させる。
 * 追加した辺と同じ重みの辺がサイクル内にあれば、
 * それは「MSTにて必ず含まれる辺、ではない」と考えられる。
 * ※以下記事でも言及されている考え方。
 *   記事では「間に合わない」と書かれているが、時間内にACできた。
 *   https://qiita.com/hidekikangeki/items/2520fdcf2c6332943347
 * 
 * サイクル検出には深さ優先探索を用いた。
 * 
 * クラスカル法によるMSTを求める際に使う変数は以下。
 *   uf, edges, MST_edges
 * サイクルを求める際に使う変数は以下。
 *   graph, weight, seen
 * graphとweightはedgesで代用できた気もするが、
 * DFSにて対象となる頂点や辺を素早く列挙するために、別途用意した。
 */

struct UnionFind
{
	vector<int> parent;  // parent[x]:xの親の番号 xが根であれば-1
	vector<int> siz;  // siz[x]:xの属するグループの頂点数

	// 頂点数nで初期化
	UnionFind(int n) : parent(n, -1), siz(n, 1) {}

	// 根を求める
	int root(int x)
	{
		if(parent[x] == -1) return x;  // xが根であれば、xを返す
		else
		{
			parent[x] = root(parent[x]);
			return parent[x];
		}
	}

	// xとyが同じグループに属するか
	bool issame(int x, int y)
	{
		return root(x) == root(y);  // 根が同一なら同じグループ
	}

	// xを含むグループとyを含むグループを併合する
	bool unite(int x, int y)
	{
		// x,yをそれぞれ根まで移動させる
		x = root(x);
		y = root(y);

		// すでに同じグループであれば何もしない
		if(x == y) return false;

		// 頂点数が少ない方が子になるようにする
		if(siz[x] < siz[y])
		{
			// xをyの子にする
			parent[x] = y;
			siz[y] += siz[x];
		}
		else
		{
			// yをxの子にする
			parent[y] = x;
			siz[x] += siz[y];
		}
		return true;
	}

	// xを含むグループの頂点数
	int size(int x)
	{
		return siz[root(x)];
	}
};

using Graph = vector<vector<int>>;  // 頂点間の接続を表す
using Edge = pair<int, pair<int,int>>;  // 辺e=(u,v)を{w(e),{u,v}}で表す

// 各頂点において接続している頂点を保持する
// DFSにて、見ている頂点からの行き先を素早く列挙するために用いる
Graph graph;

// 辺e=(u,v)間の重みをweight[u][v]で表す
// weight[u][v], weight[v][u]は同じ値を設定しておく
// DFSにて、検出した辺の重みを素早く参照するために用いる
vector<vector<int>> weight;

// seen[i]:DFSにて頂点iが探索済みならtrue
// サイクル検出のために使う
vector<bool> seen;

// MSTにて必ず含まれる辺ならtrue
// must_edge_in_MST[u][v]と[v][u]は同じ値を設定しておく
vector<vector<bool>> must_edge_in_MST;

// 頂点vを起点として、DFSを用いてサイクル検出を行う。
// サイクルを検出した場合、サイクル内の辺にtargetWeightの辺があれば
// その辺は「MSTにて必ず含まれる辺、ではない」と判断でき、must_edge_in_MST[]をfalseにする。
// v:探索する頂点
// parent:vの親
// targetWeight:サイクルにてこの重みの辺があれば、これはMSTにて必ず含まれる辺、ではない
// 戻り値:サイクルを検出したらtrueを返す
bool dfs(int v, int parent, int targetWeight)
{
	seen[v] = true;

	for(auto &e : graph[v])
	{
		if(e == parent) continue;  // 逆流を防ぐ
		if(seen[e])
		{
			// サイクル検出
			if(weight[v][e] == targetWeight)
			{
				// 辺{v,e}は、MSTにて必ず含まれる辺、ではない
				must_edge_in_MST[v][e] = false;
				must_edge_in_MST[e][v] = false;
			}
			return true;
		}
		else
		{
			if(dfs(e, v, targetWeight))
			{
				// 再帰先からサイクル検出して返ってきたら、またtrueで返す
				if(weight[v][e] == targetWeight)
				{
					must_edge_in_MST[v][e] = false;
					must_edge_in_MST[e][v] = false;
				}
				return true;
			}
		}
	}

	return false;
}

int main(void)
{
	// 0-indexed
	int i;
	int N, M;
	cin >> N >> M;
	vector<Edge> edges(M);  // 入力の辺
	vector<Edge> MST_edges;  // クラスカル法により求めたMSTの辺
	graph.assign(N, vector<int>());  // 要素数は頂点数
	weight.assign(N, vector<int>(N, 0));  // 要素数は頂点数
	must_edge_in_MST.assign(N, vector<bool>(N, true));
	for(i = 0; i < M; i++)
	{
		int u, v, w;
		cin >> u >> v >> w;
		u--;  // 0-indexedに変換
		v--;
		edges[i] = make_pair(w, make_pair(u, v));
		weight[u][v] = w;
		weight[v][u] = w;
	}
	// ここまで入力

	// 辺を重みが小さい順にソート
	sort(edges.begin(), edges.end());

	// クラスカル法
	UnionFind uf(N);
	for(i = 0; i < M; i++)
	{
		int u, v, w;
		u = edges[i].second.first;
		v = edges[i].second.second;
		w = edges[i].first;

		// 入力時ではなくこのタイミングで辺{u,v}を構築する
		// MSTではない辺であれば、後に削除する
		graph[u].push_back(v);
		graph[v].push_back(u);
		if(uf.issame(u, v))
		{
			// seen[]を初期化して、頂点uスタートでサイクルを探す
			// サイクル内に重みwの辺があるかを調べる
			seen.assign(N, false);
			dfs(u, -1, w);

			// サイクルが発生する場合は辺を追加しないので、さきほど追加した辺を削除する
			graph[u].pop_back();
			graph[v].pop_back();
		}
		else
		{
			uf.unite(u, v);
			MST_edges.push_back(edges[i]);
		}
	}

#if 0  // 確認テスト用
	for(auto &e : MST_edges)
	{
		cout << e.second.first << " " << e.second.second << ":" << e.first << " / ";
		cout << must_edge_in_MST[e.second.first][e.second.second] << endl;
	}
	cout << "-----" << endl;
#endif

	int ans_edge_num = 0;
	int ans_weight = 0;
	// クラスカル法で求めた辺に対し、「その辺がMSTにて必ず含まれる辺か？」を
	// must_edge_in_MST[]を用いてチェックしていく
	for(auto &e : MST_edges)
	{
		if(must_edge_in_MST[e.second.first][e.second.second])
		{
			// 辺eは必ず含まれる辺なので、答に追加
			ans_edge_num++;
			ans_weight += e.first;
		}
	}
	cout << ans_edge_num << " " << ans_weight << endl;

	return 0;
}
